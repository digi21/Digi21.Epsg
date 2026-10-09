#include "pch.h"
#include "PaginaAdaptadaAlTema.h"

namespace
{
	auto ClaseVentana(HWND ventana) -> CStringW
	{
		wchar_t clase[64]{};
		::GetClassNameW(ventana, clase, static_cast<int>(std::size(clase)));
		return CStringW{clase};
	}

	auto CreaControlCodejock(CStringW const& clase) -> std::unique_ptr<CWnd>
	{
		if (0 == clase.CompareNoCase(WC_BUTTONW))
			return std::make_unique<CXTPButton>();
		if (0 == clase.CompareNoCase(WC_COMBOBOXW))
			return std::make_unique<CXTPComboBox>();
		if (0 == clase.CompareNoCase(WC_EDITW))
			return std::make_unique<CXTPEdit>();
		if (0 == clase.CompareNoCase(WC_LISTBOXW))
			return std::make_unique<CXTPListBox>();
		return {};
	}
}

IMPLEMENT_DYNAMIC(PaginaAdaptadaAlTema, CXTPPropertyPage)

PaginaAdaptadaAlTema::PaginaAdaptadaAlTema(UINT idPlantilla)
	: CXTPPropertyPage{idPlantilla}
	, _tema{LeeColoresTemaAnfitrion()}
{
	if (_tema)
		_brochaFondo.CreateSolidBrush(_tema->fondo);
}

BEGIN_MESSAGE_MAP(PaginaAdaptadaAlTema, CXTPPropertyPage)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

auto PaginaAdaptadaAlTema::OnInitDialog() -> BOOL
{
	auto const resultado = CXTPPropertyPage::OnInitDialog();

	if (_tema)
	{
		SubclasificaControlesNativos();
		SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, xtpControlThemeVisualStudio2015);
	}

	return resultado;
}

// Los controles que la página ya asocia con DDX_Control se dejan como están.
auto PaginaAdaptadaAlTema::SubclasificaControlesNativos() -> void
{
	for (auto hijo = ::GetWindow(GetSafeHwnd(), GW_CHILD); hijo; hijo = ::GetWindow(hijo, GW_HWNDNEXT))
	{
		if (CWnd::FromHandlePermanent(hijo))
			continue;

		auto control = CreaControlCodejock(ClaseVentana(hijo));
		if (!control || !control->SubclassWindow(hijo))
			continue;

		_controlesTema.push_back(std::move(control));
	}
}

auto PaginaAdaptadaAlTema::OnEraseBkgnd(CDC* pDC) -> BOOL
{
	if (!_tema)
		return CXTPPropertyPage::OnEraseBkgnd(pDC);

	CRect cliente{};
	GetClientRect(&cliente);
	pDC->FillRect(&cliente, &_brochaFondo);
	return TRUE;
}

auto PaginaAdaptadaAlTema::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) -> HBRUSH
{
	auto const brocha = CXTPPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if (!_tema || (CTLCOLOR_DLG != nCtlColor && CTLCOLOR_STATIC != nCtlColor && CTLCOLOR_BTN != nCtlColor))
		return brocha;

	// Un Edit deshabilitado o de solo lectura también pide CTLCOLOR_STATIC; sus colores los pone CXTPEdit.
	if (pWnd && 0 == ClaseVentana(pWnd->GetSafeHwnd()).CompareNoCase(WC_EDITW))
		return brocha;

	pDC->SetTextColor(_tema->texto);
	pDC->SetBkColor(_tema->fondo);
	return _brochaFondo;
}
