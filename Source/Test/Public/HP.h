// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HP.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TEST_API UHP : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHP();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float MaxHP;
};
