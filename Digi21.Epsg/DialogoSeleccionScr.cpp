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
	PaginaSeleccionSrcExistente páginaExistente(activo, sistemaCoordenadasDefecto->GetName());

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

CStringW DialogoSeleccionScr::DialogSeleccionaScr(CStringW const& títuloDiálogo, CStringW const& títuloCheckboxDesconocido, shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto)
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
	std::unique_ptr<PaginaSeleccionSrcExistente> páginaExistente;

	ps.AddPage(&páginaDesconocido);
	ps.AddPage(&página3D);
	ps.AddPage(&páginaProyYVert);
	ps.AddPage(&páginaGeoYVert);
	ps.AddPage(&páginaManual);
	ps.AddPage(&páginaMemorizados);

	if (sistemaCoordenadasDefecto) {
		páginaExistente = std::make_unique<PaginaSeleccionSrcExistente>(activo, sistemaCoordenadasDefecto->GetName());
		ps.AddPage(páginaExistente.get());
	}
	ps.SetResizable();
	ps.SetActivePage(sistemaCoordenadasDefecto ? 6 : 0);

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

	return sistemaCoordenadasDefecto->GetWkt();
}
