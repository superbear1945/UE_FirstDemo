// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Engine/DataTable.h"
#include "WeaponBase.h"
#include "WeaponData.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}



// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}



void AMyCharacter::SwitchWeapon(UDataTable *WeaponDataTable, FName WeaponID)
{
    if (WeaponDataTable == nullptr || WeaponID.IsNone()) return;

    // 如果当前有武器，先销毁它
    if (CurrentWeapon)
    {
        CurrentWeapon->Destroy();
        CurrentWeapon = nullptr;
    }

    static const FString ContextString(TEXT("Player Switch Weapon"));
    
    // 1. 查找武器数据
    FWeaponData* DataRow = WeaponDataTable->FindRow<FWeaponData>(WeaponID, ContextString, true);
    if(!DataRow) return;

    // 2. 从数据中获取武器类
    TSoftClassPtr<AWeaponBase> WeaponClassPtr = DataRow->WeaponClass;
    if (WeaponClassPtr.IsNull()) return;

    // 3. 加载类并生成武器实例 (这里使用同步加载作为示例)
    if (UClass* WeaponClassToSpawn = WeaponClassPtr.LoadSynchronous())
    {
        CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClassToSpawn);
    }
}

