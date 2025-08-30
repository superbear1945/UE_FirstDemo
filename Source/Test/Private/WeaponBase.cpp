// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "UObject/SoftObjectPath.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GunBase.h"
#include "WeaponData.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CreateComponent();
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	StartLoadWeaponData(); //在Event Begin时，加载Weapon DataTable中的数据
}

void AWeaponBase::CreateComponent()
{
    // 创建根场景组件
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    // 创建武器网格组件
    WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
    WeaponMeshComponent->SetupAttachment(RootSceneComponent);

    // 创建攻击音频组件
    AttackAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackAudioComponent"));
    AttackAudioComponent->SetupAttachment(RootSceneComponent);
}

void AWeaponBase::StartLoadWeaponData()
{
    UE_LOG(LogTemp, Warning, TEXT("AWeapon::StartLoadWeaponData: %f"), GetWorld()->GetTimeSeconds());

    if (WeaponDataTable != nullptr && !WeaponID.IsNone())
    {
        static const FString ContextString(TEXT("Weapon Data"));
        
        FWeaponData* DataRow = WeaponDataTable->FindRow<FWeaponData>(WeaponID, ContextString, true);

        if (DataRow)    
        {
            // 将资产软指针路径添加到AssetPaths中，后面开启异步加载资产时使用
            TArray<FSoftObjectPath> AssetPaths;
            AssetPaths.Add(DataRow->Icon.ToSoftObjectPath());
            AssetPaths.Add(DataRow->AttackSound.ToSoftObjectPath());
            AssetPaths.Add(DataRow->BulletClass.ToSoftObjectPath());
            AssetPaths.Add(DataRow->WeaponMesh.ToSoftObjectPath());
            
            // 读取非指针内容
            Weight = DataRow->Weight;
            Price = DataRow->Price;
            Damage = DataRow->Damage;
            AttackRange = DataRow->AttackRange;
            WeaponType = DataRow->WeaponType;
            SocketOffSet = DataRow->SocketOffSet;
            FireSpeed = DataRow->FireSpeed;
            MaxMagazineAmmo = DataRow->MaxMagazineAmmo;
            CurrentAmmo = DataRow->CurrentAmmo;
            HipFireSpreadYawAngle = DataRow->HipFireSpreadYawAngle;
            BulletSpeed = DataRow->BulletSpeed;
            bHasSilencer = DataRow->bHasSilencer;


            
            // 发起异步加载AssetPaths的请求，回调函数为OnWeaponDataLoaded
            FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
            StreamableManager.RequestAsyncLoad(
                AssetPaths, 
                FStreamableDelegate::CreateUObject(
                    this, 
                    &AWeaponBase::OnWeaponDataLoaded
                    )
                );
        }
    }
	else if(!WeaponDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WeaponDataTable is not set in AWeaponBase!"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WeaponID is not set in AWeaponBase!"), *GetName());
	}
}

void AWeaponBase::OnWeaponDataLoaded()
{
    UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnWeaponDataLoaded: %f"), GetWorld()->GetTimeSeconds());

    // 再次获取数据行
    if (WeaponDataTable == nullptr || WeaponID.IsNone()) return;
    static const FString ContextString(TEXT("Weapon Data"));
    FWeaponData* DataRow = WeaponDataTable->FindRow<FWeaponData>(WeaponID, ContextString, true);
    if (!DataRow) return;


    // 获取加载完成的资产的硬指针并赋值
    LoadedIcon = DataRow->Icon.Get(); 
    LoadedAttackSound = DataRow->AttackSound.Get(); 
    LoadedWeaponMesh = DataRow->WeaponMesh.Get(); 
    LoadedBulletClass = DataRow->BulletClass.Get(); 

    // 使用已加载的资产为各个组件赋值
    InitComponent();

    // 广播资产加载完成事件
    if (OnWeaponAssetsLoaded.IsBound())
    {
        OnWeaponAssetsLoaded.Broadcast();
    }
}

void AWeaponBase::InitComponent()
{

    // 设置武器静态网格体
    if(WeaponMeshComponent != nullptr)
    {
        WeaponMeshComponent->SetStaticMesh(LoadedWeaponMesh);
    }
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Attack()
{
    // Each weapon will implement its own attack logic
}
