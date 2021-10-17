#pragma once
#include <string>
#include <ctime>

class DT_BaseEntity
{
	/* 0x000 */ int HighlightSettings; // movetype/movecollide/predictable_id/HighlightSettings
	/* 0x01c */ int moveparent;
	/* 0x024 */ int m_parentAttachmentIndex;
	/* 0x044 */ int m_fEffects;
	/* 0x048 */ int m_usableType;
	/* 0x04c */ int m_cellX;
	/* 0x050 */ int m_cellY;
	/* 0x050 */ int m_clrRender;	
	/* 0x054 */ int m_cellZ;
	/* 0x054 */ int m_clIntensity;
	/* 0x058 */ m_localOrigin : Vector;
	/* 0x064 */ int m_nModelIndex;
	/* 0x0 */ int m_bossPlayer;
	/* 0x000 */ int m_shieldHealth;
	/* 0x000 */ int m_shieldHealthMax;
	/* 0x000 */ int m_networkedFlags;
	/* 0x000 */ int m_visibilityFlags;
	/* 0x000 */ int m_iTeamNum;
	/* 0x000 */ int m_teamMemberIndex;
	/* 0x000 */ int m_squadID;
	/* 0x000 */ int m_grade;
	/* 0x000 */ int m_ignorePredictedTriggerFlags;
	/* 0x000 */ int m_passThroughFlags;
	/* 0x000 */ int m_passThroughThickness;
	/* 0x000 */ float m_passThroughDirection;
	/* 0x000 */ int m_bIsSoundCodeControllerValueSet;
	/* 0x000 */ float m_flSoundCodeControllerValue;
	/* 0x000 */ m_localAngles : Vector;
	/* 0x000 */ int m_hOwnerEntity;
	/* 0x000 */ int m_bRenderWithViewModels;
	/* 0x000 */ int m_nRenderFX;
	/* 0x000 */ int m_nRenderMode;
	/* 0x000 */ m_Collision : DT_CollisionProperty;
	/* 0x000 */ int m_CollisionGroup;
	/* 0x000 */ int m_contents;
	/* 0x000 */ int m_collideWithOwner;
	/* 0x000 */ std::string m_iSignifierName;
	/* 0x000 */ std::string m_iName;
	/* 0x000 */ int m_scriptNameIndex;
	/* 0x000 */ int m_instanceNameIndex;
	/* 0x000 */ std::string m_holdUsePrompt;
	/* 0x000 */ std::string m_pressUsePrompt;
	/* 0x000 */ int m_phaseShiftFlags;
	/* 0x000 */ int m_baseTakeDamage;
	/* 0x000 */ int m_invulnerableToDamageCount;
	/* 0x000 */ std::time_t m_attachmentLerpStartTime;
	/* 0x000 */ std::time_t m_attachmentLerpEndTime;
	/* 0x000 */ m_attachmentLerpStartOrigin : Vector;
	/* 0x000 */ m_attachmentLerpStartAngles : Vector;
	/* 0x000 */ int m_parentAttachmentModel;
	/* 0x000 */ float m_fadeDist;
	/* 0x000 */ int m_dissolveEffectEntityHandle;
	/* 0x000 */ int m_usablePriority;
	/* 0x000 */ float m_usableDistanceOverride;
	/* 0x000 */ float m_usableFOV;
	/* 0x000 */ float m_usePromptSize;
	/* 0x000 */ m_spottedByTeams : DataTable;
	/* 0x000 */ int m_firstChildEntityLink;
	/* 0x000 */ int m_firstParentEntityLink;
	/* 0x000 */ m_realmsBitMask : BitMask;
};