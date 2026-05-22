// Copyright Xuanming. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XuanmingHUD.generated.h"

/**
 * 玄冥 HUD - 用 Canvas 画血量、弹药、准星
 * 简单实现，正式版可换成 UMG/Slate
 */
UCLASS()
class XUANMING_API AXuanmingHUD : public AHUD
{
	GENERATED_BODY()

public:
	AXuanmingHUD();

	virtual void DrawHUD() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|HUD")
	FLinearColor CrosshairColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|HUD")
	float CrosshairSize = 8.0f;

protected:
	void DrawCrosshair();
	void DrawHealthBar();
	void DrawAmmo();
};
