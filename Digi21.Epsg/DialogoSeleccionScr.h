#pragma once

class DialogoSeleccionScr
{
public:
	///	<summary>
	///		Muestra el cuadro de diálogo de selección de sistema de referencia de coordenadas sin la posibilidad de seleccionar el sistema desconocido.
	///	</summary>
	static CStringW DialogSeleccionaScr(CStringW const& títuloDiálogo, std::shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto);

	///	<summary>
	///		Muestra el cuadro de diálogo de selección de sistema de referencia de coordenadas con la posibilidad de seleccionar el sistema desconocido.
	///	</summary>
	static CStringW DialogSeleccionaScr(CStringW const& títuloDiálogo, CStringW const& títuloCheckboxDesconocido, std::shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto);
};

