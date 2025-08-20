// Fill out your copyright notice in the Description page of Project Settings.


#include "AK47.h"
#include "Engine/Engine.h"
#include "Logging/LogMacros.h"
#include "Math/MathFwd.h"

// Sets default values
AAK47::AAK47()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAK47::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAK47::GetBulletShootLocation()
{
	if(MuzzleFromBP == nullptr){return FVector::ZeroVector;}
	if(!GetWorld()->GetFirstPlayerController()) { return FVector::ZeroVector; } //检测是否存在玩家
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector MuzzleLocation = MuzzleFromBP->GetComponentLocation();
	FVector PlayerForward = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector();
	FVector GunDirection = MuzzleLocation - PlayerLocation; //玩家指向枪口的方向
	return (GunDirection.ProjectOnTo(PlayerForward) + PlayerLocation);
}

// Called every frame
void AAK47::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

