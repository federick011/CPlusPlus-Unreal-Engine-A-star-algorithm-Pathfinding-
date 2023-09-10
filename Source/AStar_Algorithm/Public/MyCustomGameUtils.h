// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AStar_Algorithm/NodesManager.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyCustomGameUtils.generated.h"


/**
 * 
 */
UCLASS()
class ASTAR_ALGORITHM_API UMyCustomGameUtils : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	static void ChangeMaterialColor(AActor* staticMesh, FColor color);

	static void CheckNodesDistance(FVector& pointA, FVector& pointB, float& distance);

	static void CheckNodesDistance(AActor* actorA, AActor* actorB, float& distance);

	static void CheckNodesDistance(USceneComponent* actorA, USceneComponent* actorB, float& distance);

	static void DrawPermanentPathsBetweenNodes(UWorld* world,FVector& StartPoint, FVector& EndPoint, FColor LineColor, float tickness);

	static bool CheckNodesToAvoid(TArray<int32> _nodesToAvoid, int32* indexReference);
	
};


