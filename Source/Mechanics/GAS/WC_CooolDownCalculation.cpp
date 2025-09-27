// Fill out your copyright notice in the Description page of Project Settings.


#include "WC_CooolDownCalculation.h"

#include "WC_CostGameplayAbility.h"


float UWC_CooolDownCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{

	const UWC_CostGameplayAbility* Ability = Cast<UWC_CostGameplayAbility>(Spec.GetContext().GetAbility());

	if(!Ability)
	{
		return 0.1f;
	}

	return Ability->Cooldown;

	
}
