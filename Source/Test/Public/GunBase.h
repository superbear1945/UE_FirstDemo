// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNodeBase.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.h"
#include "UObject/ObjectMacros.h"
#include "StopShooting.h"
#include "GunBase.generated.h"

class UAnimMontage; // 前向声明，避免不必要的头文件引用

UCLASS()
class TEST_API AGunBase : public AWeaponBase, public IStopShooting
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	int MaxAmmo = 30;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	int MagazineAmmo = 30; //一个弹夹的子弹数量

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	bool IsReloading = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	bool IsMagazineEmpty = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	bool IsMagazineFull = true;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	float ReloadTime = 2.0f; // Reload time in seconds

	// Reload Audio Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	UAudioComponent *ReloadAudioComponent;

	/** 播放换弹动画蒙太奇的函数 */
    UFUNCTION(BlueprintCallable, Category = "Animation")
    float PlayReloadMontage();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//上弹用函数
	UFUNCTION(BlueprintCallable, Category = "GunBase")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "GunBase")
	bool GetIsReloading();

	virtual void StopShooting_Implementation() override;

private:
	//换弹动画的蒙太奇，需要在蓝图中初始化
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* ReloadMontage;

};
