// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Poolable.h"
#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"
#include "ObjectPoolComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UObjectPoolComponent : public UActorComponent
{

public:	
	GENERATED_BODY()

	// Sets default values for this component's properties
	UObjectPoolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void InitializePool();

	// Objects in the pool
	TArray<AActor*> ObjectPool;

	// 可用的（非活动）对象索引
    TArray<int32> AvailableIndices;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The class of the objects to pool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	TSubclassOf<AActor> PooledObjectClass;

	// Size of the object pool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	int32 PoolSize;

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetActorFromPool();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ReturnActorToPool(AActor* Actor);

};
