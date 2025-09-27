// Fill out your copyright notice in the Description page of Project Settings.


#include "WC_CostCalculation.h"

#include "WC_CostGameplayAbility.h"


float UWC_CostCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{

	const UWC_CostGameplayAbility* Ability = Cast<UWC_CostGameplayAbility>(Spec.GetContext().GetAbility());

	if(!Ability)
	{
		return 0.f;
	}

	return Ability->Cost;

	
}
