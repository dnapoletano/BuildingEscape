// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
    if(PressPlate == nullptr){
        UE_LOG(LogTemp, Error,
            TEXT("Actor %s has no collision volume set"),*(GetOwner()->GetName()));
    }
    if(ActorThatOpens == nullptr){
        ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
    }
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(PressPlate!=nullptr ){
        if(PressPlate->IsOverlappingActor(ActorThatOpens)){
                OpenDoor(DeltaTime);
                LastOpened = GetWorld()->GetTimeSeconds();
        } else {
            if(GetWorld()->GetTimeSeconds() - LastOpened >= DoorCloseDelay){
                CloseDoor(DeltaTime);
            }
        }
    }
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
    FRotator CurrentRotation {GetOwner()->GetActorRotation()};
    float CurrentYaw = FMath::Lerp(CurrentRotation.Yaw,
		InitialYaw+TargetYaw, OpeningSpeed * DeltaTime);
    CurrentRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
    FRotator CurrentRotation {GetOwner()->GetActorRotation()};
    float CurrentYaw = FMath::Lerp(CurrentRotation.Yaw,
		InitialYaw , ClosingSpeed * DeltaTime);
    CurrentRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(CurrentRotation);
}

