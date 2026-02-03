#include "CoinItem.h"
#include "Engine/World.h"
#include "MyGameState.h"
#include "MyProject8/MyProject8Character.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->IsA(AMyProject8Character::StaticClass()))
    {
        if (UWorld* World = GetWorld())
        {
            if (AMyGameState* GameState = World->GetGameState<AMyGameState>())
            {
                GameState->AddScore(PointValue);
                GameState->OnCoinCollected(); 
            }
        }
        DestroyItem();
    }
}
