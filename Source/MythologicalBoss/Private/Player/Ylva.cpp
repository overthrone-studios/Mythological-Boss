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
#include "Components/DashComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Misc/FeatData.h"

#include "Animation/AnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ConstructorHelpers.h"
#include "TimerManager.h"

AYlva::AYlva() : AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Ylva/Animation/AnimBP_Ylva.AnimBP_Ylva_C'"));

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
	FSM->AddState(12, "Dash");
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

	FSM->GetState(4)->OnEnterState.AddDynamic(this, &AYlva::OnEnterBlockingState);
	FSM->GetState(4)->OnUpdateState.AddDynamic(this, &AYlva::UpdateBlockingState);
	FSM->GetState(4)->OnExitState.AddDynamic(this, &AYlva::OnExitBlockingState);

	FSM->GetState(5)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDeathState);
	FSM->GetState(5)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDeathState);
	FSM->GetState(5)->OnExitState.AddDynamic(this, &AYlva::OnExitDeathState);

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
	StaminaRegenTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Stamina Regen Timeline"));

	// Charge attack component
	ChargeAttackComponent = CreateDefaultSubobject<UChargeAttackComponent>(FName("Charge Attack Component"));
	ChargeAttackTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Charge Attack Timeline"));

	// Attack combo component
	AttackComboComponent = CreateDefaultSubobject<UAttackComboComponent>(FName("Attack Combo Component"));
	AttackComboComponent->OnComboReset.AddDynamic(this, &AYlva::OnComboReset);

	// Dash component
	DashComponent = CreateDefaultSubobject<UDashComponent>(FName("Dash component"));

	// Configure character settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AYlva::BeginPlay()
{
	Super::BeginPlay();

	InitTimelineComponent(StaminaRegenTimeline, StaminaRegenCurve, 1.0f, FName("LoseStamina"), FName("FinishLosingStamina"));
	InitTimelineComponent(ChargeAttackTimeline, ChargeAttackCurve, 1.0f, FName("GainCharge"), FName("FinishGainingCharge"));

	// Initialize our variables
	Boss = UOverthroneFunctionLibrary::GetBossCharacter(World);
	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	YlvaAnimInstance = Cast<UYlvaAnimInstance>(GetMesh()->GetAnimInstance());
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("FSMVisualizer"));

	Combat.AttackSettings.OriginalLightAttackDamage = Combat.AttackSettings.LightAttackDamage;
	Combat.AttackSettings.OriginalHeavyAttackDamage = Combat.AttackSettings.HeavyAttackDamage;

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

	GameInstance->PlayerInfo.TeleportRadius = TeleportRadius;
	GameInstance->Player = this;

	// Bind events to our functions
	GameInstance->PlayerInfo.OnLowHealth.AddDynamic(this, &AYlva::OnLowHealth);
	GameInstance->PlayerInfo.OnLowStamina.AddDynamic(this, &AYlva::OnLowStamina);
	GameInstance->OnBossDeath.AddDynamic(this, &AYlva::OnBossDeath);

	UntouchableFeat = GameInstance->GetFeat("Untouchable");

	AnimInstance->OnMontageEnded.AddDynamic(this, &AYlva::OnAttackEnd);

	// Begin the state machine
	FSM->Start();
}

void AYlva::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameInstance->PlayerInfo.Location = GetActorLocation();

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->MovementDirection = CalculateDirection();

	YlvaAnimInstance->bIsMoving = IsMovingInAnyDirection();

	CalculateRollLean(DeltaTime);
	CalculatePitchLean(DeltaTime);

	// Lock-on mechanic
	if (LockOnSettings.bShouldLockOnTarget)
	{
		const FRotator Target = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), GameInstance->BossInfo.Location);
		const FRotator SmoothedRotation = FMath::RInterpTo(GetControlRotation(), Target, DeltaTime, 10.0f);

		const FRotator NewRotation = FRotator(LockOnSettings.LockOnPitch, SmoothedRotation.Yaw, GetControlRotation().Roll);

		GetController()->SetControlRotation(NewRotation);

		GameInstance->SetLockOnLocation(GameInstance->BossInfo.Location);
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

#if !UE_BUILD_SHIPPING
	if (Debug.bLogCameraPitch)
		ULog::Number(GetControlRotation().Pitch, "Pitch: ", true);

	if (Debug.bShowTeleportRadius)
		UKismetSystemLibrary::DrawDebugCircle(this, GetActorLocation(), TeleportRadius, 32, FColor::Red, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

	if (Debug.bLogPlayerInputValues)
	{
		ULog::Number(ForwardInput, "Forward: ", true);
		ULog::Number(RightInput, "Right: ", true);
	}

	if (Debug.bLogMovementSpeed)
		ULog::Number(CurrentMovementSpeed, "Movement Speed: ", true);
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
	PlayerInputComponent->BindAction("Run", IE_Repeat, this, &AYlva::UpdateIsRunHeld);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AYlva::StopRunning);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AYlva::Dash);

	PlayerInputComponent->BindAction("Charge Attack", IE_Repeat, this, &AYlva::ChargeUpAttack);
	PlayerInputComponent->BindAction("Charge Attack", IE_Released, this, &AYlva::ReleaseChargeAttack);

	PlayerInputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AYlva::Pause).bExecuteWhenPaused = true;
	PlayerInputComponent->BindKey(EKeys::Gamepad_Special_Right, IE_Pressed, this, &AYlva::Pause).bExecuteWhenPaused = true;

#if !UE_BUILD_SHIPPING
	// Debugging
	PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &AYlva::ShowPlayerFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AYlva::ShowBossFSMVisualizer);
	PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &AYlva::ShowMainHUD);
	PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &AYlva::ShowNoHUD);

	PlayerInputComponent->BindKey(EKeys::Five, IE_Pressed, this, &AYlva::ToggleGodMode);

	PlayerInputComponent->BindKey(EKeys::Six, IE_Pressed, this, &AYlva::Debug_Die);
	PlayerInputComponent->BindKey(EKeys::Seven, IE_Pressed, this, &AYlva::Debug_MaxHealth);
	PlayerInputComponent->BindKey(EKeys::Eight, IE_Pressed, this, &AYlva::Debug_RefillStamina);
	PlayerInputComponent->BindKey(EKeys::Nine, IE_Pressed, this, &AYlva::Debug_MaxCharge);
	PlayerInputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &AYlva::Debug_ToggleBuff);

	PlayerInputComponent->BindKey(EKeys::L, IE_Pressed, this, &AYlva::Debug_ResetFeats);
#endif
}

void AYlva::MoveForward(const float Value)
{
	if (!IsAttacking())
	{
		ForwardInput = Value;
		YlvaAnimInstance->ForwardInput = ForwardInput;
	}
	else
	{
		ForwardInput = 0.0f;
		YlvaAnimInstance->ForwardInput = 0.0f;
	}

	if (Controller && ForwardInput != 0.0f)
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
	if (!IsAttacking())
	{
		RightInput = Value;
		YlvaAnimInstance->RightInput = RightInput;
	}
	else
	{
		RightInput = 0.0f;
		YlvaAnimInstance->RightInput = 0.0f;
	}

	if (Controller && RightInput != 0.0f)
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

float AYlva::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead
	if (FSM->GetActiveStateName() == "Death")
		return DamageAmount;

	BeginTakeDamage(DamageAmount);

	// Apply damage once
	if (HealthComponent->GetCurrentHealth() > 0.0f && (!AnimInstance->bIsHit || !YlvaAnimInstance->bIsShieldHit))
	{
		ApplyDamage(DamageAmount);
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
	GetWorldTimerManager().ClearTimer(DeathExpiryTimerHandle);

	FSM->PopState();

	GameInstance->PlayerInfo.bIsDead = false;

	UGameplayStatics::OpenLevel(this, *UGameplayStatics::GetCurrentLevelName(this));
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

void AYlva::LoseHealth()
{
	Super::LoseHealth();

#if !UE_BUILD_SHIPPING
	if (Debug.bLogHealthValues)
		ULog::Number(HealthComponent->GetSmoothedHealth(), "New Health: ", true);
#endif
}

void AYlva::StartLosingHealth()
{
	Super::StartLosingHealth();

#if !UE_BUILD_SHIPPING
	if (Debug.bLogHealthValues)
	{
		ULog::Number(HealthComponent->GetPreviousHealth(), "Previous Health: ", true);
		ULog::Number(HealthComponent->GetCurrentHealth(), "Target Health: ", true);
	}
#endif
}

float AYlva::CalculateDirection() const
{
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(FollowCamera->GetComponentRotation(), GetCapsuleComponent()->GetComponentRotation());
	const FRotator NewDirection = UKismetMathLibrary::NormalizedDeltaRotator(DeltaRotation, UKismetMathLibrary::MakeRotFromX(FVector(ForwardInput, -RightInput, 0.0f)));

	return NewDirection.GetNormalized().Yaw;
}

void AYlva::CalculateRollLean(const float DeltaTime)
{
	if (FSM->GetActiveStateID() != 0 /*Idle*/ && FSM->GetActiveStateID() != 4 /*Block*/ && FSM->GetActiveStateID() != 5 /*Death*/)
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
	if (FSM->GetActiveStateID() != 1 /*Walk*/ && FSM->GetActiveStateID() != 2 /*Run*/ && !IsAttacking() && FSM->GetActiveStateID() != 5 /*Death*/)
	{
		PlayerLeanPitchAmount = FMath::FInterpTo(PlayerLeanPitchAmount, FollowCamera->GetForwardVector().Rotation().Pitch, DeltaTime, 5.0f);
		YlvaAnimInstance->LeanPitchAmount = PlayerLeanPitchAmount * MovementSettings.LeanPitchOffset;
	}
	else
	{
		PlayerLeanPitchAmount = FMath::FInterpTo(PlayerLeanPitchAmount, 0.0f, DeltaTime, 10.0f);
		YlvaAnimInstance->LeanPitchAmount = PlayerLeanPitchAmount;
	}
}

#pragma region Combat
void AYlva::LightAttack()
{
	// Are we in any of these states?
	if (FSM->GetActiveStateID() == 5 /*Death*/ ||
		FSM->GetActiveStateID() == 20 /*Damaged*/ ||
		IsChargeAttacking())
		return;

	FSM->PopState("Block");

	if (FSM->GetActiveStateID() == 22 /*Parry*/)
		FinishParryEvent();

	if (StaminaComponent->HasEnoughForLightAttack() && !AttackComboComponent->IsDelaying() && !AttackComboComponent->IsAtTreeEnd() && !IsDashing())
	{
		UAnimMontage* AttackMontageToPlay = AttackComboComponent->AdvanceCombo(Light);

		BeginLightAttack(AttackMontageToPlay);
	}
}

void AYlva::BeginLightAttack(class UAnimMontage* AttackMontage)
{
	AnimInstance->Montage_Play(AttackMontage);

	Combat.AttackSettings.LightAttackDamage *= AttackComboComponent->GetDamageMultiplier();

	if (!bGodMode)
	{
		UpdateStamina(StaminaComponent->GetLightAttackValue());
	}

	if (Combat.bRotateToCameraLookDirection)
		bUseControllerRotationYaw = true;
}

void AYlva::HeavyAttack()
{
	// Are we in any of these states?
	if (FSM->GetActiveStateID() == 5 /*Death*/ ||
		FSM->GetActiveStateID() == 20 /*Damaged*/ ||
		IsChargeAttacking())
		return;

	FSM->PopState("Block");

	if (FSM->GetActiveStateID() == 22 /*Parry*/)
		FinishParryEvent();

	if (StaminaComponent->HasEnoughForHeavyAttack() && !AttackComboComponent->IsDelaying() && !AttackComboComponent->IsAtTreeEnd() && !IsDashing())
	{
		UAnimMontage* AttackMontageToPlay = AttackComboComponent->AdvanceCombo(Heavy);

		BeginHeavyAttack(AttackMontageToPlay);
	}
}

void AYlva::BeginHeavyAttack(class UAnimMontage* AttackMontage)
{
	AnimInstance->Montage_Play(AttackMontage);

	Combat.AttackSettings.HeavyAttackDamage *= AttackComboComponent->GetDamageMultiplier();

	if (!bGodMode)
	{
		UpdateStamina(StaminaComponent->GetHeavyAttackValue());
	}

	if (Combat.bRotateToCameraLookDirection)
		bUseControllerRotationYaw = true;
}

void AYlva::ChargeUpAttack()
{
	if (!ChargeAttackComponent->IsChargeFull())
		return;

	ChargeAttackHoldFrames++;

	PlayerController->ClientPlayCameraShake(CameraShakes.Charge.Shake,CameraShakes.Charge.Intensity);

	// Run once
	if (IsChargeAttacking())
	{
		MovementComponent->SetMovementMode(MOVE_None);
		YlvaAnimInstance->bCanCharge = true;

		if (!Combat.ChargeSettings.ChargeCameraAnimInst)
		{
			if (Combat.ChargeSettings.ChargeCameraAnim)
				Combat.ChargeSettings.ChargeCameraAnimInst = CameraManager->PlayCameraAnim(Combat.ChargeSettings.ChargeCameraAnim);
		}

		const FRotator NewRotation = FRotator(Combat.ParrySettings.CameraPitchOnSuccess,GetActorForwardVector().Rotation().Yaw,GetControlRotation().Roll);
		GetController()->SetControlRotation(NewRotation);

		PlayerController->SetIgnoreLookInput(true);
	}

	ULog::Number(ChargeAttackHoldFrames,"Charge Hold: ",true);
}

void AYlva::ReleaseChargeAttack()
{
	if (ChargeAttackComponent->IsChargeFull())
	{
		PlayerController->ClientPlayCameraShake(CameraShakes.ChargeEnd.Shake,CameraShakes.ChargeEnd.Intensity);
		ResetCharge();
	}

	YlvaAnimInstance->bChargeReleased = true;

	Combat.ChargeSettings.ChargeCameraAnimInst = nullptr;

	PlayerController->ResetIgnoreLookInput();

	GetWorldTimerManager().SetTimer(ChargeAttackReleaseTimer,this,&AYlva::FinishChargeAttack,0.2f);

	MovementComponent->SetMovementMode(MOVE_Walking);
}

void AYlva::FinishChargeAttack()
{
	ChargeAttackHoldFrames = 0;

	YlvaAnimInstance->bCanCharge = false;
	YlvaAnimInstance->bChargeReleased = false;
}

void AYlva::StartParryEvent()
{
	GameInstance->PlayerInfo.bParrySucceeded = true;

	UGameplayStatics::SetGlobalTimeDilation(this, Combat.ParrySettings.TimeDilationOnSuccessfulParry);

	if (!GetWorldTimerManager().IsTimerActive(ParryEventExpiryTimer))
		GetWorldTimerManager().SetTimer(ParryEventExpiryTimer,this, &AYlva::FinishParryEvent, Combat.ParrySettings.ParryCameraAnimInst->CamAnim->AnimLength);
}

void AYlva::FinishParryEvent()
{
	FSM->PopState();
	FSM->PopState("Block");
}

void AYlva::Block()
{
	if (IsChargeAttacking())
		return;

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

	AnimInstance->Montage_Stop(0.3f,Combat.BlockSettings.BlockIdle);
	bUseControllerRotationYaw = false;

	FSM->PopState();
}

void AYlva::BeginTakeDamage(float DamageAmount)
{
}

void AYlva::ApplyDamage(const float DamageAmount)
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
		return;
	}

	if (bGodMode)
		return;

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
			HitCounter_Persistent++;

			// Enter damaged state
			FSM->PushState("Damaged");

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

void AYlva::EndTakeDamage()
{
	if (HealthComponent->GetCurrentHealth() <= 0.0f && FSM->GetActiveStateName() != "Death")
	{
		Die();
	}
}
#pragma endregion

#pragma region Movement
void AYlva::Run()
{
	if (FSM->GetActiveStateName() == "Death" || IsChargeAttacking())
		return;

	if (IsLockedOn() && IsMovingForward() && StaminaComponent->HasStamina() || !IsLockedOn() && IsMovingInAnyDirection())
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

	if (FSM->GetActiveStateName() == "Death" ||
		FSM->GetActiveStateName() == "Light Attack 1" ||
		FSM->GetActiveStateName() == "Light Attack 2" ||
		FSM->GetActiveStateName() == "Heavy Attack 1" ||
		FSM->GetActiveStateName() == "Heavy Attack 2")
		return;

	if (LockOnSettings.bShouldLockOnTarget)
		MovementComponent->MaxWalkSpeed = MovementSettings.LockOnWalkSpeed;
	else
		MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

	// Delay stamina regeneration
	if (FSM->GetActiveStateName() == "Run")
		StaminaComponent->DelayRegeneration();

	FSM->PopState();
}

void AYlva::Dash()
{
	if (FSM->GetActiveStateName() == "Death" || IsChargeAttacking())
		return;

	// If we are moving and grounded
	if (!DashComponent->IsCooldownActive() && FSM->GetActiveStateID() != 12 /*Dash*/)
	{
		// Do we have enough stamina to dash?
		if (StaminaComponent->HasEnoughForDash())
		{
			if (!bGodMode)
			{
				StartDashCooldown();
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
#pragma endregion

#pragma region Lock-On
void AYlva::ToggleLockOn()
{
	// Don't lock on if boss is dead
	if (GameInstance->BossInfo.Health <= 0.0f || FSM->GetActiveStateName() == "Death")
		return;

	LockOnSettings.bShouldLockOnTarget = !LockOnSettings.bShouldLockOnTarget;
	PlayerController->SetIgnoreLookInput(LockOnSettings.bShouldLockOnTarget);
	GameInstance->ToggleLockOnVisibility(LockOnSettings.bShouldLockOnTarget);
	MovementComponent->bUseControllerDesiredRotation = LockOnSettings.bShouldLockOnTarget ? true : false;
	MovementComponent->bOrientRotationToMovement = LockOnSettings.bShouldLockOnTarget ? false : true;
	YlvaAnimInstance->bIsLockedOn = LockOnSettings.bShouldLockOnTarget ? true : false;

	MovementComponent->MaxWalkSpeed = LockOnSettings.bShouldLockOnTarget ? MovementSettings.LockOnWalkSpeed : MovementSettings.WalkSpeed;
}

void AYlva::EnableLockOn()
{
	// Don't lock on if boss is dead
	if (GameInstance->BossInfo.Health <= 0.0f || FSM->GetActiveStateName() == "Death")
		return;

	LockOnSettings.bShouldLockOnTarget = true;
	PlayerController->SetIgnoreLookInput(true);
	GameInstance->ToggleLockOnVisibility(true);
	MovementComponent->bUseControllerDesiredRotation = true;
	MovementComponent->bOrientRotationToMovement = false;
	YlvaAnimInstance->bIsLockedOn = true;

	MovementComponent->MaxWalkSpeed = MovementSettings.LockOnWalkSpeed;
}

void AYlva::DisableLockOn()
{
	LockOnSettings.bShouldLockOnTarget = false;
	PlayerController->SetIgnoreLookInput(false);
	GameInstance->ToggleLockOnVisibility(false);
	MovementComponent->bUseControllerDesiredRotation = false;
	MovementComponent->bOrientRotationToMovement = true;
	YlvaAnimInstance->bIsLockedOn = false;

	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
}
#pragma endregion

#pragma region Controls
void AYlva::Pause()
{
	if (GameInstance->IsGamePaused())
		GameInstance->UnPauseGame();
	else
		GameInstance->PauseGame();
}

void AYlva::DisableControllerRotationYaw()
{
	bUseControllerRotationYaw = false;
}
#pragma endregion

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
#pragma endregion

#pragma region Stamina
void AYlva::RegenerateStamina(const float Rate)
{
	if (bGodMode || FSM->GetActiveStateID() == 5 /*Death*/)
		return;

	IncreaseStamina(Rate * World->DeltaTimeSeconds);
}

void AYlva::UpdateStamina(const float StaminaToSubtract)
{
	StaminaComponent->UpdatePreviousStamina();

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
	const float Alpha = StaminaRegenCurve->GetFloatValue(StaminaRegenTimeline->GetPlaybackPosition());

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
	StartGainingCharge(ChargeAttackComponent->GetChargeGain());

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
}
#pragma endregion

#pragma region Events
void AYlva::OnLowHealth()
{
	ChangeHitboxSize(Combat.AttackSettings.AttackRadiusOnLowHealth);
}

void AYlva::OnBossDeath()
{
	DisableLockOn();

	if (!HasTakenAnyDamage() && !UntouchableFeat->bIsComplete)
		OnUntouchableFeatAchieved();
}

void AYlva::OnLowStamina()
{
	// Todo Implement function

	MovementComponent->MaxWalkSpeed /= 2.0f;
}

void AYlva::OnComboReset()
{
	Combat.AttackSettings.LightAttackDamage = Combat.AttackSettings.OriginalLightAttackDamage;
	Combat.AttackSettings.HeavyAttackDamage = Combat.AttackSettings.OriginalHeavyAttackDamage;
}

void AYlva::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	AttackComboComponent->OnAttackEnd.Broadcast();
}
#pragma endregion

#pragma region Player States
#pragma region Idle
void AYlva::OnEnterIdleState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->Montage_Stop(0.3f, Combat.BlockSettings.BlockIdle);
}

void AYlva::UpdateIdleState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	PlayerController->ClientPlayCameraShake(CameraShakes.Idle.Shake, CameraShakes.Idle.Intensity);

	if (IsMovingInAnyDirection() && MovementComponent->IsMovingOnGround())
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

	PlayerController->ClientPlayCameraShake(CameraShakes.Walk.Shake, CameraShakes.Walk.Intensity);

	if (bIsRunKeyHeld && StaminaComponent->HasStamina() && GetVelocity().Size() > MovementSettings.WalkSpeed)
	{
		FSM->PushState("Run");
		return;
	}

	if (!IsMovingInAnyDirection())
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

	PlayerController->ClientPlayCameraShake(CameraShakes.Run.Shake, CameraShakes.Run.Intensity);

	if (!bGodMode)
		UpdateStamina(StaminaComponent->GetRunValue() * World->DeltaTimeSeconds);

	if (!IsMovingInAnyDirection() || MovementComponent->MaxWalkSpeed < MovementSettings.RunSpeed || StaminaComponent->IsStaminaEmpty())
	{
		StaminaComponent->DelayRegeneration();
		FSM->PopState();
		return;
	}

	// If we are locked on AND trying to run in a direction other than forward
	if ((ForwardInput < 0.0f || RightInput != 0.0f) && IsLockedOn())
	{
		StaminaComponent->DelayRegeneration();
		FSM->PopState();
	}
}

void AYlva::OnExitRunState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	if (LockOnSettings.bShouldLockOnTarget)
		MovementComponent->MaxWalkSpeed = MovementSettings.LockOnWalkSpeed;
	else
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

#pragma region Damaged
void AYlva::OnEnterDamagedState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	if (MovementSettings.bStopMovingWhenDamaged)
		MovementComponent->SetMovementMode(MOVE_None);

	AnimInstance->bIsHit = true;
}

void AYlva::UpdateDamagedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If hit animation has finished, go back to previous state
	if (AnimInstance->AnimTimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitDamagedState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);

	AnimInstance->bIsHit = false;
}
#pragma endregion 

#pragma region Death
void AYlva::OnEnterDeathState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsDead = true;

	DisableLockOn();

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

	StaminaComponent->DelayRegeneration();
}

void AYlva::UpdateShieldHitState()
{
	// If shield impact animation has finished, go back to previous state
	if (AnimInstance->AnimTimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitShieldHitState()
{
	YlvaAnimInstance->bIsShieldHit = false;
}
#pragma endregion 

#pragma region Dash
void AYlva::OnEnterDashState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsDashing = true;
	AnimInstance->ActiveStateMachine = AnimInstance->StateMachines[1];
}

void AYlva::UpdateDashState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (AnimInstance->AnimTimeRemaining < 0.1f)
	{
		FSM->PopState();
	}
}

void AYlva::OnExitDashState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsDashing = false;
	AnimInstance->ActiveStateMachine = AnimInstance->StateMachines[0];
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

	AnimInstance->Montage_Play(Combat.ParrySettings.ParryMontage);

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

	GameInstance->PlayerInfo.bParrySucceeded = false;

	ResetGlobalTimeDilation();

	GetWorldTimerManager().ClearTimer(ParryEventExpiryTimer);
}
#pragma endregion 
#pragma endregion 

void AYlva::ApplyHitStop()
{
	if (Combat.bEnableHitStop)
	{
		// Todo: Hit stop implementation
	}
}

bool AYlva::IsLightAttacking() const
{
	return AttackComboComponent->GetCurrentAttack() == Light;
}

bool AYlva::IsHeavyAttacking() const
{
	return AttackComboComponent->GetCurrentAttack() == Heavy;
}

bool AYlva::IsAttacking() const
{
	return IsLightAttacking() || IsHeavyAttacking() || IsChargeAttacking();
}

bool AYlva::IsChargeAttacking() const
{
	return ChargeAttackHoldFrames > 1;
}

bool AYlva::IsParrySuccessful() const
{
	return FSM->GetActiveStateName() == "Block" && FSM->GetActiveStateUptime() < Combat.ParrySettings.ParryWindowTime;
}

bool AYlva::IsDashing() const
{
	return FSM->GetActiveStateID() == 12;
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

bool AYlva::IsLockedOn() const
{
	return LockOnSettings.bShouldLockOnTarget;
}

void AYlva::ResetGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
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
