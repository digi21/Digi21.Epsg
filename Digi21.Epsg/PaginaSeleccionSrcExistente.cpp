#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaSeleccionSrcExistente.h"

IMPLEMENT_DYNAMIC(PaginaSeleccionSrcExistente, PaginaAdaptadaAlTema)

PaginaSeleccionSrcExistente::PaginaSeleccionSrcExistente(CXTPPropertyPage*& current, int idTemplate, CStringW const& nombreSistema)
	: PaginaAdaptadaAlTema(idTemplate)
	, _current{ current }
	, _nombreSistema{ nombreSistema }
{
}

void PaginaSeleccionSrcExistente::DoDataExchange(CDataExchange* pDX)
{
	PaginaAdaptadaAlTema::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TITULO_SRC_EXISTENTE, _nombreSistema);
}

BEGIN_MESSAGE_MAP(PaginaSeleccionSrcExistente, PaginaAdaptadaAlTema)
END_MESSAGE_MAP()

BOOL PaginaSeleccionSrcExistente::OnSetActive()
{
	_current = this;
	return TRUE;
}
