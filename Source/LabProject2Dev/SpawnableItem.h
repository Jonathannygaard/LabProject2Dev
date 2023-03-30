// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnableItem.generated.h"

UCLASS()
class LABPROJECT2DEV_API ASpawnableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnableItem();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Components")
	class USphereComponent* Collider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
		TSubclassOf<class UItem> BP_Item;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
		uint8 Amount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item")
		UItem* Item;

	UFUNCTION()
	void Kill();
};
