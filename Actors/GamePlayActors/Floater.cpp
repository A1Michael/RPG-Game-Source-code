// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = StaticMesh;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Custom staticMesh"));
	InitialLocation = FVector(0.f);
	PlacedLocation = FVector(0.f);
	WorldOrigin = FVector(0.f,0.0f,0.0f);
	MovementDirection = FVector(0.f,0.0f,0.0f);
	bShouldFloat = false;
	bInstanciateVector = false;
	InitialTorque = FVector(20000000.f, 0.0f, 0.0f);
	InitialForce = FVector(20000000.f, 0.0f, 0.0f);
	runningTime = 0.f;
	A = 0.f;
	B = 0.f;
	C = 0.f;
	D = 0.f;


}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();
	float initialX = FMath::FRandRange(-500.f, 500.f);
	float initialY = FMath::FRandRange(-500.f, 500.f);
	float initialZ = FMath::FRandRange(0.f, 500.f);

	InitialLocation.X = initialX;
	InitialLocation.Y = initialY;
	InitialLocation.Z = initialZ;

	PlacedLocation = GetActorLocation();

	if (bInstanciateVector)
	{
		// call a function
		SetActorLocation(InitialLocation);
	}
	//SetActorLocation(FVector(0.0f, 0.0f, 0.0f));

	baseZLocation = PlacedLocation.Z;
	

//	StaticMesh->AddForce(InitialForce);
//	StaticMesh->AddTorque(InitialTorque);

}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

		// if set to a pointer I get an error(Uninitial local variable used)
		
		// move the actor

		if (bShouldFloat)
		{

			FVector NewLocation = GetActorLocation(); 
			// A,B,C,D parameters used to transform the sin function
			NewLocation.Z = baseZLocation + A  * FMath::Sin(B * runningTime - C)+ D; //B IS PERIOD, 
			//	NewLocation.Z = baseZLocation +(100.f) * FMath::Sin(runningTime);


			SetActorLocation(NewLocation);
			runningTime += DeltaTime;
		}

	
}

