// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "PaperSprite.h"
#include "WeaponData.h"
#include "WeaponBase.generated.h"

// 攻击事件分发器
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackSignature, AWeaponBase*, NewWeapon);

// 【新增】当武器子资源加载完成时调用的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponAssetsLoadedSignature);


UCLASS()
class TEST_API AWeaponBase : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AWeaponBase();

    // 攻击时调用的委托
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAttackSignature OnAttack;

    // 【新增】武器子资源加载完成时广播的委托
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWeaponAssetsLoadedSignature OnWeaponAssetsLoaded;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // 初始的组件↓
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
    USceneComponent *RootSceneComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
    UStaticMeshComponent *WeaponMeshComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
    UAudioComponent *AttackAudioComponent;


    //要使用的武器数据资产
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
    UDataTable* WeaponDataTable;
    // 使用武器在数据资产中的名字
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponBase")
    FName WeaponID;


    // 武器自身属性，从DataTable加载的软引用
    // UI中显示的图标
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
    TSoftObjectPtr<UPaperSprite> Icon; // 【修改】改为软指针
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
    // 武器的静态网格体
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
    TSoftObjectPtr<UStaticMesh> WeaponMesh;
    // 各个武器的攻击音效
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
    TSoftObjectPtr<USoundBase> AttackSound;
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


    // 用于存储异步加载完成后的硬指针，并暴露给蓝图
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Loaded Assets")
    UPaperSprite* LoadedIcon;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Loaded Assets")
    UStaticMesh* LoadedWeaponMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Loaded Assets")
    USoundBase* LoadedAttackSound;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Loaded Assets")
    UClass* LoadedBulletClass;

private:
    // 【修改】加载逻辑改为异步模式
    void StartLoadWeaponData();
    void OnWeaponDataLoaded();

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
    // 【修改】Getters现在返回加载好的硬指针。在加载完成前，它们会返回nullptr。
    UPaperSprite* GetIcon() const { return LoadedIcon; }
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
