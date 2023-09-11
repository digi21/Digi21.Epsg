#include "pch.h"
#include "Digi21.Epsg.h"

using namespace Digi21::Utilidades;

#ifdef _DEBUG
#define new DEBUG_NEW  // NOLINT
#endif

BEGIN_MESSAGE_MAP(CDigi21EpsgApp, CWinApp)
END_MESSAGE_MAP()

CDigi21EpsgApp THE_APP;
HINSTANCE G_HINSTRECURSOS;

BOOL CDigi21EpsgApp::InitInstance()
{
	CargadorWinSxS::SeleccionaIdiomaPreferido(CRegistro::GetStringLocalMachine(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_APP_CONFIGURATION, L"PreferredLanguage", L"en-US"));
#ifdef _DEBUG
	G_HINSTRECURSOS = LoadMUILibrary(CargadorWinSxS::GetNombreModulo(AfxGetInstanceHandle()), MUI_LANGUAGE_NAME, 0);
	ASSERT(nullptr != G_HINSTRECURSOS);
#else
	// Después de mil vueltas con Windows en Japonés, he descubierto que hay que cargar el ensamblado MUI manualmente. En Español no es necesario, ni en inglés,
	// pero las versiones japonesas se ve que no funcionan muy bien con lo relacionado con MUI o no está suficientemente bien documentado
	try {
		G_HINSTRECURSOS = CargadorWinSxS::CargaEnsambladoMUI(
			L"Digi21.Epsg.Resources", 
			CStringW(SOLUTIONFILEVERSIONSTRING), 
			L"Digi21.Epsg.dll.mui");
	} 
	catch(std::exception& excepción)
	{
		CStringW cadena;
		cadena.Format(L"An exception was detected when loading the resource file for the component %s", TITULO_EXCEPCIONES);
		LogEventos2::AlmacenaEventoError(cadena);

		return FALSE;
	}
#endif
	return CWinApp::InitInstance();
}

int CDigi21EpsgApp::ExitInstance()
{
	FreeMUILibrary(G_HINSTRECURSOS);

	return CWinApp::ExitInstance();
}
