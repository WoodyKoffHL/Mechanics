// Fill out your copyright notice in the Description page of Project Settings.


#include "WC_Character.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

// Sets default values
AWC_Character::AWC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CppAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GAS_Component"));
	CppAbilitySystemComponent->SetIsReplicated(true);
	CppAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UWC_AttributeSet>(TEXT("AttributeSet"));

}

// Called when the game starts or when spawned
void AWC_Character::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributs();
	
}

void AWC_Character::InitializeAttributs()
{
	if(CppAbilitySystemComponent && AttributeSet)
	{
		
	}
}

// Called every frame
void AWC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

