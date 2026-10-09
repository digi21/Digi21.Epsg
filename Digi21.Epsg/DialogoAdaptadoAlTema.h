#pragma once

#include "TemaAnfitrion.h"

#include <memory>
#include <optional>
#include <vector>

// Aplica el tema de Codejock del anfitrión a una ventana creada desde una plantilla de diálogo. Sin tema no hace nada.
class AdaptacionTema
{
	std::optional<ColoresTema> _tema;
	CBrush _brochaFondo;
	CBrush _brochaLista;
	COLORREF _colorBrochaLista{};

	// Controles nativos de la plantilla subclasificados como controles de Codejock, que atienden WM_XTP_SETCONTROLTHEME.
	std::vector<std::unique_ptr<CWnd>> _controlesTema;

public:
	AdaptacionTema();

	// Se llama después de CDialog::OnInitDialog, cuando DDX_Control ya ha asociado sus controles.
	auto AplicaAVentana(CWnd& ventana) -> void;

	// false si no hay tema: el fondo lo borra la clase base.
	auto PintaFondo(CWnd& ventana, CDC& dc) const -> bool;

	// Devuelve brochaBase si no hay tema o si el control pinta sus propios colores.
	auto ColorControl(CDC& dc, CWnd* control, UINT tipoControl, HBRUSH brochaBase) -> HBRUSH;
};

// Cuadro de diálogo o página que sigue el tema de Codejock del anfitrión. Sin tema se comporta como TBase.
template <class TBase>
class DialogoAdaptadoAlTema : public TBase
{
	AdaptacionTema _adaptacion;

public:
	using TBase::TBase;

protected:
	auto OnInitDialog() -> BOOL override
	{
		auto const resultado = TBase::OnInitDialog();
		_adaptacion.AplicaAVentana(*this);
		return resultado;
	}

	// ReSharper disable once CppHidingFunction
	auto OnEraseBkgnd(CDC* pDC) -> BOOL
	{
		return _adaptacion.PintaFondo(*this, *pDC) ? TRUE : TBase::OnEraseBkgnd(pDC);
	}

	// ReSharper disable once CppHidingFunction
	auto OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) -> HBRUSH
	{
		return _adaptacion.ColorControl(*pDC, pWnd, nCtlColor, TBase::OnCtlColor(pDC, pWnd, nCtlColor));
	}

	DECLARE_MESSAGE_MAP()
};

BEGIN_TEMPLATE_MESSAGE_MAP(DialogoAdaptadoAlTema, TBase, TBase)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
