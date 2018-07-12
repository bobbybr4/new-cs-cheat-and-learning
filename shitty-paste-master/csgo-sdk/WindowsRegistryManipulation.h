#pragma once
#include <Windows.h>

// Creates a key specified by pszSubKey - you can't create
// keys directly under HKEY_LOCAL_MACHINE in Windows NT or 2000
// just for an extra bit of info.
bool CreateRegistryKey(HKEY hKeyRoot, LPCTSTR pszSubKey)
{
	HKEY hKey;
	DWORD dwFunc;
	LONG  lRet;

	//------------------------------------------------------------------------------

	SECURITY_DESCRIPTOR SD;
	SECURITY_ATTRIBUTES SA;

	if (!InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION))
		return false;

	if (!SetSecurityDescriptorDacl(&SD, true, 0, false))
		return false;

	SA.nLength = sizeof(SA);
	SA.lpSecurityDescriptor = &SD;
	SA.bInheritHandle = false;

	//------------------------------------------------------------------------------

	lRet = RegCreateKeyEx(
		hKeyRoot,
		pszSubKey,
		0,
		(LPTSTR)NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		&SA,
		&hKey,
		&dwFunc
	);

	if (lRet == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		hKey = (HKEY)NULL;
		return true;
	}

	SetLastError((DWORD)lRet);
	return false;
}

// Writes a DWORD value to the registry
bool Set_DWORDRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, unsigned long ulValue)
{
	HKEY hKey;
	LONG lRes;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	lRes = RegSetValueEx(hKey, pszValue, 0, REG_DWORD, (unsigned char*)&ulValue, sizeof(ulValue));

	RegCloseKey(hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	return true;
}

// Fetch a little endian DWORD from the registry
//(see platform SDK "Registry Value Types")
bool Get_DWORDRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, unsigned long &ulBuff)
{
	HKEY hKey;
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(ulBuff);
	LONG  lRes;

	ulBuff = 0;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_READ, &hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	lRes = RegQueryValueEx(hKey, pszValue, 0, &dwType, (LPBYTE)&ulBuff, &dwSize);

	RegCloseKey(hKey);

	if (dwType != REG_DWORD)
		//throw ERROR_WRONG_TYPE;

		if (lRes != ERROR_SUCCESS)
		{
			SetLastError(lRes);
			return false;
		}

	return true;
}