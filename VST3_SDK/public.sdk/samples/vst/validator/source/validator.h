//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Validator
// Filename    : validator.h
// Created by  : Steinberg, 04/2005
// Description : VST 3 Plug-in Validator class
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2017, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/ipluginbase.h"
#include "pluginterfaces/test/itest.h"
#include "public.sdk/source/vst/testsuite/vsttestsuite.h"
#include "public.sdk/source/vst/hosting/hostclasses.h"
#include "public.sdk/source/vst/hosting/module.h"

namespace Steinberg {
namespace Vst {

class IComponent;
class IEditController;

class VstModule;
class VstTestBase;
class TestSuite;

//------------------------------------------------------------------------
// Some Definitions
//------------------------------------------------------------------------
#define SEPARATOR \
	"-------------------------------------------------------------\n"

#define VALIDATOR_INFO \
	SEPARATOR \
	kVstVersionString " Plug-in Validator\n" \
	"Program by Steinberg (Built on " __DATE__ ")\n" \
	SEPARATOR "\n"

#define VALIDATOR_USAGE \
	"Usage: vstvalidator [options] <component path> \n" \
	"  where options are -l (use a local instance for each test instead of a global one)\n"

//------------------------------------------------------------------------
/** Main Class of Validator.
\ingroup Validator */
//------------------------------------------------------------------------
class Validator : public FObject, public ITestResult, public IHostApplication
{
public:
//------------------------------------------------------------------------
	Validator (int argc, char* argv[]);
	~Validator ();

	int run ();

//------------------------------------------------------------------------
	OBJ_METHODS (Validator, FObject)
	REFCOUNT_METHODS (FObject)
	DEFINE_INTERFACES
		DEF_INTERFACE (ITestResult)
		DEF_INTERFACE (IHostApplication)
	END_DEFINE_INTERFACES (FObject)
//------------------------------------------------------------------------
protected:
	// ITestResult
	void PLUGIN_API addErrorMessage (const tchar* msg) override;
	void PLUGIN_API addMessage (const tchar* msg) override;

	// IHostApplication
	tresult PLUGIN_API getName (String128 name) override;
	tresult PLUGIN_API createInstance (TUID cid, TUID iid, void** obj) override;

	bool filterClassCategory (FIDString category, FIDString classCategory) const;
	void createTests (IPlugProvider* plugProvider, const ConstString& plugName);
	void addTest (ITestSuite* testSuite, VstTestBase* test);
	void runTestSuite (TestSuite* suite, FIDString nameFilter = 0);

	int argc;
	char** argv;
	
	using Module = VST3::Hosting::Module;

	Module::Ptr module;
	Module::Ptr testModule;
	OPtr<TestSuite> testSuite;
	int32 numTestsFailed;
	int32 numTestsPassed;
};

//------------------------------------------------------------------------
/** This handles the Plug-in factory in its library.
\ingroup Validator */
//------------------------------------------------------------------------
class VstModule : public FObject
{
public:
//------------------------------------------------------------------------
	VstModule (const char* path);
	~VstModule ();

	bool isValid () const;
	IPluginFactory* getFactory () { return factory; }

//------------------------------------------------------------------------
	OBJ_METHODS (VstModule, FObject)
//------------------------------------------------------------------------
protected:
	void* libHandle;
	IPluginFactory* factory;
};
}
} // namespaces
