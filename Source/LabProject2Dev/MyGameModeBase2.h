// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase2.generated.h"

/**
 * 
 */
UCLASS()
class LABPROJECT2DEV_API AMyGameModeBase2 : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase2();
protected:
	
	virtual void BeginPlay() override;
	
public:

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<class AMover2> Mover_BP;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Spawning")
		TArray<AMover2*> Movers;
	UFUNCTION()
		TArray<AMover2*> GetMovers();
	
	
};
