#pragma once

#define OFFSET_ENTITYLIST			0x1931248  	// cl_entitylist
#define OFFSET_LOCAL_ENT			0x1ce0b68  	// LocalPlayer
#define OFFSET_NAMELIST				0xb740f00  	// NameList
#define OFFSET_RENDER				0x7503a60  	// ViewRender
#define OFFSET_MATRIX				0x1b3bd0	// ViewMatrix
#define OFFSET_INPUT				0x01cca130	// .?AVCInput@@
#define OFFSET_DISABLE_BLOOM		0x01cd7610 	// mat_disable_bloom
#define OFFSET_THIRD_PERSON			0x01917610 	// thirdperson_override

#define OFFSET_NAME_INDEX			0x38
#define OFFSET_SHIELD				0x0170		// m_shieldHealth	
#define OFFSET_MAX_SHIELD			0x0174		// m_shieldHealthMax
#define OFFSET_HEALTH				0x0438		// m_iHealth
#define OFFSET_TEAM					0x0448		// m_iTeamNum
#define OFFSET_NAME					0x0589		// m_iName
#define OFFSET_LIFE_STATE			0x0798		// m_lifeState
#define OFFSET_VISIBLE_TIME			0x1ad4
#define OFFSET_BLEED_OUT_STATE		0x26e8		// m_bleedoutState
#define OFFSET_OBSERVING_TARGET		0x33b8		// m_hObserverTarget
#define OFFSET_ZOOM_FOV             0x1700 + 0xb8 //m_playerData + m_curZoomFOV

#define OFFSET_BULLET_SPEED			0x1ED0
#define OFFSET_BULLET_GRAVITY		(OFFSET_BULLET_SPEED + 0x8)

#define OFFSET_CURRENT_WEAPON		0x1a6c		// m_latestPrimaryWeapons
#define OFFSET_ORIGIN				0x014C		// m_vecAbsOrigin
#define OFFSET_BONES				0x0F38		// m_bConstrainBetweenEndpoints || m_nForceBone + 0x48
#define OFFSET_AIMPUNCH				0x2450		// m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
#define OFFSET_CAMERAPOS			0x1ef0
#define OFFSET_VIEWANGLES			0x2538		// ammoPoolCapacity - 0x14
#define OFFSET_BREATH_ANGLES		(OFFSET_VIEWANGLES - 0x10)
#define OFFSET_ARMOR_TYPE			0x44a4		// DT_Player - m_armorType

#define OFFSET_SCRIPT_ID			0x1698		// m_customScriptInt
#define OFFSET_WEAPON_READY_TIME	0x1698		// m_nextReadyTime

#define OFFSET_SIGNIFIER			0x0580		// m_iSignifierName
#define OFFSET_ABS_VELOCITY			0x0140		// C_Player : m_vecAbsVelocity

// DT_HighlightSettings
#define OFFSET_GLOW_DISTANCE		0x3b4
#define OFFSET_GLOW_COLOR			0x1d0 // m_highlightParams + 0x18
#define OFFSET_GLOW_MODE			0x2c4 // m_highlightFunctionBits + 0x4
#define OFFSET_GLOW_ENABLE			0x3c8
#define OFFSET_GLOW_THROUGH_WALLS	0x3d0