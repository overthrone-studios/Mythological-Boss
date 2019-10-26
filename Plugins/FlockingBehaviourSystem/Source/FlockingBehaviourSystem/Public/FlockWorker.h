// Copyright Ali El Saleh, 2019

#pragma once

#include "Core/Public/HAL/Runnable.h"
#include "Core/Public/HAL/ThreadSingleton.h"
#include "Core/Public/HAL/RunnableThread.h"

/**
 * Multi-threading for flocking
 */
class FLOCKINGBEHAVIOURSYSTEM_API FlockWorker : public FRunnable
{
public:
	// Thread Core Functions
	explicit FlockWorker(TArray<class ABoid*>* InBoids);
	virtual ~FlockWorker();

	// Starting and stopping the thread
	void Shutdown();

protected:
	// FRunnable interface functions
	bool Init() override;
	uint32 Run() override;
	void Stop() override;

private:
	void EnsureCompletion();

	// Thread to run the worker on
	FRunnableThread* Thread;

	// The Data Ptr
	TArray<class ABoid*>* Boids;

	// Stop this thread? Uses Thread Safe Counter
	FThreadSafeCounter StopTaskCounter;
};
