#pragma once

// Hoja de propiedades con los botones Aceptar y Cancelar en el idioma de la interfaz y sin el botón Aplicar.
class HojaPropiedadesAdaptadaAlTema final : public CXTPPropertySheet
{
	DECLARE_DYNAMIC(HojaPropiedadesAdaptadaAlTema)

public:
	explicit HojaPropiedadesAdaptadaAlTema(LPCWSTR titulo);

protected:
	auto CreateButtons() -> BOOL override;
};
