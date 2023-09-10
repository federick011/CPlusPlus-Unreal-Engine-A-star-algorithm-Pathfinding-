// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomGameUtils.h"

void UMyCustomGameUtils::ChangeMaterialColor(AActor* staticMesh, FColor color)
{
	if (staticMesh == nullptr)
		return;

	UStaticMeshComponent* meshReference = staticMesh->GetComponentByClass<UStaticMeshComponent>();

	if (meshReference == nullptr)
		return;

	UMaterialInstanceDynamic* DynamicMatInstance = meshReference->CreateDynamicMaterialInstance(0);

	DynamicMatInstance->SetVectorParameterValue(FName("Color"), color);
}

void UMyCustomGameUtils::CheckNodesDistance(FVector& pointA, FVector& pointB, float& distance)
{
	distance = FVector::Dist(pointA, pointB);
}

void UMyCustomGameUtils::CheckNodesDistance(AActor* actorA, AActor* actorB, float& distance)
{
	if (actorA == nullptr || actorB == nullptr)
		return;

	FVector pointA = actorA->GetActorLocation();
	FVector pointB = actorB->GetActorLocation();
	distance = FVector::Dist(pointA, pointB);
}

void UMyCustomGameUtils::CheckNodesDistance(USceneComponent* actorA, USceneComponent* actorB, float& distance)
{
	FVector pointA = actorA->GetComponentLocation();
	FVector pointB = actorB->GetComponentLocation();
	distance = FVector::Dist(pointA, pointB);
}

void UMyCustomGameUtils::DrawPermanentPathsBetweenNodes(UWorld* world, FVector& StartPoint, FVector& EndPoint, FColor LineColor, float tickness)
{
	if (world == nullptr)
		return;

	DrawDebugLine(world, StartPoint, EndPoint, LineColor, true, 0.1f, 0, tickness);
}

bool UMyCustomGameUtils::CheckNodesToAvoid(TArray<int32> _nodesToAvoid, int32* indexReference)
{

	bool mayContinue = true;

	if (_nodesToAvoid.Num() < 1)
		return mayContinue;

	for (int i = 0;i < _nodesToAvoid.Num(); i++)
	{
		if (_nodesToAvoid[i] != *indexReference)
			continue;

		mayContinue = false;

		break;
	}

	return mayContinue;
}
