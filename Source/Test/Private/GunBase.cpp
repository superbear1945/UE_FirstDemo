// Fill out your copyright notice in the Description page of Project Settings.

#include "GunBase.h"
#include "NiagaraComponent.h"      
#include "NiagaraFunctionLibrary.h" 
#include "Engine/TimerHandle.h"
#include "Engine/World.h"
#include "BulletBase.h"
#include "MyCharacter.h"
#include "TimerManager.h"

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
	StopShooting(); // 换弹时停止射击
}

bool AGunBase::GetIsReloading()
{
	return IsReloading;
}

void AGunBase::ReloadDone()
{
	CurrentAmmo = MaxAmmo;
	IsReloading = false;
	IsMagazineFull = true;
	IsMagazineEmpty = false;
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

void AGunBase::Attack()
{
	if(IsReloading || IsMagazineEmpty) return; // 换弹或没子弹时不能开枪
	FTimerManager &TimeManager = GetWorld()->GetTimerManager();

	// 获取当前定时器剩余时间，如果大于0说明现在处于两枪中间的间隙，此时不能开枪
	float AutoShootRemainingTime = TimeManager.GetTimerRemaining(ShootTimerHandle);
	if(AutoShootRemainingTime > 0) return;

	Shoot(); // 调用Attack且不处于两枪中间的间隙时立刻开火，随后开始全自动射击的逻辑

	// 通过一个开启循环的定时器来实现全自动射击
	TimeManager.SetTimer(ShootTimerHandle,
						this,
						&AGunBase::Shoot,
						GetShootDuration(),
						true);
}

void AGunBase::Shoot_Implementation()
{
	IsMagazineEmpty = (CurrentAmmo <= 0);
	if(!IsMagazineEmpty && !IsReloading)
	{
		IsMagazineFull = false;
		ShootAudioComponent->Play();
		CurrentAmmo--;
		SpawnBulletFromPool();

		// 生成枪口火焰在蓝图中实现
		
	}
	else 
	{
		//防止在换弹中途还能换弹
		if(IsReloading) return;
		StopShooting();
		Reload();
	}
}

void AGunBase::SpawnBulletFromPool()
{
	if(BulletPool == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletPool is not set in %s!"), *GetName());
		return;
	}

	auto *BulletActor = BulletPool->GetActorFromPool();
	
	// 让子弹获取到对象池组件，以便它在命中后能归还自己
	ABulletBase *Bullet = Cast<ABulletBase>(BulletActor);
	Bullet->SetObjectPoolComponent(BulletPool);

	// 设置子弹旋转和位置
	BulletActor->SetActorLocation(MuzzleFromBP->GetComponentLocation());
	
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	// 实现不瞄准时有扩散，瞄准才是百分比准的效果
	if(Cast<AMyCharacter>(PlayerPawn)->GetIsAiming())
	{
		FRotator PlayerRotation = PlayerPawn->GetActorRotation();
		FRotator BulletRotation = BulletActor->GetActorRotation();
		BulletRotation.Yaw = PlayerRotation.Yaw;
		BulletActor->SetActorRotation(BulletRotation);
	}
	else 
	{
		FRotator PlayerRotation = PlayerPawn->GetActorRotation();
		FRotator BulletRotation = BulletActor->GetActorRotation();
		float RandomSpread = FMath::RandRange(-HipFireSpreadYawAngle, HipFireSpreadYawAngle);
		BulletRotation.Yaw = PlayerRotation.Yaw + RandomSpread;
		BulletActor->SetActorRotation(BulletRotation);
	}

	// 设置子弹速度
	FVector Direction = BulletActor->GetActorForwardVector();
	Direction *= BulletSpeed;
	Bullet->GetProjectileMovementComponent()->Velocity = Direction;
}

void AGunBase::StopShooting()
{
	ShootAudioComponent->Stop(); // 停止枪声
	
	// 在定时器计时结束后清空计时器
	if(!ShootTimerHandle.IsValid()) return;

	FTimerManager &TimeManager = GetWorld()->GetTimerManager();
	TimeManager.SetTimer
	(
		ShootTimerHandle, 
		this, 
		&AGunBase::StopShootTimerHandle, 
		TimeManager.GetTimerRemaining(ShootTimerHandle), 
		false
	);
}

void AGunBase::StopShootTimerHandle()
{
	FTimerManager &TimeManager = GetWorld()->GetTimerManager();
	TimeManager.ClearTimer(ShootTimerHandle);
}

float AGunBase::GetShootDuration()
{
	return 60 / FireSpeed;
}


