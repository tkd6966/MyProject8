// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "MyProject8/MyProject8Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AMyProject8Character* PlayerCharacter = Cast<AMyProject8Character>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);

		}
	}
	DestroyItem();
}
