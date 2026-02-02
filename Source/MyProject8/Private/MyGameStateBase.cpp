#include "MyGameStateBase.h"

AMyGameStateBase::AMyGameStateBase()
{
	Score = 0;
}

int32 AMyGameStateBase::GetScore() const
{
	return Score;
}

void AMyGameStateBase::AddScore(int32 Amount)
{
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score : %d"), Score)
}