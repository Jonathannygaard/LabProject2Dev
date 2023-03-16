// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"

#include "PlayerCharacter.h"

void UPlayer_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if(Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn && Player == nullptr)
		{
			Player = Cast<APlayerCharacter>(Pawn);
		}
	}
	IsAttack = false;
}

void UPlayer_AnimInstance::UpdateAnimationProperties()
{
	if(Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn && Player == nullptr)
		{
			Player = Cast<APlayerCharacter>(Pawn);
		}
	}

	if(Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}
	if(Player)
	{
		IsAttack = Player->GetIsAttack();
	}
}

void UPlayer_AnimInstance::ResetAttack()
{
	if (Player)
	{
		Player->ResetAttack();
	}
}
