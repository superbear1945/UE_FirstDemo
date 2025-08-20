// WeaponData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PaperSprite.h"
#include "ShowFlags.h"
#include "WeaponData.generated.h"

// 使用USTRUCT宏，使其能被UE的反射系统识别，并设为BlueprintType以便蓝图使用
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase // 必须继承自FTableRowBase
{
    GENERATED_BODY()

public:
    // 物品的显示名称
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    FText ItemName;

    // UI中显示的图标
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    TSoftObjectPtr<UPaperSprite> Icon;

    // 武器的静态网格体
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    TSoftObjectPtr<UStaticMesh> WeaponMesh;
    
    // 重量
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    float Weight;

    // 价格
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    int32 Price;

    // 基础伤害值
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    float Damage;

    // 攻击范围
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    float AttackRange;

    // 可以添加更多你需要的属性...
};