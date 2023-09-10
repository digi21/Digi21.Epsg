#include "pch.h"
#include "resource.h"
#include "DialogoBuscarScr3D.h"
#include "DialogoBúsquedaPorCódigoEpsg.h"

using namespace Digi21::Utilidades;
using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;

#ifdef _DEBUG
#define new DEBUG_NEW  // NOLINT
#endif

namespace Digi21::Geodesia::EPSG
{
	CDialogoBuscarScr3D::CDialogoBuscarScr3D(int scrSeleccionadoPorDefecto, CWnd* pParent /*=nullptr*/)
		: CDialog(IDD, pParent)
		, m_nSCR{ scrSeleccionadoPorDefecto }
	{
	}

	void CDialogoBuscarScr3D::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_COMBO_CRS, m_wndCRS);
		DDX_Text(pDX, IDC_EDIT_AREA, m_sArea);
		DDX_Text(pDX, IDC_EDIT_TIPO_SISTEMA_COORDENADAS, m_sTipoSistemaCoordenadas);
		DDX_Text(pDX, IDC_EDIT_NOMBRE_DATUM, m_sNombreDatum);
		DDX_Text(pDX, IDC_EDIT_ORIGEN_DATUM, m_sOrigenDatum);
		DDX_Text(pDX, IDC_EDIT_MERIDIANO, m_sMeridiano);
		DDX_Text(pDX, IDC_EDIT_AMBITO_USO, m_sAmbitoUso);
		DDX_Text(pDX, IDC_EDIT_NOMBRE_ELIPSOIDE, m_sElipsoide);
	}

	BEGIN_MESSAGE_MAP(CDialogoBuscarScr3D, CDialog)
		ON_CBN_SELCHANGE(IDC_COMBO_CRS, &CDialogoBuscarScr3D::OnCbnSelchangeComboCrs)
		ON_BN_CLICKED(IDC_BUTTON_BUSCAR_EPSG, &CDialogoBuscarScr3D::OnBnClickedBuscarEpsg)
		ON_BN_CLICKED(IDOK, &CDialogoBuscarScr3D::OnBnClickedOk)
	END_MESSAGE_MAP()


	// Controladores de mensaje de CDialogoBuscarScr3D

	BOOL CDialogoBuscarScr3D::OnInitDialog()
	{
		CDialog::OnInitDialog();

		m_wndCRS.ResetContent();
		CWaitCursor wc;

		auto const listaSistemasCoordenadas = GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(CoordinateSystemKind::Geographic3D);

		for(auto const& [codigo, nombre] : listaSistemasCoordenadas )
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

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}

	void CDialogoBuscarScr3D::OnCbnSelchangeComboCrs()
	{
		UpdateData();

		auto const códigoCRS = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));

		auto const fabricaSrc = GetCoordinateSystemAuthorityFactory();

		m_sArea = fabricaSrc->GetDescriptionAreaApplicationCrs(códigoCRS);
		m_sTipoSistemaCoordenadas = fabricaSrc->GetNameOfCoordinateSystem(fabricaSrc->GetCodeOfCoordinateSystemAssociatedWithCrs(códigoCRS));

		auto const códigoDatum = fabricaSrc->GetCodeOfDatumAssociatedWithCrs(códigoCRS);
		m_sNombreDatum = fabricaSrc->GetNameOfDatum(códigoDatum);
		m_sOrigenDatum = fabricaSrc->GetOriginDescriptionOfDatum(códigoDatum);
		m_sMeridiano = fabricaSrc->GetNameOfPrimeMeridian(fabricaSrc->GetCodeOfPrimeMeridianAssociatedWithCrs(códigoDatum));
		m_sAmbitoUso = fabricaSrc->GetDescriptionAreaApplicationDatum(códigoDatum);
		m_sElipsoide = fabricaSrc->GetNameOfEllipsoid(fabricaSrc->GetCodeOfEllipsoidAssociatedWithDatum(códigoDatum));
		UpdateData(FALSE);
	}

	void CDialogoBuscarScr3D::OnBnClickedOk()
	{
		UpdateData();
		m_nSCR = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));
		OnOK();
	}

	void CDialogoBuscarScr3D::OnBnClickedBuscarEpsg()
	{
		CDialogoBúsquedaPorCódigoEpsg dlg;
		dlg.m_nCódigo = m_nSCR;

		auto const fabricaSrc = GetCoordinateSystemAuthorityFactory();

		if( IDOK == dlg.DoModal() ) {
			if(3 != fabricaSrc->GetAxisOfCoordinateSystem(fabricaSrc->GetCodeOfCoordinateSystemAssociatedWithCrs(dlg.m_nCódigo)).size() )
				AfxMessageBox(_G(IDS_STRING2019));
			else {
				for (auto i=0; i<m_wndCRS.GetCount(); i++ ) {
					if( static_cast<int>(m_wndCRS.GetItemData(i)) == dlg.m_nCódigo ) {
						m_wndCRS.SetCurSel(i);
						OnCbnSelchangeComboCrs();
						break;
					}
				}

			}
		}
	}

}
