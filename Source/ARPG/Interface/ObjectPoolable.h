#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolable.generated.h"

#ifndef OBJECT_POOLABLE_ID
#define OBJECT_POOLABLE_ID
#define NOT_USE_INSTANCE_ID		-1
#endif

#ifndef IMPLEMENT_OBJECT_POOLABLE
#define IMPLEMENT_OBJECT_POOLABLE	\
private: int32 poolableObjID; bool bCanRecyclable;	\
public:	\
	FORCEINLINE virtual int32 GetID() const override { return poolableObjID; }	\
	FORCEINLINE virtual bool GetCanRecyclable() const override { return bCanRecyclable; } \
	FORCEINLINE virtual void SetCanRecyclable(bool canRecyclable) override { bCanRecyclable = canRecyclable; }

#define INITIALIZE_OBJECT_POOLABLE(id)	poolableObjID = id; bCanRecyclable = false

#define DEF_SETID public : FORCEINLINE void SetID(int32 newId) { poolableObjID = newId; }

#endif

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_API IObjectPoolable
{
	GENERATED_BODY()

public:
	// 객체를 구분할 일이 있을 경우 사용됩니다.
	virtual int32 GetID() const PURE_VIRTUAL(IObjectPoolable::GetID, return NOT_USE_INSTANCE_ID;);

	// 재사용 가능 여부를 나타내는 변수에 대한 접근자 / 설정자
	virtual bool GetCanRecyclable() const PURE_VIRTUAL(IObjectPoolable::GetCanRecyclable, return true;);
	virtual void SetCanRecyclable(bool canRecyclable) PURE_VIRTUAL(IObjectPoolable::SetCanRecyclable, );

	// 객체가 재사용될 때 호출되는 메서드
	virtual void OnRecycleStart() PURE_VIRTUAL(IObjectPoolable::OnRecycleStart, );


};
