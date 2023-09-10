#include "pch.h"
#include "Digi21.Epsg.h"
#include "DialogoSeleccionScrVertical.h"
#include "DialogoBuscarScrVertical.h"

using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;
using namespace std;

namespace Digi21::Geodesia::EPSG
{
	IMPLEMENT_DYNAMIC(CDialogoSeleccionScrVertical, CXTPResizeDialog)

	CDialogoSeleccionScrVertical::CDialogoSeleccionScrVertical(CStringW const& título, CStringW const& títuloCheckboxDesconocido, std::shared_ptr<CoordinateSystem> const& sistemaCoordenadasDefecto, CWnd* pParent /*=nullptr*/)
		: CXTPResizeDialog(IDD, pParent)
		, m_bDesconocido{}
		, título{ título }
		, títuloCheckboxDesconocido{ títuloCheckboxDesconocido }
	{
		SetFlag(xtpResizeNoSizeIcon);
		m_sWkt = sistemaCoordenadasDefecto->GetWkt();
	}

	void CDialogoSeleccionScrVertical::DoDataExchange(CDataExchange* pDX)
	{
		CXTPResizeDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_LISTA_SISTEMAS_COORDENADAS_MEMORIZADOS, m_wndListaSistemasReferenciaMemorizados);
		DDX_Text(pDX, IDC_EDIT_WKT_SISTEMA_COORDNADAS, m_sWkt);
		DDX_Check(pDX, IDC_CHECK_DESCONOCIDO, m_bDesconocido);
	}

	BEGIN_MESSAGE_MAP(CDialogoSeleccionScrVertical, CXTPResizeDialog)
		ON_BN_CLICKED(IDC_BOTON_GUARDAR_SISTEMA_COORDENADAS, &CDialogoSeleccionScrVertical::OnBnClickedBotonGuardarSistemaCoordenadas)
		ON_BN_CLICKED(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS, &CDialogoSeleccionScrVertical::OnBnClickedButtonEliminarSistemaCoordenadas)
		ON_BN_CLICKED(IDC_BUTTON_IMPORTAR, &CDialogoSeleccionScrVertical::OnBnClickedButtonImportar)
		ON_BN_CLICKED(IDC_BUTTON_EXPORTAR, &CDialogoSeleccionScrVertical::OnBnClickedButtonExportar)
		ON_BN_CLICKED(IDOK, &CDialogoSeleccionScrVertical::OnBnClickedOk)
		ON_BN_CLICKED(IDC_BOTON_BUSCAR_SISTEMA_COORDENADAS, &CDialogoSeleccionScrVertical::OnBnClickedBotonBuscarSistemaCoordenadas)
		ON_LBN_SELCHANGE(IDC_LISTA_SISTEMAS_COORDENADAS_MEMORIZADOS, OnSelchangeListaSistemasCoordenadasMemorizados)
		ON_BN_CLICKED(IDC_CHECK_DESCONOCIDO, &CDialogoSeleccionScrVertical::OnBnClickedCheckDesconocido)
				
		ON_WM_DESTROY()
	END_MESSAGE_MAP()


	// CDialogoSeleccionScrVertical message handlers
	BOOL CDialogoSeleccionScrVertical::OnInitDialog()
	{
		CXTPResizeDialog::OnInitDialog();

		SetResize(IDC_CHECK_DESCONOCIDO, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_STATIC_TITULO_SISTEMA_COORDENADAS_DE_REFERENCIA_MEMORIZADOS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_LISTA_SISTEMAS_COORDENADAS_MEMORIZADOS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BOTON_BUSCAR_SISTEMA_COORDENADAS, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BOTON_GUARDAR_SISTEMA_COORDENADAS, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_EDIT_WKT_SISTEMA_COORDNADAS, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BUTTON_IMPORTAR, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BUTTON_EXPORTAR, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDOK, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDCANCEL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);

		SetWindowText(título);
		GetDlgItem(IDC_CHECK_DESCONOCIDO)->SetWindowText(títuloCheckboxDesconocido);

		auto const sistemasCoordenadasMemorizados = CRegistro::EnumeraStringsCurrentUser(
			CARPETA_DIGI21, 
			VERSION_DIGI3D, 
			CARPETA_REGISTRO_DIGI3D_MEMORIZED_VERTICAL_COORDINATE_SYSTEMS);

		CWaitCursor wc;
		auto const fábrica = GetCoordinateSystemFactory();

		for( auto const& memorizado : sistemasCoordenadasMemorizados ) {
			auto* wkt = new CStringW(
				CRegistro::GetStringCurrentUser(
					CARPETA_DIGI21, 
					VERSION_DIGI3D, 
					CARPETA_REGISTRO_DIGI3D_MEMORIZED_VERTICAL_COORDINATE_SYSTEMS,
					memorizado));

			try {
				auto const nombre = fábrica->CreateFromWkt(*wkt)->GetName();
				auto const índice = m_wndListaSistemasReferenciaMemorizados.AddString(nombre);
				m_wndListaSistemasReferenciaMemorizados.SetItemData(índice, reinterpret_cast<DWORD_PTR>(wkt));
			}
			catch(...)
			{
				continue;
			}
		}

		try {
			m_bDesconocido = nullptr != dynamic_pointer_cast<LocalCoordinateSystem>(fábrica->CreateFromWkt(m_sWkt));
		}
		catch(...)
		{
			m_bDesconocido = true;
		}
		UpdateData(FALSE);
		OnBnClickedCheckDesconocido();

		CXTPWindowPos wndPosition;
		wndPosition.LoadWindowPos(this, L"DialogoSelecciónCrsVertical", CARPETA_REGISTRO_APP_CONFIGURATION_DIALOGLAYOUT);

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}

	void CDialogoSeleccionScrVertical::ActivaControlesSistemaConocido(BOOL activar) const
	{
		GetDlgItem(IDC_STATIC_TITULO_SISTEMA_COORDENADAS_DE_REFERENCIA_MEMORIZADOS)->EnableWindow(activar);
		GetDlgItem(IDC_LISTA_SISTEMAS_COORDENADAS_MEMORIZADOS)->EnableWindow(activar);
		GetDlgItem(IDC_BOTON_BUSCAR_SISTEMA_COORDENADAS)->EnableWindow(activar);
		GetDlgItem(IDC_BOTON_GUARDAR_SISTEMA_COORDENADAS)->EnableWindow(activar);
		GetDlgItem(IDC_EDIT_WKT_SISTEMA_COORDNADAS)->EnableWindow(activar);

		if( !activar )
			GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS)->EnableWindow(activar);
		else
			GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS)->EnableWindow(-1 != m_wndListaSistemasReferenciaMemorizados.GetCurSel());
	}

	void CDialogoSeleccionScrVertical::OnBnClickedBotonBuscarSistemaCoordenadas()
	{
		UpdateData();

		auto defaultCrs = 5782; // Alicante Height

		try {
			CWaitCursor wc;
			auto const fábrica = GetCoordinateSystemFactory();
			auto const sistemaCooordenadas = fábrica->CreateFromWkt(m_sWkt);

			if( 0 == sistemaCooordenadas->GetAuthority().CompareNoCase(L"EPSG") )
				defaultCrs = sistemaCooordenadas->GetAuthorityCode();
		}
		catch(...)
		{
		}

		CDialogoBuscarScrVertical dlg(defaultCrs, this);
		if( IDOK == dlg.DoModal() ) {
			try {
				m_sWkt = GetCoordinateSystemAuthorityFactory()->CreateCoordinateSystem(dlg.SistemaReferenciaCoordenadas)->GetWkt();
			}
			catch(...)
			{
				m_sWkt = L"";
			}

			UpdateData(FALSE);
		}
	}

	void CDialogoSeleccionScrVertical::OnBnClickedBotonGuardarSistemaCoordenadas()
	{
		UpdateData();

		// Lo almacenamos si no lo teníamos ya almacenado
		for (auto i=0;i<m_wndListaSistemasReferenciaMemorizados.GetCount(); i++ ) 
			if( 0 == m_sWkt.CompareNoCase(*reinterpret_cast<CStringW*>(m_wndListaSistemasReferenciaMemorizados.GetItemData(i)) ) )
				return;

		shared_ptr<CoordinateSystem> sistemaCooordenadas;
		try {
			CWaitCursor wc;
			sistemaCooordenadas = GetCoordinateSystemFactory()->CreateFromWkt(m_sWkt);
		}
		catch(...)
		{
			return;
		}

		m_wndListaSistemasReferenciaMemorizados.SetItemData(
			m_wndListaSistemasReferenciaMemorizados.AddString(sistemaCooordenadas->GetName()), 
			reinterpret_cast<DWORD_PTR>(new CStringW(m_sWkt)));

		CRegistro::SetStringCurrentUser(
			CARPETA_DIGI21, 
			VERSION_DIGI3D, 
			CARPETA_REGISTRO_DIGI3D_MEMORIZED_VERTICAL_COORDINATE_SYSTEMS, 
			CStringW(sistemaCooordenadas->GetName()),
			m_sWkt);
	}


	void CDialogoSeleccionScrVertical::OnBnClickedButtonEliminarSistemaCoordenadas()
	{
		if( -1 == m_wndListaSistemasReferenciaMemorizados.GetCurSel() )
			return;

		CStringW titulo;
		m_wndListaSistemasReferenciaMemorizados.GetText(m_wndListaSistemasReferenciaMemorizados.GetCurSel(), titulo);

		CRegistro::BorraValorCurrentUser( 
			CARPETA_DIGI21, 
			VERSION_DIGI3D, 
			CARPETA_REGISTRO_DIGI3D_MEMORIZED_VERTICAL_COORDINATE_SYSTEMS,
			título);

		auto const cadena = reinterpret_cast<CStringW*>(m_wndListaSistemasReferenciaMemorizados.GetItemData(m_wndListaSistemasReferenciaMemorizados.GetCurSel()));
		delete cadena;

		m_wndListaSistemasReferenciaMemorizados.DeleteString(m_wndListaSistemasReferenciaMemorizados.GetCurSel());
	}

	void CDialogoSeleccionScrVertical::OnSelchangeListaSistemasCoordenadasMemorizados() 
	{
		auto const nCurSel = m_wndListaSistemasReferenciaMemorizados.GetCurSel();	
		if( -1 == nCurSel ) {
			GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS)->EnableWindow(FALSE);
			return;
		}

		GetDlgItem(IDC_BUTTON_ELIMINAR_SISTEMA_COORDENADAS)->EnableWindow(TRUE);
		m_sWkt = *reinterpret_cast<CStringW*>(m_wndListaSistemasReferenciaMemorizados.GetItemData(nCurSel));
		UpdateData(FALSE);
	}


	void CDialogoSeleccionScrVertical::OnBnClickedButtonImportar()
	{
		// TODO: Add your control notification handler code here
	}


	void CDialogoSeleccionScrVertical::OnBnClickedButtonExportar()
	{
		//CFileDialog dlg(
		//	FALSE,
		//	L"*.prj",
		//	0,
		//	OFN_ENABLESIZING,
		//	_G(IDS_STRING2007),
		//	nullptr);

		//if(IDOK == dlg.DoModal() ) {
		//	auto coordinateSystem = GetCoordinateSystemAuthorityFactory::CreateCoordinateSystem(m_nSCR);

		//	CStdioFile os;
		//	CFileException excepción;

		//	if( !os.Open(dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate, &excepción) ) {
		//		wchar_t szError[1024];
		//		excepción.GetErrorMessage(szError, _countof(szError));

		//		CStringW cadena;
		//		cadena.Format(_G(IDS_STRING2008), dlg.GetPathName(), szError);
		//		AfxMessageBox(cadena);
		//		return;
		//	}

		//	os.WriteString(CStringW(coordinateSystem->Wkt));
		//}
	}

	void CDialogoSeleccionScrVertical::OnBnClickedCheckDesconocido()
	{
		UpdateData();
		ActivaControlesSistemaConocido(!m_bDesconocido);
	}

	void CDialogoSeleccionScrVertical::OnBnClickedOk()
	{
		UpdateData();

		if( m_bDesconocido ) {
			m_sWkt = _G(IDS_STRING2015);

			UpdateData(FALSE);
		}

		CXTPResizeDialog::OnOK();
	}


	void CDialogoSeleccionScrVertical::OnDestroy()
	{
		CXTPWindowPos wndPosition;
		wndPosition.SaveWindowPos(this, L"DialogoSelecciónCrsVertical", CARPETA_REGISTRO_APP_CONFIGURATION_DIALOGLAYOUT);
		CXTPResizeDialog::OnDestroy();
	}

}

