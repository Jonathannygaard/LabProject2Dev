// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover2.h"

#include "LocalizationDescriptor.h"
#include "MyGameModeBase2.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
AMover2::AMover2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(StaticMesh);

	Velocity = FVector::ZeroVector;
	Acceleration = FVector::ZeroVector;

	Mass = 1.f;

	OverlappedActors = TArray<AMover2*>();

	GravityActive = false;
	StandStill = false;
	GameMode = nullptr;
}

// Called when the game starts or when spawned
void AMover2::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AMyGameModeBase2>(GetWorld()->GetAuthGameMode());
	if(!GameMode)
	{
		UE_LOG(LogTemp, Warning,TEXT("Did not find the correct gamemode"))
	}

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AMover2::OnComponentBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AMover2::OnComponentEndOverlap);

	
}

// Called every frame
void AMover2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GravityActive && GameMode)
	{
		TArray<AMover2*> Movers = GameMode->GetMovers();
		for(int i{};i<Movers.Num(); i++)
		{
			if(Movers[i] != this)
			{
				Movers[i]->Attract(this);
			}
		}
	}
	
	//Update Velocity
	if(!FMath::IsNearlyZero(Acceleration.X))
	{
		Velocity.X += Acceleration.X;
	}
	if(!FMath::IsNearlyZero(Acceleration.Y))
	{
		Velocity.Y += Acceleration.Y;
	}
	if(!FMath::IsNearlyZero(Acceleration.Z))
	{
		Velocity.Z += Acceleration.Z;
	}
	Acceleration = FVector::ZeroVector;

	//Add Velocity to our position
	if(!UKismetMathLibrary::Vector_IsNearlyZero(Velocity))
	{
		SetActorLocation(GetActorLocation()+(Velocity*DeltaTime));
	}
	

}

void AMover2::AddForce(FVector Force)
{

	if(StandStill)
	{
		return;
	}
	Force *= 100.f;
	if(!FMath::IsNearlyZero(Mass))
	{
		Acceleration += Force/Mass;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to devide by 0"));
		Acceleration += Force;
	}
	
	

	
}

void AMover2::Attract(AMover2* Other)
{
	FVector Direction = GetActorLocation() - Other->GetActorLocation();

	float DistanceSq = Direction.Length();
	DistanceSq *= DistanceSq;

	DistanceSq = FMath::Clamp(DistanceSq, 10000.f, 100000000.f);

	FVector Force = Direction;
	Force.Normalize();

	float GravitationalConstant = 10000.f;
	float Strength = (Mass * Other->Mass);
	Strength /= DistanceSq;
	Strength *= GravitationalConstant;
	Force *= Strength;
	Other->AddForce(Force);
}

void AMover2::SetUsingGravity(bool Input)
{
	if(Input && GameMode)
	{
		if(!GameMode->Movers.Contains(this))
		{
			GameMode->Movers.Add(this);
		}
		GravityActive = Input;
	}
}

void AMover2::SetUsingGravity(bool Input, AMyGameModeBase2* gameMode)
{
	GameMode = gameMode;
	SetUsingGravity(Input);
}

void AMover2::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA<AMover2>() && !OverlappedActors.Contains(Cast<AMover2>(OtherActor)))
	{
		AMover2* OtherMover = Cast<AMover2>(OtherActor);
		
		
		FVector v = Velocity;
		FVector w = OtherMover->Velocity;
		float m1 = Mass;
		float m2 = OtherMover->Mass;

		Velocity = (((m1-m2)/(m1+m2))*v) + (((2*m2)/(m1+m2))*w);
		OtherMover->Velocity = (((2*m1)/(m1+m2))*v) + (((m2 - m1)/(m1 +m2))*w);

		OverlappedActors.Add(OtherMover);
		OtherMover->OverlappedActors.Add(this);
	}
}


void AMover2::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OverlappedActors.Contains(Cast<AMover2>(OtherActor)))
	{
		OverlappedActors.Remove(Cast<AMover2>(OtherActor));
	}
	
}

