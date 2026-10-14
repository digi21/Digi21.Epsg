#pragma once

#ifdef _CREANDO_DIGI21_EPSG
#define _modoDigi21Epsg __declspec(dllexport)
#else
#define _modoDigi21Epsg __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "Digi21.EpsgD.lib")
#else
#pragma comment(lib, "Digi21.Epsg.lib")
#endif
#endif

#include <memory>
#include <optional>
#include <vector>

#include <CrsKit.h>

namespace Digi21::OpenGis::Epsg
{
	class _modoDigi21Epsg EpsgManager
	{
#pragma region Selección de Sistema de referencia de coordenadas
	public:
		static CStringW DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox);
		static CStringW DialogSelectCrs(CStringW const& title, std::shared_ptr<CrsKit::CoordinateSystems::CoordinateSystem> const& coordinateSystem);
		static CStringW DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, int defaultHorizontalCrs, int defaultVerticalCrs);
		static CStringW DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox,CStringW const& wkt);
		static CStringW DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, std::shared_ptr<CrsKit::CoordinateSystems::CoordinateSystem> const& coordinateSystem);
		static CStringW DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, std::shared_ptr<CrsKit::CoordinateSystems::CoordinateSystem> const& ventanaDibujo, std::shared_ptr<CrsKit::CoordinateSystems::CoordinateSystem> const& ventanaFotogrametrica);
#pragma endregion
#pragma region Búsqueda de Sistema de referencia de coordenadas
		static std::optional<int> DialogSearchHorizontalCrs(CStringW const& title, int defaultCrs);
#pragma endregion
#pragma region Selección de Sistema de referencia de coordenadas horizontal
	public:
		static CStringW DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox);
		static CStringW DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, int defaultCrs);
		static CStringW DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox,CStringW const& wkt);
		static CStringW DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, std::shared_ptr<CrsKit::CoordinateSystems::CoordinateSystem> const& coordinateSystem);
#pragma endregion

#pragma region Selección de Sistema de referencia de coordenadas vertical
	public:
		static CStringW DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox);
		static CStringW DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, int defaultCrs);
		static CStringW DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, CStringW const& wkt);
		static CStringW DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, std::shared_ptr<CrsKit::CoordinateSystems::CoordinateSystem> const& coordinateSystem);
#pragma endregion

#pragma region Visualización de Sistemas de coordenadas de referencia
	public:
		static void DialogShowCrs(std::shared_ptr<CrsKit::CoordinateSystems::CoordinateSystem> const& coordinateSystem);
		static void DialogShowCrs(std::shared_ptr<CrsKit::CoordinateSystems::HorizontalCoordinateSystem> const& coordinateSystem);
		static void DialogShowCrs(std::shared_ptr<CrsKit::CoordinateSystems::VerticalCoordinateSystem> const& coordinateSystem);
#pragma endregion

#pragma region Opciones de transformación
	public:
		// Opciones para CreateFromCoordinateSystems: selectOperation muestra el cuadro de selección de operación
		// y resolveTransform el de transformación vertical; un sistema local o desconocido se rechaza.
		static auto CreaOpcionesTransformacion(HWND hWndPadre = nullptr) -> CrsKit::CoordinateTransformationOptions;
#pragma endregion
	};

	class _modoDigi21Epsg SelectTransformationHelper
	{
		HWND hWnd;

	public:
		SelectTransformationHelper(HWND hwndParent);
		int DialogSelectTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs, std::vector<CrsKit::CoordinateTransformations::CoordinateOperation> const& locatedOperations) const;

		static int DialogSelectTransformation(CStringW const& sourceCrs, CStringW const& destinationCrs, std::vector<CrsKit::CoordinateTransformations::CoordinateOperation> const& locatedOperations);
		static int DialogSelectTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs, std::vector<CrsKit::CoordinateTransformations::CoordinateOperation> const& locatedOperations, HWND hWnd);
	};

	class _modoDigi21Epsg CreateVerticalTransformationHelper
	{
		HWND hWnd;

	public:
		CreateVerticalTransformationHelper(HWND hwndParent);
		std::shared_ptr<CrsKit::CoordinateTransformations::IMathTransform> DialogCreateVerticalTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs) const;

		static std::shared_ptr<CrsKit::CoordinateTransformations::IMathTransform> DialogCreateVerticalTransformation(CStringW const& sourceCrs, CStringW const& destinationCrs);
		static std::shared_ptr<CrsKit::CoordinateTransformations::IMathTransform> DialogCreateVerticalTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs, HWND hWnd);
	};
			 
}
