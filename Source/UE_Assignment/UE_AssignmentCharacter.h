// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CrosshairParent.h"
#include "UE_AssignmentCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UPhysicsConstraintComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

//I didn't figure out how to make it visible in blueprints yet!
UENUM(BlueprintType)
enum class RangeStatus :uint8
{
	Grab UMETA(DisplayName = "GrabRange"),
	Pull UMETA(DisplayName = "PullRange"),
	None UMETA(DisplayName = "Projectile")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRangeStatusUpdate, const RangeStatus&, Status);

UCLASS(config=Game)
class AUE_AssignmentCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BallMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* PhysicsConstraint;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UCrosshairParent* CrosshairWidget;
	
public:
	AUE_AssignmentCharacter();

protected:
	virtual void BeginPlay();

	//virtual void Tick(float DeltaTime);
	virtual void Tick(float DeltaTime) override;

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<class UCrosshairParent> CrosshairWidgetClass;

	UPROPERTY(BlueprintAssignable)
	FOnRangeStatusUpdate OnRangeStatusUpdate;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPrimitiveComponent* HeldObject;

	void OnWeaponAttached(const float &grabValue, const float &pullValue);
	
	void Grab();

	void Pull(const float& power);

	void ShootObject(const float& power);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Release();

private:
	bool bHasWeaponAttached{ false };

	FHitResult hitResult;

	bool TraceObjects(const float& distance, FHitResult& hit);

	RangeStatus currentStatus{ RangeStatus::None };
	
	float grabDistance;

	float pullDistance;
};

