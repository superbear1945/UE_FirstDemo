// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ChangeAttackable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_ChangeAttackable::UBTS_ChangeAttackable()
{
    NodeName = TEXT("Check Attack Distance");
}

void UBTS_ChangeAttackable::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AAIController* AIController = OwnerComp.GetAIOwner();

    if (!BlackboardComp || !AIController || !AIController->GetPawn())
    {
        return;
    }

    // 从黑板获取目标Actor
    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
    if (!TargetActor)
    {
        BlackboardComp->SetValueAsBool(IsCloseToAttackKey.SelectedKeyName, false);
        return;
    }

    // 计算距离
    float Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());

    // 根据距离更新黑板
    bool bIsCloseEnough = Distance <= AttackRange;
    BlackboardComp->SetValueAsBool(IsCloseToAttackKey.SelectedKeyName, bIsCloseEnough);
}
