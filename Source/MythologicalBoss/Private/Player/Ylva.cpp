// Copyright Overthrone Studios 2019

#include "Player/Ylva.h"
#include "ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "FSM.h"
#include "Log.h"

AYlva::AYlva()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Player/Bot/Animations/AnimBP_bot.AnimBP_Bot_C'"));

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
			ULog::LogDebugMessage(ERROR, FString("AnimBP did not succeed."));
	}

	// Create a FSM
	PlayerStateMachine = CreateDefaultSubobject<UFSM>(FName("Player FSM"));
	PlayerStateMachine->AddState(0, "Idle");
	PlayerStateMachine->AddState(1, "Walk");
	PlayerStateMachine->AddState(2, "Run");
	PlayerStateMachine->AddState(3, "Attack");
	PlayerStateMachine->AddState(4, "Block");
	PlayerStateMachine->AddState(5, "Death");
	PlayerStateMachine->AddState(6, "Jump");
	PlayerStateMachine->AddState(7, "Falling");

	PlayerStateMachine->InitState(0);

	PlayerStateMachine->GetState(0)->OnEnterState.AddDynamic(this, &AYlva::OnEnterIdleState);
	PlayerStateMachine->GetState(0)->OnUpdateState.AddDynamic(this, &AYlva::UpdateIdleState);
	PlayerStateMachine->GetState(0)->OnExitState.AddDynamic(this, &AYlva::OnExitIdleState);

	PlayerStateMachine->GetState(4)->OnEnterState.AddDynamic(this, &AYlva::OnEnterBlockingState);
	PlayerStateMachine->GetState(4)->OnUpdateState.AddDynamic(this, &AYlva::UpdateBlockingState);
	PlayerStateMachine->GetState(4)->OnExitState.AddDynamic(this, &AYlva::OnExitBlockingState);

	PlayerStateMachine->GetState(1)->OnEnterState.AddDynamic(this, &AYlva::OnEnterWalkState);
	PlayerStateMachine->GetState(1)->OnUpdateState.AddDynamic(this, &AYlva::UpdateWalkState);
	PlayerStateMachine->GetState(1)->OnExitState.AddDynamic(this, &AYlva::OnExitWalkState);

	PlayerStateMachine->GetState(6)->OnEnterState.AddDynamic(this, &AYlva::OnEnterJumpState);
	PlayerStateMachine->GetState(6)->OnUpdateState.AddDynamic(this, &AYlva::UpdateJumpState);
	PlayerStateMachine->GetState(6)->OnExitState.AddDynamic(this, &AYlva::OnExitJumpState);

	PlayerStateMachine->GetState(7)->OnEnterState.AddDynamic(this, &AYlva::OnEnterFallingState);
	PlayerStateMachine->GetState(7)->OnUpdateState.AddDynamic(this, &AYlva::UpdateFallingState);
	PlayerStateMachine->GetState(7)->OnExitState.AddDynamic(this, &AYlva::OnExitFallingState);

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
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	// Configure character settings
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void AYlva::BeginPlay()
{
	Super::BeginPlay();

	// Cache the world object
	World = GetWorld();

	// Cache the movement component
	MovementComponent = GetCharacterMovement();
}

void AYlva::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AYlva::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Crash if we don't have a valid Input component
	check(PlayerInputComponent);
	
	// Set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AYlva::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AYlva::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Turn" handles devices that provide an absolute delta, such as a mouse.
	// "TurnRate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AYlva::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AYlva::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AYlva::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AYlva::StopJumping);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AYlva::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AYlva::StopBlocking);
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

void AYlva::Block()
{	
	if (PlayerStateMachine->GetActiveStateID() != 7 /*Falling*/ ||
		PlayerStateMachine->GetActiveStateID() != 6 /*Juming*/)
		PlayerStateMachine->SetActiveState("Block");
}

void AYlva::StopBlocking()
{
	if (PlayerStateMachine->GetActiveStateID() == 4 /*Blocking*/)
		PlayerStateMachine->SetActiveState("Idle");
}

void AYlva::OnJumped_Implementation()
{
	PlayerStateMachine->SetActiveState("Jump");
}

void AYlva::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	PlayerStateMachine->SetActiveState("Idle");
}

void AYlva::OnEnterIdleState()
{
	ULog::LogDebugMessage(INFO, FString("Entered Idle state"), true);
}

void AYlva::UpdateIdleState()
{
	ULog::LogDebugMessage(INFO, FString("In Idle state: ") + FString::SanitizeFloat(PlayerStateMachine->GetActiveStateUptime()), true);

	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		PlayerStateMachine->SetActiveState("Walk");

	if (GetVelocity().Z < 0.0f)
		PlayerStateMachine->SetActiveState("Falling");
}

void AYlva::OnExitIdleState()
{
	ULog::LogDebugMessage(INFO, FString("Exited Idle state"), true);
}

void AYlva::OnEnterWalkState()
{
	ULog::LogDebugMessage(INFO, FString("Entered Walk state"), true);
}

void AYlva::UpdateWalkState()
{
	ULog::LogDebugMessage(INFO, FString("In Walk state: ") + FString::SanitizeFloat(PlayerStateMachine->GetActiveStateUptime()), true);
	
	if (GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		PlayerStateMachine->SetActiveState("Idle");

	if (GetVelocity().Z < 0.0f)
		PlayerStateMachine->SetActiveState("Falling");
}

void AYlva::OnExitWalkState()
{
	ULog::LogDebugMessage(INFO, FString("Exited Walk state"), true);
}

void AYlva::OnEnterBlockingState()
{
	ULog::LogDebugMessage(INFO, FString("Entering blocking state"), true);
}

void AYlva::UpdateBlockingState()
{
	ULog::LogDebugMessage(INFO, FString("In blocking state: ") + FString::SanitizeFloat(PlayerStateMachine->GetActiveStateUptime()), true);
}

void AYlva::OnExitBlockingState()
{
	ULog::LogDebugMessage(INFO, FString("Exited blocking state"), true);
}

void AYlva::OnEnterJumpState()
{
	ULog::LogDebugMessage(INFO, FString("Entering jump state"), true);
}

void AYlva::UpdateJumpState()
{
	ULog::LogDebugMessage(INFO, FString("In jump state: ") + FString::SanitizeFloat(PlayerStateMachine->GetActiveStateUptime()), true);

	if (GetVelocity().Z < 0.0f)
		PlayerStateMachine->SetActiveState("Falling");
}

void AYlva::OnExitJumpState()
{
	ULog::LogDebugMessage(INFO, FString("Exiting jump state"), true);
}

void AYlva::OnEnterFallingState()
{
	ULog::LogDebugMessage(INFO, FString("Entered falling state"), true);
}

void AYlva::UpdateFallingState()
{
	ULog::LogDebugMessage(INFO, FString("In falling state: ") + FString::SanitizeFloat(PlayerStateMachine->GetActiveStateUptime()), true);

	if (GetVelocity().Z == 0.0f)
		PlayerStateMachine->SetActiveState("Idle");
}

void AYlva::OnExitFallingState()
{
	ULog::LogDebugMessage(INFO, FString("Exiting falling state"), true);
}
