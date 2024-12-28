#include "pch.h"
#include "Resource.h"
#include "DialogoSeleccionScr.h"
#include "PaginaSeleccionScrDesconocido.h"
#include "PaginaSeleccionScr3D.h"
#include "PaginaSeleccionScrGeograficoVertical.h"
#include "PaginaSeleccionScrProyectadoYVertical.h"
#include "PaginaSeleccionScrMemorizados.h"
#include "PaginaManual.h"
#include "PaginaSeleccionSrcExistente.h"

using namespace Digi21::Geodesia::EPSG;
using namespace Digi21::Utilidades;
using namespace std;

CStringW DialogoSeleccionScr::DialogSeleccionaScr(CStringW const& títuloDiálogo, shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto)
{
	CXTPPropertySheet ps(títuloDiálogo);
	auto* pList = new CXTPPropertyPageListNavigator{};
	pList->SetListStyle(xtpListBoxVisualStudio2015);
	ps.SetNavigator(pList);

	CXTPPropertyPage* activo = nullptr;

	CPaginaSeleccionScr3D página3D(activo);
	CPaginaSeleccionScrProyectadoYVertical páginaProyYVert(activo);
	CPaginaSeleccionScrGeograficoYVertical páginaGeoYVert(activo);
	CPaginaSeleccionScrMemorizados páginaMemorizados(activo);
	PaginaManual páginaManual(activo);
	PaginaSeleccionSrcExistente páginaExistente(activo, IDD_PAGINA_VENTANA_DIBUJO, sistemaCoordenadasDefecto->GetName());

	ps.AddPage(&página3D);
	ps.AddPage(&páginaProyYVert);
	ps.AddPage(&páginaGeoYVert);
	ps.AddPage(&páginaManual);
	ps.AddPage(&páginaMemorizados);
	ps.AddPage(&páginaExistente);
	ps.SetResizable();
	ps.SetActivePage(5);

	INT_PTR resultado;
	{
        AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
		resultado = ps.DoModal();
	}

	if( IDOK == resultado ) {
		if( activo == &página3D )
			return página3D.Wkt;

		if( activo == & páginaProyYVert)
			return páginaProyYVert.Wkt;

		if( activo == & páginaGeoYVert)
			return páginaGeoYVert.Wkt;

		if (activo == &páginaManual)
			return páginaManual.Wkt;

		if( activo == &páginaMemorizados )
			return páginaMemorizados.Wkt;
	}

	return sistemaCoordenadasDefecto->GetWkt();
}

CStringW DialogoSeleccionScr::DialogSeleccionaScr(CStringW const& títuloDiálogo, CStringW const& títuloCheckboxDesconocido, shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasVentanaDibujo, std::shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasVentanaFotogrametrica)
{
	CXTPPropertySheet ps(títuloDiálogo);
	auto* pList = new CXTPPropertyPageListNavigator{};
	pList->SetListStyle(xtpListBoxVisualStudio2015);
	ps.SetNavigator(pList);
	
	CXTPPropertyPage* activo = nullptr;

	CPaginaSeleccionScrDesconocido páginaDesconocido(activo, títuloCheckboxDesconocido);
	CPaginaSeleccionScr3D página3D(activo);
	CPaginaSeleccionScrProyectadoYVertical páginaProyYVert(activo);
	CPaginaSeleccionScrGeograficoYVertical páginaGeoYVert(activo);
	CPaginaSeleccionScrMemorizados páginaMemorizados(activo);
	PaginaManual páginaManual(activo);
	std::unique_ptr<PaginaSeleccionSrcExistente> páginaVentanaDibujo;
	std::unique_ptr<PaginaSeleccionSrcExistente> páginaVentanaFotogrametrica;

	ps.AddPage(&páginaDesconocido);
	ps.AddPage(&página3D);
	ps.AddPage(&páginaProyYVert);
	ps.AddPage(&páginaGeoYVert);
	ps.AddPage(&páginaManual);
	ps.AddPage(&páginaMemorizados);

	if (sistemaCoordenadasVentanaDibujo) {
		páginaVentanaDibujo = std::make_unique<PaginaSeleccionSrcExistente>(activo, IDD_PAGINA_VENTANA_DIBUJO, sistemaCoordenadasVentanaDibujo->GetName());
		ps.AddPage(páginaVentanaDibujo.get());
	}

	if (sistemaCoordenadasVentanaFotogrametrica) {
		páginaVentanaFotogrametrica = std::make_unique<PaginaSeleccionSrcExistente>(activo, IDD_PAGINA_VENTANA_FOTOGRAMETRICA, sistemaCoordenadasVentanaFotogrametrica->GetName());
		ps.AddPage(páginaVentanaFotogrametrica.get());
	}

	ps.SetResizable();
	ps.SetActivePage(sistemaCoordenadasVentanaDibujo || sistemaCoordenadasVentanaFotogrametrica ? 6 : 0);

	INT_PTR resultado;
	{
		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
		resultado = ps.DoModal();
	}

	if( IDOK == resultado ) {
		if( activo == & páginaDesconocido)
			return UtilidadesString::Format(_G(IDS_STRING2032), static_cast<LPCWSTR>(títuloCheckboxDesconocido));

		if( activo == &página3D )
			return página3D.Wkt;

		if( activo == & páginaProyYVert)
			return páginaProyYVert.Wkt;

		if( activo == & páginaGeoYVert)
			return páginaGeoYVert.Wkt;

		if (activo == &páginaManual)
			return páginaManual.Wkt;

		if( activo == &páginaMemorizados )
			return páginaMemorizados.Wkt;
	}

	if (sistemaCoordenadasVentanaDibujo)
		return sistemaCoordenadasVentanaDibujo->GetWkt();

	if (sistemaCoordenadasVentanaFotogrametrica)
		return sistemaCoordenadasVentanaFotogrametrica->GetWkt();

	return Digi21::OpenGis::GetCoordinateSystemFactory()->CreateCompoundUnknown()->GetWkt();
}
