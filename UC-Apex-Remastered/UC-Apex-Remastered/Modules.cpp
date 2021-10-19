#pragma check_stack(off)
#include "Modules.h"
#include "Item_IDs.h"
#include "prediction.h"
#include <random>
#include <iomanip>
#include <time.h>

vec3 oldPunch = { 0.f, 0.f, 0.f };
int lastKey = 0;
bool debug = true;

void timeFunc(void (*func)(), std::string name = "function")
{
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    func();
    std::chrono::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double length = duration.count();
    std::cout << name << " took " << length << " microseconds" << std::endl;
}

void Modules::Run()
{
    if (debug)
    {
        system("cls");
        timeFunc(Modules::UpdateVars, "Modules::UpdateVars()");
        timeFunc(Modules::UpdateKeys, "Modules::UpdateKeys()");
        timeFunc(Modules::ItemGlow, "Modules::ItemGlow()"); // This takes ~70 ms which is the main cause for the slowdown in this cheat, either put it in a
                                                            // seperate thread(stupid idea) or actually fix it. It happens because we read 65,000 signatures
                                                            // (2 reads each + loops bc readString)
        timeFunc(Modules::NewPlayerGlow, "Modules::NewPlayerGlow()");
        timeFunc(Modules::PlayerGlow, "Modules::PlayerGlow()");
        timeFunc(Modules::ThirdPerson, "Modules::ThirdPerson()");
        timeFunc(Modules::RapidFire, "Modules::RapidFire()");
    }
    else
    {
        Modules::UpdateVars();
        Modules::UpdateKeys();
        Modules::ItemGlow();
        Modules::NewPlayerGlow();
        Modules::PlayerGlow();
        Modules::ThirdPerson();
        Modules::RapidFire();
    }
    /*
    // Theoretical spectator count...
    std::vector<uintptr_t> players = Player::GetPlayers();
    // Dead players will still be in the 'players' array. However, Players who have left the game are not here.
    // In order to make this work correctly we need to find a way to monitor kills.... I don't think I'm a good enough programmer for this though...
    printf("%i Players.\n", players);
    */
}

typedef struct CEntInfo
{
    DWORD64 pEntity;
    int		SerialNumber;
    DWORD64 pPrev;
    DWORD64 pNext;
};

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/client_class.h
struct ClientClass
{
    uint64_t pCreateFn;
    uint64_t pCreateEventFn;
    uint64_t pNetworkName;
    uint64_t pRecvTable;
    uint64_t pNext;
    uint32_t ClassID;
    uint32_t ClassSize;
};

// https://github.com/CasualX/apexbot/blob/d6dfbf40b5025e6676fe75177a39ddd285285160/src/state.cpp#L104
bool getClassName(uintptr_t ent, std::string &className)
{
    uintptr_t clientNetworkableVTable = Driver.rpm<uintptr_t>(ent + 8 * 3);
    if (!clientNetworkableVTable)
        return false;
    
    uintptr_t getClientClass = Driver.rpm<uintptr_t>(clientNetworkableVTable + 8 * 3);
    if (!getClientClass)
        return false;

    uint32_t disp = Driver.rpm<uint32_t>(getClientClass + 3);
    if (!disp) {
        return false;
    }
    const uint64_t client_class_ptr = getClientClass + disp + 7;
    // Get the ClientClass instance itself.
    ClientClass clientClass = Driver.rpm<ClientClass>(client_class_ptr);

    // Finally grab some bytes to be interpreted as a nul terminated c-string.
    className = Driver.ReadString(clientClass.pNetworkName, 128);
    if (className.length() == 128)
        className = xorstr_("Unknown");
    return true;
}

uintptr_t GetEntityById(int Ent)
{
    uintptr_t EntityList = globals.moduleBase + OFFSET_ENTITYLIST; //updated
    uintptr_t BaseEntity = Driver.rpm<DWORD64>(EntityList);
    if (!BaseEntity)
        return NULL;
    return Driver.rpm<uintptr_t>(EntityList + (Ent << 5));
}

const size_t NUM_ENT_ENTRIES = 0x10000;

struct EntityInfo {
    uintptr_t EntityPtr;
    uint8_t a, b, c;
};

EntityInfo entitiesHeap[NUM_ENT_ENTRIES / 2];

void getEnts()
{
    // 10th Time recoding this, Hours wasted: 25
    std::vector<uintptr_t> entVector;

    int validEntities = 0;
    Driver.ReadRaw(globals.moduleBase + OFFSET_ENTITYLIST, (UINT_PTR)&entitiesHeap, NUM_ENT_ENTRIES / 2 * sizeof(EntityInfo));
    for (auto i = 0; i < NUM_ENT_ENTRIES / 2; i++) {
        if (entitiesHeap[i].EntityPtr != 0)
            validEntities++;
        entVector.push_back(entitiesHeap[i].EntityPtr);
    }
    Driver.ReadRaw(globals.moduleBase + OFFSET_ENTITYLIST + NUM_ENT_ENTRIES / 2 * sizeof(EntityInfo), (UINT_PTR)&entitiesHeap, NUM_ENT_ENTRIES / 2 * sizeof(EntityInfo));
    for (auto i = 0; i < NUM_ENT_ENTRIES / 2; i++) {
        if (entitiesHeap[i].EntityPtr != 0)
            validEntities++;
        entVector.push_back(entitiesHeap[i].EntityPtr);
    }
    //printf("Found %i Valid Entities\n", validEntities);
    globals.entityList = entVector;
    return;
}

void Modules::UpdateVars()
{
    globals.localPlayer = Driver.rpm<uintptr_t>(globals.moduleBase + OFFSET_LOCAL_ENT);
    globals.viewRenderer = Driver.rpm<uintptr_t>(globals.moduleBase + OFFSET_RENDER);
    getEnts();
}

void Modules::UpdateKeys()
{
    // i made a custom GetAsyncKeyState & 1 becuase the & 1 flag doesn't work on apex for me
    while (GetAsyncKeyState(VK_INSERT)) {
        lastKey = VK_INSERT;
    }
    if (lastKey == VK_INSERT) {
        lastKey = 0;
        globals.menuActive = !globals.menuActive;
    }
}

void Modules::NoRecoil()
{
    // check if localplayer is valid and rcs is on
    if (!globals.rcs || !Player::IsPlayer(globals.localPlayer) || globals.aimbotting || !GetAsyncKeyState(VK_RBUTTON)) return;

    // get current angles and recoil (punch angle)
    vec3 viewAngles = Driver.rpm<vec3>(globals.localPlayer + OFFSET_VIEWANGLES);
    vec3 punchAngle = Driver.rpm<vec3>(globals.localPlayer + OFFSET_AIMPUNCH);
    //QAngle swayAngles = Driver.rpm<QAngle>(globals.localPlayer + OFFSET_BREATH_ANGLES);

    // ViewAngles -> PunchAngles -> SwayAngles
    // calculate the new angles by adding the punchangle to the viewangles, it is important to subtract the old punch angle
    vec3 newAngle = viewAngles + (oldPunch - punchAngle) * (globals.rcsIntensity / 100.f);
    //QAngle difference = (oldPunch - punchAngle);

    newAngle.Normalize(); // clamp angles
    Driver.wpm<vec3>(globals.localPlayer + OFFSET_VIEWANGLES, newAngle); // overwrite old angles
    oldPunch = punchAngle;
}

struct GlowMode {
    int8_t GeneralGlowMode, // 0x0
        BorderGlowMode, // 0x8
        BorderSize, // 0x10
        TransparentLevel; // 0x18
};

void Modules::ItemGlow()
{
    if (!globals.itemGlow || !Player::IsPlayer(globals.localPlayer))
        return;
    
    for (int i = 0; i < globals.entityList.size(); i++) {
        uintptr_t ent = globals.entityList[i];
        if (ent == NULL) continue;

        std::string signifer = Player::GetSignifier(ent);
        if (signifer != xorstr_("prop_death_box") && signifer != xorstr_("prop_survival"))
            continue;

        if (globals.deathBoxESP && signifer == xorstr_("prop_death_box")) {
            Driver.wpm<int>(ent + OFFSET_GLOW_ENABLE, 1); // Glow Enable
            Driver.wpm<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2); // Through Walls
            Driver.wpm<GlowMode>(ent + OFFSET_GLOW_MODE, { 0, 118, 127, 100 });
        }

        int itemID = Driver.rpm<int>(ent + OFFSET_SCRIPT_ID);

        if (signifer == xorstr_("prop_survival") && itemID >= 1 && itemID <= 240) // This has to be updated when items are added to Item_IDs.h
        {
            //if (Driver.rpm<int>(targetEnt + OFFSET_GLOW_ENABLE) == 1) return; // Doing this prevents updates to itemESP working -_- I hate coding so much
            Driver.wpm<int>(ent + OFFSET_GLOW_ENABLE, 1); // Glow Enable
            Driver.wpm<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2); // Through Walls
            Driver.wpm<GlowMode>(ent + OFFSET_GLOW_MODE, { 0, 118, 100, 100 });
            Color itemColor = { 0, 0, 255 };
            // 0x30 = "mdl/weapons/at_rifle/w_at_rifle.rmdl"
            // 0x40 = "prop_survival"
            // 0x48 = 4 BYTES(Possibly what type of model this is) + "mdl/weapons/at_rifle/w_at_rifle.rmdl"
            // 0x80 = some point in _NUMBERHERE_cl_entityScriptId array
            // 0x88 = a different point in the above array

            for (auto const& [key, val] : Items::itemList) {
                if (itemID == key) {
                    // This is the stupidest shit I've ever done, I tried so many other ways and none of them worked.
                    switch (val.rarity) {
                    case Items::COMMON:
                        itemColor = (globals.commonColor * 255);
                        break;
                    case Items::RARE:
                        itemColor = (globals.rareColor * 255);
                        break;
                    case Items::EPIC:
                        itemColor = (globals.epicColor * 255);
                        break;
                    case Items::LEGENDARY:
                        itemColor = (globals.legendaryColor * 255);
                        break;
                    case Items::HEIRLOOM:
                        itemColor = (globals.heirloomColor * 255);
                        break;
                    }
                }
            }
            Driver.wpm<Color>(ent + OFFSET_GLOW_COLOR, itemColor);
        }
    }
}

struct GlowFade {
    int a, b;
    float c, d, e, f;
};

void Modules::NewPlayerGlow()
{
    if (!globals.playerGlow || globals.playerGlowStyles[globals.playerGlowStyle] != xorstr_("Advanced") || !Player::IsPlayer(globals.localPlayer))
        return;

    int localTeam = Driver.rpm<int>(globals.localPlayer + OFFSET_TEAM);
    for (int i = 0; i < 100; i++) {
        uintptr_t ent = globals.entityList[i];
        if (ent == NULL) continue;

        std::string entName = Driver.ReadString(ent + OFFSET_NAME);
        if (entName == xorstr_("player") || Player::IsPlayer(ent) || Player::IsDummy(ent)) {
            float currentEntityTime = 60000.f;
            GlowMode mode = { 101, 102, 50, 100 };
            vec3 color;
            if (localTeam == Driver.rpm<int>(ent + OFFSET_TEAM)) {
                // Teamate, Light Green
                color = { 1.65f, 2.28f, 0.87f };
            }
            else if (Driver.rpm<int>(ent + OFFSET_BLEED_OUT_STATE) != 0 && Driver.rpm<int>(ent + OFFSET_LIFE_STATE) != 0) {
                // Enemy, Downed/Dead(?), Yellow
                color = { 3.f, 3.f, 0.f };
            }
            else {
                // Enemy, Alive
                int shield = Driver.rpm<int>(ent + OFFSET_SHIELD);
                int health = Driver.rpm<int>(ent + OFFSET_HEALTH);
                if (shield > 100) { //Heirloom armor - Red
                    color = { 10.f, 0.f, 0.f };
                }
                else if (shield > 75) { //Purple armor - Purple
                    color = { 1.84f, 0.46f, 2.07f };
                }
                else if (shield > 50) { //Blue armor - Light blue
                    color = { 0.39f, 1.77f, 2.85f };
                }
                else if (shield > 0) { //White armor - White
                    color = { 3.f, 3.f, 3.f };
                }
                else if (health < 50) { //Above 50% HP - Orange
                    color = { 3.f, 1.8f, 0.f };
                }
                else { //Below 50% HP - Light Red
                    color = { 2.88f, 0.78f, 0.63f };
                }
            }
            Driver.wpm<GlowMode>(ent + OFFSET_GLOW_MODE, mode);
            Driver.wpm<vec3>(ent + OFFSET_GLOW_COLOR, color);

            Driver.wpm<float>(ent + OFFSET_GLOW_DISTANCE, 40000.f);
            Driver.wpm<float>(ent + 0x3A4, currentEntityTime);
            currentEntityTime -= 1.f;
            Driver.wpm<int>(ent + OFFSET_GLOW_ENABLE, 1);
            Driver.wpm<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 1);
            //Driver.wpm<GlowFade>(ent + 0x388, { 872415232, 872415232, currentEntityTime, currentEntityTime, currentEntityTime, currentEntityTime });
        }
    }
}

void Modules::PlayerGlow()
{
    if (!globals.playerGlow || globals.playerGlowStyles[globals.playerGlowStyle] != xorstr_("Normal") || !Player::IsPlayer(globals.localPlayer))
        return;

    for (int i = 0; i < 100; i++) {
        uintptr_t ent = globals.entityList[i];
        if (ent == NULL) continue;

        if (Player::IsPlayer(ent) || (globals.dummyGlow && Player::IsDummy(ent))) {
            Driver.wpm<int>(ent + OFFSET_GLOW_ENABLE, 1); // Enable/Disable
            Driver.wpm<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2); // Through Walls
            Driver.wpm<GlowMode>(ent + 0x2C4, { 101, 102, 50, 100 });
            Color writtenColor = globals.playerGlowColor * 255;

            Driver.wpm<Color>(ent + OFFSET_GLOW_COLOR, writtenColor);
        }
    }
}

void Modules::DrawBoxESP(float x, float y, float width, float height, int thickness, D3DCOLOR fillColor, D3DCOLOR boxColor)
{
    if (globals.fillBox)
        DrawFilledRectangle(x, y, width, height, fillColor);
    DrawBorderBox(x, y, width, height, thickness, boxColor);
}

void Modules::DrawHealthbars(float x, float y, float width, float height, int thickness, int health, int shieldHealth, int shieldType)
{
    // shitty calculation but it works
    DrawFilledRectangle(x - (thickness + 5), y, thickness, height, ARGB(255, 0, 255, 0));
    DrawFilledRectangle(x - (thickness + 5), y, thickness, height - ((height / 100) * health), ARGB(120, 255, 0, 0));
    DrawFilledRectangle(x - (thickness * 2 + 12), y, thickness, height, ARGB(255, 155, 0, 252));
    Color shieldColor = { 0, 0, 0 };

    switch (shieldType) {
    case 1:
        shieldColor = globals.commonColor;
        break;
    case 2:
        shieldColor = globals.rareColor;
        break;
    case 3:
        shieldColor = globals.epicColor;
        break;
    case 4:
        shieldColor = globals.legendaryColor;
        break;
    case 5:
        shieldColor = globals.heirloomColor;
        break;
    }
    DrawFilledRectangle(x - (thickness * 2 + 12), y, thickness, height - ((height / 125) * shieldHealth), ARGB(0, 0, 0, 0));

    if (shieldHealth > 0)
        DrawBorderBox(x - (thickness * 2 + 12) - 1, y - 1, thickness + 1, height + 1, 1, ARGB((int)shieldColor.r * 255, (int)shieldColor.g * 255, (int)shieldColor.b * 255, 0));
    DrawBorderBox(x - (thickness + 5) - 1, y - 1, thickness + 1, height + 1, 1, ARGB(255, 0, 0, 0));
}

float lastTimeScale = 1.f;
void Modules::RapidFire()
{
    if (!globals.rapidFire || !Player::IsPlayer(globals.localPlayer) || !GetAsyncKeyState(VK_LBUTTON) || GetForegroundWindow() != globals.hWnd) {
        if (lastTimeScale != 1.f)
            Driver.wpm<float>(globals.moduleBase + 0x01cd0078 + 0x18, 1);
        lastTimeScale = 1.f;
        return;
    }
    uintptr_t actWeaponID = Driver.rpm<uintptr_t>(globals.localPlayer + OFFSET_CURRENT_WEAPON) & 0xFFFF;
    uintptr_t currentWeapon = Driver.rpm<uintptr_t>(globals.moduleBase + OFFSET_ENTITYLIST + (actWeaponID << 5));

    int ammoInClip = Driver.rpm<int>(currentWeapon + 0x16c4);
    if (ammoInClip <= 0) return;

    Driver.wpm<float>(globals.moduleBase + 0x01cd0078 + 0x18, globals.rapidFireSpeed);
    lastTimeScale = globals.rapidFireSpeed;
}

// There is one line in here that causes the aimbot to break, this is an anti-pasting measure. Good luck and have fun.
// Note: No. I will not help you find or fix it. Stop asking.
float entOldVisTime[NUM_ENT_ENTRIES];
void Modules::Aimbot()
{
    int aimKey = 0;
    switch (globals.aimbotKeyID) {
    case 0: // "Right Mouse"
        aimKey = VK_RBUTTON;
        break;
    case 1: // "Left Alt"
        aimKey = VK_LMENU;
        break;
    case 2: // "Left Mouse"
        aimKey = VK_LBUTTON;
        break;
    case 3: // "Left Shift"
        aimKey = VK_LSHIFT;
        break;
    case 4: // "Space"
        aimKey = VK_SPACE;
        break;
    }
    if ((!globals.aimbot || !Player::IsPlayer(globals.localPlayer)) && !GetAsyncKeyState(aimKey))
        return;

    // TODO: Review this, it may be able to be moved ouside of the aimbot loop causing less allocations and deallocations
    ImGuiIO& io = ImGui::GetIO();
    float screenWeight = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;

    float closestDist = 99999.f;
    vec3 closestPlayerVec3 = { 0.f, 0.f, 0.f };
    int localTeamID = Driver.rpm<int>(globals.localPlayer + OFFSET_TEAM);

    /*
    Bones:
    0, 1: Groin
    2: Waist
    3: Center Chest
    4: Right Hand
    5: Low Neck
    6, 7: High Neck
    8: Head
    9: Heart
    */

    for (size_t i = 0; i < 100; i++) {
        if (globals.entityList.size() <= i) break; // If the entityList updates while we're looping through it the size could change.

        uintptr_t targetEnt = globals.entityList[i];
        if (targetEnt == NULL) continue;
        
        std::string entSignifier = Player::GetSignifier(targetEnt);
        
        if ((entSignifier != xorstr_("npc_dummie") && entSignifier != xorstr_("player")) || targetEnt == globals.localPlayer) continue; // if ((notDummy and notPlayer) or isSelf) continue;

        vec3 aimPos = Util::GetBonePos(targetEnt, 7); // Neck bone
        aimPos.z += globals.aimbotOffset;
        
        // Target movement prediction
        uintptr_t actWeaponID = Driver.rpm<uintptr_t>(globals.localPlayer + OFFSET_CURRENT_WEAPON) & 0xFFFF;
        uintptr_t currentWeapon = Driver.rpm<uintptr_t>(globals.moduleBase + OFFSET_ENTITYLIST + (actWeaponID << 5));
        float bulletSpeed = Driver.rpm<float>(currentWeapon + OFFSET_BULLET_SPEED);
        float bulletGrav = Driver.rpm<float>(currentWeapon + OFFSET_BULLET_GRAVITY);
        if (bulletSpeed > 1.f)
        {
            vec3 localPos = Driver.rpm<vec3>(globals.localPlayer + OFFSET_CAMERAPOS);
            vec3 targetVel = Driver.rpm<vec3>(targetEnt + OFFSET_ABS_VELOCITY);

            aimPos = playerPrediction(localPos, aimPos, targetVel, bulletSpeed, bulletGrav);
        }

        // Convert to screen position
        vec2 screenAimPos;
        if (Util::WorldToScreen(aimPos, screenAimPos)) // If the target is off-screen don't continue
            continue;

        float entNewVisTime = Driver.rpm<float>(targetEnt + OFFSET_VISIBLE_TIME); // The total amount of time an entity has been visible
        int targetTeamID = Driver.rpm<int>(targetEnt + OFFSET_TEAM);

        // Entity is enemy
        if (targetTeamID != localTeamID) {
            if (entNewVisTime != entOldVisTime[i]) {
                int entKnockedState = Driver.rpm<int>(targetEnt + OFFSET_BLEED_OUT_STATE);
                // If enemy not knocked
                if (entKnockedState == 0 || entSignifier == xorstr_("npc_dummie")) {
                    if (Util::Dist2D({ screenWeight / 2, screenHeight / 2 }, screenAimPos) < globals.aimbotFOV && Util::Dist2D({ screenWeight / 2, screenHeight / 2 }, screenAimPos) < closestDist) {
                        closestPlayerVec3 = aimPos;
                        closestDist = Util::Dist2D({ screenWeight / 2, screenHeight / 2 }, screenAimPos);
                    }
                }
                entOldVisTime[i] = entNewVisTime;
            }
        }
    }

    // Found a player and still aimbotting
    globals.aimbotting = closestDist != 99999.f && GetAsyncKeyState(aimKey);
    if (globals.aimbotting) {
        vec3 cameraPos = Driver.rpm<vec3>(globals.localPlayer + OFFSET_CAMERAPOS);
        vec3 viewAngle = Driver.rpm<vec3>(globals.localPlayer + OFFSET_VIEWANGLES);
        vec3 targetAngle = Util::CalcAngle(cameraPos, closestPlayerVec3);
        vec3 diff = targetAngle - viewAngle;
        vec3 newAngle = viewAngle + diff / (globals.aimbotSmoothness / 100.f);

        if (globals.rcs) {
            // We have to do no-recoil here because Modules::NoRecoil() stops working whenever the aimbot is writing the view angles
            vec3 oldAngle = Driver.rpm<vec3>(globals.localPlayer + OFFSET_VIEWANGLES);
            vec3 punchAngle = Driver.rpm<vec3>(globals.localPlayer + OFFSET_AIMPUNCH);
            vec3 breathAngle = Driver.rpm<vec3>(globals.localPlayer + OFFSET_BREATH_ANGLES);

            // subtracting punchangles and breath angles
            newAngle -= breathAngle * (globals.rcsIntensity / 100.f);
            newAngle -= (punchAngle * 0.05f) * (globals.rcsIntensity / 100.f);
            newAngle += oldAngle * (globals.rcsIntensity / 100.f);
            // do this so the rcs doesnt jump down after unlocking from the enemy
            oldPunch = punchAngle;
        }
        // Normalize to prevent bans for invalid angles.
        newAngle.Normalize();

        Driver.wpm<vec3>(globals.localPlayer + OFFSET_VIEWANGLES, newAngle);
    }
}

void Modules::ThirdPerson()
{
    Driver.wpm<int>(globals.moduleBase + OFFSET_THIRD_PERSON + 0x6C, globals.thirdPerson ? 2 : -1);
}
