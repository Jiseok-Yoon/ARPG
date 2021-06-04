// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Name;

public :
	void SetName(FText newName);
	
};
