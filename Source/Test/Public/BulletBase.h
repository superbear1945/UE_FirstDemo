// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolComponent.h"
#include "Poolable.h"
#include "UObject/ObjectMacros.h"
#include "BulletBase.generated.h"

class UObjectPoolComponent;

UCLASS()
class TEST_API ABulletBase : public AActor, public IPoolable
{
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category="Bullet")
	void HitZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//用于让子弹获取到对象池组件，需要在生成子弹后立马为该变量赋值
	UPROPERTY(BlueprintReadWrite, Category = "Object Pool")
	UObjectPoolComponent* ObjectPoolComponent;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPoolBegin_Implementation() override;
	virtual void OnPoolEnd_Implementation() override;
};
