// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Wander.h"
#include "AI/MazeGameBot.h"
#include "AI/BotController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"


UBTTask_Wander::UBTTask_Wander()
{
    WanderRadius = 100.0f;
    WanderDistance = 200.0f;
    WanderJitter = 20.0f;
    WanderRatio = 0.4f;
}

EBTNodeResult::Type UBTTask_Wander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    BotConCache = Cast<ABotController>(OwnerComp.GetAIOwner());
    BotCache = Cast<AMazeGameBot>(BotConCache->GetCharacter());

    if (BotCache && IsValid(BotCache) && BotConCache && IsValid(BotConCache))
    {
        // Setup Wander Target unit circle
        float Theta = FMath::RandRange(0.0f, 1.0f) * (PI * 2);
        WanderTarget = FVector(WanderRadius * cos(Theta), WanderRadius * sin(Theta), 0.0f);

        // Add small random vector to the target's position using a random value between -1 and 1
        WanderTarget += FVector(FMath::RandRange(-1.0f, 1.0f) * WanderJitter, FMath::RandRange(-1.0f, 1.0f) * WanderJitter, 0.0f);

        // Reproject this new vector back onto a unit circle
        WanderTarget.Normalize();

        // Increase the length of the vector to the same as the radius of the wander circle
        WanderTarget *= WanderRadius;

        // Move the target into a position WanderDist in front/back of the agent
        FVector TargetLocal;
        if (FMath::RandRange(0.0f, 1.0f) > WanderRatio)
            TargetLocal = WanderTarget + FVector(WanderDistance, 0.0f, 0.0f);
        else
            TargetLocal = WanderTarget + FVector(-WanderDistance, 0.0f, 0.0f);

        // Project target into world space
        FTransform AITransform(BotCache->GetActorRotation(), BotCache->GetActorLocation(), BotCache->GetActorScale3D());
        FVector TargetWorld = UKismetMathLibrary::TransformLocation(AITransform, TargetLocal);

        // Set new location for actor
        UBlackboardComponent* BlackboardComp = BotConCache->GetBlackboardComponent();
        BlackboardComp->SetValueAsVector("NextLocation", TargetWorld);

        return EBTNodeResult::Succeeded;

    }
    else
    {
        return EBTNodeResult::Failed;
    }
}