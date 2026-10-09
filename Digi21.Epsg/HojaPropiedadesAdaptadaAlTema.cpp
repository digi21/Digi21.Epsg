#include "pch.h"
#include "Resource.h"
#include "HojaPropiedadesAdaptadaAlTema.h"

IMPLEMENT_DYNAMIC(HojaPropiedadesAdaptadaAlTema, CXTPPropertySheet)

HojaPropiedadesAdaptadaAlTema::HojaPropiedadesAdaptadaAlTema(LPCWSTR titulo)
	: CXTPPropertySheet{titulo}
{
	// Ninguna página llama a SetModified: el botón Aplicar estaría siempre deshabilitado.
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

// CXTPPropertySheet carga los textos de los botones de los recursos de Codejock, que solo están en inglés.
auto HojaPropiedadesAdaptadaAlTema::CreateButtons() -> BOOL
{
	if (!CXTPPropertySheet::CreateButtons())
		return FALSE;

	SetDlgItemTextW(IDOK, _G(IDS_STRING2033));
	SetDlgItemTextW(IDCANCEL, _G(IDS_STRING2034));
	return TRUE;
}
