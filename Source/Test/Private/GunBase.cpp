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
	CreateComponent();
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


void AGunBase::CreateComponent()
{
	Super::CreateComponent();

	// 创建换弹音频组件
	ReloadAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ReloadAudio"));
	ReloadAudioComponent->SetupAttachment(RootSceneComponent);

	// 创建代表枪口位置的场景组件
	MuzzleSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	MuzzleSceneComponent->SetupAttachment(WeaponMeshComponent);

	// 创建管理子弹的对象池组件
	BulletPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("BulletPool"));
}

void AGunBase::InitComponent()
{
	Super::InitComponent();

	

	if(AttackAudioComponent != nullptr && LoadedAttackSound != nullptr)
	{
		AttackAudioComponent->SetSound(LoadedAttackSound);
	}

	// 初始化BulletPool的大小为弹匣总容量大小
	if(BulletPool != nullptr)
	{
		BulletPool->PoolSize = MaxMagazineAmmo;
	}
}

FVector AGunBase::GetBulletShootLocation()
{
	if(MuzzleSceneComponent == nullptr){return FVector::ZeroVector;}
	if(!GetWorld()->GetFirstPlayerController()) { return FVector::ZeroVector; } //检测是否存在玩家
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector MuzzleLocation = MuzzleSceneComponent->GetComponentLocation();
	FVector PlayerForward = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector();
	FVector GunDirection = MuzzleLocation - PlayerLocation; //玩家指向枪口的方向
	return (GunDirection.ProjectOnTo(PlayerForward) + PlayerLocation);
}

void AGunBase::Reload()
{
	IsReloading = true;
	
	ReloadTime = PlayReloadMontage();
	if(ReloadAudioComponent)
	{
		ReloadAudioComponent->Play();
	}
	StopShooting(); // 换弹时停止射击
}

bool AGunBase::GetIsReloading()
{
	return IsReloading;
}

void AGunBase::ReloadDone()
{
	CurrentAmmo = MaxMagazineAmmo;
	IsReloading = false;
	IsMagazineFull = true;
	IsMagazineEmpty = false;

	OnReloadFinish.Broadcast(); //广播换弹结束事件
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
    // 1. 如果定时器句柄当前是有效的（即正在冷却中），则直接返回。
    if (GetWorld()->GetTimerManager().IsTimerActive(ShootTimerHandle))
    {
        return;
    }

    // 2. 检查其他条件
    if(IsReloading || IsMagazineEmpty) return;

    // 3. 立即开火
    Shoot();
    

    // 4. 设置一个一次性的定时器，作为射击间隔。
    //    我们不再需要循环定时器，因为增强输入会为我们处理按住的情况。
    GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, GetShootDuration(), false);
}

void AGunBase::Shoot_Implementation()
{
	IsMagazineEmpty = (CurrentAmmo <= 0);
	if(!IsMagazineEmpty && !IsReloading)
	{
		IsMagazineFull = false;
		CurrentAmmo--;
		// 播放射击音效
		if (AttackAudioComponent && !AttackAudioComponent->IsPlaying())
    	{
    	    AttackAudioComponent->Play();
    	}
		SpawnBulletFromPool();
		OnAttack.Broadcast(this); //广播攻击事件
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

	AActor *BulletActor = BulletPool->GetActorFromPool();
	if(BulletActor == nullptr) return;
	
	// 让子弹获取到对象池组件，以便它在命中后能归还自己
	ABulletBase *Bullet = Cast<ABulletBase>(BulletActor);
	Bullet->SetObjectPoolComponent(BulletPool);

	// 设置子弹旋转和位置
	BulletActor->SetActorLocation(GetBulletShootLocation());
	
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
    if (AttackAudioComponent)
    {
        AttackAudioComponent->Stop();
    }
}



float AGunBase::GetShootDuration()
{
	return 60 / FireSpeed;
}


