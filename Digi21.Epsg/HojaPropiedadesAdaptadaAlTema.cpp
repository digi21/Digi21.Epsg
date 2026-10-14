#include "pch.h"
#include "Resource.h"
#include "HojaPropiedadesAdaptadaAlTema.h"

IMPLEMENT_DYNAMIC(HojaPropiedadesAdaptadaAlTema, CXTPPropertySheet)

HojaPropiedadesAdaptadaAlTema::HojaPropiedadesAdaptadaAlTema(LPCWSTR titulo)
	: CXTPPropertySheet{titulo}
	, _tema{LeeColoresTemaAnfitrion()}
{
	// Ninguna página llama a SetModified: el botón Aplicar estaría siempre deshabilitado.
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

BEGIN_MESSAGE_MAP(HojaPropiedadesAdaptadaAlTema, CXTPPropertySheet)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CXTPPropertySheet carga los textos de los botones de los recursos de Codejock, que solo están en inglés.
auto HojaPropiedadesAdaptadaAlTema::CreateButtons() -> BOOL
{
	if (!CXTPPropertySheet::CreateButtons())
		return FALSE;

	SetDlgItemTextW(IDOK, _G(IDS_STRING2033));
	SetDlgItemTextW(IDCANCEL, _G(IDS_STRING2034));
	return TRUE;
}

auto HojaPropiedadesAdaptadaAlTema::OnCreate(LPCREATESTRUCT lpCreateStruct) -> int
{
	if (-1 == CXTPPropertySheet::OnCreate(lpCreateStruct))
		return -1;

	if (!_tema)
		return 0;

	// El tema se asigna después de crear la hoja: si el navegador ya lo tiene en CXTPPropertySheet::OnCreate, la hoja
	// crea barras de comandos y Codejock dibuja el marco de la ventana.
	m_brBack.CreateSolidBrush(_tema->fondo);
	GetNavigator()->SetTheme(xtpControlThemeVisualStudio2015);
	SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, xtpControlThemeVisualStudio2015);
	AplicaBarraTituloTema(GetSafeHwnd(), *_tema);

	return 0;
}
