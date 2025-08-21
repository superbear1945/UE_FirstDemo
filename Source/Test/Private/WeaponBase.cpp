// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "WeaponData.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LoadWeaponData(); //在组件加载完成后，加载Weapon DataTable中的数据
}

void AWeaponBase::LoadWeaponData()
{
    if (WeaponDataTable && !WeaponID.IsNone())
    {
        static const FString ContextString(TEXT("Weapon Data"));
        FWeaponData* DataRow = WeaponDataTable->FindRow<FWeaponData>(WeaponID, ContextString, true);

        if (DataRow)
        {
            Icon = DataRow->Icon;
            Weight = DataRow->Weight;
            Price = DataRow->Price;
            WeaponBP = DataRow->WeaponBP;
            Damage = DataRow->Damage;
            AttackRange = DataRow->AttackRange;
            WeaponType = DataRow->WeaponType;
            SocketOffSet = DataRow->SocketOffSet;
            FireSpeed = DataRow->FireSpeed;
            MagazineCapacity = DataRow->MagazineCapacity;
            CurrentAmmo = DataRow->CurrentAmmo;
        }
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: WeaponDataTable or WeaponID is not set in AWeaponBase!"), *GetName());
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

