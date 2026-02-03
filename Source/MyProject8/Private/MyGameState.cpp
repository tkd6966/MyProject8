// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "MyProject8/MyProject8Character.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AMyGameState::AMyGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	CurrentWave = 1;

	FString MapName = GetWorld()->GetMapName();

	if (!MapName.Contains("MenuLevel"))
	{
		StartLevel();
	}

	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &AMyGameState::UpdateHUD, 0.1f, true);
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToScore(Amount);
		}
	}
}

void AMyGameState::OnGameOver()
{
	UpdateHUD();
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AMyProject8Character* MyCharacter = Cast<AMyProject8Character>(PC->GetPawn()))
		{
			MyCharacter->ShowMainMenu(true);
		}
	}
}

void AMyGameState::StartLevel()
{
	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
	}

	SetWaveDifficulty();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			FString::Printf(TEXT("Level %d - Wave %d Start"), CurrentLevelIndex + 1, CurrentWave));
	}	

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AMyProject8Character* MyCharacter = Cast<AMyProject8Character>(PC->GetPawn()))
		{
			MyCharacter->ShowGameHUD();
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.Num() > 0)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		if (SpawnVolume)
		{

			for (int32 i = 0; i < WaveSpawnCount; i++)
			{
				int32 ItemsPerSpawn = 20;
				for (int32 j = 0; j < ItemsPerSpawn; j++)
				{
					if (AActor* SpawnedActor = SpawnVolume->SpawnRandomItem())
					{
						if (SpawnedActor->IsA(ACoinItem::StaticClass()))
						{
							SpawnedCoinCount++;
						}
					}
				}
			}
		}
	}

	UpdateHUD();
	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMyGameState::OnLevelTimeUp, LevelDuration, false);
}

void AMyGameState::OnLevelTimeUp()
{
	EndLevel();
}

void AMyGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		GetWorldTimerManager().ClearTimer(LevelTimerHandle);

		if (CurrentWave < 3)
		{
			CurrentWave++;

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
					FString::Printf(TEXT("Moving to Wave %d!"), CurrentWave));
			}

			StartLevel(); 
		}
		else
		{
			CurrentWave = 1;
			EndLevel();
		}
	}
}

void AMyGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		AddScore(Score);
		CurrentLevelIndex++;
		MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
	}

	if (CurrentLevelIndex > MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AMyProject8Character* MyCharacter = Cast<AMyProject8Character>(PC->GetPawn()))
		{
			if (UUserWidget* HUDWidget = MyCharacter->HUDWidgetInstance)
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance()))
					{
						ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
				
				if (UTextBlock* HealthText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Health"))))
				{
					HealthText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), MyCharacter->GetHealth())));
				}
			}
		}
	}

}

void AMyGameState::SetWaveDifficulty()
{
	float LevelDifficultyBonus = CurrentLevelIndex * 5.0f;

	if (CurrentWave <= 1)
	{
		LevelDuration = 60.0f;
		WaveSpawnCount = 3;
	}
	else if (CurrentWave == 2)
	{
		LevelDuration = 45.0f;
		WaveSpawnCount = 2;
	}
	else if (CurrentWave == 3)
	{
		LevelDuration = 30.0f;
		WaveSpawnCount = 1;
	}

	LevelDuration = FMath::Max(10.0f, LevelDuration - (CurrentLevelIndex * 5.0f));
}
