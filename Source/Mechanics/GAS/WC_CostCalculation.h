// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "WC_CostCalculation.generated.h"

/**
 * 
 */
UCLASS()
class MECHANICS_API UWC_CostCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
	
};
