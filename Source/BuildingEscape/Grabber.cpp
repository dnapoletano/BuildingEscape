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
	UE_LOG(LogTemp, Log,
		TEXT("Input Component found for Actor %s"),
		*(GetOwner()->GetName()));

	InputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &UGrabber::PickUp);
	InputComponent->BindAction("PickUp", EInputEvent::IE_Released, this, &UGrabber::Release);
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PhysicsHandle->GrabbedComponent != nullptr){
		FVector PlayerCurrentLocation {0.f,0.f,0.f};
		GetViewPointAndLocation(PlayerCurrentLocation);
		PhysicsHandle->SetTargetLocation(ViewEndPoint);
	}
}

void UGrabber::PickUp()
{
	FVector PlayerCurrentLocation {0.f,0.f,0.f};
	GetViewPointAndLocation(PlayerCurrentLocation);
	FHitResult Hit;
	FCollisionQueryParams TraceParams{FName{TEXT("")},false,GetOwner()};
	if(InReach(PlayerCurrentLocation, TraceParams, OUT Hit)){
		UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			ViewEndPoint
		);
	}
}

void UGrabber::Release()
{
	if(PhysicsHandle->GrabbedComponent != nullptr){
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::GetViewPointAndLocation(FVector& PlayerCurrentLocation)
{
	FRotator ViewRotation{0.f,0.f,0.f};
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerCurrentLocation,
		OUT ViewRotation
	);
	ViewEndPoint = PlayerCurrentLocation + ViewRotation.Vector()*ViewReach;
}

bool UGrabber::InReach(const FVector& PlayerCurrentLocation,
		                     const FCollisionQueryParams& TraceParams, FHitResult& Hit) const
{
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerCurrentLocation,
		ViewEndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return (Hit.GetActor() != nullptr);
}