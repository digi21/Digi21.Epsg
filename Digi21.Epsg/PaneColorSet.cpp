#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaneColorSet.h"

extern CDigi21EpsgApp THE_APP;

CPaneColorSet::CPaneColorSet()
{
	RefreshMetrics();
}

void CPaneColorSet::RefreshMetrics()
{
	switch (THE_APP.m_theme)
	{
	case themeVS2015Light:
	case themeVS2015Dark:
	case themeVS2015Blue:
		{
			m_clrText       = XTPIniColor(L"DockingPane.Inactive", L"TextColor",       GetSysColor(COLOR_WINDOWTEXT));
			m_clrBack       = XTPIniColor(L"DockingPane.Inactive", L"CaptionColor",    GetSysColor(COLOR_WINDOW));
			m_clrBorderLite = XTPIniColor(L"DockingPane.Tabs",     L"ButtonSelected",  GetSysColor(COLOR_WINDOW));
			m_clrBorderDark = XTPIniColor(L"DockingPane.Tabs",     L"ButtonBorder",    GetSysColor(COLOR_3DSHADOW));
		}
		break;
	default: ;
	}

	m_brBack.DeleteObject();
	m_brBack.CreateSolidBrush(m_clrBack);
}

void CPaneColorSet::DrawBorders(CDC* pDC, CRect rWindow, CXTPDockingPane* pPane /*=NULL*/)
{
	COLORREF pclrDark[4] = { m_clrBorderDark, m_clrBorderDark, m_clrBorderDark, m_clrBorderDark }; // outside: left, top, right, bottom
	COLORREF pclrLite[4] = { m_clrBorderLite, m_clrBorderLite, m_clrBorderLite, m_clrBorderLite }; // inside:  left, top, right, bottom
	
	if (pPane && (THE_APP.m_theme >= themeVS2015Light))
	{
		// set top border color to inside color.
		pclrDark[1] = pclrLite[1];
		
		// if floating or a side panel, set left, right and bottom border color to inside color.
		if (pPane->IsFloating() || pPane->IsSidePanel())
		{
			pclrDark[0] = pclrLite[0];
			pclrDark[2] = pclrLite[2];
			pclrDark[3] = pclrLite[3];
		}
		
		// if tabs are visible, set bottom border to inside color.
		else if (pPane->IsTabsVisible())
		{
			pclrDark[3] = pclrLite[3];
		}
	}
	
	CXTPDrawHelpers::DrawBorder(pDC, rWindow, pclrDark, pclrLite);
}
