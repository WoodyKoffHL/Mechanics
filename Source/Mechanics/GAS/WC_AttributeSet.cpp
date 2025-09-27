// Fill out your copyright notice in the Description page of Project Settings.


#include "WC_AttributeSet.h"
#include "Net/UnrealNetwork.h"

UWC_AttributeSet::UWC_AttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, Stamina(1.f)
	, MaxStamina(1.f)
	, CharSpeed(500.f)
{
}

void UWC_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWC_AttributeSet, Health, OldValue);
}

void UWC_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWC_AttributeSet, MaxHealth, OldValue);
}

void UWC_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWC_AttributeSet, Stamina, OldValue);
}

void UWC_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWC_AttributeSet, MaxStamina, OldValue);
}

void UWC_AttributeSet::OnRep_CharSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWC_AttributeSet, CharSpeed, OldValue);
}

void UWC_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// Example: Clamp Health to be between 0 and MaxHealth
	if (Attribute == GetHealthAttribute())
	{
			Health = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());

	}
	else if (Attribute == GetStaminaAttribute())
	{
		Stamina = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetCharSpeedAttribute())
	{
		CharSpeed = FMath::Clamp(NewValue, 0.0f, 1000.0f);
	}
}

void UWC_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWC_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWC_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWC_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWC_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWC_AttributeSet, CharSpeed, COND_None, REPNOTIFY_Always);
}
