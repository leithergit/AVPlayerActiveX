// AVPlayer.cpp : Implementation of CAVPlayerApp and DLL registration.

#include "stdafx.h"
#include "AVPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CAVPlayerApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xACE87B96, 0xB12B, 0x467F, { 0x87, 0x3, 0x44, 0xCD, 0xEF, 0xBA, 0x54, 0xDF } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CAVPlayerApp::InitInstance - DLL initialization

BOOL CAVPlayerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CAVPlayerApp::ExitInstance - DLL termination

int CAVPlayerApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
