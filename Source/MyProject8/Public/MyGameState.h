// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

UCLASS()
class MYPROJECT8_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMyGameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxLevels;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 CurrentWave;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void UpdateHUD();

protected:

	int32 WaveSpawnCount;

	void SetWaveDifficulty();
};
