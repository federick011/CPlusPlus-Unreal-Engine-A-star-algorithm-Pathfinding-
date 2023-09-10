// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeBaseClass.h"

// Sets default values
ANodeBaseClass::ANodeBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANodeBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANodeBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

