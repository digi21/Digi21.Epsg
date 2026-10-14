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

	auto EsCasillaOOpcion(CXTPButton const& boton) -> bool
	{
		auto const estilo = boton.GetStyle();
		if (estilo & BS_PUSHLIKE)
			return false;

		switch (estilo & BS_TYPEMASK)
		{
		case BS_CHECKBOX:
		case BS_AUTOCHECKBOX:
		case BS_3STATE:
		case BS_AUTO3STATE:
		case BS_RADIOBUTTON:
		case BS_AUTORADIOBUTTON:
			return true;
		default:
			return false;
		}
	}

	auto EsOscuro(COLORREF color) -> bool
	{
		return (GetRValue(color) * 299 + GetGValue(color) * 587 + GetBValue(color) * 114) / 1000 < 128;
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

	for (auto hijo = ::GetWindow(ventana.GetSafeHwnd(), GW_CHILD); hijo; hijo = ::GetWindow(hijo, GW_HWNDNEXT))
	{
		auto* const control = CWnd::FromHandlePermanent(hijo);

		// El tema pinta el texto de un Edit de solo lectura casi del color de su fondo: se usa el de un Edit editable.
		auto* const edit = dynamic_cast<CXTPEdit*>(control);
		if (auto* const tema = edit ? edit->GetTheme() : nullptr; tema && (edit->GetStyle() & ES_READONLY))
			tema->m_clrTextReadOnly.SetCustomValue(static_cast<COLORREF>(tema->m_clrText));

		// El texto de un marco de grupo, una casilla o un botón de opción va sobre el fondo de la ventana; el tema lo
		// pinta con el color del texto de un botón, que en el tema azul es oscuro sobre azul.
		auto* const boton = dynamic_cast<CXTPButton*>(control);
		auto* const temaBoton = boton ? boton->GetTheme() : nullptr;
		if (temaBoton && BS_GROUPBOX == (boton->GetStyle() & BS_TYPEMASK))
			temaBoton->m_crText.SetCustomValue(_tema->texto);
		else if (temaBoton && EsCasillaOOpcion(*boton))
			temaBoton->m_crTextCheck.SetCustomValue(_tema->texto);
	}

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
	if (!_tema)
		return brochaBase;

	// Las filas de una lista que no es owner-draw se pintan con los colores de WM_CTLCOLORLISTBOX, y CXTPListBox rellena
	// el resto con el fondo de su tema: las filas usan ese mismo fondo.
	if (CTLCOLOR_LISTBOX == tipoControl)
	{
		auto* const lista = dynamic_cast<CXTPListBox*>(control);
		if (!lista || (lista->GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)))
			return brochaBase;

		auto const fondo = lista->GetBackColor();
		if (!_brochaLista.GetSafeHandle() || _colorBrochaLista != fondo)
		{
			_brochaLista.DeleteObject();
			_brochaLista.CreateSolidBrush(fondo);
			_colorBrochaLista = fondo;
		}
		dc.SetTextColor(EsOscuro(fondo) ? _tema->texto : GetSysColor(COLOR_WINDOWTEXT));
		dc.SetBkColor(fondo);
		return _brochaLista;
	}

	if (CTLCOLOR_DLG != tipoControl && CTLCOLOR_STATIC != tipoControl && CTLCOLOR_BTN != tipoControl)
		return brochaBase;

	// Un Edit deshabilitado o de solo lectura también pide CTLCOLOR_STATIC; sus colores los pone CXTPEdit.
	if (control && 0 == ClaseVentana(control->GetSafeHwnd()).CompareNoCase(WC_EDITW))
		return brochaBase;

	dc.SetTextColor(_tema->texto);
	dc.SetBkColor(_tema->fondo);
	return _brochaFondo;
}
