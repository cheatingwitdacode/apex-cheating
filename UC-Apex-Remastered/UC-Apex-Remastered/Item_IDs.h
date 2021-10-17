#pragma once
#include <map>
#include "defs.h"
#include <string>
/*
Notes:
All Gold variants of guns are Item + 4 afaik
*/

namespace Items {
	enum Rarity { HEIRLOOM, LEGENDARY, EPIC, RARE, COMMON };

	struct CProp {
		std::string itemName;
		Rarity rarity;
	};

	std::map<uint32_t, CProp> itemList = {
		{ 1, { "Kraber", Rarity::HEIRLOOM }},
		{ 2, { "Mastiff", Rarity::COMMON }},
		{ 6, { "Mastiff Gold", Rarity::LEGENDARY }},
		{ 7, { "LStar", Rarity::COMMON }},
		{ 11, { "LStar Gold", Rarity::LEGENDARY }},
		{ 12, { "Havoc", Rarity::COMMON }},
		{ 16, { "Havoc Gold", Rarity::LEGENDARY }},
		{ 17, { "Devotion", Rarity::COMMON }},
		{ 21, { "Devotion Gold", Rarity::LEGENDARY }},
		{ 22, { "Triple Take", Rarity::HEIRLOOM }},
		{ 23, { "Flatline", Rarity::COMMON }},
		{ 27, { "Flatline Gold", Rarity::LEGENDARY }},
		{ 28, { "Hemlock", Rarity::COMMON }},
		{ 32, { "Hemlock Gold", Rarity::LEGENDARY }},
		{ 33, { "G7 Scout", Rarity::COMMON }},
		{ 37, { "G7 Scout Gold", Rarity::LEGENDARY }},
		{ 38, { "Alternator", Rarity::HEIRLOOM }},
		{ 39, { "R99", Rarity::COMMON }},
		{ 43, { "R99 Gold", Rarity::LEGENDARY }},
		{ 44, { "Prowler", Rarity::COMMON }},
		{ 48, { "Prowler Gold", Rarity::LEGENDARY }},
		{ 49, { "Volt", Rarity::COMMON }},
		{ 53, { "Volt Gold", Rarity::LEGENDARY }},
		{ 54, { "Longbow", Rarity::COMMON }},
		{ 58, { "Longbow Gold", Rarity::LEGENDARY }},
		{ 59, { "Charge Rifle", Rarity::COMMON }},
		{ 63, { "Charge Rifle Gold", Rarity::LEGENDARY }},
		{ 64, { "Spitfire", Rarity::HEIRLOOM }},
		{ 65, { "R301", Rarity::COMMON }},
		{ 69, { "R301 Gold", Rarity::LEGENDARY }},
		{ 70, { "Eva-8 Auto", Rarity::COMMON }},
		{ 74, { "Eva-8 Auto Gold", Rarity::LEGENDARY }},
		{ 75, { "Peacekeeper", Rarity::COMMON }},
		{ 79, { "Peacekeeper Gold", Rarity::LEGENDARY }},
		{ 80, { "Mozambique", Rarity::COMMON }},
		{ 84, { "Mozambique Gold", Rarity::LEGENDARY }},
		{ 85, { "Wingman", Rarity::COMMON }},
		{ 89, { "Wingman Gold", Rarity::LEGENDARY }},
		{ 90, { "P2020", Rarity::COMMON }},
		{ 94, { "P2020 Gold", Rarity::LEGENDARY }},
		{ 95, { "RE45", Rarity::COMMON }},
		{ 99, { "RE45 Gold", Rarity::LEGENDARY }},
		{ 100, { "Sentinel", Rarity::COMMON }},
		{ 104, { "Sentinel Gold", Rarity::LEGENDARY }},
		{ 105, { "Bocek Bow", Rarity::COMMON }},
		{ 109, { "Bocek Bow Gold", Rarity::LEGENDARY }},
		{ 110, { "30 30 Repeater", Rarity::COMMON }},
		{ 114, { "30 30 Repeater Gold", Rarity::LEGENDARY }},

		{ 115, { "Light Ammo", Rarity::COMMON }},
		{ 116, { "Energy Ammo", Rarity::COMMON }},
		{ 117, { "Shotgun Ammo", Rarity::COMMON }},
		{ 118, { "Heavy Ammo", Rarity::COMMON }},
		{ 119, { "Sniper Ammo", Rarity::COMMON }},
		{ 120, { "Arrow", Rarity::COMMON }},

		{ 121, { "Rampage", Rarity::COMMON }},
		{ 125, { "Rampage Gold", Rarity::LEGENDARY }},

		{ 151, { "Ultimate Accelerant", Rarity::RARE }},
		{ 152, { "Pheonix Kit", Rarity::EPIC }},
		{ 153, { "Med Kit", Rarity::RARE }},
		{ 154, { "Syringe", Rarity::COMMON }},
		{ 155, { "Shield Battery", Rarity::RARE }},
		{ 156, { "Shield Cell", Rarity::COMMON }},

		{ 157, { "Helmet", Rarity::COMMON }},
		{ 158, { "Helmet", Rarity::RARE }},
		{ 159, { "Helmet", Rarity::EPIC }},
		{ 160, { "Helmet", Rarity::LEGENDARY }},

		{ 161, { "Body Armor", Rarity::COMMON }},
		{ 162, { "Body Armor", Rarity::RARE }},
		{ 163, { "Body Armor", Rarity::EPIC }},
		{ 164, { "Body Armor", Rarity::LEGENDARY }},
		{ 165, { "Body Armor", Rarity::HEIRLOOM }},

		{ 166, { "Evo Body Armor", Rarity::COMMON }},
		{ 167, { "Evo Body Armor", Rarity::RARE }},
		{ 168, { "Evo Body Armor", Rarity::EPIC }},
		{ 169, { "Evo Body Armor", Rarity::HEIRLOOM }},

		{ 170, { "Heat Shield", Rarity::HEIRLOOM }},

		{ 171, { "Knockdown Shield", Rarity::COMMON }},
		{ 172, { "Knockdown Shield", Rarity::RARE }},
		{ 173, { "Knockdown Shield", Rarity::EPIC }},
		{ 174, { "Knockdown Shield", Rarity::LEGENDARY }},

		{ 175, { "Backpack", Rarity::COMMON }},
		{ 176, { "Backpack", Rarity::RARE }},
		{ 177, { "Backpack", Rarity::EPIC }},
		{ 178, { "Backpack", Rarity::LEGENDARY }},

		{ 179, { "Thermite Grenade", Rarity::COMMON }},
		{ 180, { "Frag Grenade", Rarity::COMMON }},
		{ 181, { "Arc Star", Rarity::COMMON }},

		{ 182, { "1x HCOG 'Classic'", Rarity::COMMON }},
		{ 183, { "2x HCOG 'Bruiser'", Rarity::RARE }},
		{ 184, { "1x Holo", Rarity::COMMON }},
		{ 185, { "1x-2x Variable Holo", Rarity::RARE }},
		{ 186, { "1x Digital Threat", Rarity::LEGENDARY }},
		{ 187, { "3x HCOG 'Ranger'", Rarity::EPIC }},
		{ 188, { "2x-4x Variable AOG", Rarity::EPIC }},
		{ 189, { "6x Sniper", Rarity::RARE }},
		{ 190, { "4x-8x Variable Sniper", Rarity::EPIC }},
		{ 191, { "4x-10x Digital Sniper Threat", Rarity::LEGENDARY }},

		{ 192, { "Barrel Stabilizer", Rarity::COMMON }},
		{ 193, { "Barrel Stabilizer", Rarity::RARE }},
		{ 194, { "Barrel Stabilizer", Rarity::EPIC }},
		{ 195, { "Barrel Stabilizer", Rarity::LEGENDARY }}, // Unobtainable?

		{ 196, { "Light Magazine", Rarity::COMMON }},
		{ 197, { "Light Magazine", Rarity::RARE }},
		{ 198, { "Light Magazine", Rarity::EPIC }},
		{ 199, { "Light Magazine", Rarity::LEGENDARY }},

		{ 200, { "Heavy Magazine", Rarity::COMMON }},
		{ 201, { "Heavy Magazine", Rarity::RARE }},
		{ 202, { "Heavy Magazine", Rarity::EPIC }},
		{ 203, { "Heavy Magazine", Rarity::LEGENDARY }},

		{ 204, { "Energy Magazine", Rarity::COMMON }},
		{ 205, { "Energy Magazine", Rarity::RARE }},
		{ 206, { "Energy Magazine", Rarity::EPIC }},
		{ 207, { "Energy Magazine", Rarity::LEGENDARY }},

		{ 208, { "Sniper Magazine", Rarity::COMMON }},
		{ 209, { "Sniper Magazine", Rarity::RARE }},
		{ 210, { "Sniper Magazine", Rarity::EPIC }},
		{ 211, { "Sniper Magazine", Rarity::LEGENDARY }},

		{ 212, { "Shotgun Bolt", Rarity::COMMON }},
		{ 213, { "Shotgun Bolt", Rarity::RARE }},
		{ 214, { "Shotgun Bolt", Rarity::EPIC }},

		{ 215, { "Standard Stock", Rarity::COMMON }},
		{ 216, { "Standard Stock", Rarity::RARE }},
		{ 217, { "Standard Stock", Rarity::EPIC }},

		{ 218, { "Sniper Stock", Rarity::COMMON }},
		{ 219, { "Sniper Stock", Rarity::RARE }},
		{ 220, { "Sniper Stock", Rarity::EPIC }},

		{ 221, { "Turbocharger", Rarity::LEGENDARY }},
		{ 222, { "Skull Piercer", Rarity::LEGENDARY }},
		{ 223, { "Hammer Point", Rarity::EPIC }},
		{ 224, { "Anvil Receiver", Rarity::LEGENDARY }},
		{ 225, { "Graffiti Mod", Rarity::COMMON }},
		{ 226, { "Paintball 1", Rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 227, { "Paintball 2", Rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 228, { "Deadeye's Tempo", Rarity::EPIC }},
		{ 229, { "Quickdraw Holster", Rarity::EPIC }}, // Might be 229
		{ 230, { "Shatter Caps", Rarity::EPIC }},
		{ 231, { "Boosted Loader", Rarity::LEGENDARY }},
		{ 232, { "Mobile Respawn Beacon", Rarity::RARE }},
		{ 233, { "Vault Key", Rarity::HEIRLOOM }},
		{ 234, { "Steel Cut", Rarity::HEIRLOOM }}, // Might be overwritten every season/every time there's a new charm.
		{ 235, { "Treasure Pack", Rarity::HEIRLOOM }},
		{ 236, { "Survival Doubler", Rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 237, { "Survival Quadrupler", Rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 238, { "Heat Shield", Rarity::RARE }},
		{ 239, { "Mobile Respawn Beacon 2", Rarity::RARE }},
		{ 240, { "MRVN Arm", Rarity::EPIC }},
		{ 241, { "Vault Key", Rarity::HEIRLOOM }}
	};
}