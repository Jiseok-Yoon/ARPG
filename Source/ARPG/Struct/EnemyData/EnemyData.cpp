#include "EnemyData.h"

FEnemyData::FEnemyData()
{
	CapsuleHalfHeight = 88.0f;
	CapsuleRadius = 34.0f;
	MaxMoveSpeed = 400.0f;
	UseBehaviorTreeAssetPath = FSoftObjectPath(TEXT("/Game/Blueprints/AI/Default/BT_EnemyDefault.BT_EnemyDefault"));
}
