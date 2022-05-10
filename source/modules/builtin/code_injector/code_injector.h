#pragma once

#include "base/com/simple_com_class.h"
#include "modules/builtin/component_code_injector.h"

class CodeInjector :
    public SimpleComClassT<CodeInjector, ComRef, ICodeInjector>
{
public:
    CodeInjector();
    virtual ~CodeInjector();

    STDMETHODIMP InjectRawCode(DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode);
    STDMETHODIMP SafeInjectRawCode(DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode);

protected:
    HRESULT DoInjectRawCode(BOOL bCheckMachine, DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode);
};