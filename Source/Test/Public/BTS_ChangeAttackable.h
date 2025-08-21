// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTS_ChangeAttackable.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTS_ChangeAttackable : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_ChangeAttackable();

protected:
	// 服务的核心函数，周期性执行
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // 要检查的目标玩家
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector TargetActorKey;

    // 距离阈值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AttackRange = 100.0f;

    // 用来存储检查结果的黑板键
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector IsCloseToAttackKey;
};
