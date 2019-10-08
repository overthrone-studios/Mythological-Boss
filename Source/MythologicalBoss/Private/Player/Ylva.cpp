// Copyright Overthrone Studios 2019

#include "Player/Ylva.h"
#include "Player/YlvaAnimInstance.h"

#include "OverthroneHUD.h"
#include "OverthroneGameInstance.h"
#include "OverthroneGameState.h"

#include "FSM.h"
#include "Log.h"

#include "Widgets/HUD/MasterHUD.h"
#include "Widgets/HUD/FSMVisualizerHUD.h"
#include "Widgets/World/LockOn.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraAnim.h"
#include "Camera/CameraAnimInst.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/ChargeAttackComponent.h"
#include "Components/DashComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SwordComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Misc/FeatData.h"
#include "Misc/HitSoundData.h"

#include "Animation/AnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "DamageTypes/DmgType_Lightning.h"

#include "Engine/Engine.h"

#include "ConstructorHelpers.h"
#include "TimerManager.h"

AYlva::AYlva() : AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Ylva/Animation/ABP_Ylva.ABP_Ylva_C'"));

	// Get the skeletal mesh to use
	SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/Characters/Ylva/SKM_Ylva.SKM_Ylva'")));

	// Configure our mesh
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		if (AnimBP.Succeeded())
			GetMesh()->AnimClass = AnimBP.Class;
		else
			ULog::DebugMessage(ERROR, FString("AnimBP did not succeed."));
	}

	// Create a FSM
	FSM = CreateDefaultSubobject<UFSM>(FName("Player FSM"));
	FSM->AddState(0, "Idle");
	FSM->AddState(1, "Walk");
	FSM->AddState(2, "Run");
	FSM->AddState(4, "Block");
	FSM->AddState(5, "Death");
	FSM->AddState(6, "Charge Attack");
	FSM->AddState(7, "Shocked");
	FSM->AddState(8, "Dash Attack");
	FSM->AddState(12, "Dash");
	FSM->AddState(20, "Damaged");
	FSM->AddState(21, "Shield Hit");
	FSM->AddState(22, "Parry");

	FSM->OnEnterAnyState.AddDynamic(this, &AYlva::OnEnterAnyState);
	FSM->OnUpdateAnyState.AddDynamic(this, &AYlva::UpdateAnyState);
	FSM->OnExitAnyState.AddDynamic(this, &AYlva::OnExitAnyState);

	// Bind state events to our functions
	FSM->GetState(0)->OnEnterState.AddDynamic(this, &AYlva::OnEnterIdleState);
	FSM->GetState(0)->OnUpdateState.AddDynamic(this, &AYlva::UpdateIdleState);
	FSM->GetState(0)->OnExitState.AddDynamic(this, &AYlva::OnExitIdleState);

	FSM->GetState(1)->OnEnterState.AddDynamic(this, &AYlva::OnEnterWalkState);
	FSM->GetState(1)->OnUpdateState.AddDynamic(this, &AYlva::UpdateWalkState);
	FSM->GetState(1)->OnExitState.AddDynamic(this, &AYlva::OnExitWalkState);

	FSM->GetState(2)->OnEnterState.AddDynamic(this, &AYlva::OnEnterRunState);
	FSM->GetState(2)->OnUpdateState.AddDynamic(this, &AYlva::UpdateRunState);
	FSM->GetState(2)->OnExitState.AddDynamic(this, &AYlva::OnExitRunState);

	FSM->GetState(4)->OnEnterState.AddDynamic(this, &AYlva::OnEnterBlockingState);
	FSM->GetState(4)->OnUpdateState.AddDynamic(this, &AYlva::UpdateBlockingState);
	FSM->GetState(4)->OnExitState.AddDynamic(this, &AYlva::OnExitBlockingState);

	FSM->GetState(5)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDeathState);
	FSM->GetState(5)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDeathState);
	FSM->GetState(5)->OnExitState.AddDynamic(this, &AYlva::OnExitDeathState);

	FSM->GetState(6)->OnEnterState.AddDynamic(this, &AYlva::OnEnterChargeAttackState);
	FSM->GetState(6)->OnUpdateState.AddDynamic(this, &AYlva::UpdateChargeAttackState);
	FSM->GetState(6)->OnExitState.AddDynamic(this, &AYlva::OnExitChargeAttackState);

	FSM->GetState(7)->OnEnterState.AddDynamic(this, &AYlva::OnEnterShockedState);
	FSM->GetState(7)->OnUpdateState.AddDynamic(this, &AYlva::UpdateShockedState);
	FSM->GetState(7)->OnExitState.AddDynamic(this, &AYlva::OnExitShockedState);

	FSM->GetState(8)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDashAttackState);
	FSM->GetState(8)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDashAttackState);
	FSM->GetState(8)->OnExitState.AddDynamic(this, &AYlva::OnExitDashAttackState);

	FSM->GetState(12)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDashState);
	FSM->GetState(12)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDashState);
	FSM->GetState(12)->OnExitState.AddDynamic(this, &AYlva::OnExitDashState);

	FSM->GetState(20)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDamagedState);
	FSM->GetState(20)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDamagedState);
	FSM->GetState(20)->OnExitState.AddDynamic(this, &AYlva::OnExitDamagedState);

	FSM->GetState(21)->OnEnterState.AddDynamic(this, &AYlva::OnEnterShieldHitState);
	FSM->GetState(21)->OnUpdateState.AddDynamic(this, &AYlva::UpdateShieldHitState);
	FSM->GetState(21)->OnExitState.AddDynamic(this, &AYlva::OnExitShieldHitState);

	FSM->GetState(22)->OnEnterState.AddDynamic(this, &AYlva::OnEnterParryState);
	FSM->GetState(22)->OnUpdateState.AddDynamic(this, &AYlva::UpdateParryState);
	FSM->GetState(22)->OnExitState.AddDynamic(this, &AYlva::OnExitParryState);

	FSM->InitState(0);

	// Create a camera arm component (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("Camera Arm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	CameraBoom->TargetArmLength = 450.0f; // The camera follows at this distance behind the character	
	CameraBoom->SocketOffset = FVector(0.0f, 70.0f, 0.0f); // The camera follows at this distance behind the character	
	CameraBoom->bEnableCameraLag = true; // Enable camera lag to smooth its movement when following the character
	CameraBoom->bEnableCameraRotationLag = true; // Enable camera rotation lag to smooth its movement when rotating
	CameraBoom->CameraLagSpeed = 20.0f; // The lag speed of the camera following the character
	CameraBoom->CameraRotationLagSpeed = 20.0f; // The rotation lag speed of the camera rotating around the character	
	CameraBoom->CameraLagMaxDistance = 10.0f; // The max distance the camera is allowed to lag behind when following the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(30.0f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));

	// Set our turn rates for input
	TurnRate = 45.0f;
	LookUpRate = 45.0f;

	// Don't rotate mesh when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 2.0f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSettings.WalkSpeed;

	// Stamina component
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(FName("Stamina Component"));

	// Charge attack component
	ChargeAttackComponent = CreateDefaultSubobject<UChargeAttackComponent>(FName("Charge Attack Component"));
	ChargeAttackComponent->OnChargeFilled.AddDynamic(this, &AYlva::OnChargeMeterFull);

	// Attack combo component
	AttackComboComponent = CreateDefaultSubobject<UAttackComboComponent>(FName("Attack Combo Component"));
	AttackComboComponent->OnComboReset.AddDynamic(this, &AYlva::OnComboReset_Implementation);

	// Dash component
	DashComponent = CreateDefaultSubobject<UDashComponent>(FName("Dash component"));

	// Parry collision component
	ParryCollisionComponent = CreateDefaultSubobject<UBoxComponent>(FName("Parry Collision Component"));
	ParryCollisionComponent->SetRelativeLocation(FVector(50.0f, 6.0f, 40.0f));
	ParryCollisionComponent->SetRelativeRotation(FRotator(0.0f));
	ParryCollisionComponent->SetBoxExtent(FVector(20.0f, 25.0f, 30.0f));
	ParryCollisionComponent->SetEnableGravity(false);
	ParryCollisionComponent->bApplyImpulseOnDamage = false;
	ParryCollisionComponent->OnComponentHit.AddDynamic(this, &AYlva::OnParryBoxHit);

	// Configure character settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AYlva::BeginPlay()
{
	Super::BeginPlay();

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bForceFeedbackEnabled = true;

	InitTimeline(StaminaRegenTimeline, StaminaRegenCurve, 1.0f, FName("LoseStamina"), FName("FinishLosingStamina"));
	InitTimeline(ChargeAttackTimeline, ChargeAttackCurve, 1.0f, FName("GainCharge"), FName("FinishGainingCharge"));

	// Initialize our variables
	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	YlvaAnimInstance = Cast<UYlvaAnimInstance>(SKMComponent->GetAnimInstance());
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("FSMVisualizer"));

	LSword = Cast<USwordComponent>(GetLeftHandSword());
	RSword = Cast<USwordComponent>(GetRightHandSword());

	Combat.AttackSettings.OriginalLightAttackDamage = Combat.AttackSettings.LightAttackDamage;
	Combat.AttackSettings.OriginalHeavyAttackDamage = Combat.AttackSettings.HeavyAttackDamage;

	// Set pitch min max values
	CameraManager->ViewPitchMin = 360.0f - CameraPitchMax;
	CameraManager->ViewPitchMax = CameraPitchMin;

	// Initialize player info
	GameState->PlayerData.StartingHealth = HealthComponent->GetDefaultHealth();
	GameState->PlayerData.Health = HealthComponent->GetCurrentHealth();
	GameState->PlayerData.SmoothedHealth = HealthComponent->GetSmoothedHealth();

	GameState->PlayerData.StartingStamina = StaminaComponent->GetDefaultStamina();
	GameState->PlayerData.Stamina = StaminaComponent->GetCurrentStamina();
	GameState->PlayerData.SmoothedStamina = StaminaComponent->GetCurrentStamina();

	GameState->PlayerData.MaxCharge = ChargeAttackComponent->GetMaxCharge();
	GameState->PlayerData.Charge = ChargeAttackComponent->GetCurrentCharge();
	GameState->PlayerData.SmoothedCharge = ChargeAttackComponent->GetCurrentCharge();

	GameState->PlayerData.TeleportRadius = TeleportRadius;
	GameState->Player = this;

	// Bind events to our functions
	GameState->PlayerData.OnLowHealth.AddDynamic(this, &AYlva::OnLowHealth);
	GameState->PlayerData.OnLowStamina.AddDynamic(this, &AYlva::OnLowStamina);
	GameState->BossData.OnDeath.AddDynamic(this, &AYlva::OnBossDeath_Implementation);

	UntouchableFeat = GameInstance->GetFeat("Untouchable");

	AnimInstance->OnMontageEnded.AddDynamic(this, &AYlva::OnAttackEnd_Implementation);

	ParryCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bCanDash = true;

	// Begin the state machine
	FSM->Start();

#if !UE_BUILD_SHIPPING
	CapsuleComp->SetHiddenInGame(false);
	CapsuleComp->SetVisibility(true);

	ParryCollisionComponent->SetHiddenInGame(false);
#else
	CapsuleComponent->SetHiddenInGame(true);
	CapsuleComponent->SetVisibility(false);

	ParryCollisionComponent->SetHiddenInGame(true);
#endif
}

void AYlva::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead)
	{
		return;
	}

	StaminaRegenTimeline.TickTimeline(DeltaTime);
	ChargeAttackTimeline.TickTimeline(DeltaTime);

	ControlRotation = GetControlRotation();
	DistanceToBoss = GetDistanceToBoss();
	DirectionToBoss = GetDirectionToBoss();
	GameState->PlayerData.Location = CurrentLocation;

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->MovementDirection = CalculateDirection();

	YlvaAnimInstance->bIsMoving = IsMovingInAnyDirection() && !IsAttacking();

	CalculateRollLean(DeltaTime);
	CalculatePitchLean(DeltaTime);

	// Lock-on mechanic
	if (IsLockedOn())
	{
		LockOnTo(GameState->BossData.Location, DeltaTime);
	}

	// Stamina regen mechanic
	if (StaminaComponent->IsRegenFinished())
		RegenerateStamina(StaminaComponent->GetRegenRate());

	// Charge loss mechanic
	if (ChargeAttackComponent->CanLoseChargeOvertime() && 
		ChargeAttackComponent->IsLosingCharge() && 
		!ChargeAttackComponent->IsChargeFull() && 
		!ChargeAttackComponent->IsChargeEmpty())
	{
		DecreaseCharge(ChargeAttackComponent->GetChargeLossRate() * DeltaTime);
	}

	// Charge Attack Mechanic
	if (bChargeKeyPressed && ChargeAttackComponent->IsChargeFull())
	{
		ChargeKeyHeldFrames++;
		if (ChargeKeyHeldFrames > Combat.ChargeSettings.MaxChargeKeyHeldFrames && !IsChargeAttacking())
		{
			FSM->PopState();
			FSM->PushState(6); // Charge Attack
		}
	}

	if ((GameState->PlayerData.CurrentRange == BRM_Close || GameState->PlayerData.CurrentRange == BRM_SuperClose) && IsAttacking())
	{
		float RotationSpeed = Combat.AttackSettings.CloseRangeAttackRotationSpeed;

		if (GameState->PlayerData.CurrentRange == BRM_SuperClose)
			RotationSpeed = Combat.AttackSettings.SuperCloseRangeAttackRotationSpeed;

		FaceBoss(DeltaTime, RotationSpeed);
	}

#if !UE_BUILD_SHIPPING
	if (Debug.bShowTeleportRadius)
		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation, TeleportRadius, 32, FColor::Red, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

	OverthroneHUD->UpdateOnScreenDebugMessage(1, "Camera Pitch: " + FString::SanitizeFloat(ControlRotation.Pitch));

	OverthroneHUD->UpdateOnScreenDebugMessage(2, "Player Forward Input: " + FString::SanitizeFloat(ForwardInput));
	OverthroneHUD->UpdateOnScreenDebugMessage(3, "Player Right Input: " + FString::SanitizeFloat(RightInput));

	OverthroneHUD->UpdateOnScreenDebugMessage(4, "Movement Speed: " + FString::SanitizeFloat(CurrentMovementSpeed));

	OverthroneHUD->UpdateOnScreenDebugMessage(5, "Player Health: " + FString::FromInt(HealthComponent->GetCurrentHealth()));
	OverthroneHUD->UpdateOnScreenDebugMessage(6, "Player Stamina: " + FString::FromInt(StaminaComponent->GetCurrentStamina()));

	OverthroneHUD->UpdateOnScreenDebugMessage(8, "Current Attack: " + UOverthroneEnums::PlayerAttackTypeToString(GameState->PlayerData.CurrentAttackType)/* AttackComboComponent->GetCurrentAttackAsString()*/);

	OverthroneHUD->UpdateOnScreenDebugMessage(9, "Player Move Direction: " + FString::SanitizeFloat(AnimInstance->MovementDirection));

	OverthroneHUD->UpdateOnScreenDebugMessage(10, "Player Displayed Health: " + FString::SanitizeFloat(HealthComponent->GetSmoothedHealth()));

	OverthroneHUD->UpdateOnScreenDebugMessage(11, "Player Displayed Stamina: " + FString::SanitizeFloat(StaminaComponent->GetSmoothedStamina()));

	OverthroneHUD->UpdateOnScreenDebugMessage(12, "Moved Right by: " + FString::SanitizeFloat(DistanceMovedInRightDirection));

	OverthroneHUD->UpdateOnScreenDebugMessage(13, "Distance to Spear: " + FString::SanitizeFloat(FVector::Dist(CurrentLocation, GameState->BossData.SpearLocation)));
#endif
}

void AYlva::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Crash if we don't have a valid Input component
	check(PlayerInputComponent);

	// Set up game-play key bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AYlva::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AYlva::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices
	// "Turn" handles devices that provide an absolute delta, such as a mouse.
	// "TurnRate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AYlva::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AYlva::LookUpAtRate);

	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &AYlva::ToggleLockOn);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AYlva::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AYlva::StopBlocking);

	PlayerInputComponent->BindAction("Light Attack", IE_Pressed, this, &AYlva::LightAttack);
	PlayerInputComponent->BindAction("Heavy Attack", IE_Pressed, this, &AYlva::HeavyAttack);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AYlva::Run);
	PlayerInputComponent->BindAction("Run", IE_Repeat, this, &AYlva::UpdateIsRunHeld);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AYlva::StopRunning);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AYlva::Dash);

	PlayerInputComponent->BindAction("Charge Attack", IE_Pressed, this, &AYlva::StartChargeAttack);
	PlayerInputComponent->BindAction("Charge Attack", IE_Released, this, &AYlva::ReleaseChargeAttack);

	PlayerInputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AYlva::Pause).bExecuteWhenPaused = true;
	PlayerInputComponent->BindKey(EKeys::Gamepad_Special_Right, IE_Pressed, this, &AYlva::Pause).bExecuteWhenPaused = true;

#if !UE_BUILD_SHIPPING
	// Debugging
	PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &AYlva::ShowPlayerFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::NumPadOne, IE_Pressed, this, &AYlva::ShowPlayerFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AYlva::ShowBossFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::NumPadTwo, IE_Pressed, this, &AYlva::ShowBossFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &AYlva::ShowMainHUD);
	PlayerInputComponent->BindKey(EKeys::NumPadThree, IE_Pressed, this, &AYlva::ShowMainHUD);
	PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &AYlva::ShowNoHUD);
	PlayerInputComponent->BindKey(EKeys::NumPadFour, IE_Pressed, this, &AYlva::ShowNoHUD);

	PlayerInputComponent->BindKey(EKeys::Five, IE_Pressed, this, &AYlva::ToggleGodMode);
	PlayerInputComponent->BindKey(EKeys::NumPadFive, IE_Pressed, this, &AYlva::ToggleGodMode);

	PlayerInputComponent->BindKey(EKeys::Six, IE_Pressed, this, &AYlva::Debug_Die);
	PlayerInputComponent->BindKey(EKeys::NumPadSix, IE_Pressed, this, &AYlva::Debug_Die);
	PlayerInputComponent->BindKey(EKeys::Seven, IE_Pressed, this, &AYlva::Debug_MaxHealth);
	PlayerInputComponent->BindKey(EKeys::NumPadSeven, IE_Pressed, this, &AYlva::Debug_MaxHealth);
	PlayerInputComponent->BindKey(EKeys::Eight, IE_Pressed, this, &AYlva::Debug_RefillStamina);
	PlayerInputComponent->BindKey(EKeys::NumPadEight, IE_Pressed, this, &AYlva::Debug_RefillStamina);
	PlayerInputComponent->BindKey(EKeys::Nine, IE_Pressed, this, &AYlva::Debug_MaxCharge);
	PlayerInputComponent->BindKey(EKeys::NumPadNine, IE_Pressed, this, &AYlva::Debug_MaxCharge);
	PlayerInputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &AYlva::Debug_ToggleBuff);
	PlayerInputComponent->BindKey(EKeys::NumPadZero, IE_Pressed, this, &AYlva::Debug_ToggleBuff);

	PlayerInputComponent->BindKey(EKeys::L, IE_Pressed, this, &AYlva::Debug_ResetFeats);
	PlayerInputComponent->BindKey(EKeys::B, IE_Pressed, this, &AYlva::Debug_BossStage1);
	PlayerInputComponent->BindKey(EKeys::N, IE_Pressed, this, &AYlva::Debug_BossStage2);
	PlayerInputComponent->BindKey(EKeys::M, IE_Pressed, this, &AYlva::Debug_BossStage3);
	PlayerInputComponent->BindKey(EKeys::J, IE_Pressed, this, &AYlva::SpawnGhost);
	PlayerInputComponent->BindKey(EKeys::P, IE_Pressed, this, &AYlva::Debug_ToggleLowStaminaAnimBlendOut);
#endif
}

void AYlva::MoveForward(const float Value)
{
	ForwardInput = Value;

	if (IsDashing() || bIsDead || IsMoveInputIgnored())
		return;

	if (!IsAttacking())
	{
		AnimInstance->ForwardInput = ForwardInput;
	}
	else
	{
		AnimInstance->ForwardInput = 0.0f;
	}

	if (ForwardInput != 0.0f)
	{
		// Find out which way is forward
		const FRotator Rotation = ControlRotation;
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AYlva::MoveRight(const float Value)
{
	RightInput = Value;

	if (IsDashing() || bIsDead || IsMoveInputIgnored())
		return;

	if (!IsAttacking())
	{
		AnimInstance->RightInput = RightInput;
	}
	else
	{
		AnimInstance->RightInput = 0.0f;
	}

	if (RightInput != 0.0f)
	{
		// Find out which way is right
		const FRotator Rotation = ControlRotation;
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, Value);

		RightMovementEnd = CurrentLocation;
	}
	else
	{
		DistanceMovedInRightDirection = 0.0f;
		RightMovementStart = CurrentLocation;
		RightMovementEnd = CurrentLocation;
	}
}

void AYlva::AddControllerYawInput(const float Val)
{
	if (IsLockedOn())
		return;
	
	Super::AddControllerYawInput(Val);
}

void AYlva::TurnAtRate(const float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRate * World->DeltaTimeSeconds);
}

void AYlva::LookUpAtRate(const float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * LookUpRate * World->DeltaTimeSeconds);
}

void AYlva::LockOnTo(const FVector& TargetLocation, const float DeltaTime)
{
	const FRotator& Target = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	const FRotator& SmoothedRotation = FMath::RInterpTo(ControlRotation, Target, DeltaTime, LockOnRotationSpeed);

	const float& NewPitch = FMath::Clamp(Target.Pitch, LockOnPitchMin, LockOnPitchMax);

	const FRotator& NewRotation = FRotator(-NewPitch, SmoothedRotation.Yaw, ControlRotation.Roll);

	PlayerController->SetControlRotation(NewRotation);
}

void AYlva::FaceBoss(const float DeltaTime, const float RotationSpeed)
{
	SetActorRotation(FMath::Lerp(CurrentRotation, FRotator(CurrentRotation.Pitch, DirectionToBoss.Rotation().Yaw, CurrentRotation.Roll), RotationSpeed * DeltaTime));
}

void AYlva::FaceBoss_Instant()
{
	SetActorRotation(FRotator(CurrentRotation.Pitch, DirectionToBoss.Rotation().Yaw, CurrentRotation.Roll));
}

float AYlva::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead or while dashing
	if (bIsDead || bPerfectlyTimedDash || IsDashAttacking() || IsDashing() && GameState->BossData.CurrentCounterType == ACM_NoCounter)
	{
		return DamageAmount;
	}

	BeginTakeDamage(DamageAmount);

	// Apply damage once
	if (HealthComponent->GetCurrentHealth() > 0.0f && (!AnimInstance->bIsHit || !YlvaAnimInstance->bIsShieldHit))
	{
		ApplyDamage(DamageAmount, DamageEvent);
	}

	EndTakeDamage();

	return DamageAmount;
}

void AYlva::Die()
{
	Super::Die();

	YlvaAnimInstance->LeaveAllStates();

	FSM->RemoveAllStatesFromStack();
	FSM->PushState("Death");
}

void AYlva::Respawn()
{
	TimerManager->ClearTimer(DeathExpiryTimerHandle);

	FSM->PopState();

	GameState->PlayerData.bIsDead = false;

	UGameplayStatics::OpenLevel(this, *UGameplayStatics::GetCurrentLevelName(this));
}

void AYlva::ChangeHitboxSize(const float NewRadius)
{
	Combat.AttackSettings.AttackRadius = NewRadius;
}

void AYlva::UpdateCharacterInfo()
{
	GameState->PlayerData.Health = HealthComponent->GetCurrentHealth();
	GameState->PlayerData.Stamina = StaminaComponent->GetCurrentStamina();
	GameState->PlayerData.Charge = ChargeAttackComponent->GetSmoothedCharge();

	GameState->PlayerData.SmoothedHealth = HealthComponent->GetSmoothedHealth();
	GameState->PlayerData.SmoothedStamina = StaminaComponent->GetSmoothedStamina();
	GameState->PlayerData.SmoothedCharge = ChargeAttackComponent->GetSmoothedCharge();
}

void AYlva::BroadcastLowHealth()
{
	GameState->PlayerData.OnLowHealth.Broadcast();
	bWasLowHealthEventTriggered = true;
}

void AYlva::BroadcastLowStamina()
{
	GameState->PlayerData.OnLowStamina.Broadcast();
	bWasLowStaminaEventTriggered = true;
}

float AYlva::CalculateDirection() const
{
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(FollowCamera->GetComponentRotation(), CapsuleComp->GetComponentRotation());
	const FRotator NewDirection = UKismetMathLibrary::NormalizedDeltaRotator(DeltaRotation, UKismetMathLibrary::MakeRotFromX(FVector(ForwardInput, -RightInput, 0.0f)));

	return NewDirection.GetNormalized().Yaw;
}

void AYlva::CalculateRollLean(const float DeltaTime)
{
	if (!IsIdle() && !IsBlocking() && !bIsDead)
	{
		const float Turn = FMath::Clamp(GetInputAxisValue("Turn"), -1.0f, 1.0f);
		const float InterpSpeed = IsMovingInAnyDirection() ? 1.0f : 10.0f;

		PlayerLeanRollAmount = FMath::FInterpTo(PlayerLeanRollAmount, Turn, DeltaTime, InterpSpeed);
		YlvaAnimInstance->LeanRollAmount = PlayerLeanRollAmount * MovementSettings.LeanRollOffset;
	}
	else
	{
		PlayerLeanRollAmount = FMath::FInterpTo(PlayerLeanRollAmount, 0.0f, DeltaTime, 10.0f);
		YlvaAnimInstance->LeanRollAmount = PlayerLeanRollAmount * MovementSettings.LeanRollOffset;
	}
}

void AYlva::CalculatePitchLean(const float DeltaTime)
{
	if (!IsMovingInAnyDirection() && !IsAttacking() && !bIsDead && FVector::DotProduct(ForwardVector, FollowCamera->GetForwardVector()) > 0.5f)
	{
		PlayerLeanPitchAmount = FMath::FInterpTo(PlayerLeanPitchAmount, FollowCamera->GetForwardVector().Rotation().Pitch, DeltaTime, 2.0f);
		YlvaAnimInstance->LeanPitchAmount = PlayerLeanPitchAmount * MovementSettings.LeanPitchOffset;
	}
	else
	{
		PlayerLeanPitchAmount = FMath::FInterpTo(PlayerLeanPitchAmount, 0.0f, DeltaTime, 1.0f);
		YlvaAnimInstance->LeanPitchAmount = PlayerLeanPitchAmount * MovementSettings.LeanPitchOffset;
	}
}

bool AYlva::HasMovedRightBy(const float Distance)
{
	DistanceMovedInRightDirection = FVector::Dist(RightMovementStart, RightMovementEnd);

	if (IsMovingRight())
		return DistanceMovedInRightDirection >= Distance;

	return false;
}

bool AYlva::HasMovedLeftBy(const float Distance)
{
	DistanceMovedInRightDirection = FVector::Dist(RightMovementStart, RightMovementEnd);

	if (IsMovingLeft())
		return DistanceMovedInRightDirection >= Distance;

	return false;
}

void AYlva::OnAttackLanded()
{
	if (GameState->PlayerData.CurrentAttackType == ATP_Special)
	{
		IncreaseHealth(ChargeAttackComponent->GetHealthGainOnChargeAttack());
	}
}

float AYlva::GetDistanceToBoss() const
{
	return FVector::Dist(GameState->BossData.Location, CurrentLocation);
}

FVector AYlva::GetDirectionToBoss() const
{
	return (GameState->BossData.Location - CurrentLocation).GetSafeNormal(0.01f);
}

#pragma region Combat
void AYlva::LightAttack()
{
	// Are we in any of these states?
	if (bIsDead || IsDamaged() || IsChargeAttacking() || IsDashAttacking())
		return;

	if (bPerfectlyTimedDash)
	{
		FSM->PopState();
		FSM->PushState("Dash Attack");
		return;
	}

	// Finish the parry event early if we decide to attack
	if (IsParrying())
		FSM->PopState();

	// Queue the attack, if we can
	if (IsAttacking() && 
		AnimInstance->Montage_GetPosition(AttackComboComponent->GetCurrentAttackAnim()) > Combat.AttackQueue.LightAttackTriggerTime && 
		AttackQueue.IsEmpty())
	{
		AttackQueue.Pop();
		AttackQueue.Enqueue(ATP_Light);

		// Set a timer to clear the attack queue after 'X' seconds. Prevents auto attacking when the difference (in seconds) is too long.
		if (!TimerManager->IsTimerActive(TH_AttackQueueExpiry))
			TimerManager->SetTimer(TH_AttackQueueExpiry, this, &AYlva::ClearAttackQueue, Combat.AttackQueue.ExpiryTime, false);
		
		TimerManager->SetTimer(TH_AttackQueue, this, &AYlva::Attack_Queued, 0.1f);

		#if !UE_BUILD_SHIPPING
		if (Debug.bLogAttackQueue)
			ULog::Info(CUR_FUNC + ": Queueing attack...", true);
		#endif
		return;
	}

	if (StaminaComponent->HasEnoughForLightAttack() && // Do we have enough stamina for light attack?
		AttackComboComponent->CanAttack() &&
		!AttackComboComponent->IsAtTreeEnd() && // Are we not at the combo tree's end?
		!IsDashing() && 
		AttackQueue.IsEmpty())
	{
		UAnimMontage* AttackMontageToPlay = AttackComboComponent->AdvanceCombo(ATP_Light);

		CurrentForceFeedback = Combat.LightAttackForce;
		GameState->PlayerData.CurrentAttackType = AttackComboComponent->GetCurrentAttack();

		FSM->PopState();
		
		BeginLightAttack(AttackMontageToPlay);
	}
}

void AYlva::BeginLightAttack(class UAnimMontage* AttackMontage)
{
	OnBeginLightAttack(); // Handled in blueprints

	if (IsLockedOn())
		MovementComponent->bUseControllerDesiredRotation = false;

	Combat.AttackSettings.LightAttackDamage *= AttackComboComponent->GetDamageMultiplier();

	if (!bGodMode)
	{
		UpdateStamina(StaminaComponent->GetLightAttackValue());
	}

	if ((StaminaComponent->IsLowStamina() || StaminaComponent->IsStaminaEmpty()) && bEnableBlendOutOnLowStamina)
	{
		PreviousAttackMontageBlendOutTime = AttackMontage->BlendOut.GetBlendTime();
		PreviousAttackMontageBlendOutTriggerTime = AttackMontage->BlendOutTriggerTime;

		AttackMontage->BlendOut.SetBlendTime(BlendOutTimeOnLowStamina);
		AttackMontage->BlendOutTriggerTime = AttackMontage->SequenceLength - BlendOutTriggerTimeOnLowStamina;
	}

	AnimInstance->Montage_Play(AttackMontage);
}

void AYlva::HeavyAttack()
{
	// Are we in any of these states?
	if (bIsDead || IsDamaged() || IsChargeAttacking() || IsDashAttacking())
		return;

	if (bPerfectlyTimedDash)
	{
		FSM->PopState();
		FSM->PushState("Dash Attack");
		return;
	}

	// Finish the parry event early if we decide to attack
	if (IsParrying())
		FSM->PopState();

	// Queue the attack, if we can
	if (IsAttacking() && 
		AnimInstance->Montage_GetPosition(AttackComboComponent->GetCurrentAttackAnim()) > Combat.AttackQueue.HeavyAttackTriggerTime && 
		AttackQueue.IsEmpty())
	{
		AttackQueue.Pop();
		AttackQueue.Enqueue(ATP_Heavy);
		
		// Set a timer to clear the attack queue after 'X' seconds. Prevents auto attacking when the difference (in seconds) is too long.
		if (!TimerManager->IsTimerActive(TH_AttackQueueExpiry))
			TimerManager->SetTimer(TH_AttackQueueExpiry, this, &AYlva::ClearAttackQueue, Combat.AttackQueue.ExpiryTime, false);
	
		TimerManager->SetTimer(TH_AttackQueue, this, &AYlva::Attack_Queued, 0.1f);

		#if !UE_BUILD_SHIPPING
		if (Debug.bLogAttackQueue)
			ULog::Info(CUR_FUNC + ": Queueing attack...", true);
		#endif
		return;
	}

	if (StaminaComponent->HasEnoughForHeavyAttack() && // Do we have enough stamina for heavy attack?
		AttackComboComponent->CanAttack() && 
		!AttackComboComponent->IsAtTreeEnd() && // Are we not at the combo tree's end?
		!IsDashing() && 
		AttackQueue.IsEmpty())
	{
		UAnimMontage* AttackMontageToPlay = AttackComboComponent->AdvanceCombo(ATP_Heavy);

		CurrentForceFeedback = Combat.HeavyAttackForce;
		GameState->PlayerData.CurrentAttackType = AttackComboComponent->GetCurrentAttack();

		FSM->PopState();

		BeginHeavyAttack(AttackMontageToPlay);
	}
}

void AYlva::BeginHeavyAttack(class UAnimMontage* AttackMontage)
{
	OnBeginHeavyAttack(); // Handled in blueprints

	if (IsLockedOn())
		MovementComponent->bUseControllerDesiredRotation = false;

	Combat.AttackSettings.HeavyAttackDamage *= AttackComboComponent->GetDamageMultiplier();

	if (!bGodMode)
	{
		UpdateStamina(StaminaComponent->GetHeavyAttackValue());
	}

	if ((StaminaComponent->IsLowStamina() || StaminaComponent->IsStaminaEmpty()) && bEnableBlendOutOnLowStamina)
	{
		AttackMontage->BlendOut.SetBlendTime(BlendOutTimeOnLowStamina);
		AttackMontage->BlendOutTriggerTime = AttackMontage->SequenceLength - BlendOutTriggerTimeOnLowStamina;
	}

	AnimInstance->Montage_Play(AttackMontage);
}

void AYlva::Attack_Queued()
{
	EAttackType_Player AttackType;
	
	AttackQueue.Peek(AttackType);
	AttackQueue.Pop();
	
	switch (AttackType)
	{
	case ATP_Light:
		LightAttack();
	break;

	case ATP_Heavy:
		HeavyAttack();
	break;

	default:
	break;
	}
}

void AYlva::ClearAttackQueue()
{
	AttackQueue.Pop();

#if !UE_BUILD_SHIPPING
	if (Debug.bLogAttackQueue)
		ULog::Info("Attack Queue cleared!", true);
#endif
}

void AYlva::StartChargeAttack()
{
	bChargeKeyPressed = true;
}

void AYlva::ReleaseChargeAttack()
{
	ChargeKeyHeldFrames = 0;
	bChargeKeyPressed = false;

	if (YlvaAnimInstance->bChargeReleased && !IsChargeAttacking())
		return;

	FSM->PopState("Charge Attack");
}

void AYlva::FinishChargeAttack()
{
	ChargeAttackHoldFrames = 0;

	GameState->PlayerData.CurrentAttackType = ATP_None;

	YlvaAnimInstance->bCanCharge = false;
	YlvaAnimInstance->bChargeReleased = false;
}

void AYlva::Block()
{
	if (bIsDead || IsChargeAttacking() || IsDashing()  || IsDamaged() || IsParrying() || IsDashAttacking())
		return;

	FSM->PopState();
	FSM->PushState("Block");
}

void AYlva::StopBlocking()
{
	if (bIsDead)
		return;

	FSM->PopState("Block");
}

void AYlva::BeginTakeDamage(float DamageAmount)
{
	GameState->PlayerData.bHasTakenDamage = true;
}

void AYlva::ApplyDamage(const float DamageAmount, const FDamageEvent& DamageEvent)
{
	if (IsParrySuccessful() && GameState->IsBossAttackParryable())
	{
		FSM->PopState();
		FSM->PushState("Parry");
		return;
	}

	if (bGodMode || IsParrying())
		return;

	if (IsDashing() && FVector::Dist(CurrentLocation, GameState->BossData.SpearLocation) < 200.0f)
		return;

	// Test against states
	switch (FSM->GetActiveStateID())
	{
		case 4 /*Block*/:
			// Shake the camera
			GameState->CurrentCameraShake = CameraManager->PlayCameraShake(CameraShakes.ShieldHit.Shake, CameraShakes.ShieldHit.Intensity);

			// Are we facing away from the boss?
			if (FVector::DotProduct(ForwardVector, DirectionToBoss) < 0.3f)
			{
				FSM->PushState("Damaged");

				UpdateHealth(DamageAmount);
				
				LSword->Revert();
				RSword->Revert();

				return;
			}

			FSM->PushState("Shield Hit");

			// Update stats
			if (StaminaComponent->IsLowStamina() || GameState->IsBossAttackNoCounter() || GameState->IsBossAttackParryable())
			{
				HitCounter++;
				bHasBeenDamaged = true;
				
				LSword->Revert();
				RSword->Revert();

				UpdateHealth(DamageAmount * Combat.BlockSettings.DamageBuffer);
			}

			UpdateStamina(StaminaComponent->GetShieldHitValue());
		break;

		default:
			HitCounter++;

			bHasBeenDamaged = true;

			// If we got hit while charge attacking
			if (IsChargeAttacking())
				FSM->PopState();

			// Determine the damage state to enter in
			if (DamageEvent.DamageTypeClass == UDmgType_Lightning::StaticClass())
				FSM->PushState("Shocked");
			else
				FSM->PushState("Damaged");

			// Shake the camera
			GameState->CurrentCameraShake = CameraManager->PlayCameraShake(CameraShakes.Damaged.Shake, CameraShakes.Damaged.Intensity);

			UpdateHealth(DamageAmount);

			// Determine whether to reset the charge meter or not
			if (ChargeAttackComponent->WantsResetAfterMaxHits() && HitCounter == ChargeAttackComponent->GetMaxHits())
			{
				HitCounter = 0;
				ResetCharge();
			}
			else
			{
				DecreaseCharge();
			}

			LSword->Revert();
			RSword->Revert();
		break;
	}
}

void AYlva::EndTakeDamage()
{
	if (HealthComponent->GetCurrentHealth() <= 0.0f && !bIsDead)
	{
		Die();
	}
}
#pragma endregion

#pragma region Movement
void AYlva::Run()
{
	if (bIsDead || IsChargeAttacking() || IsDashAttacking())
		return;

	bCanRun = !bCanRun;

	if (IsLockedOn() && StaminaComponent->HasStamina() && bCanRun || !IsLockedOn() && IsMovingInAnyDirection() && bCanRun)
	{
		FSM->PopState();
		FSM->PushState("Run");
	}
}

void AYlva::UpdateIsRunHeld()
{
	bIsRunKeyHeld = true;
}

void AYlva::StopRunning()
{
	bIsRunKeyHeld = false;

	if (bIsDead || bCanRun)
		return;

	// Delay stamina regeneration if we were running
	if (IsRunning())
	{

		FSM->PopState("Run");
	}
}

void AYlva::Dash()
{
	AnimInstance->ForwardInput = ForwardInput;
	AnimInstance->RightInput = RightInput;

	if (IsDashAttacking())
		return;

	if (IsAttacking())
	{
		StopAnimMontage();
		AttackComboComponent->ClearCurrentAttack();
	}

	// Man, I really wanted to put this in one line, but nooooo XCode is being a dick
	if (IsRunning())
		bWasRunning = true;
	else
		bWasRunning = false;

	if ((bIsDead || IsChargeAttacking() || IsBlocking()) && TimerManager->IsTimerActive(TH_DashQueue))
		return;

	if (IsDashing() && DashQueue.IsEmpty() && !bCanDash && !TimerManager->IsTimerActive(TH_DashQueue))
	{
		DashQueue.Enqueue(1);

	#if !UE_BUILD_SHIPPING
		if (Debug.bLogDashQueue)
			ULog::Info("Queuing dash...", true);
	#endif
		return;
	}

	if (!DashComponent->IsCooldownActive() && !IsDashing())
	{
		bCanDash = false;

		// Do we have enough stamina to dash?
		if (StaminaComponent->HasEnoughForDash())
		{
			if (!bGodMode)
			{
				StartDashCooldown();

				if (DashQueue.IsEmpty())
					UpdateStamina(StaminaComponent->GetDashValue());
			}

			FSM->PushState("Dash");
		}
	}
}

void AYlva::StartDashCooldown()
{
	DashComponent->StartCooldown();
}

void AYlva::Dash_Queued()
{
	Dash();
	DashComponent->UnPauseCooldown();

	DashQueue.Pop();

	bCanDash = true;
}
#pragma endregion

#pragma region Lock-On
void AYlva::ToggleLockOn()
{
	// Don't lock on if boss is dead
	if (GameState->BossData.bIsDead || bIsDead)
		return;

	LockOnSettings.bLockedOn = !LockOnSettings.bLockedOn;

	LockOnSettings.bLockedOn ? EnableLockOn() : DisableLockOn();
}

void AYlva::EnableLockOn()
{
	// Don't lock on if boss is dead OR if we are dead
	if (GameState->BossData.bIsDead || bIsDead)
		return;

	LockOnSettings.bLockedOn = true;
	PlayerController->SetIgnoreLookInput(true);
	GameState->LockOn->OnToggleLockOn.Broadcast(false);
	YlvaAnimInstance->bIsLockedOn = true;
	CameraBoom->CameraRotationLagSpeed *= 4;

	if (!IsRunning() && !IsDashing())
	{
		MovementComponent->bOrientRotationToMovement = false;
		MovementComponent->bUseControllerDesiredRotation = true;

		MovementComponent->MaxWalkSpeed = MovementSettings.LockOnWalkSpeed;
	}
}

void AYlva::DisableLockOn()
{
	LockOnSettings.bLockedOn = false;
	PlayerController->SetIgnoreLookInput(false);
	GameState->LockOn->OnToggleLockOn.Broadcast(true);
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->bUseControllerDesiredRotation = false;
	YlvaAnimInstance->bIsLockedOn = false;
	CameraBoom->CameraRotationLagSpeed = 20.0f;

	if (!IsRunning())
		MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
}
#pragma endregion

#pragma region Controls
void AYlva::Pause()
{
	if (GameState->IsGamePaused())
	{
		OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(2);
		OverthroneHUD->GetMasterHUD()->HideDebugInfo();

		GameState->UnPauseGame();
	}
	else
	{
		OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(3);
		OverthroneHUD->GetMasterHUD()->HideDebugInfo();
		
		GameState->PauseGame();
	}
}
#pragma endregion

#if !UE_BUILD_SHIPPING
#pragma region Debug
void AYlva::Debug_Die()
{
	UpdateHealth(HealthComponent->GetCurrentHealth());

	UpdateCharacterInfo();

	Die();
}

void AYlva::Debug_ResetFeats()
{
	GameInstance->ResetFeats();
}

void AYlva::Debug_MaxHealth()
{
	HealthComponent->ResetHealth();

	UpdateCharacterInfo();
}

void AYlva::Debug_RefillStamina()
{
	StaminaComponent->ResetStamina();

	UpdateCharacterInfo();
}

void AYlva::Debug_MaxCharge()
{
	ChargeAttackComponent->SetCharge(ChargeAttackComponent->GetMaxCharge());

	UpdateCharacterInfo();
}

void AYlva::Debug_ToggleBuff()
{
	bIsBuffed = !bIsBuffed;

	if (bIsBuffed)
	{
		OverthroneHUD->GetMasterHUD()->HighlightBox(9);
		
		Combat.AttackSettings.LightAttackDamage *= 2;
		Combat.AttackSettings.HeavyAttackDamage *= 2;
		Combat.AttackSettings.AttackRadius *= 2;
	}
	else
	{
		OverthroneHUD->GetMasterHUD()->UnhighlightBox(9);

		Combat.AttackSettings.LightAttackDamage /= 2;
		Combat.AttackSettings.HeavyAttackDamage /= 2;
		Combat.AttackSettings.AttackRadius /= 2;
	}
}

void AYlva::Debug_ToggleLowStaminaAnimBlendOut()
{
	bEnableBlendOutOnLowStamina = !bEnableBlendOutOnLowStamina;

	if (bEnableBlendOutOnLowStamina)
		OverthroneHUD->GetMasterHUD()->HighlightBox(16);
	else
		OverthroneHUD->GetMasterHUD()->UnhighlightBox(16);
}

void AYlva::Debug_BossStage1()
{
	GameState->EnterBossStage(Stage_1);
}

void AYlva::Debug_BossStage2()
{
	GameState->EnterBossStage(Stage_2);
}

void AYlva::Debug_BossStage3()
{
	GameState->EnterBossStage(Stage_3);
}

void AYlva::ShowPlayerFSMVisualizer()
{
	OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(0);

	OverthroneHUD->bHideDebugText = false;
}

void AYlva::ShowBossFSMVisualizer()
{
	OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(1);

	OverthroneHUD->bHideDebugText = false;
}

void AYlva::ShowMainHUD()
{
	OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(2);
	OverthroneHUD->GetMasterHUD()->HideTitle();
	OverthroneHUD->GetMasterHUD()->HideBoxes();

	OverthroneHUD->bHideDebugText = false;
}

void AYlva::ShowNoHUD()
{
	switch (OverthroneHUD->GetMasterHUD()->GetVisibility())
	{
	case ESlateVisibility::Visible:
		OverthroneHUD->GetMasterHUD()->SetVisibility(ESlateVisibility::Hidden);
	break;

	case ESlateVisibility::Hidden:
		OverthroneHUD->GetMasterHUD()->SetVisibility(ESlateVisibility::Visible);
	break;

	default:
	break;
	}

	OverthroneHUD->bHideDebugText = !OverthroneHUD->bHideDebugText;
}
#pragma endregion
#endif

#pragma region Stamina
void AYlva::RegenerateStamina(const float Rate)
{
	if (bGodMode || bIsDead)
		return;

	IncreaseStamina(Rate * World->DeltaTimeSeconds);
}

void AYlva::UpdateStamina(const float StaminaToSubtract)
{
	StaminaComponent->UpdatePreviousStamina();

	// Stop animating displayed stamina
	if (StaminaRegenTimeline.IsPlaying())
		StaminaRegenTimeline.Stop();

	DecreaseStamina(StaminaToSubtract);

	if (StaminaComponent->GetDecreaseDelay() > 0.0f)
	{
		TimerManager->SetTimer(StaminaComponent->GetDelayTimerHandle(), this, &AYlva::StartLosingStamina, StaminaComponent->GetDecreaseDelay(), false);
	}
	else
		StartLosingStamina();

	StaminaComponent->DelayRegeneration();
}

void AYlva::IncreaseStamina(const float Amount)
{
	StaminaComponent->IncreaseStamina(Amount);

	UpdateCharacterInfo();

	if (!StaminaComponent->IsLowStamina() && bWasLowStaminaEventTriggered)
	{
		MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

		bWasLowStaminaEventTriggered = false;
	}
}

void AYlva::DecreaseStamina(const float Amount)
{
	StaminaComponent->DecreaseStamina(Amount);

	UpdateCharacterInfo();

	// Are we on low stamina?
	if (StaminaComponent->IsLowStamina() && !bWasLowStaminaEventTriggered)
	{
		BroadcastLowStamina();
	}
}

void AYlva::SetStamina(const float NewStaminaAmount)
{
	StaminaComponent->SetStamina(NewStaminaAmount);

	UpdateCharacterInfo();

	// Are we on low stamina?
	if (StaminaComponent->IsLowStamina() && !bWasLowStaminaEventTriggered)
	{
		BroadcastLowStamina();
	}
	else if (!StaminaComponent->IsLowStamina() && bWasLowStaminaEventTriggered)
	{
		MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

		bWasLowStaminaEventTriggered = false;
	}
}

void AYlva::ResetStamina()
{
	StaminaComponent->ResetStamina();

	UpdateCharacterInfo();

	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
}

void AYlva::StartLosingStamina()
{
	StaminaRegenTimeline.PlayFromStart();
}

void AYlva::LoseStamina()
{
	const float Alpha = StaminaRegenCurve->GetFloatValue(StaminaRegenTimeline.GetPlaybackPosition());

	StaminaComponent->SetSmoothedStamina(FMath::Lerp(StaminaComponent->GetPreviousStamina(), StaminaComponent->GetCurrentStamina(), Alpha));

	UpdateCharacterInfo();
}

void AYlva::FinishLosingStamina()
{
	StaminaComponent->UpdatePreviousStamina();

	UpdateCharacterInfo();
}
#pragma endregion

#pragma region Charge Attack
void AYlva::IncreaseCharge()
{
	if (IsLightAttacking())
		StartGainingCharge(ChargeAttackComponent->GetChargeGainForLight());
	else if (IsHeavyAttacking())
		StartGainingCharge(ChargeAttackComponent->GetChargeGainForHeavy());

	UpdateCharacterInfo();

	if (ChargeAttackComponent->CanLoseChargeOvertime() && !ChargeAttackComponent->IsChargeFull())
		ChargeAttackComponent->DelayChargeLoss();
}

void AYlva::DecreaseCharge()
{
	ChargeAttackComponent->DecreaseCharge(ChargeAttackComponent->GetChargeLoss());

	UpdateCharacterInfo();
}

void AYlva::DecreaseCharge(const float Amount)
{
	ChargeAttackComponent->DecreaseCharge(Amount);

	UpdateCharacterInfo();
}

void AYlva::ResetCharge()
{
	ChargeAttackComponent->ResetCharge();

	UpdateCharacterInfo();
}

void AYlva::StartGainingCharge(const float Amount)
{
	ChargeAttackComponent->IncreaseCharge(Amount);

	ChargeAttackTimeline.PlayFromStart();
}

void AYlva::GainCharge()
{
	const float Time = ChargeAttackCurve->GetFloatValue(ChargeAttackTimeline.GetPlaybackPosition());

	ChargeAttackComponent->SetSmoothedCharge(FMath::Lerp(ChargeAttackComponent->GetPreviousCharge(), ChargeAttackComponent->GetCurrentCharge(), Time));

	UpdateCharacterInfo();
}

void AYlva::FinishGainingCharge()
{
	if (ChargeAttackComponent->CanLoseChargeOvertime() && !ChargeAttackComponent->IsChargeFull())
		ChargeAttackComponent->DelayChargeLoss();

	UpdateCharacterInfo();
}
#pragma endregion

#pragma region God Mode
void AYlva::EnableGodMode()
{
	bGodMode = true;

	OverthroneHUD->GetMasterHUD()->HighlightBox(4 /*God mode box*/);

	ResetHealth();
	ResetStamina();
}

void AYlva::DisableGodMode()
{
	bGodMode = false;

	OverthroneHUD->GetMasterHUD()->UnhighlightBox(4 /*God mode box*/);
}

void AYlva::ToggleGodMode()
{
	bGodMode = !bGodMode;

	if (bGodMode)
	{
		OverthroneHUD->GetMasterHUD()->HighlightBox(4 /*God mode box*/);

		ResetHealth();
		ResetStamina();
	}
	else
	{
		OverthroneHUD->GetMasterHUD()->UnhighlightBox(4 /*God mode box*/);
	}

	OverthroneHUD->UpdateOnScreenDebugMessage(7, "God mode: " + FString(bGodMode ? "On" : "Off"));
}
#pragma endregion

#pragma region Events
void AYlva::OnLowHealth()
{
	ChangeHitboxSize(Combat.AttackSettings.AttackRadiusOnLowHealth);
}

void AYlva::OnBossDeath_Implementation()
{
	OnBossDeath();

	DisableLockOn();

	if (!HasTakenAnyDamage() && !UntouchableFeat->bIsComplete)
		OnUntouchableFeatAchieved();
}

void AYlva::OnLowStamina()
{
	// Todo Implement function
}

void AYlva::OnComboReset_Implementation()
{
	OnComboReset();

	Combat.AttackSettings.LightAttackDamage = Combat.AttackSettings.OriginalLightAttackDamage;
	Combat.AttackSettings.HeavyAttackDamage = Combat.AttackSettings.OriginalHeavyAttackDamage;
}

void AYlva::OnAttackEnd_Implementation(UAnimMontage* Montage, const bool bInterrupted)
{
	OnAttackEnd(Montage, bInterrupted);

	if (IsLockedOn())
		MovementComponent->bUseControllerDesiredRotation = true;

	if (!bInterrupted)
	{
		GameState->PlayerData.CurrentAttackType = ATP_None;
		AttackComboComponent->ClearCurrentAttack();
	}

	if ((StaminaComponent->IsLowStamina() || StaminaComponent->IsStaminaEmpty()) && bEnableBlendOutOnLowStamina)
	{
		Montage->BlendOut.SetBlendTime(PreviousAttackMontageBlendOutTime); 
		Montage->BlendOutTriggerTime = PreviousAttackMontageBlendOutTriggerTime;
	}
}

void AYlva::OnChargeMeterFull()
{
	RSword->Glow();
	LSword->Glow();
}

void AYlva::OnParryBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	bIsParryBoxHit = true;
	GameState->PlayerData.bParrySucceeded = true;
}
#pragma endregion

#pragma region Any States
void AYlva::OnEnterAnyState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());
}

void AYlva::UpdateAnyState()
{
	FSMVisualizer->UpdateStateFrames(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateFrames());
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
}

void AYlva::OnExitAnyState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	FSMVisualizer->UpdatePreviousStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
	FSMVisualizer->UpdatePreviousStateFrames(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateFrames());
}
#pragma endregion

#pragma region Player States
#pragma region Idle
void AYlva::OnEnterIdleState()
{
	AnimInstance->Montage_Stop(0.3f, Combat.BlockSettings.BlockIdle);
}

void AYlva::UpdateIdleState()
{
	GameState->CurrentCameraShake = CameraManager->PlayCameraShake(CameraShakes.Idle.Shake, CameraShakes.Idle.Intensity);

	if (IsMovingInAnyDirection() && MovementComponent->IsMovingOnGround())
		FSM->PushState("Walk");
}

void AYlva::OnExitIdleState()
{
}
#pragma endregion

#pragma region Walk
void AYlva::OnEnterWalkState()
{
}

void AYlva::UpdateWalkState()
{
	GameState->CurrentCameraShake = CameraManager->PlayCameraShake(CameraShakes.Walk.Shake, CameraShakes.Walk.Intensity);

	if (!IsMovingInAnyDirection())
		FSM->PopState();
}

void AYlva::OnExitWalkState()
{
}
#pragma endregion

#pragma region Run
void AYlva::OnEnterRunState()
{
	YlvaAnimInstance->bIsRunning = true;

	if (IsLockedOn())
	{
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
	}

	MovementComponent->MaxWalkSpeed = MovementSettings.RunSpeed;
}

void AYlva::UpdateRunState()
{
	GameState->CurrentCameraShake = CameraManager->PlayCameraShake(CameraShakes.Run.Shake, CameraShakes.Run.Intensity);

	if (!bGodMode)
		UpdateStamina(StaminaComponent->GetRunValue() * World->DeltaTimeSeconds);

	if (!IsMovingInAnyDirection() || CurrentMovementSpeed <= 0.0f || StaminaComponent->IsStaminaEmpty())
	{
		FSM->PopState();
	}
}

void AYlva::OnExitRunState()
{
	StaminaComponent->DelayRegeneration();

	if (IsLockedOn())
	{
		MovementComponent->bOrientRotationToMovement = false;
		MovementComponent->bUseControllerDesiredRotation = true;
	}

	YlvaAnimInstance->bIsRunning = bWasRunning;

	if (IsLockedOn())
		MovementComponent->MaxWalkSpeed = MovementSettings.LockOnWalkSpeed;
	else
		MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

	bCanRun = false;
}
#pragma endregion

#pragma region Shield Block
void AYlva::OnEnterBlockingState()
{
	YlvaAnimInstance->bIsBlocking = true;
	bUseControllerRotationYaw = true;

	AttackComboComponent->ClearCurrentAttack();
	AttackComboComponent->ResetCombo();
	GameState->PlayerData.CurrentAttackType = ATP_None;

	ParryCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	AnimInstance->Montage_Play(Combat.BlockSettings.BlockIdle);
}

void AYlva::UpdateBlockingState()
{
	if (FSM->GetActiveStateFrames() > Combat.ParrySettings.MaxParryFrame)
	{
		ParryCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
	else
	{
		CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	}
}

void AYlva::OnExitBlockingState()
{
	StopAnimMontage(Combat.BlockSettings.BlockIdle);

	YlvaAnimInstance->bIsBlocking = false;
	bUseControllerRotationYaw = false;

	bIsParryBoxHit = false;

	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ParryCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
#pragma endregion

#pragma region Damaged
void AYlva::OnEnterDamagedState()
{
	if (MovementSettings.bStopMovingWhenDamaged)
		MovementComponent->SetMovementMode(MOVE_None);

	GameState->PlayerData.bHasTakenDamage = true;
	bHasBeenDamaged = true;

	StopAnimMontage();
	AttackComboComponent->ClearCurrentAttack();

	if (GameState->BossData.CurrentCounterType == ACM_NoCounter)
		YlvaAnimInstance->bIsHitByNoCounter = true;
	else
		AnimInstance->bIsHit = true;
}

void AYlva::UpdateDamagedState()
{
	ParryCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// If hit animation has finished, go back to previous state
	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AYlva::OnExitDamagedState()
{
	MovementComponent->SetMovementMode(MOVE_Walking);

	GameState->PlayerData.bHasTakenDamage = false;
	bHasBeenDamaged = false;

	YlvaAnimInstance->bIsHitByNoCounter = false;
	AnimInstance->bIsHit = false;
}
#pragma endregion 

#pragma region Death
void AYlva::OnEnterDeathState()
{
	AnimInstance->StopAllMontages(0.1f);

	bIsDead = true;
	GameState->PlayerData.bIsDead = true;
	AnimInstance->bIsDead = true;

	if (IsLockedOn())
		DisableLockOn();

	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	MovementComponent->DisableMovement();

	GameState->PlayerData.OnDeath.Broadcast();

	//TimerManager->SetTimer(DeathExpiryTimerHandle, this, &AYlva::Respawn, RespawnDelay);

	// Stop all vibrations
	for (const auto& Effect : PlayerController->ActiveForceFeedbackEffects)
	{
		StopVibrateController(Effect.ForceFeedbackEffect);
	}

	OnDeath();
}

void AYlva::UpdateDeathState()
{
}

void AYlva::OnExitDeathState()
{
	bIsDead = false;
	GameState->PlayerData.bIsDead = false;
	AnimInstance->bIsDead = false;
}
#pragma endregion 

#pragma region Charge Attack
void AYlva::OnEnterChargeAttackState()
{
	MovementComponent->SetMovementMode(MOVE_None);
	YlvaAnimInstance->bCanCharge = true;

	GameState->PlayerData.CurrentAttackType = ATP_Special;

	if (!Combat.ChargeSettings.ChargeCameraAnimInst)
	{
		if (Combat.ChargeSettings.ChargeCameraAnim)
			Combat.ChargeSettings.ChargeCameraAnimInst = CameraManager->PlayCameraAnim(Combat.ChargeSettings.ChargeCameraAnim);
	}

	const FRotator NewRotation = FRotator(Combat.ParrySettings.CameraPitchOnSuccess, ForwardVector.Rotation().Yaw, ControlRotation.Roll);
	PlayerController->SetControlRotation(NewRotation);

	PlayerController->SetIgnoreLookInput(true);

	VibrateController(Combat.ChargeSettings.ChargeAttackForce, true);
}

void AYlva::UpdateChargeAttackState()
{
	FaceBoss(World->DeltaTimeSeconds);

	LockOnTo(GameState->BossData.Location, World->DeltaTimeSeconds);

	ChargeAttackHoldFrames++;

	GameState->CurrentCameraShake = CameraManager->PlayCameraShake(CameraShakes.Charge.Shake,CameraShakes.Charge.Intensity);

	if (Combat.ChargeSettings.ChargeCameraAnimInst && Combat.ChargeSettings.ChargeCameraAnimInst->CurTime >= Combat.ChargeSettings.ChargeCameraAnim->AnimLength/2.0f)
		Combat.ChargeSettings.ChargeCameraAnimInst->PlayRate = 0.0f;

#if !UE_BUILD_SHIPPING
	ULog::Number(ChargeAttackHoldFrames, "Charge Hold: ",true);
#endif
}

void AYlva::OnExitChargeAttackState()
{
	PlayerController->ResetIgnoreLookInput();
	MovementComponent->SetMovementMode(MOVE_Walking);

	StopVibrateController(Combat.ChargeSettings.ChargeAttackForce);

	LSword->Revert();
	RSword->Revert();

	if (ChargeAttackHoldFrames < ChargeAttackComponent->GetChargeHoldFrames())
	{
		if (Combat.ChargeSettings.ChargeCameraAnimInst)
			Combat.ChargeSettings.ChargeCameraAnimInst->PlayRate = 3.0f;

		Combat.ChargeSettings.ChargeCameraAnimInst = nullptr;

		ResetCharge();
		FinishChargeAttack();

		return;
	}

	if (Combat.ChargeSettings.ChargeCameraAnimInst)
		Combat.ChargeSettings.ChargeCameraAnimInst->PlayRate = 1.0f;

	Combat.ChargeSettings.ChargeCameraAnimInst = nullptr;

	if (!bHasBeenDamaged)
	{
		YlvaAnimInstance->bChargeReleased = true;
		CurrentForceFeedback = Combat.ChargeSettings.ChargeAttackEndForce;
	}
	else
	{
		ResetCharge();
		FinishChargeAttack();

		return;
	}

	if (ChargeAttackComponent->IsChargeFull())
	{
		GameState->CurrentCameraShake = CameraManager->PlayCameraShake(CameraShakes.ChargeEnd.Shake, CameraShakes.ChargeEnd.Intensity);
		ResetCharge();
	}

	TimerManager->SetTimer(TH_ChargeAttackRelease, this, &AYlva::FinishChargeAttack, 0.5f);
}
#pragma endregion 

#pragma region Shocked
void AYlva::OnEnterShockedState()
{
	YlvaAnimInstance->bIsShocked = true;

	MovementComponent->SetMovementMode(MOVE_None);
}

void AYlva::UpdateShockedState()
{
	if (FSM->GetActiveStateUptime() > ShockTime)
		FSM->PopState();
}

void AYlva::OnExitShockedState()
{
	YlvaAnimInstance->bIsShocked = false;

	MovementComponent->SetMovementMode(MOVE_Walking);
}
#pragma endregion 

#pragma region Dash Attack
void AYlva::OnEnterDashAttackState()
{
	YlvaAnimInstance->bCanDashAttack = true;

	GameState->PlayerData.CurrentAttackType = ATP_Dash;

	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);

	if (IsLockedOn())
	{
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
	}

	if (!Combat.DashAttackSettings.CameraAnimInst)
	{
		Combat.DashAttackSettings.CameraAnimInst = CameraManager->PlayCameraAnim(Combat.DashAttackSettings.CameraAnim);
	}
	
	const FRotator NewRotation = FRotator(Combat.DashAttackSettings.CameraPitchOnSuccess, ForwardVector.Rotation().Yaw, ControlRotation.Roll);
	PlayerController->SetControlRotation(NewRotation);
	
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->SetIgnoreMoveInput(true);

	UGameplayStatics::SetGlobalTimeDilation(this, Combat.DashAttackSettings.TimeDilationWhileAttacking);
}

void AYlva::UpdateDashAttackState()
{
	const float& NewTimeDilation = FMath::InterpExpoIn(Combat.DashAttackSettings.TimeDilationWhileAttacking, 1.0f, FMath::Clamp(FSM->GetActiveStateUptime(), 0.0f, 1.0f));
	UGameplayStatics::SetGlobalTimeDilation(this, NewTimeDilation);

	if (FSM->GetActiveStateUptime() < 0.1f)
	{
		FaceBoss_Instant();
		LockOnTo(GameState->BossData.Location, World->DeltaTimeSeconds);
	}

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AYlva::OnExitDashAttackState()
{
	YlvaAnimInstance->bCanDashAttack = false;

	bPerfectlyTimedDash = false;

	GameState->PlayerData.CurrentAttackType = ATP_None;

	Combat.DashAttackSettings.CameraAnimInst = nullptr;

	PlayerController->ResetIgnoreMoveInput();
	PlayerController->ResetIgnoreLookInput();

	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	
	if (IsLockedOn())
	{
		MovementComponent->bOrientRotationToMovement = false;
		MovementComponent->bUseControllerDesiredRotation = true;
	}
	
	ResetGlobalTimeDilation();
}
#pragma endregion 

#pragma region Shield Hit
void AYlva::OnEnterShieldHitState()
{
	if (GameState->BossData.CurrentCounterType == ACM_NoCounter)
	{
		AttackComboComponent->ClearCurrentAttack();
		YlvaAnimInstance->bIsHitByNoCounter = true;
	}
	else
		YlvaAnimInstance->bIsShieldHit = true;

	GameState->BossData.OnAttackBlocked.Broadcast();

	const int32 RandomIndex = FMath::RandRange(0, Combat.BlockSettings.ShieldHitSoundData->HitSounds.Num()-1);
	UGameplayStatics::PlaySoundAtLocation(this, Combat.BlockSettings.ShieldHitSoundData->HitSounds[RandomIndex], CurrentLocation);

	StaminaComponent->DelayRegeneration();
}

void AYlva::UpdateShieldHitState()
{
	// If shield impact animation has finished, go back to previous state
	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AYlva::OnExitShieldHitState()
{
	YlvaAnimInstance->bIsHitByNoCounter = false;
	YlvaAnimInstance->bIsShieldHit = false;

	bHasBeenDamaged = false;
}
#pragma endregion 

#pragma region Dash
void AYlva::OnEnterDashState()
{
	if (GameState->BossData.CurrentCounterType == ACM_NoCounter)
		EnableInvincibility();

	AnimInstance->bIsDashing = true;
	YlvaAnimInstance->bIsRunning = bWasRunning;

	if (IsLockedOn())
	{
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
	}

	AttackComboComponent->ResetCombo();
}

void AYlva::UpdateDashState()
{
	if (FVector::Dist(CurrentLocation, GameState->BossData.SpearLocation) < 200.0f &&
		GameState->IsBossAttacking() && 
		!bPerfectlyTimedDash && 
		!IsDamaged())
	{
		#if !UE_BUILD_SHIPPING
		ULog::Info("Perfectly timed dash!", true);
		#endif

		EnableInvincibility();

		bPerfectlyTimedDash = true;

		// Todo: Play sound
		UGameplayStatics::SetGlobalTimeDilation(this, Combat.DashAttackSettings.TimeDilationOnPerfectDash);
	}

	if (AnimInstance->AnimTimeRemaining < 0.1f)
	{
		FSM->PopState();
	}
}

void AYlva::OnExitDashState()
{
	DisableInvincibility();

	if (IsLockedOn())
	{
		MovementComponent->bOrientRotationToMovement = false;
		MovementComponent->bUseControllerDesiredRotation = true;
	}

	AnimInstance->bIsDashing = false;
	bWasRunning = false;

	if (bPerfectlyTimedDash)
	{
		bPerfectlyTimedDash = false;
	
		ResetGlobalTimeDilation();
	}

	if (!DashQueue.IsEmpty())
	{
		DashComponent->PauseCooldown();
		TimerManager->SetTimer(TH_DashQueue, this, &AYlva::Dash_Queued, 0.2f);
	}
}
#pragma endregion 

#pragma region Parry
void AYlva::OnEnterParryState()
{
	StopAnimMontage();

	YlvaAnimInstance->bCanParry = true;

	const int32 RandomIndex = FMath::RandRange(0, Combat.ParrySettings.ParryHitSoundData->HitSounds.Num()-1);
	UGameplayStatics::PlaySoundAtLocation(this, Combat.ParrySettings.ParryHitSoundData->HitSounds[RandomIndex], CurrentLocation);

	PlayerController->SetIgnoreLookInput(true);

	GameState->BossData.OnAttackParryed.Broadcast();

	UGameplayStatics::SetGlobalTimeDilation(this, Combat.ParrySettings.TimeDilationOnSuccessfulParry);
}

void AYlva::UpdateParryState()
{
	const float& NewTimeDilation = FMath::InterpExpoIn(Combat.ParrySettings.TimeDilationOnSuccessfulParry, 1.0f, 4 * FMath::Clamp(FSM->GetActiveStateUptime(), 0.0f, 1.0f));
	UGameplayStatics::SetGlobalTimeDilation(this, NewTimeDilation);

	if (UGameplayStatics::GetGlobalTimeDilation(this) >= 1.0f)
		FSM->PopState();
}

void AYlva::OnExitParryState()
{
	PlayerController->SetIgnoreLookInput(false);

	YlvaAnimInstance->bCanParry = false;
	GameState->PlayerData.bParrySucceeded = false;

	ResetGlobalTimeDilation();
}
#pragma endregion 
#pragma endregion 

void AYlva::ApplyHitStop()
{
	if (Combat.bEnableHitStop)
	{
		PauseAnims();
		TimerManager->SetTimer(HitStopTimerHandle, this, &AYlva::UnPauseAnims, Combat.HitStopTime);
	}
}

void AYlva::StopMovement()
{
	CurrentMovementSpeed = 0;
	ForwardInput = 0.0f;
	RightInput = 0.0f;

	MovementComponent->DisableMovement();
	PlayerController->SetIgnoreMoveInput(true);

	AnimInstance->LeaveAllStates();
	FSM->RemoveAllStatesFromStack();
}

void AYlva::ResumeMovement()
{
	MovementComponent->SetMovementMode(MOVE_Walking);
	PlayerController->ResetIgnoreMoveInput();
}

void AYlva::SpawnGhost()
{
	World->SpawnActor(GhostClass, &SKMComponent->GetComponentTransform());
}

bool AYlva::IsLightAttacking() const
{
	return AttackComboComponent->GetCurrentAttack() == ATP_Light;
}

bool AYlva::IsHeavyAttacking() const
{
	return AttackComboComponent->GetCurrentAttack() == ATP_Heavy;
}

bool AYlva::IsAttacking() const
{
	return IsLightAttacking() || IsHeavyAttacking() || IsChargeAttacking();
}

bool AYlva::IsParrySuccessful() const
{
	return	IsBlocking() && 
			bIsParryBoxHit && 
			FSM->GetActiveStateFrames() > Combat.ParrySettings.MinParryFrame && 
			FSM->GetActiveStateFrames() < Combat.ParrySettings.MaxParryFrame &&
			FVector::DotProduct(ForwardVector, DirectionToBoss) > 0.5f;
}

bool AYlva::IsMovingForward() const
{
	return ForwardInput > 0.0f && RightInput == 0.0f;
}

bool AYlva::IsMovingBackward() const
{
	return ForwardInput < 0.0f && RightInput == 0.0f;
}

bool AYlva::IsMovingRight() const
{
	return ForwardInput == 0.0f && RightInput > 0.0f;
}

bool AYlva::IsMovingLeft() const
{
	return ForwardInput == 0.0f && RightInput < 0.0f;
}

bool AYlva::IsMovingInAnyDirection() const
{
	return IsMovingBackward() || IsMovingForward() || IsMovingRight() || IsMovingLeft() || ForwardInput != 0.0f || RightInput != 0.0f;
}

bool AYlva::IsLowStamina() const
{
	return StaminaComponent->IsLowStamina();
}

bool AYlva::IsLockedOn() const
{
	return LockOnSettings.bLockedOn;
}

bool AYlva::IsIdle() const
{
	return FSM->GetActiveStateID() == 0;
}

bool AYlva::IsWalking() const
{
	return FSM->GetActiveStateID() == 1;
}

bool AYlva::IsRunning() const
{
	return FSM->GetActiveStateID() == 2;
}

bool AYlva::IsBlocking() const
{
	return FSM->GetActiveStateID() == 4;
}

bool AYlva::IsChargeAttacking() const
{
	return FSM->GetActiveStateID() == 6 || GameState->PlayerData.CurrentAttackType == ATP_Special;
}

bool AYlva::IsDashing() const
{
	return FSM->GetActiveStateID() == 12;
}

bool AYlva::IsDashAttacking() const
{
	return FSM->GetActiveStateID() == 8;
}

bool AYlva::IsDamaged() const
{
	return FSM->GetActiveStateID() == 20 || bHasBeenDamaged;
}

bool AYlva::IsParrying() const
{
	return FSM->GetActiveStateID() == 22;
}

class UForceFeedbackEffect* AYlva::GetCurrentForceFeedback() const
{
	return CurrentForceFeedback;
}

void AYlva::ResetGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
}

UStaticMeshComponent* AYlva::GetLeftHandSword() const
{
	if (Components.Num() == 0)
	{
		#if !UE_BUILD_SHIPPING
		ULog::Info("Components array is empty. You must populate this to retrieve a component", true);
		#endif

		return nullptr;
	}

	for (auto Component : Components.Array())
	{
		if (Component->GetName() == "Sword_L" || Component->GetName() == "Sword")
		{
			return Cast<UStaticMeshComponent>(Component);
		}
	}

	#if !UE_BUILD_SHIPPING
	ULog::Info("Could not find the left hand sword", true);
	#endif

	return nullptr;
}

UStaticMeshComponent* AYlva::GetRightHandSword() const
{
	if (Components.Num() == 0)
	{
		#if !UE_BUILD_SHIPPING
		ULog::Info("Components array is empty. You must populate this to retrieve a component", true);
		#endif

		return nullptr;
	}

	for (auto Component : Components.Array())
	{
		if (Component->GetName() == "Sword_R" || Component->GetName() == "Sword")
		{
			return Cast<UStaticMeshComponent>(Component);
		}
	}

	#if !UE_BUILD_SHIPPING
	ULog::Info("Could not find the right hand sword", true);
	#endif

	return nullptr;
}

void AYlva::OnUntouchableFeatAchieved()
{
	GameInstance->AchievedFeat = UntouchableFeat;

	UntouchableFeat->OnFeatAchieved.Broadcast();
}

void AYlva::AddDebugMessages()
{
	OverthroneHUD->AddOnScreenDebugMessage("Player", FColor::White, 0.0f, 1.1f);
	OverthroneHUD->AddOnScreenDebugMessage("Camera Pitch: ", FColor::Orange);
	OverthroneHUD->AddOnScreenDebugMessage("Player Forward Input: ", FColor::Green);
	OverthroneHUD->AddOnScreenDebugMessage("Player Right Input: ", FColor::Green);
	OverthroneHUD->AddOnScreenDebugMessage("Movement Speed: ", FColor::Yellow);
	OverthroneHUD->AddOnScreenDebugMessage("Player Health: ", FColor::Yellow);
	OverthroneHUD->AddOnScreenDebugMessage("Player Stamina: ", FColor::Yellow);
	OverthroneHUD->AddOnScreenDebugMessage("God mode: Off", FColor::White);
	OverthroneHUD->AddOnScreenDebugMessage("Current attack: ", FColor::White);
	OverthroneHUD->AddOnScreenDebugMessage("Player Direction: ", FColor::Cyan);
	OverthroneHUD->AddOnScreenDebugMessage("Displayed Health: ", FColor::Yellow);
	OverthroneHUD->AddOnScreenDebugMessage("Displayed Stamina: ", FColor::Yellow);
	OverthroneHUD->AddOnScreenDebugMessage("Moved Right by: ", FColor::Green);
	OverthroneHUD->AddOnScreenDebugMessage("Distance to Spear: ", FColor::Green);
}
