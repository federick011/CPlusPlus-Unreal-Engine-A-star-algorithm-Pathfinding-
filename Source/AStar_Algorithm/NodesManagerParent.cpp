// Fill out your copyright notice in the Description page of Project Settings.


#include "NodesManagerParent.h"
#include "MyCustomGameUtils.h"

// Sets default values
ANodesManagerParent::ANodesManagerParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ANodesManagerParent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ANodesManagerParent::Initialize()
{
	if (starNodeIndex != goalNodeIndex)
	{
		lastStarNodeIndex = starNodeIndex;

		lastGoalNodeIndex = goalNodeIndex;
	}

	nodeManagerComponent->InitializedNodes();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANodesManagerParent::GenerateNewRandomGoalNodes, 1.0f, true);
}

void ANodesManagerParent::InitializedNodesValuesDistances()
{
	FNodeProperties* starPoint = GetNodeProperties(&starNodeIndex);

	FNodeProperties* goalPoint = GetNodeProperties(&goalNodeIndex);
	
	for (int i = 0; i < nodeManagerComponent->baseNodes.Num(); i++)
	{
		float distanceToGoal;

		UMyCustomGameUtils::CheckNodesDistance(GetNodeBaseMesh(&i), GetNodeBaseMesh(&goalNodeIndex), distanceToGoal);

		GetNodeProperties(&i)->distanceFromGoal_HCost = distanceToGoal;
	}

	if (starPoint == nullptr)
		return;

	if (goalPoint == nullptr)
		return;

	UMyCustomGameUtils::ChangeMaterialColor(starPoint->baseNodeStaticMesh, FColor::Blue);
	UMyCustomGameUtils::ChangeMaterialColor(goalPoint->baseNodeStaticMesh, FColor::Red);
}

void ANodesManagerParent::OpenNodeToCheck()
{
	int32 currentNodeIndexToCheck = starNodeIndex;

	InitializeStarNode(currentNodeIndexToCheck);
	
	if (GetNodeProperties(&currentNodeIndexToCheck)->siblingNodesIndex.Num() < 1)
		return;

	TArray<int32> openNodes;
	TArray<int32> closedNodes;

	auto ShowGoalMessage = [](int32* index)
	{
		FString customText = FString::Printf(TEXT("Reach our goal!!!! id: %d"), *index);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, customText);
	};

	auto SetNewValuesToEvaluate = [](int32 &indexFromTheMininum, float &minFCost, float totalWeight_FCost, int32* index)
	{
		indexFromTheMininum = *index;
		minFCost = totalWeight_FCost;
	};

	while (currentNodeIndexToCheck != goalNodeIndex)
	{
		FNodeProperties *nodeProperties = GetNodeProperties(&currentNodeIndexToCheck);

		float minFCost = -1;

		int32 indexFromTheMininum = -1;

		bool mayBreakCicle = false;
		bool mayJumpWhileSicle = false;

		for (int i = 0; i < nodeProperties->siblingNodesIndex.Num(); i++)
		{
			int32* siblingIndex = &nodeProperties->siblingNodesIndex[i];

			if (*siblingIndex == goalNodeIndex)
			{
				GetNodeProperties(siblingIndex)->lastNode = currentNodeIndexToCheck;

				currentNodeIndexToCheck = *siblingIndex;
				ShowGoalMessage(siblingIndex);
				mayBreakCicle = true;
				break;
			}

			if (GetNodeProperties(&currentNodeIndexToCheck)->lastNode == *siblingIndex)
			{
				continue;
			}

			openNodes.Add(*siblingIndex);
		}

		if (mayBreakCicle)
			break;

		for (int j = 0; j < nodeProperties->siblingNodesIndex.Num(); j++)
		{
			int32* siblingIndex = &nodeProperties->siblingNodesIndex[j];

			if (*siblingIndex == goalNodeIndex)
			{
				GetNodeProperties(siblingIndex)->lastNode = currentNodeIndexToCheck;

				currentNodeIndexToCheck = *siblingIndex;
				ShowGoalMessage(siblingIndex);
				mayBreakCicle = true;
				break;
			}
			
			if (GetNodeProperties(&currentNodeIndexToCheck)->lastNode == *siblingIndex)
			{
				if(nodeProperties->siblingNodesIndex.Num() == 1)
				{
					float minOpensFCost = -1;

					int32 indexOpensFromTheMininum = -1;

					if (openNodes.Num() < 1)
					{
						mayBreakCicle = true;
						break;
					}

					for (int a = 0; a < openNodes.Num(); a++)
					{
						if (openNodes[a] == *siblingIndex)
							continue;

						if (minOpensFCost < 0)
						{
							indexOpensFromTheMininum = openNodes[a];
							minOpensFCost = GetNodeProperties(&openNodes[a])->totalWeight_FCost;
						}
						else if (GetNodeProperties(&openNodes[a])->totalWeight_FCost < minOpensFCost)
						{
							indexOpensFromTheMininum = openNodes[a];
							minOpensFCost = GetNodeProperties(&openNodes[a])->totalWeight_FCost;
						}
					}
					
					currentNodeIndexToCheck = indexOpensFromTheMininum;
					mayJumpWhileSicle = true;
					break;
				}
				continue;
			}

			if (mayBreakCicle)
				break;
			
			float ditanceFromLastNode;

			UMyCustomGameUtils::CheckNodesDistance(GetNodeBaseMesh(&currentNodeIndexToCheck), GetNodeBaseMesh(siblingIndex), ditanceFromLastNode);

			if(GetNodeProperties(siblingIndex)->totalWeight_FCost < 0)
			{
				GetNodeProperties(siblingIndex)->distanceFromStarNode_GCost =
					GetNodeProperties(&currentNodeIndexToCheck)->distanceFromStarNode_GCost + ditanceFromLastNode;

				GetNodeProperties(siblingIndex)->totalWeight_FCost =
					GetNodeProperties(siblingIndex)->distanceFromStarNode_GCost + GetNodeProperties(siblingIndex)->distanceFromGoal_HCost;
			}
			else 
			{
				float totalGCost = GetNodeProperties(&currentNodeIndexToCheck)->distanceFromStarNode_GCost + ditanceFromLastNode;
				float totalFCost = totalGCost + GetNodeProperties(siblingIndex)->distanceFromGoal_HCost;

				if (GetNodeProperties(siblingIndex)->totalWeight_FCost < totalFCost)
				{
					GetNodeProperties(siblingIndex)->distanceFromStarNode_GCost = totalGCost;
					GetNodeProperties(siblingIndex)->totalWeight_FCost = totalFCost;
				}
			}

			if (minFCost < 0)
			{
				SetNewValuesToEvaluate(indexFromTheMininum, minFCost, GetNodeProperties(siblingIndex)->totalWeight_FCost, siblingIndex);
			}
			else if (GetNodeProperties(siblingIndex)->totalWeight_FCost < minFCost)
			{
				SetNewValuesToEvaluate(indexFromTheMininum, minFCost, GetNodeProperties(siblingIndex)->totalWeight_FCost, siblingIndex);
			}

			GetNodeProperties(siblingIndex)->lastNode = currentNodeIndexToCheck;
		}		

		if (mayJumpWhileSicle)
			continue;

		if (mayBreakCicle)
			break;

		GetNodeProperties(&indexFromTheMininum)->lastNode = currentNodeIndexToCheck;

		//openNodes.Empty();
		if (openNodes.Contains(currentNodeIndexToCheck))
			openNodes.Remove(currentNodeIndexToCheck);

		if (!closedNodes.Contains(currentNodeIndexToCheck))
			closedNodes.Add(currentNodeIndexToCheck);

		//Update our new current node
		currentNodeIndexToCheck = indexFromTheMininum;
	}

	GenerateNewVisualPath(currentNodeIndexToCheck);
}

FNodeProperties* ANodesManagerParent::GetNodeProperties(int32* index)
{
	if (*index < 0)
		return nullptr;

	if (nodeManagerComponent == nullptr)
		return nullptr;

	return &nodeManagerComponent->baseNodes[*index];
}

AActor* ANodesManagerParent::GetNodeBaseMesh(int32* index)
{
	if (nodeManagerComponent == nullptr)
		return nullptr;

	return nodeManagerComponent->baseNodes[*index].baseNodeStaticMesh;
}

void ANodesManagerParent::InitializeStarNode(int32& _starIndex)
{
	GetNodeProperties(&_starIndex)->lastNode = -1;

	GetNodeProperties(&_starIndex)->distanceFromStarNode_GCost = 0;

	GetNodeProperties(&_starIndex)->totalWeight_FCost = GetNodeProperties(&_starIndex)->distanceFromStarNode_GCost + GetNodeProperties(&_starIndex)->distanceFromGoal_HCost;
}

void ANodesManagerParent::ResetNodesToFindNewPaths()
{
	if (nodeManagerComponent->baseNodes.Num() < 1)
		return;

	for (int32 j = 0; j < nodeManagerComponent->baseNodes.Num(); j++)
	{

		if (!UMyCustomGameUtils::CheckNodesToAvoid(nodeManagerComponent->nodesToAvoid, &j))
			continue;

		GetNodeProperties(&nodeManagerComponent->baseNodes[j].index)->distanceFromGoal_HCost = -1;
		GetNodeProperties(&nodeManagerComponent->baseNodes[j].index)->distanceFromStarNode_GCost = -1;
		GetNodeProperties(&nodeManagerComponent->baseNodes[j].index)->totalWeight_FCost = -1;

		UMyCustomGameUtils::ChangeMaterialColor(GetNodeBaseMesh(&nodeManagerComponent->baseNodes[j].index), FColor::White);
	}
}

void ANodesManagerParent::GenerateNewRandomGoalNodes()
{
	TArray<int32> allowedNodes;
	for(int a=0; a < nodeManagerComponent->baseNodes.Num();a++)
	{
		if (!UMyCustomGameUtils::CheckNodesToAvoid(nodeManagerComponent->nodesToAvoid, &a))
			continue;
		
		allowedNodes.Add(nodeManagerComponent->baseNodes[a].index);
	}
	
	int32 RandomSiblingIndex = FMath::RandRange(0, allowedNodes.Num());

	goalNodeIndex = allowedNodes[RandomSiblingIndex];

	ResetNodesToFindNewPaths();

	InitializedNodesValuesDistances();

	OpenNodeToCheck();
}

void ANodesManagerParent::GenerateNewVisualPath(int32 lastNode)
{
	FString myCustomText;

	while (lastNode != starNodeIndex)
	{
		if (lastNode >= 0 && lastNode < nodeManagerComponent->baseNodes.Num())
		{
			if ((lastNode + 1) < nodeManagerComponent->baseNodes.Num())
			{
				FVector StartPoint = GetNodeBaseMesh(&lastNode)->GetActorLocation();
				int nextIndex = lastNode + 1;
				FVector EndPoint = GetNodeBaseMesh(&nextIndex)->GetActorLocation();
			}

			myCustomText = FString::Printf(TEXT("Node Checked id: %d"), lastNode);
			GEngine->AddOnScreenDebugMessage(-1, 60.0f * 100.0f, FColor::Yellow, myCustomText);

			if (lastNode != goalNodeIndex)
				UMyCustomGameUtils::ChangeMaterialColor(GetNodeBaseMesh(&lastNode), FColor::Cyan);

			lastNode = GetNodeProperties(&lastNode)->lastNode;
		}
		else
		{
			break;
		}
	}
}

void ANodesManagerParent::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	nodeManagerComponent = FindComponentByClass<UNodesManager>();
}

void ANodesManagerParent::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

