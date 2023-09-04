// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class ACTORS_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite , Category = " ActorMeshComponent")
	UStaticMeshComponent* StaticMesh;

	//Location used by setActorLocation() when BeginPlay() is called
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, Category = "Vector Information ")
	FVector InitialLocation;

	// Location when the actor is instanciated by dragging into the editor
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Vector Information ")
	FVector PlacedLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Vector Information ")
	FVector WorldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector Information ")
	FVector MovementDirection;

	// can be edited anywhere , but can only be read in the BP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vector Information ")
	bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vector Information ")
	bool bInstanciateVector;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Vector Information")
	FVector InitialForce;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Vector Information")
	FVector InitialTorque;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	float runningTime;
	float baseZLocation;
	
public:
	//Amplitude - how much we oscillate up and down 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " vector information")
	float A;
	//period - period = 2 * PI / ABS(B)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "vector information")
	float B;
	// phase shift is (C/B)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "vector information")
	float C;

	// vertical shift - i D
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "vector information")
	float D;

};

