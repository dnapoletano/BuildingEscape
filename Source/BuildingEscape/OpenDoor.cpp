// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Algo/Accumulate.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
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
    SetAudioComponent();
    if(PressPlate == nullptr){
        UE_LOG(LogTemp, Error,
            TEXT("Actor %s has no collision volume set"),*(GetOwner()->GetName()));
    }
	InitialY = GetOwner()->GetActorLocation().Y;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(PressPlate!=nullptr ){
        if(CalcMassOfActors() >= MassThreshold){
            OpenDoor(DeltaTime);
            LastOpened = GetWorld()->GetTimeSeconds();
        } else {
            if(GetWorld()->GetTimeSeconds() - LastOpened >= DoorCloseDelay){
                CloseDoor(DeltaTime);
            }
        }
    }
}

void UOpenDoor::SetAudioComponent()
{
    AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
    if(AudioComponent == nullptr){
        UE_LOG(LogTemp, Error,
            TEXT("Actor %s has no Audio Component set"),*(GetOwner()->GetName()));
    }
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
    FVector CurrentLocation {GetOwner()->GetActorLocation()};
    float CurrentY = FMath::Lerp(CurrentLocation.Y,
		InitialY+OpenY, OpeningSpeed * DeltaTime);
    CurrentLocation.Y = CurrentY;
    GetOwner()->SetActorLocation(CurrentLocation);
    if(AudioComponent==nullptr) return;
    if((not AudioComponent->IsPlaying()) and (SoundPlayed % 2 == 0)){
        AudioComponent->Play();
        ++SoundPlayed;
    }
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
    FVector CurrentLocation {GetOwner()->GetActorLocation()};
    float CurrentY = FMath::Lerp(CurrentLocation.Y,
		InitialY , ClosingSpeed * DeltaTime);
    CurrentLocation.Y = CurrentY;
    // GetOwner()->SetActorRotation(CurrentRotation);
    GetOwner()->SetActorLocation(CurrentLocation);
    if(AudioComponent==nullptr) return;
    /// Play the door sound only an odd number of times
    if((not AudioComponent->IsPlaying()) and (not (SoundPlayed % 2 == 0))){
        AudioComponent->Play();
        ++SoundPlayed;
    }
}

float UOpenDoor::CalcMassOfActors() const
{
    if(PressPlate == nullptr) return 0.0f;
    float TotalMass = 0.0f;
    TSet<AActor*> OverlappingActors;
    PressPlate->GetOverlappingActors(OUT OverlappingActors);
    for(const auto& Actor: OverlappingActors){
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    return TotalMass;
}