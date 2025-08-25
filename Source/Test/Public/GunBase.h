// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNodeBase.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponentPool.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "WeaponBase.h"
#include "UObject/ObjectMacros.h"
#include "NiagaraComponent.h"
#include "StopShooting.h"
#include "ObjectPoolComponent.h"
#include "MyCharacter.h"
#include "GunBase.generated.h"

class UAnimMontage; // 前向声明，避免不必要的头文件引用

UCLASS()
class TEST_API AGunBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 播放换弹动画蒙太奇的函数
    UFUNCTION(BlueprintCallable, Category = "GunBase|Animation")
    float PlayReloadMontage();

	// 获取子弹发射的位置
	UFUNCTION(BlueprintPure,BlueprintCallable, Category = "GunBase")
	FVector GetBulletShootLocation();

	// 从对象池中生成子弹，在Shoot方法中被调用
	void SpawnBulletFromPool();

	
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

	// 需要在蓝图中赋值↓
	// Reload Audio Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
	UAudioComponent *ReloadAudioComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase", meta = (AllowPrivateAccess = "true"))
	USceneComponent *MuzzleFromBP;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase")
    UAudioComponent* ShootAudioComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
	UNiagaraSystem *MuzzleFireEffect;

	//换弹动画的蒙太奇资产，需要在蓝图中初始化
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* ReloadMontage;

	// 存放子弹的对象池组件
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GunBase|ObjectPool")
	UObjectPoolComponent *BulletPool;

	// 定义一个定时器句柄，用于全自动武器射击的逻辑
	FTimerHandle ShootTimerHandle; 
	void StopShootTimerHandle(); // 关闭射击定时器的函数

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//上弹用函数
	UFUNCTION(BlueprintCallable, Category = "GunBase")
	void Reload();

	// 换弹蒙太奇上弹结束通知的回调函数
	UFUNCTION(BlueprintCallable, Category = "GunBase")
	void ReloadDone();

	UFUNCTION(BlueprintCallable, Category = "GunBase")
	bool GetIsReloading();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GunBase")
	void Shoot();
	virtual void Shoot_Implementation();

	UFUNCTION(BlueprintCallable, Category = "GunBase")
	void StopShooting();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "GunBase")
	USceneComponent* GetMuzzleFromBP() const{return MuzzleFromBP;}

	// 获取两枪之间的射击间隔时间
	float GetShootDuration();

	virtual void Attack() override;
private:
	

};
