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



// Called every frame
void AAK47::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

