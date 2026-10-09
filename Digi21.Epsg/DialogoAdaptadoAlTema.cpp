#include "pch.h"
#include "DialogoAdaptadoAlTema.h"

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

AdaptacionTema::AdaptacionTema()
	: _tema{LeeColoresTemaAnfitrion()}
{
	if (_tema)
		_brochaFondo.CreateSolidBrush(_tema->fondo);
}

auto AdaptacionTema::AplicaAVentana(CWnd& ventana) -> void
{
	if (!_tema)
		return;

	// Los controles que la ventana ya asocia con DDX_Control se dejan como están.
	for (auto hijo = ::GetWindow(ventana.GetSafeHwnd(), GW_CHILD); hijo; hijo = ::GetWindow(hijo, GW_HWNDNEXT))
	{
		if (CWnd::FromHandlePermanent(hijo))
			continue;

		auto control = CreaControlCodejock(ClaseVentana(hijo));
		if (!control || !control->SubclassWindow(hijo))
			continue;

		_controlesTema.push_back(std::move(control));
	}

	ventana.SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, xtpControlThemeVisualStudio2015);

	// Las páginas de una hoja son ventanas hijas: la barra de título es la de la hoja.
	if (!(ventana.GetStyle() & WS_CHILD))
		AplicaBarraTituloTema(ventana.GetSafeHwnd(), *_tema);
}

auto AdaptacionTema::PintaFondo(CWnd& ventana, CDC& dc) const -> bool
{
	if (!_tema)
		return false;

	CRect cliente{};
	ventana.GetClientRect(&cliente);
	dc.FillSolidRect(&cliente, _tema->fondo);
	return true;
}

auto AdaptacionTema::ColorControl(CDC& dc, CWnd* control, UINT tipoControl, HBRUSH brochaBase) -> HBRUSH
{
	if (!_tema || (CTLCOLOR_DLG != tipoControl && CTLCOLOR_STATIC != tipoControl && CTLCOLOR_BTN != tipoControl))
		return brochaBase;

	// Un Edit deshabilitado o de solo lectura también pide CTLCOLOR_STATIC; sus colores los pone CXTPEdit.
	if (control && 0 == ClaseVentana(control->GetSafeHwnd()).CompareNoCase(WC_EDITW))
		return brochaBase;

	dc.SetTextColor(_tema->texto);
	dc.SetBkColor(_tema->fondo);
	return _brochaFondo;
}
