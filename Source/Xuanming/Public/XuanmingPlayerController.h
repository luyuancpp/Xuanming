// Copyright Xuanming. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "XuanmingPlayerController.generated.h"

class UInputMappingContext;

/**
 * 玄冥 PlayerController - 玩家的输入和 UI 入口
 * 客户端和服务器各持有一份本玩家的 PC
 */
UCLASS()
class XUANMING_API AXuanmingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AXuanmingPlayerController();

	/** 玩家默认输入映射上下文（在 BP 里指派 IMC 资产） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:
	virtual void BeginPlay() override;
};
