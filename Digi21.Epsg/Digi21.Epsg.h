#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CDigi21EpsgApp final : public CWinApp
{
public:
	CDigi21EpsgApp() = default;

// Overrides
public:
	BOOL InitInstance() override;
	int ExitInstance() override;
	auto DoMessageBox(LPCTSTR texto, UINT tipo, UINT idAyuda) -> int override;

	DECLARE_MESSAGE_MAP()
};
