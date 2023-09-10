#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaSeleccionSrcExistente.h"

IMPLEMENT_DYNAMIC(PaginaSeleccionSrcExistente, CXTPPropertyPage)

PaginaSeleccionSrcExistente::PaginaSeleccionSrcExistente(CXTPPropertyPage*& _current, CStringW const& título)
	: CXTPPropertyPage(IDD)
	, current{ _current }
	, _titulo{ título }
{
}

void PaginaSeleccionSrcExistente::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TITULO_SRC_EXISTENTE, _titulo);
}

BEGIN_MESSAGE_MAP(PaginaSeleccionSrcExistente, CXTPPropertyPage)
END_MESSAGE_MAP()

BOOL PaginaSeleccionSrcExistente::OnSetActive()
{
	current = this;
	return TRUE;
}
