// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableItem.h"
#include "Item.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpawnableItem::ASpawnableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(StaticMesh);

	Amount = 1;
	

}

// Called when the game starts or when spawned
void ASpawnableItem::BeginPlay()
{
	Super::BeginPlay();

	UObject* obj = NewObject<UObject>(this, BP_Item);
	Item = Cast<UItem>(obj);

	if(Item != nullptr)
	{
		if(Item->Mesh != nullptr)
		{
			StaticMesh->SetStaticMesh(Item->Mesh);
		}
	}
}

// Called every frame
void ASpawnableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnableItem::Kill()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

