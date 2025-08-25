// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestModule.h"
#include "Modules/ModuleManager.h"

class FTestModuleImpl :public FDefaultModuleImpl
{
	virtual void StartupModule() override
	{
		int a = 10;
	}
};

IMPLEMENT_MODULE(FTestModuleImpl, TestModule);
