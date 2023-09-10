#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaSeleccionScrGeograficoVertical.h"
#include "DialogoBúsquedaPorCódigoEpsg.h"

constexpr auto ORIENTACION_EJES_ESTANDAR = 0;
constexpr auto ORIENTACION_EJES_LATITUD_LONGITUD = 1;
constexpr auto ORIENTACION_EJES_LONGITUD_LATITUD = 2;

using namespace Digi21::Utilidades;
using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;
using namespace std;

namespace Digi21::Geodesia::EPSG
{
	IMPLEMENT_DYNAMIC(CPaginaSeleccionScrGeograficoYVertical, CXTPPropertyPage)

	CPaginaSeleccionScrGeograficoYVertical::CPaginaSeleccionScrGeograficoYVertical(CXTPPropertyPage*& _current)
		: CXTPPropertyPage(IDD)
		, current{ _current }
	{
	}

	void CPaginaSeleccionScrGeograficoYVertical::DoDataExchange(CDataExchange* pDX)
	{
		CXTPPropertyPage::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_COMBO_CRS_HORIZONTAL, m_wndCrsHorizontal);
		DDX_CBIndex(pDX, IDC_COMBO_ORIENTACION_EJES, m_nOrientaciónEjes );
		DDX_Control(pDX, IDC_COMBO_CRS_VERTICAL, m_wndCrsVertical);
		DDX_Check(pDX, IDC_CHECK_VERTICAL_DESCONOCIDO, m_bVerticalDesconocido);
	}

	BEGIN_MESSAGE_MAP(CPaginaSeleccionScrGeograficoYVertical, CXTPPropertyPage)
		ON_CBN_SELCHANGE(IDC_COMBO_CRS_HORIZONTAL, &CPaginaSeleccionScrGeograficoYVertical::OnCbnSelchangeComboCrsHorizontal)
		ON_BN_CLICKED(IDC_BUTTON_BUSCAR_EPSG_HORIZONTAL, &CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBuscarHorizontalEpsg)
		ON_CBN_SELCHANGE(IDC_COMBO_CRS_VERTICAL, &CPaginaSeleccionScrGeograficoYVertical::OnCbnSelchangeComboCrsVertical)
		ON_BN_CLICKED(IDC_BUTTON_BUSCAR_EPSG_VERTICAL, &CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBuscarVerticalEpsg)
		ON_BN_CLICKED(IDC_BOTON_EXPORTAR_SISTEMA_COORDENADAS_HORIZONTAL, &CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBotonExportarSistemaCoordenadas)
		ON_BN_CLICKED(IDC_BOTON_MEMORIZAR_SISTEMA_COORDENADAS_HORIZONTAL, &CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBotonMemorizarSistemaCoordenadas)
		ON_BN_CLICKED(IDC_CHECK_VERTICAL_DESCONOCIDO, &CPaginaSeleccionScrGeograficoYVertical::OnBnClickedCheckVerticalDesconocido)
		ON_BN_CLICKED(IDC_BOTON_COPIAR, &CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBotonCopiar)
	END_MESSAGE_MAP()

	BOOL CPaginaSeleccionScrGeograficoYVertical::OnSetActive()
	{
		current = this;
		return TRUE;
	}

	BOOL CPaginaSeleccionScrGeograficoYVertical::OnInitDialog()
	{
		CXTPPropertyPage::OnInitDialog();

		SetResize(IDC_COMBO_CRS_HORIZONTAL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_COMBO_ORIENTACION_EJES, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_BUTTON_BUSCAR_EPSG_HORIZONTAL, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_COMBO_CRS_VERTICAL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_BUTTON_BUSCAR_EPSG_VERTICAL, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_BOTON_EXPORTAR_SISTEMA_COORDENADAS_HORIZONTAL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BOTON_MEMORIZAR_SISTEMA_COORDENADAS_HORIZONTAL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BOTON_COPIAR, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);

		CWaitCursor wc;

		RellenaScrGeográficos();
		RellenaScrVerticales();

		SeleccionaHorizontal(m_nScrHorizontal);
		SeleccionaVertical(m_nScrVertical);

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}

	shared_ptr<CoordinateSystem> CreaSistemaCoordenadas(int nHorizontal, int orientaciónEjes, BOOL verticalDesconocido, int nVertical)
	{
		auto const geográfico = GetCoordinateSystemAuthorityFactory()->CreateGeographicCoordinateSystem(nHorizontal);
		shared_ptr<CoordinateSystem> vertical;
				
		if( verticalDesconocido ) {
			vertical = GetCoordinateSystemFactory()->CreateFromWkt(_G(IDS_STRING2021));
		} else {
			vertical = GetCoordinateSystemAuthorityFactory()->CreateVerticalCoordinateSystem(nVertical);
		}

		shared_ptr<CompoundCoordinateSystem> compuesto;
		auto nombre = UtilidadesString::Format(L"%s + %s", static_cast<LPCWSTR>(geográfico->GetName()), static_cast<LPCWSTR>(vertical->GetName()));

		switch(orientaciónEjes)
		{
		case ORIENTACION_EJES_LATITUD_LONGITUD:
			compuesto = make_shared<CompoundCoordinateSystem>(
				nombre,
				GetCoordinateSystemFactory()->ModifyWithAxisNorthEast(geográfico),
				vertical);
			break;
		case ORIENTACION_EJES_LONGITUD_LATITUD:
			compuesto = make_shared<CompoundCoordinateSystem>(
				nombre,
				GetCoordinateSystemFactory()->ModifyWithAxisEastNorth(geográfico),
				vertical);
			break;
		default:
			compuesto = make_shared<CompoundCoordinateSystem>(
				nombre,
				geográfico,
				vertical);
			break;
		}

		return compuesto;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBotonExportarSistemaCoordenadas()
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

	void CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBotonMemorizarSistemaCoordenadas()
	{
		UpdateData();

		shared_ptr<CoordinateSystem> sistemaCooordenadas;
		try {
			CWaitCursor wc;
			sistemaCooordenadas = CreaSistemaCoordenadas(m_nScrHorizontal, m_nOrientaciónEjes, m_bVerticalDesconocido, m_nScrVertical);
		}
		catch(...)
		{
			return;
		}

		CRegistro::SetStringCurrentUser(
			CARPETA_DIGI21, 
			VERSION_DIGI3D, 
			CARPETA_REGISTRO_DIGI3D_MEMORIZED_COORDINATE_SYSTEMS, 
			sistemaCooordenadas->GetName(),
			sistemaCooordenadas->GetWkt());
	}

#pragma region Propiedades
	CStringW CPaginaSeleccionScrGeograficoYVertical::GetWkt() const
	{
		return CreaSistemaCoordenadas(m_nScrHorizontal, m_nOrientaciónEjes, m_bVerticalDesconocido, m_nScrVertical)->GetWkt();
	}
#pragma endregion

	void CPaginaSeleccionScrGeograficoYVertical::OnCbnSelchangeComboCrsHorizontal()
	{
		UpdateData();
		m_nScrHorizontal = static_cast<int>(m_wndCrsHorizontal.GetItemData(m_wndCrsHorizontal.GetCurSel()));
		UpdateData(FALSE);
	}

	void CPaginaSeleccionScrGeograficoYVertical::OnCbnSelchangeComboCrsVertical()
	{
		UpdateData();
		m_nScrVertical = static_cast<int>(m_wndCrsVertical.GetItemData(m_wndCrsVertical.GetCurSel()));
		UpdateData(FALSE);
	}

	void CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBuscarHorizontalEpsg()
	{
		CDialogoBúsquedaPorCódigoEpsg dlg;
		dlg.m_nCódigo = m_nScrHorizontal;

		if( IDOK == dlg.DoModal() ) {
			if(2 != GetCoordinateSystemAuthorityFactory()->GetAxisOfCoordinateSystem(GetCoordinateSystemAuthorityFactory()->GetCodeOfCoordinateSystemAssociatedWithCrs(dlg.m_nCódigo)).size() ) 
				AfxMessageBox(_G(IDS_STRING2022));
			else {
				SeleccionaHorizontal(dlg.m_nCódigo);
			}
		}
	}

	void CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBuscarVerticalEpsg()
	{
		CDialogoBúsquedaPorCódigoEpsg dlg;
		dlg.m_nCódigo = m_nScrVertical;

		if( IDOK == dlg.DoModal() ) {
			if(1 != GetCoordinateSystemAuthorityFactory()->GetAxisOfCoordinateSystem(GetCoordinateSystemAuthorityFactory()->GetCodeOfCoordinateSystemAssociatedWithCrs(dlg.m_nCódigo)).size())
				AfxMessageBox(_G(IDS_STRING2023));
			else {
				SeleccionaVertical(dlg.m_nCódigo);
			}
		}
	}

	void CPaginaSeleccionScrGeograficoYVertical::RellenaScrGeográficos()
	{
		for (auto const& [codigo, nombre] : GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(CoordinateSystemKind::Geographic2D))
			m_wndCrsHorizontal.SetItemData(m_wndCrsHorizontal.AddString(nombre), codigo);
	}

	void CPaginaSeleccionScrGeograficoYVertical::RellenaScrVerticales()
	{
		for (auto const& [codigo, nombre] : GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(CoordinateSystemKind::Vertical))
			m_wndCrsVertical.SetItemData(m_wndCrsVertical.AddString(nombre), codigo);
	}

	void CPaginaSeleccionScrGeograficoYVertical::SeleccionaHorizontal( int código )
	{
		for (auto i=0; i<m_wndCrsHorizontal.GetCount(); i++ ) {
			if( static_cast<int>(m_wndCrsHorizontal.GetItemData(i)) == código ) {
				m_wndCrsHorizontal.SetCurSel(i);
				OnCbnSelchangeComboCrsHorizontal();
				break;
			}
		}
	}

	void CPaginaSeleccionScrGeograficoYVertical::SeleccionaVertical( int código )
	{
		for (auto i=0; i<m_wndCrsVertical.GetCount(); i++ ) {
			if( static_cast<int>(m_wndCrsVertical.GetItemData(i)) == código ) {
				m_wndCrsVertical.SetCurSel(i);
				OnCbnSelchangeComboCrsVertical();
				break;
			}
		}
	}

	void CPaginaSeleccionScrGeograficoYVertical::OnBnClickedCheckVerticalDesconocido()
	{
		UpdateData();

		GetDlgItem(IDC_COMBO_CRS_VERTICAL)->EnableWindow(!m_bVerticalDesconocido);
		GetDlgItem(IDC_BUTTON_BUSCAR_EPSG_VERTICAL)->EnableWindow(!m_bVerticalDesconocido);
	}

	void CPaginaSeleccionScrGeograficoYVertical::OnBnClickedBotonCopiar()
	{
		if (!OpenClipboard())
		{
			TRACE(L"Cannot open the Clipboard");
			return;
		}
		// Remove the current Clipboard contents
		if (!EmptyClipboard())
		{
			TRACE(L"Cannot empty the Clipboard");
			return;
		}

		auto wkt = Wkt;

		auto const size = sizeof(wchar_t)*(1 + wkt.GetLength());
		auto const hResult = GlobalAlloc(GMEM_MOVEABLE, size);
		auto const lptstrCopy = static_cast<LPTSTR>(GlobalLock(hResult));
		memcpy(lptstrCopy, wkt.GetBuffer(), size);
		GlobalUnlock(hResult);

#ifndef _UNICODE
				if (::SetClipboardData(CF_TEXT, hResult) == nullptr)
#else
		if (SetClipboardData(CF_UNICODETEXT, hResult) == nullptr)
#endif 
		{
			TRACE(CErroresWindows::GetLastError());
			TRACE("Unable to set Clipboard data");

			GlobalFree(hResult); // Edit by Leo Davidson 16/Sep/2010: Free buffer if clipboard didn't take it.
			CloseClipboard();
			return;
		}

		CloseClipboard();
	}
}
