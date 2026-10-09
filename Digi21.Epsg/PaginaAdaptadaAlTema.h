#pragma once

#include "TemaAnfitrion.h"

#include <memory>
#include <optional>
#include <vector>

// Página de propiedades que sigue el tema de Codejock del anfitrión. Sin tema se comporta como CXTPPropertyPage.
class PaginaAdaptadaAlTema : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(PaginaAdaptadaAlTema)

	std::optional<ColoresTema> _tema;
	CBrush _brochaFondo;

	// Controles nativos de la plantilla subclasificados como controles de Codejock, que atienden WM_XTP_SETCONTROLTHEME.
	std::vector<std::unique_ptr<CWnd>> _controlesTema;

public:
	explicit PaginaAdaptadaAlTema(UINT idPlantilla);

protected:
	auto OnInitDialog() -> BOOL override;
	// ReSharper disable once CppHidingFunction
	auto OnEraseBkgnd(CDC* pDC) -> BOOL;
	// ReSharper disable once CppHidingFunction
	auto OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) -> HBRUSH;

private:
	auto SubclasificaControlesNativos() -> void;

	DECLARE_MESSAGE_MAP()
};
