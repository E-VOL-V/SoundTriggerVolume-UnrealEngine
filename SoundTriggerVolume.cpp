// (C)EVOLV //
 
 
 #include "Interactables/SoundTriggerActor.h"
 #include "GameFramework/SpringArmComponent.h"
 #include "GameFramework/Actor.h"


 // INITIALIZE ACTOR //
 ASoundTriggerActor::ASoundTriggerActor()
 {
  	// Can Tick?
 	PrimaryActorTick.bCanEverTick = false;
 	
 	// INTERACTION SYSTEM //
 	CollisionBoxScale = FVector(100.f, 100.f, 100.f);
 	
 	// SOUND SYSTEM //
 	bPlayedSound = false;
 	bCanBeReplayed = false;
 
 	// Play sound in the actor location
 	bPlayInPlace = true;
 	
 	// Play sound away from the actors location
 	bPlayAway = false;
 	
 	// Initialize the default root
 	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
 	SetRootComponent(DefaultSceneRoot);
 	
 	// Initialize the box that handles collisions
 	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("CollisionMesh"));
 	CollisionMesh->SetupAttachment(DefaultSceneRoot);
 	CollisionMesh->SetBoxExtent(FVector(74.0f, 125.0f, 100.0f));
 	CollisionMesh->SetRelativeLocation(FVector(0.0f,0.0f, 0.0f));
 	
 	// Initialize the in place audio component
 	AC_InPlace = CreateDefaultSubobject<UAudioComponent>(FName("AC_InPlace"));
 	AC_InPlace->SetupAttachment(DefaultSceneRoot);
 	AC_InPlace->SetRelativeLocation(FVector(0.0f,0.0f, 180.0f));

 	// Initialize spring arm
 	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
 	SpringArm->SetupAttachment(AC_InPlace);
 	SpringArm->TargetArmLength = 300.f;
 	
 	// Initialize the away audio component
 	AC_Away = CreateDefaultSubobject<UAudioComponent>(FName("AC_Away"));
 	AC_Away->SetupAttachment(SpringArm);
 
 }
 
 // WHEN THE GAME STARTS //
 void ASoundTriggerActor::BeginPlay()
 {
 	Super::BeginPlay();
 	//Set up the collision system
 	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this,&ASoundTriggerActor::OverlapBegin);

 	//Set in place sound effect
 	AC_InPlace->SetSound(SoundEffect);
 	AC_InPlace->Stop();
 	
 	//Set away sound effect
 	AC_Away->SetSound(SoundEffect);
 	AC_Away->Stop();
 }
 
 // WHEN PLAYER INTERACTS WITH THE ACTOR //
 void ASoundTriggerActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
 	// Trigger Sound Events
 	if(bCanBeReplayed)
 	{
 		if(bPlayInPlace && !AC_InPlace->IsPlaying())
 		{
 			AC_InPlace->Play(0.0f);
 		}
 		else if (bPlayAway && !AC_Away->IsPlaying())
 		{
 			AC_Away->Play(0.0f);
 		}
 	}
 	else if(!bPlayedSound)
 	{
 		if(bPlayInPlace && !AC_InPlace->IsPlaying())
 		{
 			AC_InPlace->Play(0.0f);
 			bPlayedSound = true;
 		}
 		else if (bPlayAway && !AC_Away->IsPlaying())
 		{
 			AC_Away->Play(0.0f);
 			bPlayedSound = true;
 		}
 	}
 }
