// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"
#include "Animation/AnimCurveTypes.h"
#include "GameFramework/Actor.h"
#include "Math/MathFwd.h"
#include "Templates/SubclassOf.h"
#include "BulletBase.h"

// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UObjectPoolComponent::InitializePool()
{
	// Ensure we have a valid world and class
	if(GetWorld() == nullptr || !PooledObjectClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPool: Invalid World or PooledObjectClass"));
		return; 
	}
	
	// Check if the PooledObject implement the interface
	if(!PooledObjectClass->ImplementsInterface(UPoolable::StaticClass())) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPool: PooledObjectClass does not implement IPoolable interface"));
		return;
	}

	if(PoolSize <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPool: PoolSize must be greater than 0"));
		return;
	}

	ObjectPool.Reserve(PoolSize);
	AvailableIndices.Reserve(PoolSize);

	for(int32 i = 0; i < PoolSize; i++)
	{
		// 准备生成参数，这是一个好习惯
    	FActorSpawnParameters SpawnParams;
    	SpawnParams.Owner = GetOwner(); // 将对象池组件的所有者设为生成物的所有者
    	SpawnParams.Instigator = GetOwner()->GetInstigator();
    	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 总是生成，即使位置重叠

		AActor* NewActor = GetWorld()->SpawnActor(
            PooledObjectClass,          // 第一个参数：要生成的 UClass
            &FTransform::Identity,      // 第二个参数：初始变换（位置、旋转、缩放）
            SpawnParams                 // 第三个参数：其他生成选项
        );

		if(!NewActor)
			UE_LOG(LogTemp, Error, TEXT("ObjectPool: Failed to spawn actor at index %d"), i);

		ObjectPool.Add(NewActor);
		AvailableIndices.Add(i);
		IPoolable::Execute_OnPoolEnd(NewActor);
	}	
	
}

AActor* UObjectPoolComponent::GetActorFromPool()
{
	// If the pool is not initialized or empty, return nullptr
	if(ObjectPool.Num() == 0 || AvailableIndices.Num() == 0) 
	{
		return nullptr;
	}
	
	int32 Index = AvailableIndices.Pop();
	AActor* RetrievedActor = ObjectPool[Index];
	IPoolable::Execute_OnPoolBegin(RetrievedActor);
	return RetrievedActor;
}

void UObjectPoolComponent::ReturnActorToPool(AActor* ReturnedActor)
{
	// Don't Return null or not in pool
	if(!ReturnedActor || !ObjectPool.Contains(ReturnedActor)) return;

	int32 Index = ObjectPool.Find(ReturnedActor);

	// Check if Actor was returned before
	if(AvailableIndices.Contains(Index)) return;

	AvailableIndices.Add(Index);
	IPoolable::Execute_OnPoolEnd(ReturnedActor);
}
	


void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

