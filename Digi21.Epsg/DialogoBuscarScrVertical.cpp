#include "pch.h"
#include "Digi21.Epsg.h"
#include "DialogoBuscarScrVertical.h"

using namespace Digi21::OpenGis;
using namespace Digi21::Utilidades;

#ifdef _DEBUG
#define new DEBUG_NEW  // NOLINT
#endif

namespace Digi21::Geodesia::EPSG
{

	CDialogoBuscarScrVertical::CDialogoBuscarScrVertical(int scrSeleccionadoPorDefecto, CWnd* pParent /*=nullptr*/)
		: CDialog(IDD, pParent)
		, m_nSCR(__max(SISTEMA_COORDENADAS_LOCAL, scrSeleccionadoPorDefecto))
	{
	}

	void CDialogoBuscarScrVertical::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_COMBO_CRS, m_wndCRS);
		DDX_Text(pDX, IDC_EDIT_AREA, m_sArea);
		DDX_Text(pDX, IDC_EDIT_TIPO_SISTEMA_COORDENADAS, m_sTipoSistemaCoordenadas);
		DDX_Text(pDX, IDC_EDIT_NOMBRE_DATUM, m_sNombreDatum);
		DDX_Text(pDX, IDC_EDIT_ORIGEN_DATUM, m_sOrigenDatum);
		DDX_Text(pDX, IDC_EDIT_AMBITO_USO, m_sAmbitoUso);
	}

	BEGIN_MESSAGE_MAP(CDialogoBuscarScrVertical, CDialog)
		ON_CBN_SELCHANGE(IDC_COMBO_CRS, &CDialogoBuscarScrVertical::OnCbnSelchangeComboCrs)
		ON_BN_CLICKED(IDOK, &CDialogoBuscarScrVertical::OnBnClickedOk)
	END_MESSAGE_MAP()


	BOOL CDialogoBuscarScrVertical::OnInitDialog()
	{
		CDialog::OnInitDialog();

		CWaitCursor wc;

		auto const listaSistemasCoordenadas = GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(OpenGis::CoordinateSystems::CoordinateSystemKind::Vertical);

		for (auto const& [codigo, nombre] : listaSistemasCoordenadas)
			m_wndCRS.SetItemData(m_wndCRS.AddString(nombre), codigo);

		if( SISTEMA_COORDENADAS_LOCAL != m_nSCR ) 
			RellenaCamposEnFunciónScrActivo();

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}

	void CDialogoBuscarScrVertical::OnCbnSelchangeComboCrs()
	{
		UpdateData();

		auto const códigoCrs = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));

		auto const fabricaSrc = GetCoordinateSystemAuthorityFactory();

		m_sArea = fabricaSrc->GetDescriptionAreaApplicationCrs(códigoCrs);
		m_sTipoSistemaCoordenadas = fabricaSrc->GetNameOfCoordinateSystem(fabricaSrc->GetCodeOfCoordinateSystemAssociatedWithCrs(códigoCrs));

		auto const códigoDatum = fabricaSrc->GetCodeOfDatumAssociatedWithCrs(códigoCrs);
		m_sNombreDatum = fabricaSrc->GetNameOfDatum(códigoDatum);
		m_sOrigenDatum = fabricaSrc->GetOriginDescriptionOfDatum(códigoDatum);
		m_sAmbitoUso = fabricaSrc->GetDescriptionAreaApplicationDatum(códigoDatum);
		UpdateData(FALSE);
	}

	void CDialogoBuscarScrVertical::OnBnClickedOk()
	{
		UpdateData();

		m_nSCR = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));

		OnOK();
	}

	void CDialogoBuscarScrVertical::RellenaCamposEnFunciónScrActivo()
	{
		for (auto i=0; i<m_wndCRS.GetCount(); i++ ) {
			if( static_cast<int>(m_wndCRS.GetItemData(i)) == m_nSCR ) {
				m_wndCRS.SetCurSel(i);
				OnCbnSelchangeComboCrs();
				break;
			}
		}
	}

}
