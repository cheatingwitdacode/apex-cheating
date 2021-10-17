#pragma once
#include "Util.h"
vec3 BulletPredict(vec3 start, vec3 targetPos, vec3 targetVel, float bulletSpeed, float bulletGravity)
{
	vec3 newPos = targetPos;
	float distance = Util::GetDistance(targetPos);
	float VerticalTime = Util::GetDistance(targetPos) / bulletSpeed;
	newPos.y += (bulletGravity * 0.5f) * (VerticalTime * VerticalTime);
	float HorizontalTime = distance / bulletSpeed;
	newPos += (targetVel * HorizontalTime);
	return newPos;
}
vec3 playerPrediction(vec3 localPos, vec3 targetPos, vec3 targetVel, float bulletSpeed, float bulletGrav)
{
	float distance = Util::GetDistance(targetPos);
	float timeToTarget = distance / bulletSpeed; // In seconds
	vec3 bulletGravChange = { 0, bulletGrav * timeToTarget, 0 };
	vec3 movementChange = targetVel * timeToTarget;
	vec3 newPos = targetPos - movementChange;// +bulletGravChange;
	return newPos;
}