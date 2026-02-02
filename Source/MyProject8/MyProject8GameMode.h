// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyProject8GameMode.generated.h"

UCLASS(minimalapi)
class AMyProject8GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMyProject8GameMode();

protected:
	virtual void BeginPlay() override;
};



