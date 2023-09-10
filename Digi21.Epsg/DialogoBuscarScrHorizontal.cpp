#include "pch.h"
#include "resource.h"
#include "DialogoBuscarScrHorizontal.h"
#include "DialogoB�squedaPorC�digoEpsg.h"

using namespace Digi21::Utilidades;
using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;

#ifdef _DEBUG
#define new DEBUG_NEW  // NOLINT
#endif

namespace Digi21::Geodesia::EPSG
{


	// Cuadro de di�logo de CDialogoBuscarScrHorizontal
	CDialogoBuscarScrHorizontal::CDialogoBuscarScrHorizontal(int scrSeleccionadoPorDefecto, CWnd* pParent /*=nullptr*/)
		: CDialog(IDD, pParent)
		, m_nSCR{ scrSeleccionadoPorDefecto }
	{
	}

	void CDialogoBuscarScrHorizontal::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_COMBO_TIPOS_CRS, m_wndTiposCRS);
		DDX_Control(pDX, IDC_COMBO_CRS, m_wndCRS);
		DDX_Text(pDX, IDC_EDIT_AREA, m_sArea);
		DDX_Text(pDX, IDC_EDIT_TIPO_SISTEMA_COORDENADAS, m_sTipoSistemaCoordenadas);
		DDX_Text(pDX, IDC_EDIT_NOMBRE_DATUM, m_sNombreDatum);
		DDX_Text(pDX, IDC_EDIT_ORIGEN_DATUM, m_sOrigenDatum);
		DDX_Text(pDX, IDC_EDIT_MERIDIANO, m_sMeridiano);
		DDX_Text(pDX, IDC_EDIT_AMBITO_USO, m_sAmbitoUso);
		DDX_Text(pDX, IDC_EDIT_NOMBRE_ELIPSOIDE, m_sElipsoide);
	}

	BEGIN_MESSAGE_MAP(CDialogoBuscarScrHorizontal, CDialog)
		ON_CBN_SELCHANGE(IDC_COMBO_TIPOS_CRS, &CDialogoBuscarScrHorizontal::OnCbnSelchangeComboTiposCrs)
		ON_CBN_SELCHANGE(IDC_COMBO_CRS, &CDialogoBuscarScrHorizontal::OnCbnSelchangeComboCrs)
		ON_BN_CLICKED(IDC_BUTTON_BUSCAR_EPSG, &CDialogoBuscarScrHorizontal::OnBnClickedBuscarEpsg)
		ON_BN_CLICKED(IDOK, &CDialogoBuscarScrHorizontal::OnBnClickedOk)
	END_MESSAGE_MAP()

	BOOL CDialogoBuscarScrHorizontal::OnInitDialog()
	{
		CDialog::OnInitDialog();

		m_wndTiposCRS.AddString(_G(IDS_STRING2005));
		m_wndTiposCRS.AddString(_G(IDS_STRING2006));

		RellenaCamposEnFunci�nScrActivo();
		ActivaDesactivaBot�nAceptar();

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}

	void CDialogoBuscarScrHorizontal::OnCbnSelchangeComboTiposCrs()
	{
		m_wndCRS.ResetContent();
		CWaitCursor wc;

		CStringW tipoSistemaCoordenadas;

		switch(m_wndTiposCRS.GetCurSel())
		{
		case 0:
			tipoSistemaCoordenadas = CoordinateSystemKind::Projected;
			break;
		case 1:
			tipoSistemaCoordenadas = CoordinateSystemKind::Geographic2D;
			break;
		}

		auto const listaSistemasCoordenadas = GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(tipoSistemaCoordenadas);

		for (auto const& [codigo, nombre] : listaSistemasCoordenadas)
			m_wndCRS.SetItemData(m_wndCRS.AddString(nombre), codigo);

		m_wndCRS.EnableWindow(TRUE);

		m_sArea = L"";
		m_sTipoSistemaCoordenadas = L"";
		m_sNombreDatum = L"";
		m_sOrigenDatum = L"";
		m_sMeridiano = L"";
		m_sAmbitoUso = L"";
		m_sElipsoide = L"";
		UpdateData(FALSE);

		ActivaDesactivaBot�nAceptar();
	}

	void CDialogoBuscarScrHorizontal::OnCbnSelchangeComboCrs()
	{
		UpdateData();

		auto const c�digoCRS = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));

		auto const fabricaSrc = GetCoordinateSystemAuthorityFactory();

		m_sArea = fabricaSrc->GetDescriptionAreaApplicationCrs(c�digoCRS);
		m_sTipoSistemaCoordenadas = fabricaSrc->GetNameOfCoordinateSystem(fabricaSrc->GetCodeOfCoordinateSystemAssociatedWithCrs(c�digoCRS));

		auto const c�digoDatum = fabricaSrc->GetCodeOfDatumAssociatedWithCrs(c�digoCRS);
		m_sNombreDatum = fabricaSrc->GetNameOfDatum(c�digoDatum);
		m_sOrigenDatum = fabricaSrc->GetOriginDescriptionOfDatum(c�digoDatum);
		m_sMeridiano = fabricaSrc->GetNameOfPrimeMeridian(fabricaSrc->GetCodeOfPrimeMeridianAssociatedWithCrs(c�digoDatum));
		m_sAmbitoUso = fabricaSrc->GetDescriptionAreaApplicationDatum(c�digoDatum);
		m_sElipsoide = fabricaSrc->GetNameOfEllipsoid(fabricaSrc->GetCodeOfEllipsoidAssociatedWithDatum(c�digoDatum));
		UpdateData(FALSE);

		ActivaDesactivaBot�nAceptar();
	}

	void CDialogoBuscarScrHorizontal::OnBnClickedOk()
	{
		UpdateData();
		m_nSCR = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));
		OnOK();
	}

	void CDialogoBuscarScrHorizontal::OnBnClickedBuscarEpsg()
	{
		CDialogoB�squedaPorC�digoEpsg dlg;
		dlg.m_nC�digo = m_nSCR;
				
		if( IDOK == dlg.DoModal() ) {
			m_nSCR = dlg.m_nC�digo;
			RellenaCamposEnFunci�nScrActivo();
			ActivaDesactivaBot�nAceptar();
		}
	}

	void CDialogoBuscarScrHorizontal::ActivaDesactivaBot�nAceptar() const
	{
		GetDlgItem(IDOK)->EnableWindow(-1 != m_wndCRS.GetCurSel());
	}

	void CDialogoBuscarScrHorizontal::RellenaCamposEnFunci�nScrActivo()
	{
		auto const fabricaSrc = GetCoordinateSystemAuthorityFactory();

		CStringW const tipoSCR = fabricaSrc->GetKindOfCoordinateSystem(m_nSCR);

		if(0 == tipoSCR.CompareNoCase(L"projected"))
			m_wndTiposCRS.SetCurSel(0);
		else if(0 == tipoSCR.CompareNoCase(L"geographic 2D"))
			m_wndTiposCRS.SetCurSel(1);
		else
			m_wndTiposCRS.SetCurSel(-1);

		OnCbnSelchangeComboTiposCrs();

		for (auto i=0; i<m_wndCRS.GetCount(); i++ ) {
			if( static_cast<int>(m_wndCRS.GetItemData(i)) == m_nSCR ) {
				m_wndCRS.SetCurSel(i);
				OnCbnSelchangeComboCrs();
				break;
			}
		}
	}

}
