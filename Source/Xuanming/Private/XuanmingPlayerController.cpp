// Copyright Xuanming. All Rights Reserved.

#include "XuanmingPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AXuanmingPlayerController::AXuanmingPlayerController()
{
}

void AXuanmingPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[Xuanming] PlayerController BeginPlay (IsLocal=%d)"), IsLocalController());

	// 只在本地控制器上注册输入映射上下文
	if (IsLocalController() && DefaultMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
