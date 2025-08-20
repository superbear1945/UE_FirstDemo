// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None		UMETA(DisplayName = "None"),
	Melee		UMETA(DisplayName = "Melee"),
	Ranged		UMETA(DisplayName = "Ranged"),
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponBase")
	float Damage = 1.0f;

	//各个武器相对于角色武器插槽的偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponBase")
	FTransform SocketOffSet;

	//武器种类，自身是远程、近战还是不算武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponBase")
	EWeaponType MyWeaponType;

	
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
