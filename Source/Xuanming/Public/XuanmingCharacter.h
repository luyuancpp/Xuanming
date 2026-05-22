// Copyright Xuanming. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XuanmingCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class AXuanmingWeapon;
struct FInputActionValue;

/**
 * 玄冥 Character - FPS 玩家角色
 * 包含输入处理、相机、武器持有、服务器权威血量与伤害
 */
UCLASS()
class XUANMING_API AXuanmingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AXuanmingCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	// === 输入 IA 资产（在 BP 里指派）===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Input")
	TObjectPtr<UInputAction> IA_Fire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Input")
	TObjectPtr<UInputAction> IA_Crouch;

	// === 状态 ===
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Xuanming|Combat")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Combat")
	float MaxHealth = 100.0f;

	// === 武器 ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xuanming|Weapon")
	TSubclassOf<AXuanmingWeapon> DefaultWeaponClass;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Xuanming|Weapon")
	TObjectPtr<AXuanmingWeapon> CurrentWeapon;

	UFUNCTION()
	void OnRep_Health();

	/** 由武器查询：相机/眼睛位置，用于射线起点 */
	UFUNCTION(BlueprintCallable, Category = "Xuanming|Combat")
	FVector GetEyeLocation() const;

	UFUNCTION(BlueprintCallable, Category = "Xuanming|Combat")
	FRotator GetEyeRotation() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Xuanming|Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	// === 输入处理函数 ===
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_Jump_Started(const FInputActionValue& Value);
	void Input_Jump_Completed(const FInputActionValue& Value);
	void Input_Fire_Started(const FInputActionValue& Value);
	void Input_Fire_Completed(const FInputActionValue& Value);
	void Input_Crouch_Toggled(const FInputActionValue& Value);

	/** 服务器端生成默认武器并附加到角色 */
	void SpawnDefaultWeapon();

	/** 死亡处理（仅服务器） */
	virtual void HandleDeath();
};
