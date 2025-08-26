// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "PaperSprite.h"
#include "WeaponData.h"
#include "WeaponBase.generated.h"

UCLASS()
class TEST_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//要使用的武器数据资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
	UDataTable* WeaponDataTable;

	// 使用武器在数据资产中的名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
	FName WeaponID;


	// 武器自身属性，需要通过DataTable加载
	// UI中显示的图标
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    UPaperSprite *Icon;
    // 重量
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    float Weight;
    // 价格
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    int32 Price;
    
    // 武器类特有属性
    // 武器的蓝图类
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    TSoftClassPtr<AWeaponBase> WeaponBP;
    // 基础伤害值
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    float Damage;
    // 攻击范围
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    float AttackRange;
    // 武器类型
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    EWeaponType WeaponType;
    //各个武器相对于角色武器插槽的偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
	FTransform SocketOffSet;
    // 各个武器的攻击音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
    USoundBase* AttackSound;
    
    // 远程武器特有属性
    // 子弹类别
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GunBase")
    TSoftClassPtr<class ABulletBase> BulletClass;
    // 射速
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    float FireSpeed;
    // 弹夹容量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    int MaxMagazineAmmo;
    // 当前弹药数量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    int CurrentAmmo;
    // 腰射时子弹偏移角度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    float HipFireSpreadYawAngle;
    // 子弹初速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    float BulletSpeed;
    // 是否装备消音器
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    bool bHasSilencer;

private:
	void LoadWeaponData(); //加载DataTable中的数据

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // 装备武器时，调整位置插槽等
    UFUNCTION(BlueprintCallable, Category = "Default")
    virtual void OnEquipped();

    // 每个武器都要拥有攻击方法
	UFUNCTION(BlueprintCallable, Category = "GunBase")
	virtual void Attack();

    // C++ Getters for Weapon Properties
    UPaperSprite* GetIcon() const { return Icon; }
    float GetWeight() const { return Weight; }
    int32 GetPrice() const { return Price; }
    TSoftClassPtr<AWeaponBase> GetWeaponBP() const { return WeaponBP; }
    float GetDamage() const { return Damage; }
    float GetAttackRange() const { return AttackRange; }
    EWeaponType GetWeaponType() const { return WeaponType; }
    const FTransform& GetSocketOffSet() const { return SocketOffSet; }
    float GetFireSpeed() const { return FireSpeed; }
    int32 GetMaxMagazineAmmo() const { return MaxMagazineAmmo; }
    int32 GetCurrentAmmo() const { return CurrentAmmo; }
};
