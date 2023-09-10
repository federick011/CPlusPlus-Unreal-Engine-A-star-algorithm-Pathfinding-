// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NodesManager.generated.h"

USTRUCT(BlueprintType)
struct FNodeProperties
{
	GENERATED_BODY()
		
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 lastNode = -1;//The parent node that was checked before this

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<AActor> baseNodeStaticMesh;
		

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> siblingNodesIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float distanceFromStarNode_GCost = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float distanceFromGoal_HCost = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float totalWeight_FCost = -1;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASTAR_ALGORITHM_API UNodesManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNodesManager();

	//Lets creae the init position as a refeecne for our nodes to spawn
	UPROPERTY(EditAnywhere)
	float initPositionReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector distanceBetweenNodes;

	UPROPERTY(EditAnywhere)
		FVector gridSize;

	UPROPERTY(EditAnywhere)
		TArray<FNodeProperties> baseNodes;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> meshReference;

	UPROPERTY(EditAnywhere)
		TArray<int32> nodesToAvoid;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CreateConectionsBetweenNodes();

	void DrawActiveConections();

	AActor* SpawnNodes(FTransform* transform, TSubclassOf<AActor> reference);

public:	

	void InitializedNodes();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
