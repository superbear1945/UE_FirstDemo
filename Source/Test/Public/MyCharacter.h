// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/UnrealType.h"
#include "WeaponBase.h"
#include "MyCharacter.generated.h"

// 切换武器时使用的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchWeaponSignature, AWeaponBase*, NewWeapon);
// 换弹结束的委托
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadFinishedSignature, AWeaponBase*, ReloadedWeapon);

UCLASS()
class TEST_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSwitchWeaponSignature OnSwitchWeapon; // 切换武器时触发的委托
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 指向当前装备的武器实例。UPROPERTY()宏可以防止它被垃圾回收。
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	bool IsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	float WalkSpeed = 200;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	float JogSpeed = 400;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	float SprintSpeed = 660;

	/** 当异步加载的武器类完成后被调用的函数 */
	void OnWeaponLoadCompleted(TSoftClassPtr<AWeaponBase> LoadedWeaponClassPtr);

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

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopSprint();

	bool GetIsAiming() const { return IsAiming; }

private:
    UCharacterMovementComponent *CharacterMovementComponent;
};
