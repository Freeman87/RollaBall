// Fill out your copyright notice in the Description page of Project Settings.


#include "RollaBallGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include <YT_RollaBall/Items/RollaBallItemBase.h>
#include "RollaBallWidget.h"

void ARollaBallGameModeBase::BeginPlay()
{


	TArray<AActor*> Items;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARollaBallItemBase::StaticClass(), Items);
	ItemsInLevel = Items.Num();

	if (GameWidgetClass)
	{
		GameWidget = Cast<URollaBallWidget>(CreateWidget(GetWorld(), GameWidgetClass));

		if (GameWidget)
		{
			GameWidget->AddToViewport();
			UpdateItemText();
		}
	}

	 //Start the timer
	GetWorld()->GetTimerManager().SetTimer(ElapsedTimeTimerHandle, this, &ARollaBallGameModeBase::UpdateElapsedTime, 0.1f, true);
}


void ARollaBallGameModeBase::UpdateItemText()
{
	if (GameWidget)
	{
		GameWidget->SetItemText(ItemsCollected, ItemsInLevel);
	}
}

void ARollaBallGameModeBase::ItemCollected()
{
	ItemsCollected++;
	UpdateItemText();

	if (ItemsInLevel == ItemsCollected)
	{

	}
}

void ARollaBallGameModeBase::UpdateElapsedTime()
{
	ElapsedTime += 0.1f; // Increment elapsed time

	if (GameWidget) 
	{
		//FString::Printf(TEXT())
		GameWidget->SetTimerText(ElapsedTime);
	}
}

void ARollaBallGameModeBase::StopElapsedTime()
{
	// Stop the timer
	GetWorld()->GetTimerManager().ClearTimer(ElapsedTimeTimerHandle);
}
