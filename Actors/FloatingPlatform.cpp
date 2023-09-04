// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	InterpelateSpeed = 1.0f;
	InterpelateTime = 1.0f;
	bInterping = false;



}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	bInterping = false;
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpelateTime); // wait one second before calling ToggleIntering	

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bInterping)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Interpolate = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpelateSpeed);
		SetActorLocation(Interpolate);

		float DistanceTravelled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTravelled <= 1.f)
		{
			ToggleInterping();

			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpelateTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}
	


}

void AFloatingPlatform::ToggleInterping()
{
	// Toggele between states (interpolate back and forth)
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector & VecOne, FVector & VecTwo)
{
	FVector TempValue = VecOne;
	VecOne = VecTwo;
	VecTwo = TempValue;
}

