#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Shared/ITTTypes.h"
#include "ITTGameInstance.generated.h"

class UUserWidget;
UCLASS()
class ITTAKESTWO_COPY_API UITTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 방장(Host)이 선택한 캐릭터 역할
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
	EPlayerRole HostSelectedRole = EPlayerRole::None;

	// 클라이언트(Client)가 선택한 캐릭터 역할
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
	EPlayerRole ClientSelectedRole = EPlayerRole::None;
	
	// -------------------------------------------------------------------
	// -------------------------------------------------------------------
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	
public:	
	UFUNCTION(BlueprintCallable)
	void StartPreRenderVFX(UWorld* LoadedWorld);
	
private:
	void BeginLoadingScreen(const FString& MapName);
	void EndLoadingScreen(UWorld* LoadedWorld);
	
	void ShowLoadingWidget(UWorld* World);
	void HideLoadingWidget();
	void SetLocalInputEnabled(UWorld* World, bool bEnabled);
	void FinishLoadingAfterDelay();
	bool ShouldShowLoadingForWorld(UWorld* LoadedWorld) const;
private:
	UFUNCTION()
	void FinishLoadingScreenAfterPreRenderVFX();
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loading", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingWidget;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loading", meta=(AllowPrivateAccess="true"))
	float LoadingHideDelay = 3.f;

	FTimerHandle LoadingHideTimerHandle;

	
	
	
};
