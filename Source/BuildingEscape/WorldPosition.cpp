// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();
	const FString& ActorName {GetOwner()->GetName()};
	UE_LOG(LogTemp, Warning, TEXT("This component is attached to : %s"), *ActorName);
	const FString& ObjectPosition {GetOwner()->GetActorLocation().ToString()};
	UE_LOG(LogTemp, Warning, TEXT("The Object position is : %s"), *ObjectPosition);
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

