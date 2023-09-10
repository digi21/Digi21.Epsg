#include "pch.h"
#include "Digi21.Epsg.h"
#include "DialogoMostrarScrVertical.h"

using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;
using namespace Digi21::Utilidades;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW  // NOLINT
#endif

namespace Digi21::Geodesia::EPSG
{

	CDialogoMostrarScrVertical::CDialogoMostrarScrVertical(shared_ptr<VerticalCoordinateSystem> const& sistemaCoordenadas, CWnd* pParent /*=nullptr*/)
		: CDialog(IDD, pParent)
		, _sistemaCoordenadas{ sistemaCoordenadas }
	{
	}

	void CDialogoMostrarScrVertical::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_COMBO_CRS, m_wndCRS);
		DDX_Text(pDX, IDC_EDIT_AREA, m_sArea);
		DDX_Text(pDX, IDC_EDIT_TIPO_SISTEMA_COORDENADAS, m_sTipoSistemaCoordenadas);
		DDX_Text(pDX, IDC_EDIT_NOMBRE_DATUM, m_sNombreDatum);
		DDX_Text(pDX, IDC_EDIT_ORIGEN_DATUM, m_sOrigenDatum);
		DDX_Text(pDX, IDC_EDIT_AMBITO_USO, m_sAmbitoUso);
	}

	BEGIN_MESSAGE_MAP(CDialogoMostrarScrVertical, CDialog)
		ON_CBN_SELCHANGE(IDC_COMBO_CRS, &CDialogoMostrarScrVertical::OnCbnSelchangeComboCrs)
		ON_BN_CLICKED(IDOK, &CDialogoMostrarScrVertical::OnBnClickedOk)
	END_MESSAGE_MAP()

	BOOL CDialogoMostrarScrVertical::OnInitDialog()
	{
		CDialog::OnInitDialog();

		CWaitCursor wc;

		auto const listaSistemasCoordenadas = GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(CoordinateSystemKind::Vertical);

		for (auto const& [codigo, nombre] : listaSistemasCoordenadas)
			m_wndCRS.SetItemData(m_wndCRS.AddString(nombre), codigo);

		for (auto i=0; i<m_wndCRS.GetCount(); i++ ) {
			if( static_cast<int>(m_wndCRS.GetItemData(i)) == _sistemaCoordenadas->GetAuthorityCode()) {
				m_wndCRS.SetCurSel(i);
				break;
			}
		}
		m_wndCRS.EnableWindow(FALSE);

		OnCbnSelchangeComboCrs();

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}

	void CDialogoMostrarScrVertical::OnCbnSelchangeComboCrs()
	{
		UpdateData();

		if( 0 == _sistemaCoordenadas->GetAuthority().CompareNoCase(L"EPSG")) {
			auto const códigoCRS = static_cast<int>(m_wndCRS.GetItemData(m_wndCRS.GetCurSel()));

			auto const fabricaSrc = GetCoordinateSystemAuthorityFactory();

			m_sArea = fabricaSrc->GetDescriptionAreaApplicationCrs(códigoCRS);
			m_sTipoSistemaCoordenadas = fabricaSrc->GetNameOfCoordinateSystem(fabricaSrc->GetCodeOfCoordinateSystemAssociatedWithCrs(códigoCRS));

			auto const códigoDatum = fabricaSrc->GetCodeOfDatumAssociatedWithCrs(códigoCRS);
			m_sNombreDatum = fabricaSrc->GetNameOfDatum(códigoDatum);
			m_sOrigenDatum = fabricaSrc->GetOriginDescriptionOfDatum(códigoDatum);
			m_sAmbitoUso = fabricaSrc->GetDescriptionAreaApplicationDatum(códigoDatum);
		} else {
			m_sArea = L"";
			m_sTipoSistemaCoordenadas = L"";
			m_sNombreDatum = dynamic_pointer_cast<IInfo>(_sistemaCoordenadas->GetVerticalDatum())->GetName();
			m_sOrigenDatum = L"";
			m_sAmbitoUso = L"";
		}
		UpdateData(FALSE);
	}

	void CDialogoMostrarScrVertical::OnBnClickedOk()
	{
		OnOK();
	}

}
