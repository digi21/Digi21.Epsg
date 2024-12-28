#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaSeleccionSrcExistente.h"

IMPLEMENT_DYNAMIC(PaginaSeleccionSrcExistente, CXTPPropertyPage)

PaginaSeleccionSrcExistente::PaginaSeleccionSrcExistente(CXTPPropertyPage*& current, int idTemplate, CStringW const& nombreSistema)
	: CXTPPropertyPage(idTemplate)
	, _current{ current }
	, _nombreSistema{ nombreSistema }
{
}

void PaginaSeleccionSrcExistente::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TITULO_SRC_EXISTENTE, _nombreSistema);
}

BEGIN_MESSAGE_MAP(PaginaSeleccionSrcExistente, CXTPPropertyPage)
END_MESSAGE_MAP()

BOOL PaginaSeleccionSrcExistente::OnSetActive()
{
	_current = this;
	return TRUE;
}
