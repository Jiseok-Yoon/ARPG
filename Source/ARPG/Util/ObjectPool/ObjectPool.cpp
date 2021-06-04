// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"

IObjectPoolable* UObjectPool::GetRecycledObject() const
{
	// 요소의 개수가 0 이라면 nullptr 리턴
	if (PoolObjects.Num() == 0) return nullptr;

	// 추가된 객체중 재사용 가능한 객체를 찾습니다.
	for (auto poolObject : PoolObjects)
	{
		if (poolObject->GetCanRecyclable())
		{
			poolObject->SetCanRecyclable(false);
			poolObject->OnRecycleStart();

			return poolObject;
		}
	}

	return nullptr;
}
