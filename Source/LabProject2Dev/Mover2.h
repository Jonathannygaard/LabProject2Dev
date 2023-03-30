// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mover2.generated.h"

UCLASS()
class LABPROJECT2DEV_API AMover2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMover2();

	UPROPERTY(EditAnywhere, Category="Components")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Components")
		class USphereComponent* Collider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
		FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
		FVector Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
		float Mass;
	
	TArray<AMover2*> OverlappedActors;

	bool GravityActive;
	bool StandStill;

	class AMyGameModeBase2* GameMode;

	void AddForce(FVector Force);
	void Attract(AMover2* Other);

	void SetUsingGravity(bool Input);
	void SetUsingGravity(bool Input, AMyGameModeBase2* Gamemode);

	UFUNCTION()
			void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
				UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
				bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp,
				class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
					int32 OtherBodyIndex);
};

