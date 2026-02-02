// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

AMyGameState::AMyGameState()
{
	Score = 0;
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score : %d"), Score)
}