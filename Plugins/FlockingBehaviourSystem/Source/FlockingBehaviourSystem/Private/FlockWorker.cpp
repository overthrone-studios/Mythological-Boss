// Copyright Ali El Saleh, 2019

#include "FlockWorker.h"
#include "Boid.h"

FlockWorker::FlockWorker(TArray<ABoid*>* InBoids)
{
	Boids = InBoids;

	Thread = FRunnableThread::Create(this, TEXT("FlockWorker"), 0, TPri_Highest);
}

FlockWorker::~FlockWorker()
{
	delete Thread;
	Thread = nullptr;
}

bool FlockWorker::Init()
{
	return true;
}

uint32 FlockWorker::Run()
{
	FPlatformProcess::Sleep(0.01f);

	//FCriticalSection Mutex;

	while (StopTaskCounter.GetValue() == 0)
	{
		//Mutex.Lock();
		for (auto Boid : *Boids)
		{
			Boid->ApplyBehaviors(*Boids);
		}
		//Mutex.Unlock();
	}

	return 0;
}

void FlockWorker::Stop()
{
	StopTaskCounter.Increment();
}

void FlockWorker::Shutdown()
{
	EnsureCompletion();
	delete this;
}

void FlockWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

