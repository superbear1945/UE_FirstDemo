// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Poolable.h"
#include "UObject/ObjectMacros.h"
#include "EnemyBase.generated.h"


UCLASS()
class TEST_API AEnemyBase : public ACharacter, public IPoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float AttackDamage = 7.f;

	APawn *playerPawn;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float rotateSpeed = 5.f;
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void AimToPlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
