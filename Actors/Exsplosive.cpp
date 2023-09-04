 // Fill out your copyright notice in the Description page of Project Settings.


#include "Exsplosive.h"
#include "Main.h"

AExsplosive::AExsplosive()
{
	Damage = 20.f;

}

void AExsplosive::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("EXplsoive::OnVerLapBegin()"));

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->DecreaseHealth(Damage);

			Destroy();
		}

	}
}

void AExsplosive::onEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::onEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("Explosive::OnVerLapEnd()"));
}
