// Copyright Overthrone Studios 2019

#include "Player/Ylva.h"
#include "Player/YlvaAnimInstance.h"
#include "Public/OverthroneHUD.h"
#include "Public/OverthroneGameInstance.h"
#include "Widgets/HUD/MasterHUD.h"
#include "Widgets/HUD/FSMVisualizerHUD.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ConstructorHelpers.h"
#include "TimerManager.h"
#include "FSM.h"
#include "Log.h"
#include "OverthroneFunctionLibrary.h"

AYlva::AYlva()
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
	FSM->AddState(7, "Fall");
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

	FSM->GetState(7)->OnEnterState.AddDynamic(this, &AYlva::OnEnterFallingState);
	FSM->GetState(7)->OnUpdateState.AddDynamic(this, &AYlva::UpdateFallingState);
	FSM->GetState(7)->OnExitState.AddDynamic(this, &AYlva::OnExitFallingState);

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
	GetCapsuleComponent()->bReturnMaterialOnMove = true;

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

	// Configure character settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	bCanBeDamaged = true;
}

void AYlva::BeginPlay()
{
	Super::BeginPlay();

	bCanBeDamaged = true;

	Health = StartingHealth;
	Stamina = StartingStamina;

	// Cache the world object
	World = GetWorld();

	// Cache the boss character
	Boss = UOverthroneFunctionLibrary::GetBossCharacter(World);

	// Cache the player controller
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Cache the movement component
	MovementComponent = GetCharacterMovement();

	// Cache our anim instance
	AnimInstance = Cast<UYlvaAnimInstance>(GetMesh()->GetAnimInstance());

	// Cache the player HUD
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->PlayerStartingHealth = StartingHealth;
	GameInstance->PlayerHealth = Health;
	GameInstance->PlayerStartingStamina = StartingStamina;
	GameInstance->PlayerStamina = Stamina;
	GameInstance->OnBossDeath.AddDynamic(this, &AYlva::OnBossDeath);
	GameInstance->Player = this;

	// Cache the FSM Visualizer HUD
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD(UFSMVisualizerHUD::StaticClass()));

	// Begin the state machine
	FSM->Start();
}

void AYlva::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameInstance->PlayerLocation = GetActorLocation();

	if (LockOnSettings.bShouldLockOnTarget)
	{
		const FRotator Target = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), GameInstance->BossLocation);
		const FRotator SmoothedRotation = FMath::RInterpTo(GetControlRotation(), Target, DeltaTime, 10.0f);

		const FRotator NewRotation = FRotator(LockOnSettings.LockOnPitch, SmoothedRotation.Yaw, GetControlRotation().Roll);

		GetController()->SetControlRotation(NewRotation);

		GameInstance->SetLockOnLocation(GameInstance->BossLocation);
		GameInstance->SetLockOnRotation(NewRotation - FRotator(0.0f, 180.0f, 0.0f));
	}
}

void AYlva::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Crash if we don't have a valid Input component
	check(PlayerInputComponent);

	// Set up game-play key bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AYlva::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AYlva::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
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

void AYlva::MoveForward(const float Value)
{
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
	if (GameInstance->BossHealth <= 0.0f || FSM->GetActiveStateName() == "Death")
		return;

	LockOnSettings.bShouldLockOnTarget = !LockOnSettings.bShouldLockOnTarget;
	PlayerController->SetIgnoreLookInput(LockOnSettings.bShouldLockOnTarget);
	GameInstance->ToggleLockOnVisibility(LockOnSettings.bShouldLockOnTarget);
}

void AYlva::EnableLockOn()
{
	// Don't lock on if boss is dead
	if (GameInstance->BossHealth <= 0.0f || FSM->GetActiveStateName() == "Death")
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
	if (FSM->GetActiveStateID() != 7 /*Fall*/ &&
		FSM->GetActiveStateID() != 5 /*Death*/ &&
		FSM->GetActiveStateID() != 20 /*Damaged*/
		)
	{
		FSM->PopState();
		FSM->PushState("Block");
	}
}

void AYlva::StopBlocking()
{
	FSM->PopState("Block");
}

void AYlva::LightAttack()
{
	// Are we in any of these states?
	if (FSM->GetActiveStateID() == 7 /*Fall*/ ||
		FSM->GetActiveStateID() == 5 /*Death*/ ||
		FSM->GetActiveStateID() == 20 /*Damaged*/)
		return;

	if (FSM->GetActiveStateID() == 22 /*Parry*/)
		FSM->PopState();

	DelayStaminaRegeneration();

	if (FSM->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		FSM->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		FSM->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		FSM->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		Stamina > Combat.StaminaSettings.LightAttackStamina)
	{
		FSM->PushState("Light Attack 1");
		bUseControllerRotationYaw = true;
		MovementComponent->SetMovementMode(MOVE_None);
	}
	else if (
		FSM->GetActiveStateID() == 3 /*Light Attack 1*/ &&
		FSM->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		FSM->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		FSM->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		FSM->GetActiveStateUptime() > 0.4f &&
		Stamina > Combat.StaminaSettings.LightAttackStamina)
	{
		FSM->PopState("Light Attack 1");
		FSM->PushState("Light Attack 2");
		bUseControllerRotationYaw = true;
		MovementComponent->SetMovementMode(MOVE_None);
	}
	else if (
		FSM->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		FSM->GetActiveStateID() == 8 /*Light Attack 2*/ &&
		FSM->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		FSM->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		FSM->GetActiveStateUptime() > 0.6f &&
		Stamina > Combat.StaminaSettings.LightAttackStamina)
	{
		FSM->PopState("Light Attack 2");
		FSM->PushState("Light Attack 1");
		bUseControllerRotationYaw = true;
		MovementComponent->SetMovementMode(MOVE_None);
	}
}

void AYlva::HeavyAttack()
{
	// Are we in any of these states?
	if (FSM->GetActiveStateID() == 7 /*Fall*/ ||
		FSM->GetActiveStateID() == 5 /*Death*/ ||
		FSM->GetActiveStateID() == 20 /*Damaged*/)
		return;

	if (FSM->GetActiveStateID() == 22 /*Parry*/)
		FSM->PopState();

	DelayStaminaRegeneration();

	if (FSM->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		FSM->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		FSM->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		FSM->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		Stamina > Combat.StaminaSettings.HeavyAttackStamina)
	{
		FSM->PushState("Heavy Attack 1");
		bUseControllerRotationYaw = true;
		MovementComponent->SetMovementMode(MOVE_None);
	}
	else if (
		FSM->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		FSM->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		FSM->GetActiveStateID() == 9 /*Heavy Attack 1*/ &&
		FSM->GetActiveStateUptime() > 0.7f &&
		Stamina > Combat.StaminaSettings.HeavyAttackStamina)
	{
		FSM->PopState("Heavy Attack 1");
		FSM->PushState("Heavy Attack 2");
		bUseControllerRotationYaw = true;
		MovementComponent->SetMovementMode(MOVE_None);
	}
	else if (
		FSM->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		FSM->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		FSM->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		FSM->GetActiveStateID() == 10 /*Heavy Attack 2*/ &&
		FSM->GetActiveStateUptime() > 0.5f &&
		Stamina > Combat.StaminaSettings.HeavyAttackStamina)
	{
		FSM->PopState("Heavy Attack 2");
		FSM->PushState("Heavy Attack 1");
		bUseControllerRotationYaw = true;
		MovementComponent->SetMovementMode(MOVE_None);
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
	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround() && Stamina > Combat.StaminaSettings.RunStamina * World->DeltaTimeSeconds)
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
		DelayStaminaRegeneration();

	FSM->PopState();
}

void AYlva::Dash()
{
	if (FSM->GetActiveStateName() == "Death")
		return;

	// If we are moving and grounded
	if (GetVelocity().Size() > 0.0f && MovementComponent->IsMovingOnGround())
	{
		// Do the dash
		FVector VelocityNormalized = GetVelocity();
		VelocityNormalized.Normalize();
		VelocityNormalized.Z = 0;

		// Do we have enough stamina to dash?
		if (!bGodMode && Stamina > Combat.StaminaSettings.DashStamina)
		{
			UpdateStamina(Combat.StaminaSettings.DashStamina);
			LaunchCharacter(VelocityNormalized * MovementSettings.DashForce, true, true);

			DelayStaminaRegeneration();
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
	if (bGodMode || FSM->GetActiveStateID() == 5 /*Death*/ || GetWorldTimerManager().IsTimerActive(StaminaRegenTimerHandle))
		return;

	if (Stamina < StartingStamina)
	{
		Stamina += Rate * World->DeltaTimeSeconds;
		GameInstance->PlayerStamina = Stamina;
	}
}

void AYlva::Die()
{
	SetHealth(0.0f);

	bCanBeDamaged = false;

	AnimInstance->LeaveAllStates();

	FSM->RemoveAllStatesFromStack();
	FSM->PushState("Death");
}

void AYlva::Respawn()
{
	GetWorldTimerManager().ClearTimer(DeathStateExpiryTimer);

	FSM->PopState();

	GameInstance->bIsPlayerDead = false;

	UGameplayStatics::OpenLevel(this, *UGameplayStatics::GetCurrentLevelName(this));
}

#pragma region Idle
void AYlva::OnEnterIdleState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());
}

void AYlva::UpdateIdleState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	RegenerateStamina(StaminaRegenerationRate);

	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		FSM->PushState("Walk");

	if (GetVelocity().Z < 0.0f)
		FSM->PushState("Fall");
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

	AnimInstance->bIsWalking = true;
}

void AYlva::UpdateWalkState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	RegenerateStamina(StaminaRegenerationRate);

	if (GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		FSM->PopState("Walk");

	if (GetVelocity().Z < 0.0f)
		FSM->PushState("Fall");
}

void AYlva::OnExitWalkState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsWalking = false;
}
#pragma endregion

#pragma region Run
void AYlva::OnEnterRunState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = MovementSettings.RunSpeed;
	AnimInstance->bIsRunning = true;
}

void AYlva::UpdateRunState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (!bGodMode)
		UpdateStamina(Combat.StaminaSettings.RunStamina * World->DeltaTimeSeconds);

	if (GetVelocity().IsZero() || 
		MovementComponent->MaxWalkSpeed < MovementSettings.RunSpeed || 
		Stamina <= Combat.StaminaSettings.RunStamina)
		FSM->PopState();

	if (GetVelocity().Z < 0.0f)
		FSM->PushState("Fall");
}

void AYlva::OnExitRunState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;
	AnimInstance->bIsRunning = false;
}
#pragma endregion

#pragma region Fall
void AYlva::OnEnterFallingState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsFalling = true;
}

void AYlva::UpdateFallingState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (GetVelocity().Z == 0.0f)
		FSM->PopState("Fall");
}

void AYlva::OnExitFallingState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsFalling = false;
}
#pragma endregion

#pragma region Shield Block
void AYlva::OnEnterBlockingState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsBlocking = true;
	bUseControllerRotationYaw = true;
}

void AYlva::UpdateBlockingState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	RegenerateStamina(StaminaRegenerationRate);

	if (GetVelocity().Z < 0.0f)
	{
		FSM->PopState("Block");
		FSM->PushState("Fall");
	}
}

void AYlva::OnExitBlockingState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsBlocking = false;
	bUseControllerRotationYaw = false;
}
#pragma endregion

#pragma region Light Attack 1
void AYlva::OnEnterLightAttackState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptLightAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(Combat.StaminaSettings.LightAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
}

void AYlva::UpdateLightAttackState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitLightAttackState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);

	AnimInstance->bAcceptLightAttack = false;
}
#pragma endregion

#pragma region Light Attack 2
void AYlva::OnEnterLightAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondLightAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(Combat.StaminaSettings.LightAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
}

void AYlva::UpdateLightAttack2State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitLightAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);

	AnimInstance->bAcceptSecondLightAttack = false;
}
#pragma endregion

#pragma region Heavy Attack 1
void AYlva::OnEnterHeavyAttackState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptHeavyAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(Combat.StaminaSettings.HeavyAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
}

void AYlva::UpdateHeavyAttackState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		FSM->PopState();
}

void AYlva::OnExitHeavyAttackState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);

	AnimInstance->bAcceptHeavyAttack = false;
}
#pragma endregion

#pragma region Heavy Attack 2
void AYlva::OnEnterHeavyAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondHeavyAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(Combat.StaminaSettings.HeavyAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
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

	MovementComponent->SetMovementMode(MOVE_None);

	bIsHit = true;
	AnimInstance->bIsHit = true;
}

void AYlva::UpdateDamagedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	RegenerateStamina(StaminaRegenerationRate);

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

	GameInstance->bIsPlayerDead = true;
	GameInstance->OnPlayerDeath.Broadcast();

	GetWorldTimerManager().SetTimer(DeathStateExpiryTimer, this, &AYlva::Respawn, RespawnDelay);
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
	AnimInstance->bIsShieldHit = true;
	bIsHit = true;

	DelayStaminaRegeneration();
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
	AnimInstance->bIsShieldHit = false;
	bIsHit = false;
}
#pragma endregion 

#pragma region Parry
void AYlva::OnEnterParryState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsShieldHit = true;

	StartParryEvent();
}

void AYlva::UpdateParryState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	RegenerateStamina(StaminaRegenerationRate);
}

void AYlva::OnExitParryState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsShieldHit = false;

	GameInstance->bParrySucceeded = false;

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
	if (Health > 0.0f && !bIsHit)
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

				// Update values
				UpdateHealth(DamageAmount * Combat.BlockSettings.DamageBuffer);
				UpdateStamina(Combat.StaminaSettings.ShieldHitStamina);
			break;

			default:
				// Enter damaged state
				FSM->PushState("Damaged");

				// Apply hit stop
				if (Combat.bEnableHitStop)
				{
					PauseAnims();
					GetWorldTimerManager().SetTimer(HitStopTimerHandle, this, &AYlva::UnPauseAnims, Combat.HitStopTime);
				}

				// Shake the camera
				PlayerController->ClientPlayCameraShake(CameraShakes.Damaged.Shake, CameraShakes.Damaged.Intensity);

				UpdateHealth(DamageAmount);
			break;
		}
	}

	if (Health <= 0.0f && FSM->GetActiveStateName() != "Death")
	{
		Die();
	}

	return DamageAmount;
}

void AYlva::OnBossDeath()
{
	DisableLockOn();
}

void AYlva::SetStamina(const float NewStaminaAmount)
{
	Stamina = FMath::Clamp(NewStaminaAmount, 0.0f, StartingStamina);
	GameInstance->PlayerStamina = Stamina;
}

void AYlva::SetHealth(const float NewHealthAmount)
{
	Health = FMath::Clamp(NewHealthAmount, 0.0f, StartingHealth);
	GameInstance->PlayerHealth = Health;
}

void AYlva::UpdateStamina(const float AmountToSubtract)
{
	Stamina = FMath::Clamp(Stamina - AmountToSubtract, 0.0f, StartingStamina);
	GameInstance->PlayerStamina = Stamina;
}

void AYlva::UpdateHealth(const float AmountToSubtract)
{
	Health = FMath::Clamp(Health - AmountToSubtract, 0.0f, StartingHealth);
	GameInstance->PlayerHealth = Health;
}

void AYlva::ResetStamina()
{
	Stamina = StartingStamina;
	GameInstance->PlayerStamina = Stamina;
}

void AYlva::ResetHealth()
{
	Health = StartingHealth;
	GameInstance->PlayerHealth = Health;
}

void AYlva::ResetGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
}

void AYlva::PauseAnims()
{
	GetMesh()->bPauseAnims = true;
	Boss->GetMesh()->bPauseAnims = true;
}

void AYlva::UnPauseAnims()
{
	GetMesh()->bPauseAnims = false;
	Boss->GetMesh()->bPauseAnims = false;
}

void AYlva::StartParryEvent()
{
	GameInstance->bParrySucceeded = true;

	UGameplayStatics::SetGlobalTimeDilation(this, Combat.ParrySettings.TimeDilationOnSuccessfulParry);

	GetWorldTimerManager().SetTimer(ParryEventExpiryTimer, this, &AYlva::FinishParryEvent, Combat.ParrySettings.TimeUntilParryEventIsCompleted);
}

void AYlva::FinishParryEvent()
{
	FSM->PopState();
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

void AYlva::DelayStaminaRegeneration()
{
	GetWorldTimerManager().SetTimer(StaminaRegenTimerHandle, Combat.StaminaSettings.StaminaRegenDelay, false);
}
