// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Main.h"
APickUp::APickUp()
{
	CoinCount = 10;
}

void APickUp::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("PickUp::OnVerLapBegin()"));


	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->IncreaseCoins(CoinCount);
			Main->PickUpLocations.Emplace(GetActorLocation());
			
			Destroy();
		}
	}
}

void APickUp::onEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::onEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("PickUp::OnVerLapEnd()"));
}

