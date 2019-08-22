// Copyright Overthrone Studios 2019

#include "Player/Ylva.h"
#include "Player/YlvaAnimInstance.h"

#include "OverthroneHUD.h"
#include "OverthroneGameInstance.h"
#include "OverthroneFunctionLibrary.h"
#include "FSM.h"
#include "Log.h"

#include "Widgets/HUD/MasterHUD.h"
#include "Widgets/HUD/FSMVisualizerHUD.h"

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
#include "Components/AttackComboComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ConstructorHelpers.h"
#include "TimerManager.h"

AYlva::AYlva() : AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Player/Bot/Animations/AnimBP_Bot.AnimBP_Bot_C'"));

	// Get the skeletal mesh to use
	SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/Characters/Player/Bot/SKM_Bot.SKM_Bot'")));

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
	FSM->AddState(3, "Light Attack 1");
	FSM->AddState(4, "Block");
	FSM->AddState(5, "Death");
	FSM->AddState(8, "Light Attack 2");
	FSM->AddState(9, "Heavy Attack 1");
	FSM->AddState(10, "Heavy Attack 2");
	FSM->AddState(20, "Damaged");
	FSM->AddState(21, "Shield Hit");
	FSM->AddState(22, "Parry");

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

	FSM->GetState(3)->OnEnterState.AddDynamic(this, &AYlva::OnEnterLightAttackState);
	FSM->GetState(3)->OnUpdateState.AddDynamic(this, &AYlva::UpdateLightAttackState);
	FSM->GetState(3)->OnExitState.AddDynamic(this, &AYlva::OnExitLightAttackState);

	FSM->GetState(4)->OnEnterState.AddDynamic(this, &AYlva::OnEnterBlockingState);
	FSM->GetState(4)->OnUpdateState.AddDynamic(this, &AYlva::UpdateBlockingState);
	FSM->GetState(4)->OnExitState.AddDynamic(this, &AYlva::OnExitBlockingState);

	FSM->GetState(5)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDeathState);
	FSM->GetState(5)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDeathState);
	FSM->GetState(5)->OnExitState.AddDynamic(this, &AYlva::OnExitDeathState);

	FSM->GetState(8)->OnEnterState.AddDynamic(this, &AYlva::OnEnterLightAttack2State);
	FSM->GetState(8)->OnUpdateState.AddDynamic(this, &AYlva::UpdateLightAttack2State);
	FSM->GetState(8)->OnExitState.AddDynamic(this, &AYlva::OnExitLightAttack2State);

	FSM->GetState(9)->OnEnterState.AddDynamic(this, &AYlva::OnEnterHeavyAttackState);
	FSM->GetState(9)->OnUpdateState.AddDynamic(this, &AYlva::UpdateHeavyAttackState);
	FSM->GetState(9)->OnExitState.AddDynamic(this, &AYlva::OnExitHeavyAttackState);

	FSM->GetState(10)->OnEnterState.AddDynamic(this, &AYlva::OnEnterHeavyAttack2State);
	FSM->GetState(10)->OnUpdateState.AddDynamic(this, &AYlva::UpdateHeavyAttack2State);
	FSM->GetState(10)->OnExitState.AddDynamic(this, &AYlva::OnExitHeavyAttack2State);

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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 2.0f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSettings.WalkSpeed;

	// Stamina component
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(FName("Stamina Component"));
	StaminaRegenTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Stamina Regen Timeline"));

	// Charge attack component
	ChargeAttackComponent = CreateDefaultSubobject<UChargeAttackComponent>(FName("Charge Attack Component"));
	ChargeAttackTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Charge Attack Timeline"));

	// Attack combo component
	AttackComboComponent = CreateDefaultSubobject<UAttackComboComponent>(FName("Attack Combo Component"));

	// Configure character settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AYlva::BeginPlay()
{
	Super::BeginPlay();

	InitTimelineComponent(StaminaRegenTimeline, StaminaRegenCurve, 1.0f, FName("LoseStamina"), FName("FinishLosingStamina"));
	InitTimelineComponent(ChargeAttackTimeline, ChargeAttackCurve, 1.0f, FName("GainCharge"), FName("FinishGainingCharge"));

	// Get the swords
	R_SwordMesh = GetRightHandSword();
	L_SwordMesh = GetLeftHandSword();

	// Initialize our variables
	Boss = UOverthroneFunctionLibrary::GetBossCharacter(World);
	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	YlvaAnimInstance = Cast<UYlvaAnimInstance>(GetMesh()->GetAnimInstance());
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("FSMVisualizer"));

	// Set pitch min max values
	CameraManager->ViewPitchMin = 360.0f - CameraPitchMax;
	CameraManager->ViewPitchMax = CameraPitchMin;

	// Initialize player info
	GameInstance->PlayerInfo.StartingHealth = HealthComponent->GetDefaultHealth();
	GameInstance->PlayerInfo.Health = HealthComponent->GetCurrentHealth();
	GameInstance->PlayerInfo.SmoothedHealth = HealthComponent->GetSmoothedHealth();
	GameInstance->PlayerInfo.StartingStamina = StaminaComponent->GetDefaultStamina();
	GameInstance->PlayerInfo.Stamina = StaminaComponent->GetCurrentStamina();
	GameInstance->PlayerInfo.SmoothedStamina = StaminaComponent->GetCurrentStamina();
	GameInstance->PlayerInfo.MaxCharge = ChargeAttackComponent->GetMaxCharge();
	GameInstance->PlayerInfo.Charge = ChargeAttackComponent->GetCurrentCharge();
	GameInstance->PlayerInfo.SmoothedCharge = ChargeAttackComponent->GetCurrentCharge();
	GameInstance->Player = this;

	// Bind events to our functions
	GameInstance->PlayerInfo.OnLowHealth.AddDynamic(this, &AYlva::OnLowHealth);
	GameInstance->PlayerInfo.OnLowStamina.AddDynamic(this, &AYlva::OnLowStamina);
	GameInstance->OnBossDeath.AddDynamic(this, &AYlva::OnBossDeath);

	//AnimInstance->bLogDirection = true;

	// Begin the state machine
	FSM->Start();
}

void AYlva::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculate direction
	const auto DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(FollowCamera->GetComponentRotation(), GetCapsuleComponent()->GetComponentRotation());
	const auto NewDirection = UKismetMathLibrary::NormalizedDeltaRotator(DeltaRotation, UKismetMathLibrary::MakeRotFromX(FVector(InputZ, -InputX, 0.0f)));
	AnimInstance->MovementDirection = NewDirection.GetNormalized().Yaw;

	GameInstance->PlayerInfo.Location = GetActorLocation();

	// Lock-on mechanic
	if (LockOnSettings.bShouldLockOnTarget)
	{
		const FRotator Target = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), GameInstance->BossInfo.Location);
		const FRotator SmoothedRotation = FMath::RInterpTo(GetControlRotation(), Target, DeltaTime, 10.0f);

		const FRotator NewRotation = FRotator(LockOnSettings.LockOnPitch, SmoothedRotation.Yaw, GetControlRotation().Roll);

		GetController()->SetControlRotation(NewRotation);

		GameInstance->SetLockOnLocation(GameInstance->BossInfo.Location);
		GameInstance->SetLockOnRotation(NewRotation - FRotator(0.0f, 180.0f, 0.0f));
	}

	// Stamina regen mechanic
	if (!StaminaRegenTimeline->IsPlaying())
		RegenerateStamina(StaminaComponent->GetRegenRate());

	// Charge loss mechanic
	if (ChargeAttackComponent->CanLoseChargeOvertime() && 
		ChargeAttackComponent->IsLosingCharge() && 
		!ChargeAttackComponent->IsChargeFull() && 
		!ChargeAttackComponent->IsChargeEmpty())
	{
		DecreaseCharge(ChargeAttackComponent->GetChargeLossRate() * DeltaTime);
	}

#if !UE_BUILD_SHIPPING
	if (Debug.bLogCameraPitch)
		ULog::Number(GetControlRotation().Pitch, "Pitch: ", true);
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
	PlayerInputComponent->BindAction("Light Attack", IE_Released, this, &AYlva::DisableControllerRotationYaw);
	PlayerInputComponent->BindAction("Heavy Attack", IE_Pressed, this, &AYlva::HeavyAttack);
	PlayerInputComponent->BindAction("Heavy Attack", IE_Released, this, &AYlva::DisableControllerRotationYaw);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AYlva::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AYlva::StopRunning);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AYlva::Dash);

	PlayerInputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AYlva::Pause).bExecuteWhenPaused = true;
	PlayerInputComponent->BindKey(EKeys::Gamepad_Special_Right, IE_Pressed, this, &AYlva::Pause).bExecuteWhenPaused = true;

#if !UE_BUILD_SHIPPING
	// Debugging
	PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &AYlva::ShowPlayerFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AYlva::ShowBossFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &AYlva::ShowMainHUD);
	PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &AYlva::ShowNoHUD);

	PlayerInputComponent->BindKey(EKeys::Five, IE_Pressed, this, &AYlva::ToggleGodMode);
#endif
}

void AYlva::ChangeHitboxSize(const float NewRadius)
{
	Combat.AttackSettings.AttackRadius = NewRadius;
}

void AYlva::UpdateCharacterInfo()
{
	GameInstance->PlayerInfo.Health = HealthComponent->GetCurrentHealth();
	GameInstance->PlayerInfo.Stamina = StaminaComponent->GetCurrentStamina();
	GameInstance->PlayerInfo.Charge = ChargeAttackComponent->GetSmoothedCharge();
	GameInstance->PlayerInfo.SmoothedHealth = HealthComponent->GetSmoothedHealth();
	GameInstance->PlayerInfo.SmoothedStamina = StaminaComponent->GetSmoothedStamina();
	GameInstance->PlayerInfo.SmoothedCharge = ChargeAttackComponent->GetSmoothedCharge();
	GameInstance->PlayerInfo.Location = GetActorLocation();
}

void AYlva::BroadcastLowHealth()
{
	GameInstance->PlayerInfo.OnLowHealth.Broadcast();
	bWasLowHealthEventTriggered = true;
}

void AYlva::BroadcastLowStamina()
{
	GameInstance->PlayerInfo.OnLowStamina.Broadcast();
	bWasLowStaminaEventTriggered = true;
}

void AYlva::StartLosingHealth()
{
	Super::StartLosingHealth();

	if (Debug.bLogHealthValues)
	{
		ULog::Number(HealthComponent->GetPreviousHealth(), "Previous Health: ", true);
		ULog::Number(HealthComponent->GetCurrentHealth(), "Target Health: ", true);
	}
}

void AYlva::LoseHealth()
{
	Super::LoseHealth();

	if (Debug.bLogHealthValues)
		ULog::Number(HealthComponent->GetSmoothedHealth(), "New Health: ", true);
}

void AYlva::MoveForward(const float Value)
{
	InputZ = Value;

	if (Controller && Value != 0.0f)
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AYlva::MoveRight(const float Value)
{
	InputX = Value;

	if (Controller && Value != 0.0f)
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AYlva::TurnAtRate(const float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRate * World->GetDeltaSeconds());
}

void AYlva::LookUpAtRate(const float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * LookUpRate * World->GetDeltaSeconds());
}

void AYlva::ToggleLockOn()
{
	// Don't lock on if boss is dead
	if (GameInstance->BossInfo.Health <= 0.0f || FSM->GetActiveStateName() == "Death")
		return;

	LockOnSettings.bShouldLockOnTarget = !LockOnSettings.bShouldLockOnTarget;
	PlayerController->SetIgnoreLookInput(LockOnSettings.bShouldLockOnTarget);
	GameInstance->ToggleLockOnVisibility(LockOnSettings.bShouldLockOnTarget);
}

void AYlva::EnableLockOn()
{
	// Don't lock on if boss is dead
	if (GameInstance->BossInfo.Health <= 0.0f || FSM->GetActiveStateName() == "Death")
		return;

	LockOnSettings.bShouldLockOnTarget = true;
	PlayerController->SetIgnoreLookInput(true);
	GameInstance->ToggleLockOnVisibility(true);
}

void AYlva::DisableLockOn()
{
	LockOnSettings.bShouldLockOnTarget = false;
	PlayerController->SetIgnoreLookInput(false);
	GameInstance->ToggleLockOnVisibility(false);
}

void AYlva::Block()
{
	if (FSM->GetActiveStateID() != 5 /*Death*/ &&
		FSM->GetActiveStateID() != 20 /*Damaged*/ &&
		FSM->GetActiveStateID() != 22 /*Parry*/)
	{
		FSM->PopState();
		FSM->PushState("Block");
	}
}

void AYlva::StopBlocking()
{
	if (FSM->GetActiveStateID() == 5 /*Death*/)
		return;

	AnimInstance->Montage_Stop(0.3f, Combat.BlockSettings.BlockIdle);
	bUseControllerRotationYaw = false;

	//if (Combat.ParrySettings.ParryCameraAnimInst && Combat.ParrySettings.ParryCameraAnimInst->bFinished)
		FSM->PopState();
}

void AYlva::UpdateStamina(const float StaminaToSubtract)
{
	StaminaComponent->SetSmoothedStamina(StaminaComponent->GetSmoothedStamina());

	// Stop animating displayed stamina
	if (StaminaRegenTimeline->IsPlaying())
		StaminaRegenTimeline->Stop();

	DecreaseStamina(StaminaToSubtract);

	if (StaminaComponent->GetDecreaseDelay() > 0.0f)
	{
		GetWorldTimerManager().SetTimer(StaminaComponent->GetDelayTimerHandle(), this, &AYlva::StartLosingStamina, StaminaComponent->GetDecreaseDelay(), false);
	}
	else
		StartLosingStamina();

	StaminaComponent->DelayRegeneration();
}

void AYlva::BeginLightAttack(class UAnimMontage* AttackMontage)
{
	//FSM->PopState();
	//FSM->PushState(LightAttackName);

	AnimInstance->Montage_Play(AttackMontage);

	if (Combat.bRotateToCameraLookDirection)
		bUseControllerRotationYaw = true;

	if (!bGodMode)
	{
		UpdateStamina(StaminaComponent->GetLightAttackValue());
	}

	if (MovementSettings.bStopMovingWhenAttacking)
		MovementComponent->SetMovementMode(MOVE_None);
}

void AYlva::BeginHeavyAttack(class UAnimMontage* AttackMontage)
{
	//FSM->PopState();
	//FSM->PushState(HeavyAttackName);

	AnimInstance->Montage_Play(AttackMontage);

	if (Combat.bRotateToCameraLookDirection)
		bUseControllerRotationYaw = true;

	if (!bGodMode)
	{
		UpdateStamina(StaminaComponent->GetHeavyAttackValue());
	}

	if (MovementSettings.bStopMovingWhenAttacking)
		MovementComponent->SetMovementMode(MOVE_None);
}

void AYlva::LightAttack()
{
	// Are we in any of these states?
	if (FSM->GetActiveStateID() == 5 /*Death*/ ||
		FSM->GetActiveStateID() == 20 /*Damaged*/)
		return;

	FSM->PopState("Block");

	if (FSM->GetActiveStateID() == 22 /*Parry*/)
		FinishParryEvent();

	if (StaminaComponent->HasEnoughForLightAttack())
	{
		UAnimMontage* AttackMontageToPlay = AttackComboComponent->AdvanceCombo(Light);

		BeginLightAttack(AttackMontageToPlay);
	}
}

void AYlva::HeavyAttack()
{
	// Are we in any of these states?
	if (FSM->GetActiveStateID() == 5 /*Death*/ ||
		FSM->GetActiveStateID() == 20 /*Damaged*/)
		return;

	FSM->PopState("Block");

	if (FSM->GetActiveStateID() == 22 /*Parry*/)
		FinishParryEvent();

	if (StaminaComponent->HasEnoughForHeavyAttack())
	{
		UAnimMontage* AttackMontageToPlay = AttackComboComponent->AdvanceCombo(Heavy);
		
		BeginHeavyAttack(AttackMontageToPlay);
	}
}

void AYlva::DisableControllerRotationYaw()
{
	bUseControllerRotationYaw = false;
}

void AYlva::Run()
{
	if (FSM->GetActiveStateName() == "Death")
		return;

	// If we are moving and grounded
	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround() && StaminaComponent->HasStamina())
	{
		MovementComponent->MaxWalkSpeed = MovementSettings.RunSpeed;

		FSM->PopState();
		FSM->PushState("Run");
	}
}

void AYlva::StopRunning()
{
	if (FSM->GetActiveStateName() == "Death" ||
		FSM->GetActiveStateName() == "Light Attack 1" ||
		FSM->GetActiveStateName() == "Light Attack 2" ||
		FSM->GetActiveStateName() == "Heavy Attack 1" ||
		FSM->GetActiveStateName() == "Heavy Attack 2")
		return;

	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

	// Delay stamina regeneration
	if (FSM->GetActiveStateName() == "Run")
		StaminaComponent->DelayRegeneration();

	FSM->PopState();
}

void AYlva::Dash()
{
	if (FSM->GetActiveStateName() == "Death")
		return;

	// If we are moving and grounded
	if (GetVelocity().Size() > 0.0f && MovementComponent->IsMovingOnGround() && !GetWorldTimerManager().IsTimerActive(DashCooldownTimer))
	{
		// Do the dash
		FVector VelocityNormalized = GetVelocity();
		VelocityNormalized.Normalize();
		VelocityNormalized.Z = 0;

		// Do we have enough stamina to dash?
		if (!bGodMode && StaminaComponent->HasEnoughForDash())
		{
			StartDashCooldown();

			UpdateStamina(StaminaComponent->GetDashValue());

			LaunchCharacter(VelocityNormalized * MovementSettings.DashForce, true, true);
		}
		else if (bGodMode)
		{
			LaunchCharacter(VelocityNormalized * MovementSettings.DashForce, true, true);
		}
	}
}

void AYlva::ShowPlayerFSMVisualizer()
{
	OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(0);
}

void AYlva::ShowBossFSMVisualizer()
{
	OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(1);
}

void AYlva::ShowMainHUD()
{
	OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(2);
	OverthroneHUD->GetMasterHUD()->HideTitle();
	OverthroneHUD->GetMasterHUD()->HideBoxes();
}

void AYlva::ShowNoHUD()
{
	OverthroneHUD->GetMasterHUD()->SetVisibility(ESlateVisibility::Hidden);
}

void AYlva::Pause()
{
	if (GameInstance->IsGamePaused())
		GameInstance->UnPauseGame();
	else
		GameInstance->PauseGame();
}

void AYlva::RegenerateStamina(const float Rate)
{
	if (bGodMode || FSM->GetActiveStateID() == 5 /*Death*/ || !StaminaComponent->IsRegenFinished())
		return;

	IncreaseStamina(Rate * World->DeltaTimeSeconds);
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
	GetWorldTimerManager().ClearTimer(DeathExpiryTimerHandle);

	FSM->PopState();

	GameInstance->PlayerInfo.bIsDead = false;

	UGameplayStatics::OpenLevel(this, *UGameplayStatics::GetCurrentLevelName(this));
}

#pragma region Idle
void AYlva::OnEnterIdleState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->Montage_Stop(0.3f, Combat.BlockSettings.BlockIdle);
}

void AYlva::UpdateIdleState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	RegenerateStamina(StaminaComponent->GetRegenRate());

	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		FSM->PushState("Walk");
}

void AYlva::OnExitIdleState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
}
#pragma endregion

#pragma region Walk
void AYlva::OnEnterWalkState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());
}

void AYlva::UpdateWalkState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		FSM->PopState();
}

void AYlva::OnExitWalkState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
}
#pragma endregion

#pragma region Run
void AYlva::OnEnterRunState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	if (StaminaComponent->IsLowStamina())
		MovementComponent->MaxWalkSpeed = MovementSettings.RunSpeed/2.0f;
	else
		MovementComponent->MaxWalkSpeed = MovementSettings.RunSpeed;
}

void AYlva::UpdateRunState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (!bGodMode)
		UpdateStamina(StaminaComponent->GetRunValue() * World->DeltaTimeSeconds);

	if (GetVelocity().IsZero() || MovementComponent->MaxWalkSpeed < MovementSettings.RunSpeed || StaminaComponent->IsStaminaEmpty())
	{
		StaminaComponent->DelayRegeneration();
		FSM->PopState();
	}
}

void AYlva::OnExitRunState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
}
#pragma endregion

#pragma region Shield Block
void AYlva::OnEnterBlockingState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	bUseControllerRotationYaw = true;

	AnimInstance->Montage_Play(Combat.BlockSettings.BlockIdle);
}

void AYlva::UpdateBlockingState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
}

void AYlva::OnExitBlockingState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	bUseControllerRotationYaw = false;
}
#pragma endregion

#pragma region Light Attack 1
void AYlva::OnEnterLightAttackState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->Montage_Play(AttackComboComponent->GetCurrentLightAttackAnim());
}

void AYlva::UpdateLightAttackState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
}

void AYlva::OnExitLightAttackState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);
}
#pragma endregion

#pragma region Light Attack 2
void AYlva::OnEnterLightAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->Montage_Play(AttackComboComponent->GetCurrentLightAttackAnim());
}

void AYlva::UpdateLightAttack2State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

}

void AYlva::OnExitLightAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());


	MovementComponent->SetMovementMode(MOVE_Walking);
}
#pragma endregion

#pragma region Heavy Attack 1
void AYlva::OnEnterHeavyAttackState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->Montage_Play(AttackComboComponent->GetCurrentHeavyAttackAnim());
}

void AYlva::UpdateHeavyAttackState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	//if (!AnimInstance->Montage_IsPlaying(Combat.AttackSettings.HeavyAttack1))
	//	FSM->PopState();
}

void AYlva::OnExitHeavyAttackState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());


	MovementComponent->SetMovementMode(MOVE_Walking);
}
#pragma endregion

#pragma region Heavy Attack 2
void AYlva::OnEnterHeavyAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondHeavyAttack = true;
}

void AYlva::UpdateHeavyAttack2State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitHeavyAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);

	AnimInstance->bAcceptSecondHeavyAttack = false;
}
#pragma endregion 

#pragma region Damaged
void AYlva::OnEnterDamagedState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	if (MovementSettings.bStopMovingWhenAttacking)
		MovementComponent->SetMovementMode(MOVE_None);

	bIsHit = true;
	AnimInstance->bIsHit = true;
}

void AYlva::UpdateDamagedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If hit animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitDamagedState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);

	bIsHit = false;
	AnimInstance->bIsHit = false;
}
#pragma endregion 

#pragma region Death
void AYlva::OnEnterDeathState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsDead = true;

	MovementComponent->DisableMovement();

	GameInstance->PlayerInfo.bIsDead = true;
	GameInstance->OnPlayerDeath.Broadcast();

	GetWorldTimerManager().SetTimer(DeathExpiryTimerHandle, this, &AYlva::Respawn, RespawnDelay);
}

void AYlva::UpdateDeathState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
}

void AYlva::OnExitDeathState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsDead = false;
}
#pragma endregion 

#pragma region Shield Hit
void AYlva::OnEnterShieldHitState()
{
	YlvaAnimInstance->bIsShieldHit = true;
	bIsHit = true;

	StaminaComponent->DelayRegeneration();
}

void AYlva::UpdateShieldHitState()
{
	// If shield impact animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitShieldHitState()
{
	YlvaAnimInstance->bIsShieldHit = false;
	bIsHit = false;
}
#pragma endregion 

#pragma region Parry
void AYlva::OnEnterParryState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	if (!Combat.ParrySettings.ParryCameraAnimInst)
	{
		if (Combat.ParrySettings.ParryCameraAnim)
			Combat.ParrySettings.ParryCameraAnimInst = CameraManager->PlayCameraAnim(Combat.ParrySettings.ParryCameraAnim);
	}

	const FRotator NewRotation = FRotator(Combat.ParrySettings.CameraPitchOnSuccess, GetActorForwardVector().Rotation().Yaw, GetControlRotation().Roll);
	GetController()->SetControlRotation(NewRotation);

	PlayerController->SetIgnoreLookInput(true);

	YlvaAnimInstance->bIsShieldHit = true;

	StartParryEvent();
}

void AYlva::UpdateParryState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
}

void AYlva::OnExitParryState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	if (Combat.ParrySettings.ParryCameraAnimInst->CurTime == 0.0f)
		Combat.ParrySettings.ParryCameraAnimInst->Stop();

	Combat.ParrySettings.ParryCameraAnimInst = nullptr;	

	PlayerController->SetIgnoreLookInput(false);

	YlvaAnimInstance->bIsShieldHit = false;
	GameInstance->PlayerInfo.bParrySucceeded = false;

	ResetGlobalTimeDilation();

	GetWorldTimerManager().ClearTimer(ParryEventExpiryTimer);
}
#pragma endregion 

float AYlva::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead
	if (FSM->GetActiveStateName() == "Death")
		return DamageAmount;

	// Apply damage once
	if (HealthComponent->GetCurrentHealth() > 0.0f && !bIsHit)
	{
		// Pop the active state that's not Idle or Block
		if (FSM->GetActiveStateName() != "Idle" &&
			FSM->GetActiveStateName() != "Block")
		{
			FSM->PopState();
		}

		if (IsParrySuccessful())
		{
			FSM->PushState("Parry");
			return DamageAmount;
		}

		if (bGodMode)
			return DamageAmount;

		// Test against other states
		switch (FSM->GetActiveStateID())
		{
			case 4 /*Block*/:
				// Enter shield hit state
				FSM->PopState();
				FSM->PushState("Shield Hit");

				// Shake the camera
				PlayerController->ClientPlayCameraShake(CameraShakes.ShieldHit.Shake, CameraShakes.ShieldHit.Intensity);

				// Update stats
				UpdateHealth(DamageAmount * Combat.BlockSettings.DamageBuffer);
				UpdateStamina(StaminaComponent->GetShieldHitValue());
			break;

			default:
				HitCounter++;

				// Enter damaged state
				FSM->PushState("Damaged");

				// Apply hit stop
				PauseAnimsWithTimer();

				// Shake the camera
				PlayerController->ClientPlayCameraShake(CameraShakes.Damaged.Shake, CameraShakes.Damaged.Intensity);

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
			break;
		}
	}

	if (HealthComponent->GetCurrentHealth() <= 0.0f && FSM->GetActiveStateName() != "Death")
	{
		Die();
	}

	return DamageAmount;
}

void AYlva::OnBossDeath()
{
	DisableLockOn();
}

void AYlva::OnLowHealth()
{
	ChangeHitboxSize(Combat.AttackSettings.AttackRadiusOnLowHealth);
}

void AYlva::OnLowStamina()
{
	ULog::Info("Low stamina!", true);

	// Todo Implement function

	MovementComponent->MaxWalkSpeed /= 2.0f;
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
		// Todo update player values
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

void AYlva::IncreaseStamina(const float Amount)
{
	StaminaComponent->IncreaseStamina(Amount);

	UpdateCharacterInfo();

	if (!StaminaComponent->IsLowStamina() && bWasLowStaminaEventTriggered)
	{
		// Todo update player values
		MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

		bWasLowStaminaEventTriggered = false;
	}
}

void AYlva::ResetStamina()
{
	StaminaComponent->ResetStamina();

	UpdateCharacterInfo();

	// Todo update player values
	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
}

void AYlva::StartLosingStamina()
{
	StaminaRegenTimeline->PlayFromStart();
}

void AYlva::LoseStamina()
{
	const float Time = StaminaRegenCurve->GetFloatValue(StaminaRegenTimeline->GetPlaybackPosition());

	StaminaComponent->SetSmoothedStamina(FMath::Lerp(StaminaComponent->GetPreviousStamina(), StaminaComponent->GetCurrentStamina(), Time));

	UpdateCharacterInfo();
}

void AYlva::FinishLosingStamina()
{
	StaminaComponent->DelayRegeneration();

	UpdateCharacterInfo();
}

void AYlva::StartGainingCharge(const float Amount)
{
	ChargeAttackComponent->IncreaseCharge(Amount);

	ChargeAttackTimeline->PlayFromStart();
}

void AYlva::GainCharge()
{
	const float Time = ChargeAttackCurve->GetFloatValue(ChargeAttackTimeline->GetPlaybackPosition());

	ChargeAttackComponent->SetSmoothedCharge(FMath::Lerp(ChargeAttackComponent->GetPreviousCharge(), ChargeAttackComponent->GetCurrentCharge(), Time));

	UpdateCharacterInfo();
}

void AYlva::FinishGainingCharge()
{
	if (ChargeAttackComponent->CanLoseChargeOvertime() && !ChargeAttackComponent->IsChargeFull())
		ChargeAttackComponent->DelayChargeLoss();

	UpdateCharacterInfo();
}

void AYlva::IncreaseCharge()
{
	if (ChargeAttackComponent->IsUsingSmoothBar())
		StartGainingCharge(ChargeAttackComponent->GetChargeGain());
	else
		ChargeAttackComponent->IncreaseCharge(ChargeAttackComponent->GetChargeGain());

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

void AYlva::ResetGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
}

void AYlva::AttachSword() const
{
	if (R_SwordMesh)
	{
		R_SwordMesh->SetWorldLocation(GetMesh()->GetSocketLocation(R_SwordStartSocketName));
		R_SwordMesh->SetWorldRotation(GetMesh()->GetSocketRotation(R_SwordStartSocketName) + StartRightSwordRotation);
		
		R_SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, R_SwordStartSocketName);
	}
}

void AYlva::DetachSword()
{
	if (Combat.SwordStickTime <= 0.0f)
		return;

	if (R_SwordMesh)
	{
		R_SwordMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetWorldTimerManager().SetTimer(SwordDetachmentExpiryTimer, this, &AYlva::AttachSword, Combat.SwordStickTime, false);
	}
}

void AYlva::StartParryEvent()
{
	GameInstance->PlayerInfo.bParrySucceeded = true;

	UGameplayStatics::SetGlobalTimeDilation(this, Combat.ParrySettings.TimeDilationOnSuccessfulParry);

	if (!GetWorldTimerManager().IsTimerActive(ParryEventExpiryTimer))
		GetWorldTimerManager().SetTimer(ParryEventExpiryTimer, this, &AYlva::FinishParryEvent, Combat.ParrySettings.ParryCameraAnimInst->CamAnim->AnimLength);
}

void AYlva::FinishParryEvent()
{
	FSM->PopState();
	FSM->PopState("Block");
}

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
}

bool AYlva::IsParrySuccessful()
{
	return FSM->GetActiveStateName() == "Block" && FSM->GetActiveStateUptime() < Combat.ParrySettings.ParryWindowTime;
}

void AYlva::StartDashCooldown()
{
	GetWorldTimerManager().SetTimer(DashCooldownTimer, MovementSettings.DashCooldown, false);
}

void AYlva::ApplyHitStop()
{
	if (Combat.bEnableHitStop)
	{
		DetachSword();
	}
}

bool AYlva::IsLightAttacking() const
{
	return FSM->GetActiveStateID() == 3 || FSM->GetActiveStateID() == 8;
}

bool AYlva::IsHeavyAttacking() const
{
	return FSM->GetActiveStateID() == 9 || FSM->GetActiveStateID() == 10;
}

void AYlva::PauseAnimsWithTimer()
{
	PauseAnims();
	GetWorldTimerManager().SetTimer(HitStopTimerHandle, this, &AYlva::UnPauseAnims, Combat.HitStopTime);
}

UStaticMeshComponent* AYlva::GetLeftHandSword()
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
			StartLeftSwordRotation = Cast<UStaticMeshComponent>(Component)->RelativeRotation;
			return Cast<UStaticMeshComponent>(Component);
		}
	}

	#if !UE_BUILD_SHIPPING
	ULog::Info("Could not find the left hand sword", true);
	#endif

	return nullptr;
}

UStaticMeshComponent* AYlva::GetRightHandSword()
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
			StartRightSwordRotation = Cast<UStaticMeshComponent>(Component)->RelativeRotation;
			return Cast<UStaticMeshComponent>(Component);
		}
	}

	#if !UE_BUILD_SHIPPING
	ULog::Info("Could not find the right hand sword", true);
	#endif

	return nullptr;
}
