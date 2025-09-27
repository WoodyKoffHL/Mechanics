// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WC_CostGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MECHANICS_API UWC_CostGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	float Cost;

	UPROPERTY(EditDefaultsOnly)
	float Cooldown;
	
	
};
