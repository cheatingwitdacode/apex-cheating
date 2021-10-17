#pragma once
#include "globals.h"
#include "defs.h"
#include <string>
#include <vector>

namespace Player
{
	std::vector<uintptr_t> GetPlayers();
	bool IsPlayer(uintptr_t player);
	bool IsDummy(uintptr_t ent);
	std::string GetSignifier(uintptr_t ent);
	bool IsAlive(uintptr_t player);
	bool IsDead(uintptr_t player);
	bool IsValidPlayer(uintptr_t player);
	bool IsValidEnemy(uintptr_t player);
	void PredictPos(uintptr_t target, vec3* bonePos);
	bool IsVisible(uintptr_t ent, int index);
	std::string GetName(uintptr_t player);
}