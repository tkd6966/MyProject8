#include "CoinItem.h"
#include "Engine/World.h"
#include "MyGameStateBase.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag	("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AMyGameStateBase* GameState = World->GetGameState<AMyGameStateBase>())
			{
				GameState->AddScore(PointValue);
			}
		}
		DestroyItem();
	}
}
