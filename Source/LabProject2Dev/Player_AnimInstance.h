// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LABPROJECT2DEV_API UPlayer_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;

public:
	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable)
		void ResetAttack();

	UPROPERTY(BlueprintReadOnly)
		float MovementSpeed;

	UPROPERTY(BlueprintReadOnly)
		APawn* Pawn;

	UPROPERTY(BlueprintReadOnly)
		class APlayerCharacter* Player;

	UPROPERTY(BlueprintReadWrite)
		bool IsAttack;

	
};
