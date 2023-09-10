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

CStringW DialogoSeleccionScr::DialogSeleccionaScr(CStringW const& t�tuloDi�logo, shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto)
{
	CXTPPropertySheet ps(t�tuloDi�logo);
	auto* pList = new CXTPPropertyPageListNavigator{};
	pList->SetListStyle(xtpListBoxVisualStudio2015);
	ps.SetNavigator(pList);

	CXTPPropertyPage* activo = nullptr;

	CPaginaSeleccionScr3D p�gina3D(activo);
	CPaginaSeleccionScrProyectadoYVertical p�ginaProyYVert(activo);
	CPaginaSeleccionScrGeograficoYVertical p�ginaGeoYVert(activo);
	CPaginaSeleccionScrMemorizados p�ginaMemorizados(activo);
	PaginaManual p�ginaManual(activo);
	PaginaSeleccionSrcExistente p�ginaExistente(activo, sistemaCoordenadasDefecto->GetName());

	ps.AddPage(&p�gina3D);
	ps.AddPage(&p�ginaProyYVert);
	ps.AddPage(&p�ginaGeoYVert);
	ps.AddPage(&p�ginaManual);
	ps.AddPage(&p�ginaMemorizados);
	ps.AddPage(&p�ginaExistente);
	ps.SetResizable();
	ps.SetActivePage(5);

	INT_PTR resultado;
	{
        AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
		resultado = ps.DoModal();
	}

	if( IDOK == resultado ) {
		if( activo == &p�gina3D )
			return p�gina3D.Wkt;

		if( activo == & p�ginaProyYVert)
			return p�ginaProyYVert.Wkt;

		if( activo == & p�ginaGeoYVert)
			return p�ginaGeoYVert.Wkt;

		if (activo == &p�ginaManual)
			return p�ginaManual.Wkt;

		if( activo == &p�ginaMemorizados )
			return p�ginaMemorizados.Wkt;
	}

	return sistemaCoordenadasDefecto->GetWkt();
}

CStringW DialogoSeleccionScr::DialogSeleccionaScr(CStringW const& t�tuloDi�logo, CStringW const& t�tuloCheckboxDesconocido, shared_ptr<Digi21::OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto)
{
	CXTPPropertySheet ps(t�tuloDi�logo);
	auto* pList = new CXTPPropertyPageListNavigator{};
	pList->SetListStyle(xtpListBoxVisualStudio2015);
	ps.SetNavigator(pList);
	
	CXTPPropertyPage* activo = nullptr;

	CPaginaSeleccionScrDesconocido p�ginaDesconocido(activo, t�tuloCheckboxDesconocido);
	CPaginaSeleccionScr3D p�gina3D(activo);
	CPaginaSeleccionScrProyectadoYVertical p�ginaProyYVert(activo);
	CPaginaSeleccionScrGeograficoYVertical p�ginaGeoYVert(activo);
	CPaginaSeleccionScrMemorizados p�ginaMemorizados(activo);
	PaginaManual p�ginaManual(activo);
	std::unique_ptr<PaginaSeleccionSrcExistente> p�ginaExistente;

	ps.AddPage(&p�ginaDesconocido);
	ps.AddPage(&p�gina3D);
	ps.AddPage(&p�ginaProyYVert);
	ps.AddPage(&p�ginaGeoYVert);
	ps.AddPage(&p�ginaManual);
	ps.AddPage(&p�ginaMemorizados);

	if (sistemaCoordenadasDefecto) {
		p�ginaExistente = std::make_unique<PaginaSeleccionSrcExistente>(activo, sistemaCoordenadasDefecto->GetName());
		ps.AddPage(p�ginaExistente.get());
	}
	ps.SetResizable();
	ps.SetActivePage(sistemaCoordenadasDefecto ? 6 : 0);

	INT_PTR resultado;
	{
		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
		resultado = ps.DoModal();
	}

	if( IDOK == resultado ) {
		if( activo == & p�ginaDesconocido)
			return UtilidadesString::Format(_G(IDS_STRING2032), static_cast<LPCWSTR>(t�tuloCheckboxDesconocido));

		if( activo == &p�gina3D )
			return p�gina3D.Wkt;

		if( activo == & p�ginaProyYVert)
			return p�ginaProyYVert.Wkt;

		if( activo == & p�ginaGeoYVert)
			return p�ginaGeoYVert.Wkt;

		if (activo == &p�ginaManual)
			return p�ginaManual.Wkt;

		if( activo == &p�ginaMemorizados )
			return p�ginaMemorizados.Wkt;
	}

	return sistemaCoordenadasDefecto->GetWkt();
}
