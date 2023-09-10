#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaSeleccionScr3D.h"
#include "DialogoBúsquedaPorCódigoEpsg.h"

using namespace Digi21::Utilidades;
using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;
using namespace std;

constexpr auto ORIENTACION_EJES_ESTANDAR = 0;
constexpr auto ORIENTACION_EJES_LATITUD_LONGITUD = 1;
constexpr auto ORIENTACION_EJES_LONGITUD_LATITUD = 2;

namespace Digi21::Geodesia::EPSG
{

	IMPLEMENT_DYNAMIC(CPaginaSeleccionScr3D, CXTPPropertyPage)

	CPaginaSeleccionScr3D::CPaginaSeleccionScr3D(CXTPPropertyPage*& _current)
		: CXTPPropertyPage(IDD)
		, current{ _current }
	{
	}

	void CPaginaSeleccionScr3D::DoDataExchange(CDataExchange* pDX)
	{
		CXTPPropertyPage::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_COMBO_CRS, m_wndCRS);
		DDX_CBIndex(pDX, IDC_COMBO_ORIENTACION_EJES, m_nOrientaciónEjes);
	}

	BEGIN_MESSAGE_MAP(CPaginaSeleccionScr3D, CXTPPropertyPage)
		ON_CBN_SELCHANGE(IDC_COMBO_CRS, &CPaginaSeleccionScr3D::OnCbnSelchangeComboCrs)
		ON_BN_CLICKED(IDC_BUTTON_BUSCAR_EPSG, &CPaginaSeleccionScr3D::OnBnClickedBuscarEpsg)
		ON_BN_CLICKED(IDC_BOTON_EXPORTAR_SISTEMA_COORDENADAS_HORIZONTAL, &CPaginaSeleccionScr3D::OnBnClickedBotonExportarSistemaCoordenadas)
		ON_BN_CLICKED(IDC_BOTON_MEMORIZAR_SISTEMA_COORDENADAS_HORIZONTAL, &CPaginaSeleccionScr3D::OnBnClickedBotonMemorizarSistemaCoordenadas)
		ON_BN_CLICKED(IDC_BOTON_COPIAR, &CPaginaSeleccionScr3D::OnBnClickedBotonCopiar)
	END_MESSAGE_MAP()

	BOOL CPaginaSeleccionScr3D::OnSetActive()
	{
		current = this;
		return TRUE;
	}

	BOOL CPaginaSeleccionScr3D::OnInitDialog()
	{
		CXTPPropertyPage::OnInitDialog();

		SetResize(IDC_COMBO_CRS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_COMBO_ORIENTACION_EJES, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_BUTTON_BUSCAR_EPSG, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
		SetResize(IDC_BOTON_EXPORTAR_SISTEMA_COORDENADAS_HORIZONTAL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BOTON_MEMORIZAR_SISTEMA_COORDENADAS_HORIZONTAL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
		SetResize(IDC_BOTON_COPIAR, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);

		m_wndCRS.ResetContent();
		CWaitCursor wc;

		auto const listaSistemasCoordenadas = GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(CoordinateSystemKind::Geographic3D);

		for (auto const& [codigo, nombre] : listaSistemasCoordenadas)
			m_wndCRS.SetItemData(m_wndCRS.AddString(nombre), codigo);

		SeleccionaCódigo(m_nSCR);

		m_wndCRS.EnableWindow(TRUE);

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}


#pragma region Propiedades
	CStringW CPaginaSeleccionScr3D::GetWkt() const
	{
		auto const geográfico = GetCoordinateSystemAuthorityFactory()->CreateGeographicCoordinateSystem(m_nSCR);

		switch (m_nOrientaciónEjes)
		{
		case ORIENTACION_EJES_LATITUD_LONGITUD:
			return GetCoordinateSystemFactory()->ModifyWithAxisNorthEast(geográfico)->GetWkt();
		case ORIENTACION_EJES_LONGITUD_LATITUD:
			return GetCoordinateSystemFactory()->ModifyWithAxisEastNorth(geográfico)->GetWkt();
		default:
			return geográfico->GetWkt();
		}
	}
#pragma endregion

	void CPaginaSeleccionScr3D::OnCbnSelchangeComboCrs()
	{
		UpdateData();
		m_nSCR = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));
		UpdateData(FALSE);
	}

	void CPaginaSeleccionScr3D::OnBnClickedBuscarEpsg()
	{
		CDialogoBúsquedaPorCódigoEpsg dlg;
		dlg.m_nCódigo = m_nSCR;

		if (IDOK == dlg.DoModal()) {
			if (3 != GetCoordinateSystemAuthorityFactory()->GetAxisOfCoordinateSystem(GetCoordinateSystemAuthorityFactory()->GetCodeOfCoordinateSystemAssociatedWithCrs(dlg.m_nCódigo)).size())
				AfxMessageBox(_G(IDS_STRING2019));
			else {
				SeleccionaCódigo(dlg.m_nCódigo);
			}
		}
	}

	void CPaginaSeleccionScr3D::OnBnClickedBotonExportarSistemaCoordenadas()
	{
		CFileDialog dlg(FALSE, L"*.prj",
						nullptr,
						OFN_ENABLESIZING,
						_G(IDS_STRING2017),
						AfxGetMainWnd());

		if (IDOK == dlg.DoModal()) {
			CStdioFile os;
			CFileException e;

			if (os.Open(dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate, &e)) {
				os.WriteString(this->Wkt);
			}
			else {
				wchar_t cadena[256];
				e.GetErrorMessage(cadena, _countof(cadena));
				AfxMessageBox(cadena);
			}
		}
	}

	void CPaginaSeleccionScr3D::OnBnClickedBotonMemorizarSistemaCoordenadas()
	{
		UpdateData();

		shared_ptr<CoordinateSystem> sistemaCooordenadas = nullptr;
		try {
			CWaitCursor wc;
			sistemaCooordenadas = GetCoordinateSystemAuthorityFactory()->CreateCoordinateSystem(m_nSCR);
		}
		catch (...)
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

	void CPaginaSeleccionScr3D::SeleccionaCódigo(int código)
	{
		for (auto i = 0; i < m_wndCRS.GetCount(); i++) {
			if (static_cast<int>(m_wndCRS.GetItemData(i)) == código) {
				m_wndCRS.SetCurSel(i);
				OnCbnSelchangeComboCrs();
				break;
			}
		}
	}

	void CPaginaSeleccionScr3D::OnBnClickedBotonCopiar()
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
