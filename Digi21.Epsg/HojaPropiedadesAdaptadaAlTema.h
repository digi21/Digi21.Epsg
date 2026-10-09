#pragma once

#include "TemaAnfitrion.h"

#include <optional>

// Hoja de propiedades que sigue el tema de Codejock del anfitrión, con los botones Aceptar y Cancelar en el idioma de la
// interfaz y sin el botón Aplicar. Sin tema conserva los colores del sistema.
class HojaPropiedadesAdaptadaAlTema final : public CXTPPropertySheet
{
	DECLARE_DYNAMIC(HojaPropiedadesAdaptadaAlTema)

	std::optional<ColoresTema> _tema;

public:
	explicit HojaPropiedadesAdaptadaAlTema(LPCWSTR titulo);

protected:
	auto CreateButtons() -> BOOL override;
	// ReSharper disable once CppHidingFunction
	auto OnCreate(LPCREATESTRUCT lpCreateStruct) -> int;

	DECLARE_MESSAGE_MAP()
};
