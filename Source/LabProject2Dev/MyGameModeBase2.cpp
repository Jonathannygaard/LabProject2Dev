// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase2.h"
#include "Mover2.h"
#include "VectorTypes.h"

AMyGameModeBase2::AMyGameModeBase2()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMyGameModeBase2::BeginPlay()
{
	Super::BeginPlay();

	int Amount = 10;

	UWorld* World = GetWorld();

	for(int i{}; i < Amount; i++)
	{
		float Angle = FMath::RandRange(0,360);
		float Radian = FMath::DegreesToRadians(Angle);

		FVector Position = FVector(cos(Radian), sin(Radian), 0);
		Position *= 2500.f;

		FVector Velocity = FVector::CrossProduct(Position, FVector(0,0,-1));
		Velocity.Normalize();
		Velocity *= 1000.f;

		AMover2* Mover = Cast<AMover2> (World->SpawnActor<AActor>(Mover_BP, Position, FRotator::ZeroRotator));
		Mover->Velocity = Velocity;
		Mover->SetUsingGravity(true, this);
	}
	AMover2* Sun = Cast<AMover2> (World->SpawnActor<AActor>(Mover_BP, FVector(0,0,0), FRotator::ZeroRotator));
	Sun->SetUsingGravity(true,this);
	Sun->StandStill = true;
	Sun->Mass = 100.f;
	Sun->SetActorScale3D(FVector(3,3,3));
	
}

void AMyGameModeBase2::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

TArray<AMover2*> AMyGameModeBase2::GetMovers()
{
	return Movers;
}
