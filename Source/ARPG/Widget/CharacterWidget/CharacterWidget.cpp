#include "CharacterWidget.h"
#include "Components/TextBlock.h"

void UCharacterWidget::SetName(FText newName)
{
	Text_Name->SetText(newName);
}
