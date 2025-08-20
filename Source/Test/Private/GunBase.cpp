// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "Engine/World.h"

// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	//防止开局时先播放一次换弹音效
	if(ReloadAudioComponent)
		ReloadAudioComponent->bAutoActivate = false;

}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGunBase::Reload()
{
	IsReloading = true;
	
	if (ReloadAudioComponent)
	{
		ReloadAudioComponent->Play();
	}
	
	ReloadTime = PlayReloadMontage();
}

bool AGunBase::GetIsReloading()
{
	return IsReloading;
}

void AGunBase::StopShooting_Implementation()
{
	// Implement logic in Blueprint
}

float AGunBase::PlayReloadMontage()
{
	// 1. 确保我们指定的蒙太奇是有效的
    if (!ReloadMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReloadMontage is not set in the Character Blueprint!"));
        return 0.0f;
    }

    // 2. 获取动画实例
	// 获取玩家Pawn
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is null!"));
		return 0.0f;
	}
	
	// 获取玩家的Mesh组件
	USkeletalMeshComponent* PlayerMesh = PlayerPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (!PlayerMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerMesh is null!"));
		return 0.0f;
	}

	// 获取动画实例
	UAnimInstance* PlayerAnimInstance = PlayerMesh->GetAnimInstance();

    if (!PlayerAnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null!"));
        return 0.0f;
    }

    // 3. 播放蒙太奇
    // Montage_Play返回的是动画的长度，你可以用它来设置定时器等
    return PlayerAnimInstance->Montage_Play(ReloadMontage, 1.0f); // 第二个参数是播放速率
}


