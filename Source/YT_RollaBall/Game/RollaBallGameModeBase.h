// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RollaBallGameModeBase.generated.h"

class URollaBallWidget;
/**
 * 
 */
UCLASS()
class YT_ROLLABALL_API ARollaBallGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	FTimerHandle ElapsedTimeTimerHandle;

	float ElapsedTime;

protected:

	int32 ItemsCollected = 0;
	int32 ItemsInLevel = 0;

	// TODO Implement Widget Variables
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<class UUserWidget> GameWidgetClass;

	UPROPERTY()
	URollaBallWidget* GameWidget;

	virtual void BeginPlay() override;

	void UpdateItemText();
	
public: 

	void ItemCollected();

	void UpdateElapsedTime();
	void StopElapsedTime();

};
