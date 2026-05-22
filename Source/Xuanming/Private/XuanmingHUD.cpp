// Copyright Xuanming. All Rights Reserved.

#include "XuanmingHUD.h"
#include "XuanmingCharacter.h"
#include "XuanmingWeapon.h"
#include "Engine/Canvas.h"

AXuanmingHUD::AXuanmingHUD()
{
}

void AXuanmingHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas)
	{
		return;
	}
	DrawCrosshair();
	DrawHealthBar();
	DrawAmmo();
}

void AXuanmingHUD::DrawCrosshair()
{
	const float CX = Canvas->ClipX * 0.5f;
	const float CY = Canvas->ClipY * 0.5f;
	const float S = CrosshairSize;
	// 十字准星：四条短线
	DrawLine(CX - S * 2, CY, CX - S, CY, CrosshairColor, 1.5f);
	DrawLine(CX + S, CY, CX + S * 2, CY, CrosshairColor, 1.5f);
	DrawLine(CX, CY - S * 2, CX, CY - S, CrosshairColor, 1.5f);
	DrawLine(CX, CY + S, CX, CY + S * 2, CrosshairColor, 1.5f);
}

void AXuanmingHUD::DrawHealthBar()
{
	APawn* P = GetOwningPawn();
	AXuanmingCharacter* Ch = Cast<AXuanmingCharacter>(P);
	if (!Ch)
	{
		return;
	}

	const float BarW = 240.0f;
	const float BarH = 18.0f;
	const float X = 40.0f;
	const float Y = Canvas->ClipY - 60.0f;
	const float Ratio = FMath::Clamp(Ch->Health / FMath::Max(1.0f, Ch->MaxHealth), 0.0f, 1.0f);

	// 背景
	DrawRect(FLinearColor(0, 0, 0, 0.5f), X, Y, BarW, BarH);
	// 血条（红->黄->绿）
	FLinearColor HpColor = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, Ratio);
	DrawRect(HpColor, X, Y, BarW * Ratio, BarH);

	// 文字
	const FString HpText = FString::Printf(TEXT("HP %d / %d"),
		FMath::RoundToInt(Ch->Health), FMath::RoundToInt(Ch->MaxHealth));
	DrawText(HpText, FLinearColor::White, X + 4, Y - 18, nullptr, 1.0f);
}

void AXuanmingHUD::DrawAmmo()
{
	APawn* P = GetOwningPawn();
	AXuanmingCharacter* Ch = Cast<AXuanmingCharacter>(P);
	if (!Ch || !Ch->CurrentWeapon)
	{
		return;
	}
	const AXuanmingWeapon* W = Ch->CurrentWeapon;
	const FString AmmoText = FString::Printf(TEXT("AMMO  %d / %d"),
		W->CurrentAmmo, W->MagazineSize);
	const float X = Canvas->ClipX - 220.0f;
	const float Y = Canvas->ClipY - 50.0f;
	DrawText(AmmoText, FLinearColor::Yellow, X, Y, nullptr, 1.4f);
}
