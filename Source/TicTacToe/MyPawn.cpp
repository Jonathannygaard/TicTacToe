// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <UObject/ConstructorHelpers.h>
#include "Containers/Array.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spheres.Init(NULL, 9);
	for (int i{}; i < 9; i++)
	{
		Spheres[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(FString::FromInt(i)));
	}

	RootComponent = Spheres[4];
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	for (int i{}; i < 9; i++)
	{
		if(i != 4)
		{
			Spheres[i]->SetupAttachment(GetRootComponent());
		}
		Spheres[i]->SetStaticMesh(MeshComponentAsset.Object);
	}

	Red = CreateDefaultSubobject<UMaterial>(TEXT("Red"));
	Blue = CreateDefaultSubobject<UMaterial>(TEXT("Blue"));
	White = CreateDefaultSubobject<UMaterial>(TEXT("White"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 500.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Spheres[0]->SetRelativeLocation(FVector(200.f, -200.f, 0.f));
	Spheres[1]->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	Spheres[2]->SetRelativeLocation(FVector(200.f, 200.f, 0.f));
	Spheres[3]->SetRelativeLocation(FVector(0.f, -200.f, 0.f));
	Spheres[4]->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Spheres[5]->SetRelativeLocation(FVector(0.f, 200.f, 0.f));
	Spheres[6]->SetRelativeLocation(FVector(-200.f, -200.f, 0.f));
	Spheres[7]->SetRelativeLocation(FVector(-200.f, 0.f, 0.f));
	Spheres[8]->SetRelativeLocation(FVector(-200.f, 200.f, 0.f));

	TurnCounter = 0;
	SphereStatus.Init(NULL, 9);
	SphereColor.Init(NULL, 9);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	for (int i{}; i < 9; i++)
	{
		Spheres[i]->SetupAttachment(GetRootComponent());
		Spheres[i]->SetMaterial(0, White);
	}

}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WinController();
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("1", IE_Pressed, this, &AMyPawn::OnePressed);
	PlayerInputComponent->BindAction("2", IE_Pressed, this, &AMyPawn::TwoPressed);
	PlayerInputComponent->BindAction("3", IE_Pressed, this, &AMyPawn::ThreePressed);
	PlayerInputComponent->BindAction("4", IE_Pressed, this, &AMyPawn::FourPressed);
	PlayerInputComponent->BindAction("5", IE_Pressed, this, &AMyPawn::FivePressed);
	PlayerInputComponent->BindAction("6", IE_Pressed, this, &AMyPawn::SixPressed);
	PlayerInputComponent->BindAction("7", IE_Pressed, this, &AMyPawn::SevenPressed);
	PlayerInputComponent->BindAction("8", IE_Pressed, this, &AMyPawn::EightPressed);
	PlayerInputComponent->BindAction("9", IE_Pressed, this, &AMyPawn::NinePressed);
	PlayerInputComponent->BindAction("r", IE_Pressed, this, &AMyPawn::rPressed);

}

void AMyPawn::OnePressed()
{
	TurnController(0);
}

void AMyPawn::TwoPressed()
{
	TurnController(1);
}

void AMyPawn::ThreePressed()
{
	TurnController(2);
}

void AMyPawn::FourPressed()
{
	TurnController(3);
}

void AMyPawn::FivePressed()
{
	TurnController(4);
}

void AMyPawn::SixPressed()
{
	TurnController(5);
}

void AMyPawn::SevenPressed()
{
	TurnController(6);
}

void AMyPawn::EightPressed()
{
	TurnController(7);
}

void AMyPawn::NinePressed()
{
	TurnController(8);
}

void AMyPawn::rPressed()
{
	for (int i{ 0 }; i < 9; i++)
	{
		Spheres[i]->SetMaterial(0, White);
		SphereColor[i] = 0;
		SphereStatus[i] = false;
	}
	GameOver = false;
	TurnCounter = 0;
}

void AMyPawn::TurnController(int SphereIndex)
{
	if (GameOver != true)
	{

		if (SphereStatus[SphereIndex] == true)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("That index is already taken"));
			return;
		}
		if (TurnCounter % 2 == 0)
		{
			Spheres[SphereIndex]->SetMaterial(0, Blue);
			SphereColor[SphereIndex] = 1;
		}
		else if (TurnCounter % 2 == 1)
		{
			Spheres[SphereIndex]->SetMaterial(0, Red);
			SphereColor[SphereIndex] = 2;
		}

		SphereStatus[SphereIndex] = true;
		TurnCounter++;
	}

}

void AMyPawn::WinController()
{

	if (SphereColor[0] == SphereColor[3] && SphereColor[3] == SphereColor[6])
	{
		if (Spheres[0]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[0]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	if (SphereColor[0] == SphereColor[1] && SphereColor[1] == SphereColor[2])
	{
		if (Spheres[0]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[0]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	if (SphereColor[0] == SphereColor[4] && SphereColor[4] == SphereColor[8])
	{
		if (Spheres[0]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[0]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	if (SphereColor[1] == SphereColor[4] && SphereColor[4] == SphereColor[7])
	{
		if (Spheres[1]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[1]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	if (SphereColor[2] == SphereColor[5] && SphereColor[5] == SphereColor[8])
	{
		if (Spheres[2]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[2]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	if (SphereColor[2] == SphereColor[4] && SphereColor[4] == SphereColor[6])
	{
		if (Spheres[2]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[2]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	if (SphereColor[3] == SphereColor[4] && SphereColor[4] == SphereColor[5])
	{
		if (Spheres[3]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[3]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	if (SphereColor[6] == SphereColor[7] && SphereColor[7] == SphereColor[8])
	{
		if (Spheres[7]->GetMaterial(0) == Blue)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 1 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
		if (Spheres[7]->GetMaterial(0) == Red)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Player 2 won"));
			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
			GameOver = true;
		}
	}
	else if (TurnCounter == 9 && GameOver == false)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Draw"));
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, TEXT("Press r to restart the game"));
	}

}

