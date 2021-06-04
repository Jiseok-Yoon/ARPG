#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDragDropOperation.generated.h"


// 슬롯 드래그 드랍 작업 객체를 나타내기 위한 클래스
UCLASS()
class ARPG_API USlotDragDropOperation final : 
	public UDragDropOperation
{
	GENERATED_BODY()

public :
	// 드래그를 시작시킨 슬롯 객체를 나타냅니다.
	class UBaseSlot* OriginatedDragSlot;
	
};
