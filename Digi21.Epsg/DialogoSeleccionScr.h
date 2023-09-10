#pragma once

class DialogoSeleccionScr
{
public:
	///	<summary>
	///		Muestra el cuadro de di�logo de selecci�n de sistema de referencia de coordenadas sin la posibilidad de seleccionar el sistema desconocido.
	///	</summary>
	static CStringW DialogSeleccionaScr(CStringW const& t�tuloDi�logo, std::shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto);

	///	<summary>
	///		Muestra el cuadro de di�logo de selecci�n de sistema de referencia de coordenadas con la posibilidad de seleccionar el sistema desconocido.
	///	</summary>
	static CStringW DialogSeleccionaScr(CStringW const& t�tuloDi�logo, CStringW const& t�tuloCheckboxDesconocido, std::shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto);
};

