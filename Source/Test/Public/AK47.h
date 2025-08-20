// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector2DHalf.h"
#include "UObject/ObjectMacros.h"
#include "GunBase.h"
#include "AK47.generated.h"


UCLASS()
class TEST_API AAK47 : public AGunBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAK47();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AK47")
	USceneComponent *MuzzleFromBP;

	UFUNCTION(BlueprintPure,BlueprintCallable, Category = "AK47")
	FVector GetBulletShootLocation();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	

};
