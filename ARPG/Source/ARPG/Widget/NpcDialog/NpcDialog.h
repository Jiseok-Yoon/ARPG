#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"

#include "Struct/NpcInfo/NpcInfo.h"
#include "Struct/NpcDialogInfo/NpcDialogInfo.h"

#include "NpcDialog.generated.h"

DECLARE_MULTICAST_DELEGATE(FNpcDialogEventSignature)

UCLASS()
class ARPG_API UNpcDialog final : 
	public UUserWidget
{
	GENERATED_BODY()

private :
	TSubclassOf<class UClosableWnd> BP_ClosableWnd;
	TSubclassOf<class UNpcShopWnd> BP_NpcShopWnd;

	class UDataTable* DT_ShopInfo;

public :
	// Npc Dialog Widget �� ���� ��� ȣ��� �븮��
	FNpcDialogEventSignature OnDlgClosed;

private :
	// Npc ���� â ��ü�� ��Ÿ���ϴ�.
	class UNpcShopWnd* NpcShopWnd;

	FNpcInfo* NpcInfo;

private :
	// ǥ�õǴ� ��ȭ ������ ��Ÿ���ϴ�.
	FNpcDialogInfo DialogInfos;

	// ���� �� ��° ��ȭ�� ǥ���ϴ� �� ��Ÿ���ϴ�.
	UPROPERTY()
	int32 CurrentDialogIndex;

	// ������ ��ȭ���� ��Ÿ���ϴ�.
	bool bIsLastDialog;


private :
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Exit;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Next;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_GoToFirst;
	
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Shop;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_NpcName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Dialog;

public :
	UNpcDialog(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public :
	// ���̾�α� ��ü �ʱ�ȭ
	void InitializeNpcDialog(FNpcInfo* npcInfo);

private :
	// ���� ��ȭ ��ư�� ���ü��� �����մϴ�.
	void SetNextDialogButtonVisibility(bool bVisible);

	// ������ ������ ��ȭ�� ǥ���մϴ�.
	void ShowDialog(int32 newDialogIndex);


#pragma region ButtonEvent

private :
	// ���̾�α׸� �ʱ�ȭ�մϴ�.
	UFUNCTION()
	void InitializeDialog();

	UFUNCTION()
	void CreateShopWnd();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnNextDialogButtonClicked();

#pragma endregion
	
};
