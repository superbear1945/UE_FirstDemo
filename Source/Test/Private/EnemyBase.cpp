// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::AimToPlayer()
{
	FVector playerLocation = playerPawn->GetActorLocation();
	FVector selfLocation = this->GetActorLocation();
	FVector direction = playerLocation - selfLocation;
	direction.Z = 0;
	direction.Normalize();
	FRotator newRotation = direction.Rotation();
	// 使用插值实现平滑旋转
	FRotator currentRotation = this->GetActorRotation();
	FRotator smoothRotation = FMath::RInterpTo(currentRotation, newRotation, GetWorld()->GetDeltaSeconds(), rotateSpeed);
	this->SetActorRotation(smoothRotation);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

