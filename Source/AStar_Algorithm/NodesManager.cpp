// Fill out your copyright notice in the Description page of Project Settings.


#include "NodesManager.h"
#include "GameFramework/Actor.h"
#include "MyCustomGameUtils.h"
#include <Components/TextRenderComponent.h>

// Sets default values for this component's properties
UNodesManager::UNodesManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNodesManager::InitializedNodes()
{

	float posX = initPositionReference;
	float posY = initPositionReference;

	FTransform newTransform;

	int32 indexNode = 0;

	for (int32 i = 0; i < gridSize.Y; i++)
	{
		posY = initPositionReference;

		for(int j = 0; j < gridSize.X; j++)
		{
			FVector customLocation = FVector(posX, posY, 0);

			newTransform.TransformPosition(customLocation);

			AActor* clone = SpawnNodes(&newTransform, meshReference);

			clone->SetActorLocation(customLocation);

			FNodeProperties baseNode;

			baseNode.index = indexNode;
			baseNode.baseNodeStaticMesh = clone;

			baseNodes.Add(baseNode);

			if(!UMyCustomGameUtils::CheckNodesToAvoid(nodesToAvoid , &indexNode))
				UMyCustomGameUtils::ChangeMaterialColor(baseNodes[baseNodes.Num() - 1].baseNodeStaticMesh, FColor::Black);

			posY += distanceBetweenNodes.Y;

			indexNode++;
		}
		
		posX += distanceBetweenNodes.X;
	}

	CreateConectionsBetweenNodes();
}

void UNodesManager::CreateConectionsBetweenNodes()
{
	

	for (int32 a = 0; a < baseNodes.Num(); a++)
	{
		//int32 RandomSiblingIndex = FMath::RandRange(0, baseNodes.Num());
		
		UTextRenderComponent* text = Cast<UTextRenderComponent>(baseNodes[a].baseNodeStaticMesh->GetComponentByClass<UTextRenderComponent>());

		if (text != nullptr)
		{
			FString sMyIndex = FString::Printf(TEXT("%d"), baseNodes[a].index);
			text->SetText(FText::FromString(sMyIndex));
		}
			

		if (!UMyCustomGameUtils::CheckNodesToAvoid(nodesToAvoid,&a))
			continue;

		FVector parenLocation = baseNodes[a].baseNodeStaticMesh->GetActorLocation();

		for (int32 j = 0; j < baseNodes.Num(); j++)
		{
			if (!UMyCustomGameUtils::CheckNodesToAvoid(nodesToAvoid, &j))
				continue;

			FVector childLocation = baseNodes[j].baseNodeStaticMesh->GetActorLocation();

			if(childLocation.X == parenLocation.X && childLocation.Y == parenLocation.Y + distanceBetweenNodes.Y ||
				childLocation.X == parenLocation.X && childLocation.Y == parenLocation.Y - distanceBetweenNodes.Y ||
				childLocation.Y == parenLocation.Y && childLocation.X == parenLocation.X + distanceBetweenNodes.X ||
				childLocation.Y == parenLocation.Y && childLocation.X == parenLocation.X - distanceBetweenNodes.X)
			{
				if (!baseNodes[j].siblingNodesIndex.Contains(baseNodes[a].index))
					baseNodes[j].siblingNodesIndex.Add(baseNodes[a].index);

				if (!baseNodes[a].siblingNodesIndex.Contains(baseNodes[j].index))
				{
					baseNodes[a].siblingNodesIndex.Add(baseNodes[j].index);
				}
			}
		}
	}

	//DrawActiveConections();
}

void UNodesManager::DrawActiveConections()
{
	for (int32 a = 0; a < baseNodes.Num(); a++)
	{
		FVector StartPoint = baseNodes[a].baseNodeStaticMesh->GetActorLocation();

		for (int32 j = 0; j < baseNodes[a].siblingNodesIndex.Num(); j++)
		{
			FVector EndPoint = baseNodes[baseNodes[a].siblingNodesIndex[j]].baseNodeStaticMesh->GetActorLocation();

			UMyCustomGameUtils::DrawPermanentPathsBetweenNodes(GetWorld(), StartPoint, EndPoint, FColor::Green, 1.0F);
		}
	}
}

AActor* UNodesManager::SpawnNodes(FTransform* transform, TSubclassOf<AActor> reference)
{
	FActorSpawnParameters spawnParamters;
	spawnParamters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	return GetWorld()->SpawnActor<AActor>(reference, *transform, spawnParamters);
}

