// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/Rose/RoseCharacter.h"

#include "Actors/Characters/Players/Rose/RoseAnimInstance.h"


// Sets default values
ARoseCharacter::ARoseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARoseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARoseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// void ARoseCharacter::Selected()
// {
// 	URoseAnimInstance* Anim = Cast<URoseAnimInstance>(GetMesh()->GetAnimInstance());
// 	if (Anim)
// 	{
// 		Anim->Select();
// 	}
// }

void ARoseCharacter::Multicast_Selected_Implementation()
{
	// 실제 애니메이션 실행 로직은 여기에 작성
	URoseAnimInstance* Anim = Cast<URoseAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->Select();
	}
}

