// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common		UMETA(DisplayName = "Common"),
	EIR_UnCommon	UMETA(DisplayName = "UnCommon"),
	EIR_Epic		UMETA(DisplayName = "Epic"),
	EIR_Legendary	UMETA(DisplayName = "Legendary"),

	EIR_MAX			UMETA(DisplayName = "DisplayMAX")
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LABPROJECT2DEV_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Variables")
		UStaticMesh* Mesh {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Variables")
		UTexture2D* Image {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Variables")
		float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Variables")
		EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Variables")
		FString Name;
	
	
};
