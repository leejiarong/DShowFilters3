/*
 * (C) 2003-2006 Gabest
 * (C) 2006-2012 see Authors.txt
 *
 * This file is part of MPC-HC.
 *
 * MPC-HC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-HC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include "FilterApp.h"

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

const TCHAR V_KEYPATH[]		= _T("SoftWare\\OEM\\Acer\\Video2");
const TCHAR SUP_PLAYER[]	= _T("Player%d");
const TCHAR SUP_ALL[]		= _T("*");
const int MAX_SUP_PLAYERS	= 10;

CFilterApp::CFilterApp()
{
}

BOOL CFilterApp::InitInstance()
{
    if (!__super::InitInstance()) {
        return FALSE;
    }

    //SetRegistryKey(_T("Acer"));

	STARTUPINFO si;
	GetStartupInfo(&si);
	m_AppName = CString(si.lpTitle);
	m_AppName.Replace('\\', '/');
	m_AppName = m_AppName.Mid(m_AppName.ReverseFind('/')+1);
	m_AppName.MakeLower();

	//while list
	TCHAR KEYPATH[25];
	wsprintf(KEYPATH ,_T("%s"), V_KEYPATH);
	HKEY hKey;
	ULONG nError;
	nError = RegOpenKeyExW(HKEY_LOCAL_MACHINE, KEYPATH, 0, KEY_READ, &hKey);
	if(nError == ERROR_SUCCESS)
	{
		for(int i = 0; i < MAX_SUP_PLAYERS; i++)
		{
			TCHAR szBuffer[MAX_PATH];
			DWORD dwBufferSize = sizeof(szBuffer);

			CString csValueName, csValue;
			csValueName.Format(SUP_PLAYER, i);

			nError = RegQueryValueExW(hKey, csValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
			if(nError == ERROR_SUCCESS)
			{
				csValue = szBuffer;
				csValue.MakeLower();

				if(m_AppName == csValue ||
					csValue == SUP_ALL) //remove restrictions
				{
					m_bEnable = TRUE;
					break;
				}
			}
			else
				break;
		}
	}
	if(nError != ERROR_SUCCESS)
		TRACE(_T("CFilterApp::InitInstance open reg key error [%ld]\n"), nError);

	RegCloseKey(hKey);

    DllEntryPoint(AfxGetInstanceHandle(), DLL_PROCESS_ATTACH, 0);

    return TRUE;
}

BOOL CFilterApp::ExitInstance()
{
    DllEntryPoint(AfxGetInstanceHandle(), DLL_PROCESS_DETACH, 0);

    return __super::ExitInstance();
}

BEGIN_MESSAGE_MAP(CFilterApp, CWinApp)
END_MESSAGE_MAP()
