// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

/// macro used to highlight out parameters
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	/// At this point in time, no physics handle or input component
	/// is set, so we do just that here.
	FindPhysicsHandle();
	SetUpInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle  = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr){
		UE_LOG(LogTemp, Error,
			TEXT("A Physics Handle component has not been correctly found for Actor %s"),
			*(GetOwner()->GetName()));
	}
}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent == nullptr){
		UE_LOG(LogTemp, Error,
			TEXT("A Input Component has not been correctly found for Actor %s"),
			*(GetOwner()->GetName()));
	}

	InputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &UGrabber::PickUp);
	InputComponent->BindAction("PickUp", EInputEvent::IE_Released, this, &UGrabber::Release);
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PhysicsHandle == nullptr) return;
	if(PhysicsHandle->GrabbedComponent != nullptr){
		PhysicsHandle->SetTargetLocationAndRotation(GetViewEndPoint(),ActorInitialRotation);
	}
}

void UGrabber::PickUp()
{
	if(PhysicsHandle == nullptr) return;
	FHitResult Hit;
	FCollisionQueryParams TraceParams{FName{TEXT("")},false,GetOwner()};
	if(InReach(TraceParams, OUT Hit)){
		UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
		ActorInitialRotation = Hit.GetActor()->GetActorRotation();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			GetViewEndPoint(),
			ActorInitialRotation
		);
	}
}

void UGrabber::Release()
{
	if(PhysicsHandle == nullptr) return;
	if(PhysicsHandle->GrabbedComponent != nullptr){
		PhysicsHandle->ReleaseComponent();
	}
}

FVector UGrabber::GetViewEndPoint() const
{
	FVector PlayerCurrentLocation{0.f,0.f,0.f};
	FRotator ViewRotation{0.f,0.f,0.f};
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerCurrentLocation,
		OUT ViewRotation
	);
	return PlayerCurrentLocation + ViewRotation.Vector()*ViewReach;
}

bool UGrabber::InReach(const FCollisionQueryParams& TraceParams, FHitResult& Hit)
{
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetOwner()->GetActorLocation(),
		GetViewEndPoint(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return (Hit.GetActor() != nullptr);
}