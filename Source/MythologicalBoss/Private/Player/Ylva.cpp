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
	PlayerStateMachine = CreateDefaultSubobject<UFSM>(FName("Player FSM"));
	PlayerStateMachine->AddState(0, "Idle");
	PlayerStateMachine->AddState(1, "Walk");
	PlayerStateMachine->AddState(2, "Run");
	PlayerStateMachine->AddState(3, "Light Attack 1");
	PlayerStateMachine->AddState(4, "Block");
	PlayerStateMachine->AddState(5, "Death");
	PlayerStateMachine->AddState(7, "Fall");
	PlayerStateMachine->AddState(8, "Light Attack 2");
	PlayerStateMachine->AddState(9, "Heavy Attack 1");
	PlayerStateMachine->AddState(10, "Heavy Attack 2");
	PlayerStateMachine->AddState(20, "Damaged");
	PlayerStateMachine->AddState(21, "Shield Hit");
	PlayerStateMachine->AddState(22, "Parry");

	// Bind state events to our functions
	PlayerStateMachine->GetState(0)->OnEnterState.AddDynamic(this, &AYlva::OnEnterIdleState);
	PlayerStateMachine->GetState(0)->OnUpdateState.AddDynamic(this, &AYlva::UpdateIdleState);
	PlayerStateMachine->GetState(0)->OnExitState.AddDynamic(this, &AYlva::OnExitIdleState);

	PlayerStateMachine->GetState(1)->OnEnterState.AddDynamic(this, &AYlva::OnEnterWalkState);
	PlayerStateMachine->GetState(1)->OnUpdateState.AddDynamic(this, &AYlva::UpdateWalkState);
	PlayerStateMachine->GetState(1)->OnExitState.AddDynamic(this, &AYlva::OnExitWalkState);

	PlayerStateMachine->GetState(2)->OnEnterState.AddDynamic(this, &AYlva::OnEnterRunState);
	PlayerStateMachine->GetState(2)->OnUpdateState.AddDynamic(this, &AYlva::UpdateRunState);
	PlayerStateMachine->GetState(2)->OnExitState.AddDynamic(this, &AYlva::OnExitRunState);

	PlayerStateMachine->GetState(3)->OnEnterState.AddDynamic(this, &AYlva::OnEnterLightAttackState);
	PlayerStateMachine->GetState(3)->OnUpdateState.AddDynamic(this, &AYlva::UpdateLightAttackState);
	PlayerStateMachine->GetState(3)->OnExitState.AddDynamic(this, &AYlva::OnExitLightAttackState);

	PlayerStateMachine->GetState(4)->OnEnterState.AddDynamic(this, &AYlva::OnEnterBlockingState);
	PlayerStateMachine->GetState(4)->OnUpdateState.AddDynamic(this, &AYlva::UpdateBlockingState);
	PlayerStateMachine->GetState(4)->OnExitState.AddDynamic(this, &AYlva::OnExitBlockingState);

	PlayerStateMachine->GetState(5)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDeathState);
	PlayerStateMachine->GetState(5)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDeathState);
	PlayerStateMachine->GetState(5)->OnExitState.AddDynamic(this, &AYlva::OnExitDeathState);

	PlayerStateMachine->GetState(7)->OnEnterState.AddDynamic(this, &AYlva::OnEnterFallingState);
	PlayerStateMachine->GetState(7)->OnUpdateState.AddDynamic(this, &AYlva::UpdateFallingState);
	PlayerStateMachine->GetState(7)->OnExitState.AddDynamic(this, &AYlva::OnExitFallingState);

	PlayerStateMachine->GetState(8)->OnEnterState.AddDynamic(this, &AYlva::OnEnterLightAttack2State);
	PlayerStateMachine->GetState(8)->OnUpdateState.AddDynamic(this, &AYlva::UpdateLightAttack2State);
	PlayerStateMachine->GetState(8)->OnExitState.AddDynamic(this, &AYlva::OnExitLightAttack2State);

	PlayerStateMachine->GetState(9)->OnEnterState.AddDynamic(this, &AYlva::OnEnterHeavyAttackState);
	PlayerStateMachine->GetState(9)->OnUpdateState.AddDynamic(this, &AYlva::UpdateHeavyAttackState);
	PlayerStateMachine->GetState(9)->OnExitState.AddDynamic(this, &AYlva::OnExitHeavyAttackState);

	PlayerStateMachine->GetState(10)->OnEnterState.AddDynamic(this, &AYlva::OnEnterHeavyAttack2State);
	PlayerStateMachine->GetState(10)->OnUpdateState.AddDynamic(this, &AYlva::UpdateHeavyAttack2State);
	PlayerStateMachine->GetState(10)->OnExitState.AddDynamic(this, &AYlva::OnExitHeavyAttack2State);

	PlayerStateMachine->GetState(20)->OnEnterState.AddDynamic(this, &AYlva::OnEnterDamagedState);
	PlayerStateMachine->GetState(20)->OnUpdateState.AddDynamic(this, &AYlva::UpdateDamagedState);
	PlayerStateMachine->GetState(20)->OnExitState.AddDynamic(this, &AYlva::OnExitDamagedState);

	PlayerStateMachine->GetState(21)->OnEnterState.AddDynamic(this, &AYlva::OnEnterShieldHitState);
	PlayerStateMachine->GetState(21)->OnUpdateState.AddDynamic(this, &AYlva::UpdateShieldHitState);
	PlayerStateMachine->GetState(21)->OnExitState.AddDynamic(this, &AYlva::OnExitShieldHitState);

	PlayerStateMachine->GetState(22)->OnEnterState.AddDynamic(this, &AYlva::OnEnterParryState);
	PlayerStateMachine->GetState(22)->OnUpdateState.AddDynamic(this, &AYlva::UpdateParryState);
	PlayerStateMachine->GetState(22)->OnExitState.AddDynamic(this, &AYlva::OnExitParryState);

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
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Configure character settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void AYlva::BeginPlay()
{
	Super::BeginPlay();

	Health = StartingHealth;
	Stamina = StartingStamina;

	// Cache the world object
	World = GetWorld();

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

	PlayerStateMachine->InitState(0);
}

void AYlva::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameInstance->PlayerLocation = GetActorLocation();

	if (bShouldLockOnTarget)
	{
		const FRotator Target = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), GameInstance->BossLocation);
		const FRotator SmoothedRotation = FMath::RInterpTo(GetControlRotation(), Target, DeltaTime, 10.0f);

		const FRotator NewRotation = FRotator(LockOnPitch, SmoothedRotation.Yaw, GetControlRotation().Roll);

		GetController()->SetControlRotation(NewRotation);

		GameInstance->SetLockOnLocation(GameInstance->BossLocation);
		GameInstance->SetLockOnRotation(NewRotation - FRotator(0.0f, 180.0f, 0.0f));
	}

	RegenerateStamina(StaminaRegenerationRate);
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
	PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &AYlva::ShowFSMVisualizer);
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
	if (GameInstance->BossHealth <= 0.0f)
		return;

	bShouldLockOnTarget = !bShouldLockOnTarget;
	PlayerController->SetIgnoreLookInput(bShouldLockOnTarget);
	GameInstance->ToggleLockOnVisibility(bShouldLockOnTarget);
}

void AYlva::EnableLockOn()
{
	// Don't lock on if boss is dead
	if (GameInstance->BossHealth <= 0.0f)
		return;

	bShouldLockOnTarget = true;
	PlayerController->SetIgnoreLookInput(true);
	GameInstance->ToggleLockOnVisibility(true);
}

void AYlva::DisableLockOn()
{
	bShouldLockOnTarget = false;
	PlayerController->SetIgnoreLookInput(false);
	GameInstance->ToggleLockOnVisibility(false);
}

void AYlva::Block()
{
	if (PlayerStateMachine->GetActiveStateID() != 7 /*Fall*/ &&
		PlayerStateMachine->GetActiveStateID() != 5 /*Death*/ &&
		PlayerStateMachine->GetActiveStateID() != 20 /*Damaged*/
		)
	{
		PlayerStateMachine->PopState();
		PlayerStateMachine->PushState("Block");
	}
}

void AYlva::StopBlocking()
{
	PlayerStateMachine->PopState("Block");
}

void AYlva::LightAttack()
{
	// Are we in any of these states?
	if (PlayerStateMachine->GetActiveStateID() == 7 /*Fall*/ ||
		PlayerStateMachine->GetActiveStateID() == 5 /*Death*/ ||
		PlayerStateMachine->GetActiveStateID() == 20 /*Damaged*/)
		return;

	if (PlayerStateMachine->GetActiveStateID() == 22 /*Parry*/)
		PlayerStateMachine->PopState();

	if (
		PlayerStateMachine->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		PlayerStateMachine->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		Stamina > LightAttackStamina)
	{
		PlayerStateMachine->PushState("Light Attack 1");
		bUseControllerRotationYaw = true;
	}
	else if (
		PlayerStateMachine->GetActiveStateID() == 3 /*Light Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		PlayerStateMachine->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		PlayerStateMachine->GetActiveStateUptime() > 0.4f &&
		Stamina > LightAttackStamina)
	{
		PlayerStateMachine->PopState("Light Attack 1");
		PlayerStateMachine->PushState("Light Attack 2");
		bUseControllerRotationYaw = true;
	}
	else if (
		PlayerStateMachine->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() == 8 /*Light Attack 2*/ &&
		PlayerStateMachine->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		PlayerStateMachine->GetActiveStateUptime() > 0.6f &&
		Stamina > LightAttackStamina)
	{
		PlayerStateMachine->PopState("Light Attack 2");
		PlayerStateMachine->PushState("Light Attack 1");
		bUseControllerRotationYaw = true;
	}
}

void AYlva::HeavyAttack()
{
	// Are we in any of these states?
	if (PlayerStateMachine->GetActiveStateID() == 7 /*Fall*/ ||
		PlayerStateMachine->GetActiveStateID() == 5 /*Death*/ ||
		PlayerStateMachine->GetActiveStateID() == 20 /*Damaged*/)
		return;

	if (PlayerStateMachine->GetActiveStateID() == 22 /*Parry*/)
		PlayerStateMachine->PopState();

	if (
		PlayerStateMachine->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		PlayerStateMachine->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 10 /*Heavy Attack 2*/ &&
		Stamina > HeavyAttackStamina)
	{
		PlayerStateMachine->PushState("Heavy Attack 1");
		bUseControllerRotationYaw = true;
	}
	else if (
		PlayerStateMachine->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		PlayerStateMachine->GetActiveStateID() == 9 /*Heavy Attack 1*/ &&
		PlayerStateMachine->GetActiveStateUptime() > 0.7f &&
		Stamina > HeavyAttackStamina)
	{
		PlayerStateMachine->PopState("Heavy Attack 1");
		PlayerStateMachine->PushState("Heavy Attack 2");
		bUseControllerRotationYaw = true;
	}
	else if (
		PlayerStateMachine->GetActiveStateID() != 3 /*Light Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() != 8 /*Light Attack 2*/ &&
		PlayerStateMachine->GetActiveStateID() != 9 /*Heavy Attack 1*/ &&
		PlayerStateMachine->GetActiveStateID() == 10 /*Heavy Attack 2*/ &&
		PlayerStateMachine->GetActiveStateUptime() > 0.5f &&
		Stamina > HeavyAttackStamina)
	{
		PlayerStateMachine->PopState("Heavy Attack 2");
		PlayerStateMachine->PushState("Heavy Attack 1");
		bUseControllerRotationYaw = true;
	}
}

void AYlva::DisableControllerRotationYaw()
{
	bUseControllerRotationYaw = false;
}

void AYlva::Run()
{
	// If we are moving and grounded
	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround() && Stamina > RunStamina * World->DeltaTimeSeconds)
	{
		MovementComponent->MaxWalkSpeed = RunSpeed;

		PlayerStateMachine->PopState();
		PlayerStateMachine->PushState("Run");
	}
}

void AYlva::StopRunning()
{
	MovementComponent->MaxWalkSpeed = WalkSpeed;

	PlayerStateMachine->PopState("Run");
}

void AYlva::Dash()
{
	if (PlayerStateMachine->GetActiveStateName() == "Death")
		return;

	// If we are moving and grounded
	if (GetVelocity().Size() > 0.0f && MovementComponent->IsMovingOnGround())
	{
		// Do the dash
		FVector VelocityNormalized = GetVelocity();
		VelocityNormalized.Normalize();
		VelocityNormalized.Z = 0;

		if (!bGodMode && Stamina > DashStamina)
		{
			UpdateStamina(DashStamina);
		}

		LaunchCharacter(VelocityNormalized * DashForce, true, true);
	}
}

void AYlva::ShowFSMVisualizer()
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
}

void AYlva::ShowNoHUD()
{
	OverthroneHUD->GetMasterHUD()->SwitchToHUDIndex(3);
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
	if (bGodMode)
		return;

	if (Stamina < StartingStamina)
	{
		Stamina += Rate * World->DeltaTimeSeconds;
		GameInstance->PlayerStamina = Stamina;
	}
}

void AYlva::Respawn()
{
	GetWorldTimerManager().ClearTimer(DeathStateExpiryTimer);

	PlayerStateMachine->PopState();

	GameInstance->bIsPlayerDead = false;

	UGameplayStatics::OpenLevel(this, *UGameplayStatics::GetCurrentLevelName(this));
}

#pragma region Idle
void AYlva::OnEnterIdleState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());
}

void AYlva::UpdateIdleState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		PlayerStateMachine->PushState("Walk");

	if (GetVelocity().Z < 0.0f)
		PlayerStateMachine->PushState("Fall");
}

void AYlva::OnExitIdleState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());
}
#pragma endregion

#pragma region Walk
void AYlva::OnEnterWalkState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsWalking = true;
}

void AYlva::UpdateWalkState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	if (GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		PlayerStateMachine->PopState("Walk");

	if (GetVelocity().Z < 0.0f)
		PlayerStateMachine->PushState("Fall");
}

void AYlva::OnExitWalkState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsWalking = false;
}
#pragma endregion

#pragma region Run
void AYlva::OnEnterRunState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = RunSpeed;
	AnimInstance->bIsRunning = true;
}

void AYlva::UpdateRunState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	if (!bGodMode)
		UpdateStamina(RunStamina * World->DeltaTimeSeconds);

	if (GetVelocity().IsZero() || MovementComponent->MaxWalkSpeed < RunSpeed || Stamina <= RunStamina * World->DeltaTimeSeconds)
		PlayerStateMachine->PopState();

	if (GetVelocity().Z < 0.0f)
		PlayerStateMachine->PushState("Fall");
}

void AYlva::OnExitRunState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = WalkSpeed;
	AnimInstance->bIsRunning = false;
}
#pragma endregion

#pragma region Fall
void AYlva::OnEnterFallingState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsFalling = true;
}

void AYlva::UpdateFallingState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	if (GetVelocity().Z == 0.0f)
		PlayerStateMachine->PopState("Fall");
}

void AYlva::OnExitFallingState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsFalling = false;
}
#pragma endregion

#pragma region Shield Block
void AYlva::OnEnterBlockingState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsBlocking = true;
	bUseControllerRotationYaw = true;
}

void AYlva::UpdateBlockingState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	if (GetVelocity().Z < 0.0f)
	{
		PlayerStateMachine->PopState("Block");
		PlayerStateMachine->PushState("Fall");
	}
}

void AYlva::OnExitBlockingState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsBlocking = false;
	bUseControllerRotationYaw = false;
}
#pragma endregion

#pragma region Light Attack 1
void AYlva::OnEnterLightAttackState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptLightAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(LightAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
}

void AYlva::UpdateLightAttackState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		PlayerStateMachine->PopState();
}

void AYlva::OnExitLightAttackState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptLightAttack = false;
}
#pragma endregion

#pragma region Light Attack 2
void AYlva::OnEnterLightAttack2State()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondLightAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(LightAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
}

void AYlva::UpdateLightAttack2State()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		PlayerStateMachine->PopState();
}

void AYlva::OnExitLightAttack2State()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondLightAttack = false;
}
#pragma endregion

#pragma region Heavy Attack 1
void AYlva::OnEnterHeavyAttackState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptHeavyAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(HeavyAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
}

void AYlva::UpdateHeavyAttackState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		PlayerStateMachine->PopState();
}

void AYlva::OnExitHeavyAttackState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptHeavyAttack = false;
}
#pragma endregion

#pragma region Heavy Attack 2
void AYlva::OnEnterHeavyAttack2State()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondHeavyAttack = true;

	if (Stamina > 0)
	{
		if (!bGodMode)
			UpdateStamina(HeavyAttackStamina);
	}
	else
	{
		SetStamina(0);
	}
}

void AYlva::UpdateHeavyAttack2State()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		PlayerStateMachine->PopState();
}

void AYlva::OnExitHeavyAttack2State()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondHeavyAttack = false;
}
#pragma endregion 

#pragma region Damaged
void AYlva::OnEnterDamagedState()
{
	AnimInstance->bIsHit = true;
}

void AYlva::UpdateDamagedState()
{
	// If hit animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		PlayerStateMachine->PopState();
}

void AYlva::OnExitDamagedState()
{
	AnimInstance->bIsHit = false;
}
#pragma endregion 

#pragma region Death
void AYlva::OnEnterDeathState()
{
	AnimInstance->bIsDead = true;

	MovementComponent->DisableMovement();

	GameInstance->bIsPlayerDead = true;
	GameInstance->OnPlayerDeath.Broadcast();

	GetWorldTimerManager().SetTimer(DeathStateExpiryTimer, this, &AYlva::Respawn, 1.8f);
}

void AYlva::UpdateDeathState()
{

}

void AYlva::OnExitDeathState()
{
	AnimInstance->bIsDead = false;
}
#pragma endregion 

#pragma region Shield Hit
void AYlva::OnEnterShieldHitState()
{
	AnimInstance->bIsShieldHit = true;
}

void AYlva::UpdateShieldHitState()
{
	// If shield impact animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		PlayerStateMachine->PopState();
}

void AYlva::OnExitShieldHitState()
{
	AnimInstance->bIsShieldHit = false;
}
#pragma endregion 

#pragma region Parry
void AYlva::OnEnterParryState()
{
	FSMVisualizer->HighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsShieldHit = true;

	StartParryEvent();
}

void AYlva::UpdateParryState()
{
	FSMVisualizer->UpdateStateUptime(PlayerStateMachine->GetActiveStateName().ToString(), PlayerStateMachine->GetActiveStateUptime());

}

void AYlva::OnExitParryState()
{
	FSMVisualizer->UnhighlightState(PlayerStateMachine->GetActiveStateName().ToString());

	AnimInstance->bIsShieldHit = false;

	GameInstance->bParrySucceeded = false;

	ResetGlobalTimeDilation();

	GetWorldTimerManager().ClearTimer(ParryEventExpiryTimer);
}
#pragma endregion 

float AYlva::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bGodMode)
		return DamageAmount;

	if (Health > 0.0f && !AnimInstance->bIsHit)
	{
		if (PlayerStateMachine->GetActiveStateName() != "Idle" &&
			PlayerStateMachine->GetActiveStateName() != "Block")
		{
			PlayerStateMachine->PopState();
		}

		// Did we successfully parry?
		const bool bParrySucceeded = PlayerStateMachine->GetActiveStateName() == "Block" && PlayerStateMachine->GetActiveStateUptime() < ParryWindowTime;
		if (bParrySucceeded)
		{
			PlayerStateMachine->PushState("Parry");
		}
		// Not succeeded and still blocking?
		else if (PlayerStateMachine->GetActiveStateName() == "Block")
		{
			PlayerStateMachine->PopState();
			PlayerStateMachine->PushState("Shield Hit");

			UpdateHealth(DamageAmount * DamageBuffer);
			UpdateStamina(ShieldHitStamina);
		}
		// Not blocking
		else
		{
			PlayerStateMachine->PushState("Damaged");

			UpdateHealth(DamageAmount);
		}
	}

	if (Health <= 0.0f && !AnimInstance->bIsHit && PlayerStateMachine->GetActiveStateName() != "Death")
	{
		SetHealth(0.0f);

		PlayerStateMachine->RemoveAllStatesFromStack();
		PlayerStateMachine->PushState("Death");
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
	//Stamina -= AmountToSubtract;
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

void AYlva::StartParryEvent()
{
	GameInstance->bParrySucceeded = true;

	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilationOnSuccessfulParry);

	GetWorldTimerManager().SetTimer(ParryEventExpiryTimer, this, &AYlva::FinishParryEvent, TimeUntilParryEventIsCompleted);
}

void AYlva::FinishParryEvent()
{
	PlayerStateMachine->PopState();
}

void AYlva::EnableGodMode()
{
	bGodMode = true;

	ResetHealth();
	ResetStamina();

	ULog::DebugMessage(INFO, FString("God mode: On"), true);
}

void AYlva::DisableGodMode()
{
	bGodMode = false;

	ULog::DebugMessage(INFO, FString("God mode: Off"), true);
}

void AYlva::ToggleGodMode()
{
	bGodMode = !bGodMode;

	FString Status;

	if (bGodMode)
	{
		Status = "On";

		ResetHealth();
		ResetStamina();
	}
	else
		Status = "Off";

	ULog::DebugMessage(INFO, FString("God mode: ") + Status, true);
}
