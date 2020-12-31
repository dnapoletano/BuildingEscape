// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float ViewReach = 100.f;

	FRotator ActorInitialRotation;

	UPROPERTY()
  class UInputComponent*         InputComponent = nullptr;
	UPROPERTY()
	class UPhysicsHandleComponent* PhysicsHandle  = nullptr;

	void FindPhysicsHandle();
	void SetUpInputComponent();

	void PickUp();
	void Release();

	FVector GetViewEndPoint() const;
	bool InReach(const FCollisionQueryParams& TraceParams, FHitResult& Hit);
};
