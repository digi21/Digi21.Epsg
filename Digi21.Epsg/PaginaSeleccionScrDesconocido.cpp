#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaSeleccionScrDesconocido.h"

IMPLEMENT_DYNAMIC(CPaginaSeleccionScrDesconocido, CXTPPropertyPage)

CPaginaSeleccionScrDesconocido::CPaginaSeleccionScrDesconocido(CXTPPropertyPage*& _current, CStringW const& título)
	: CXTPPropertyPage(IDD)
	, current{ _current }
	, m_sTituloDesconocido{ título }
{
}

void CPaginaSeleccionScrDesconocido::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TITULO_DESCONOCIDO, m_sTituloDesconocido);
}

BEGIN_MESSAGE_MAP(CPaginaSeleccionScrDesconocido, CXTPPropertyPage)
END_MESSAGE_MAP()

BOOL CPaginaSeleccionScrDesconocido::OnSetActive()
{
	current = this;
	return TRUE;
}
