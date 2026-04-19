// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/MayCharacter.h"

AMayCharacter::AMayCharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Characters/May_Castle/SkeletalMeshes/May_Castle.May_Castle'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}
}
