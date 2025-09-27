// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WC_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MECHANICS_API UWC_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWC_AttributeSet();

	//Health
	UPROPERTY (BlueprintReadOnly, category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UWC_AttributeSet, Health);
	
	//MaxHealth
	UPROPERTY(BlueprintReadOnly, category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWC_AttributeSet, MaxHealth);

	//Stamina
	UPROPERTY (BlueprintReadOnly, category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UWC_AttributeSet, Stamina);
	
	//MaxStamina
	UPROPERTY(BlueprintReadOnly, category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UWC_AttributeSet, MaxStamina);

	//CharSpeed
	UPROPERTY(BlueprintReadOnly, category = "CharSpeed", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData CharSpeed;
	ATTRIBUTE_ACCESSORS(UWC_AttributeSet, CharSpeed);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CharSpeed(const FGameplayAttributeData& OldValue);
	
};
