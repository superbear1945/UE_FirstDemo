// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UObject/UnrealType.h"
#include "WeaponBase.h"
#include "MyCharacter.generated.h"

UCLASS()
class TEST_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 指向当前装备的武器实例。UPROPERTY()宏可以防止它被垃圾回收。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	AWeaponBase* CurrentWeapon;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 按下攻击键后调用的函数
	UFUNCTION(BlueprintCallable, Category = "Player")
	void Attack();
	
	UFUNCTION(BlueprintCallable, Category = "Player")
	void SwitchWeapon(UDataTable *WeaponDataTable, FName WeaponID);

};
