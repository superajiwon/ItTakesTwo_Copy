// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/View/Main_View.h"
#include "UI/View/CooldownBox.h"

void UMain_View::NativeConstruct()
{
	Super::NativeConstruct();
}

// May 스킬 쿨타임 시작
void UMain_View::StartMaySkill1Cooldown()
{
	if (MaySkill1)
	{
		MaySkill1->StartCooldown();
	}
}

void UMain_View::StartMaySkill2Cooldown()
{
	if (MaySkill2)
	{
		MaySkill2->StartCooldown();
	}
}

void UMain_View::StartMaySkill3Cooldown()
{
	if (MaySkill3)
	{
		MaySkill3->StartCooldown();
	}
}

// Cody 스킬 쿨타임 시작
void UMain_View::StartCodySkill1Cooldown()
{
	if (CodySkill1)
	{
		CodySkill1->StartCooldown();
	}
}

void UMain_View::StartCodySkill2Cooldown()
{
	if (CodySkill2)
	{
		CodySkill2->StartCooldown();
	}
}

void UMain_View::StartCodySkill3Cooldown()
{
	if (CodySkill3)
	{
		CodySkill3->StartCooldown();
	}
}
