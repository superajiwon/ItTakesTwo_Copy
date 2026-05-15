// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/Rose/RoseAnimInstance.h"

void URoseAnimInstance::Select()
{
	bIsSelected = true;
}

// void URoseAnimInstance::AnimNotify_StartGame()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("[Server_StartGame] ServerTravel to Lv_Dungeon"));
// 	GetWorld()->ServerTravel(TEXT("/Game/Maps/Lv_Dungeon"), true);
// }

