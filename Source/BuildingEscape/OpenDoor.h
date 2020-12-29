// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float InitialYaw = 0.0f;
    UPROPERTY(EditAnywhere)
 	float OpenAngle  = 90.0f;
    UPROPERTY(EditAnywhere)
 	float OpeningSpeed  = 0.8f;
    UPROPERTY(EditAnywhere)
 	float ClosingSpeed  = 2.0f;
    UPROPERTY(EditAnywhere)
 	float DoorCloseDelay = 0.2f;
    float LastOpened = 0.0f;
    UPROPERTY(EditAnywhere)
    class ATriggerVolume* PressPlate;
    UPROPERTY(EditAnywhere)
    class AActor* ActorThatOpens;

    void OpenDoor(float DeltaTime);
    void CloseDoor(float DeltaTime);
};
