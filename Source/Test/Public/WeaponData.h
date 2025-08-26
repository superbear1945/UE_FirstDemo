// WeaponData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PaperSprite.h"
#include "ShowFlags.h"
#include "BulletBase.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None		UMETA(DisplayName = "None"),
	Melee		UMETA(DisplayName = "Melee"),
	AutoRanged	UMETA(DisplayName = "Ranged"),
	SemiRanged	UMETA(DisplayName = "SemiRanged"),
};

// 使用USTRUCT宏，使其能被UE的反射系统识别，并设为BlueprintType以便蓝图使用
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase // 必须继承自FTableRowBase
{
    GENERATED_BODY()

public:
    // 物品ID名称
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    FName ItemID;

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
    TSoftClassPtr<class AWeaponBase> WeaponClass;

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
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangedWeapon")
    TSoftClassPtr<class ABulletBase> BulletClass;

    // 射速
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RangedWeapon")
    float FireSpeed;

    // 弹夹容量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RangedWeapon")
    int MaxMagazineAmmo;

    // 当前弹药数量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RangedWeapon")
    int CurrentAmmo;

    // 腰射时子弹偏移角度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RangedWeapon")
    float HipFireSpreadYawAngle;

    // 子弹初速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RangedWeapon")
    float BulletSpeed;

    
};
