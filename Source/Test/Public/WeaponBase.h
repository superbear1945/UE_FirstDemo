// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "PaperSprite.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None		UMETA(DisplayName = "None"),
	Melee		UMETA(DisplayName = "Melee"),
	AutoRanged	UMETA(DisplayName = "Ranged"),
	SemiRanged	UMETA(DisplayName = "SemiRanged"),
};

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

	virtual void PostInitializeComponents() override;

	//要使用的武器数据资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
	UDataTable* WeaponDataTable;

	// 使用武器在数据资产中的名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
	FName WeaponID;


	// 武器自身属性，需要通过DataTable加载
	// UI中显示的图标
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    TSoftObjectPtr<UPaperSprite> Icon;
    
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

    
    // 远程武器特有属性
    // 射速
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    float FireSpeed;

    // 弹夹容量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    int MagazineCapacity;

    // 当前弹药数量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunBase")
    int CurrentAmmo;

	

private:
	void LoadWeaponData(); //加载DataTable中的数据

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
