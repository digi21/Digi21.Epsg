#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaSeleccionScrMemorizados.h"

using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;

namespace Digi21::Geodesia::EPSG
{
	IMPLEMENT_DYNAMIC(CPaginaSeleccionScrMemorizados, CXTPPropertyPage)

	CPaginaSeleccionScrMemorizados::CPaginaSeleccionScrMemorizados(CXTPPropertyPage*& _current)
		: CXTPPropertyPage(IDD)
		, current{ _current }
	{
	}

	BOOL CPaginaSeleccionScrMemorizados::OnSetActive()
	{
		current = this;
		RellenaListaMemorizados();
		return TRUE;
	}

	void CPaginaSeleccionScrMemorizados::DoDataExchange(CDataExchange* pDX)
	{
		CXTPPropertyPage::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_LISTA_SISTEMAS_COORDENADAS_MEMORIZADOS_HORIZONTAL, m_wndListaSistemasReferenciaMemorizados);
	}

	BEGIN_MESSAGE_MAP(CPaginaSeleccionScrMemorizados, CXTPPropertyPage)
		ON_BN_CLICKED(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS_HORIZONTAL, &CPaginaSeleccionScrMemorizados::OnBnClickedButtonEliminarSistemaCoordenadas)
		ON_LBN_SELCHANGE(IDC_LISTA_SISTEMAS_COORDENADAS_MEMORIZADOS_HORIZONTAL, &CPaginaSeleccionScrMemorizados::OnSelchangeListaSistemasCoordenadasMemorizados)
		ON_BN_CLICKED(IDC_BOTON_EXPORTAR_SISTEMA_COORDENADAS_HORIZONTAL, &CPaginaSeleccionScrMemorizados::OnBnClickedBotonExportarSistemaCoordenadas)
	END_MESSAGE_MAP()

	BOOL CPaginaSeleccionScrMemorizados::OnInitDialog()
	{
		CXTPPropertyPage::OnInitDialog();

		SetResize(IDC_STATIC_TITULO_SISTEMA_COORDENADAS_DE_REFERENCIA_MEMORIZADOS_HORIZONTAL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_LISTA_SISTEMAS_COORDENADAS_MEMORIZADOS_HORIZONTAL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS_HORIZONTAL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BOTON_EXPORTAR_SISTEMA_COORDENADAS_HORIZONTAL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);

		return TRUE;
	}

#pragma region Propiedades
	CStringW CPaginaSeleccionScrMemorizados::GetWkt() const
	{
		return m_sWkt;
	}
#pragma endregion

	void CPaginaSeleccionScrMemorizados::RellenaListaMemorizados()
	{
		m_wndListaSistemasReferenciaMemorizados.ResetContent();

		auto const sistemasCoordenadasMemorizados = CRegistro::EnumeraStringsCurrentUser(
			CARPETA_DIGI21, 
			VERSION_DIGI3D, 
			CARPETA_REGISTRO_DIGI3D_MEMORIZED_COORDINATE_SYSTEMS);

		CWaitCursor wc;
		auto const fábrica = GetCoordinateSystemFactory();

		for( size_t i=0; i<sistemasCoordenadasMemorizados.size(); i++ ) {
			auto* wkt = new CStringW(
				CRegistro::GetStringCurrentUser(
					CARPETA_DIGI21, 
					VERSION_DIGI3D, 
					CARPETA_REGISTRO_DIGI3D_MEMORIZED_COORDINATE_SYSTEMS,
					sistemasCoordenadasMemorizados[i]));

			try {
				auto const nombre = fábrica->CreateFromWkt(*wkt)->GetName();
				auto const índice = m_wndListaSistemasReferenciaMemorizados.AddString(nombre);
				m_wndListaSistemasReferenciaMemorizados.SetItemData(índice, reinterpret_cast<DWORD_PTR>(wkt));
			}
			catch(...)
			{
			}
		}
	}

	void CPaginaSeleccionScrMemorizados::ActivaControlesSistema(BOOL activar) const
	{
		if( !activar )
			GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS_HORIZONTAL)->EnableWindow(activar);
		else
			GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS_HORIZONTAL)->EnableWindow(-1 != m_wndListaSistemasReferenciaMemorizados.GetCurSel());
	}

	void CPaginaSeleccionScrMemorizados::OnBnClickedButtonEliminarSistemaCoordenadas()
	{
		if( -1 == m_wndListaSistemasReferenciaMemorizados.GetCurSel() )
			return;

		CStringW título;
		m_wndListaSistemasReferenciaMemorizados.GetText(m_wndListaSistemasReferenciaMemorizados.GetCurSel(), título);

		CRegistro::BorraValorCurrentUser( 
			CARPETA_DIGI21, 
			VERSION_DIGI3D, 
			CARPETA_REGISTRO_DIGI3D_MEMORIZED_COORDINATE_SYSTEMS,
			título);

		auto const cadena = reinterpret_cast<CStringW*>(m_wndListaSistemasReferenciaMemorizados.GetItemData(m_wndListaSistemasReferenciaMemorizados.GetCurSel()));
		delete cadena;

		m_wndListaSistemasReferenciaMemorizados.DeleteString(m_wndListaSistemasReferenciaMemorizados.GetCurSel());
	}

	void CPaginaSeleccionScrMemorizados::OnBnClickedBotonExportarSistemaCoordenadas()
	{
		CFileDialog dlg( FALSE, L"*.prj",
						nullptr,
						OFN_ENABLESIZING,
						_G(IDS_STRING2017),
						AfxGetMainWnd());

		if(IDOK == dlg.DoModal() ) {
			CStdioFile os;
			CFileException e;

			if( os.Open(dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate, &e) ) {
				os.WriteString(this->Wkt);
			} else {
				wchar_t cadena[256];
				e.GetErrorMessage(cadena, _countof(cadena));
				AfxMessageBox(cadena);
			}
		}
	}


	void CPaginaSeleccionScrMemorizados::OnSelchangeListaSistemasCoordenadasMemorizados() 
	{
		auto const nCurSel = m_wndListaSistemasReferenciaMemorizados.GetCurSel();	
		if( -1 == nCurSel ) {
			GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS_HORIZONTAL)->EnableWindow(FALSE);
			return;
		}

		GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS_HORIZONTAL)->EnableWindow(TRUE);
		m_sWkt = *reinterpret_cast<CStringW*>(m_wndListaSistemasReferenciaMemorizados.GetItemData(nCurSel));
		UpdateData(FALSE);
	}

}
