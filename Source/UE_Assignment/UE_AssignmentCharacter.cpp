// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_AssignmentCharacter.h"
#include "UE_AssignmentProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUE_AssignmentCharacter

AUE_AssignmentCharacter::AUE_AssignmentCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Create Mesh and physics Constrains
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeldObjectSocket"));
	BallMesh->SetupAttachment(Mesh1P);
	BallMesh->SetHiddenInGame(true);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(BallMesh);

}

void AUE_AssignmentCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();



}

void AUE_AssignmentCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//Continuously trace if we have weaopn attached to know whether we can grab, pull or just shoot projectile!
	if (HeldObject == nullptr) {
		if (bHasWeaponAttached) {
			if (TraceObjects(grabDistance, hitResult)) {
				if (currentStatus != RangeStatus::Grab) {
					currentStatus = RangeStatus::Grab;
					OnRangeStatusUpdate.Broadcast(currentStatus);
				
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "You are within Grab Range!");
				}
			}
			else if (TraceObjects(pullDistance, hitResult)) {
				if (currentStatus != RangeStatus::Pull) {
					currentStatus = RangeStatus::Pull;
					OnRangeStatusUpdate.Broadcast(currentStatus);

					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "You are within pull Range!");
				}
			}
			else if(currentStatus != RangeStatus::None){
				currentStatus = RangeStatus::None;
				OnRangeStatusUpdate.Broadcast(currentStatus);

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "You are far away!");
			}
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////// Input

void AUE_AssignmentCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE_AssignmentCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE_AssignmentCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AUE_AssignmentCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AUE_AssignmentCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

bool AUE_AssignmentCharacter::TraceObjects(const float& distance, FHitResult& hit) {
	if (GetController() == nullptr)
	{
		return false;
	}

	//We will trace for objects that has collision type physics body, and if found return true!
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector StartLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

	FVector EndLocation = StartLocation + PlayerController->PlayerCameraManager->GetActorForwardVector() * distance;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	bool didHit = GetWorld()->LineTraceSingleByObjectType(hit, StartLocation, EndLocation, ObjectQueryParams, CollisionQueryParams);

	return didHit;
}

void AUE_AssignmentCharacter::OnWeaponAttached(const float &grabValue, const float &pullValue) {
	bHasWeaponAttached = true;

	//Character now is holding a weapon so he need the crosshairWidget to appear!
	CrosshairWidget = CreateWidget<UCrosshairParent>(GetWorld(), CrosshairWidgetClass);
	if (CrosshairWidget != nullptr)	CrosshairWidget->AddToViewport();

	grabDistance = grabValue;
	pullDistance = pullValue;
}

void AUE_AssignmentCharacter::Grab() {

	HeldObject = hitResult.GetComponent();

	BallMesh->SetWorldLocation(hitResult.ImpactPoint,false, nullptr,ETeleportType::TeleportPhysics);

	PhysicsConstraint->SetConstrainedComponents(BallMesh, NAME_None, HeldObject, hitResult.BoneName);

	HeldObject->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AUE_AssignmentCharacter::Pull(const float& power) {
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector direction = -PlayerController->PlayerCameraManager->GetActorForwardVector() * power;

	hitResult.GetComponent()->AddImpulse(direction, hitResult.BoneName);
}

void AUE_AssignmentCharacter::ShootObject(const float& power) {
	PhysicsConstraint->BreakConstraint();

	HeldObject->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector direction = PlayerController->PlayerCameraManager->GetActorForwardVector() * power;

	//HeldObject->AddImpulseAtLocation(direction, hitResult.ImpactPoint, hitResult.BoneName);
	HeldObject->AddImpulse(direction, hitResult.BoneName);

	HeldObject = nullptr;
}

void AUE_AssignmentCharacter::Release() {
	if (HeldObject != nullptr) {
		PhysicsConstraint->BreakConstraint();

		HeldObject->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

		HeldObject = nullptr;
	}
}
