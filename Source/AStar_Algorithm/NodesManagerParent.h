// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NodesManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodesManagerParent.generated.h"


UCLASS()
class ASTAR_ALGORITHM_API ANodesManagerParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodesManagerParent();

	UPROPERTY()
	TObjectPtr<UNodesManager> nodeManagerComponent;

	UPROPERTY(VisibleAnywhere)
		int32 lastStarNodeIndex;

	UPROPERTY(EditAnywhere)
		int32 starNodeIndex;

	UPROPERTY(VisibleAnywhere)
		int32 lastGoalNodeIndex;

	UPROPERTY(EditAnywhere)
		int32 goalNodeIndex;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Initialize();

	void InitializedNodesValuesDistances();

	void OpenNodeToCheck();

	FNodeProperties* GetNodeProperties(int32* index);

	AActor* GetNodeBaseMesh(int32* index);

	void InitializeStarNode(int32& index);

	void ResetNodesToFindNewPaths();

	void GenerateNewRandomGoalNodes();

	void GenerateNewVisualPath(int32 lastNode);
	

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

	virtual void PostInitializeComponents() override;
};
