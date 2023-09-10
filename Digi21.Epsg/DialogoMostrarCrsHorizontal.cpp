#include "pch.h"
#include "resource.h"
#include "DialogoMostrarCrsHorizontal.h"

using namespace std;
using namespace Digi21::Utilidades;
using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis;

#ifdef _DEBUG
#define new DEBUG_NEW  // NOLINT
#endif

namespace Digi21::Geodesia::EPSG
{


	// Cuadro de diálogo de CDialogoMostrarCrsHorizontal
	CDialogoMostrarCrsHorizontal::CDialogoMostrarCrsHorizontal(std::shared_ptr<HorizontalCoordinateSystem> const& sistemaCoordenadas, CWnd* pParent /*=nullptr*/)
		: CDialog(IDD, pParent)
		, _sistemaCoordenadas{ sistemaCoordenadas }
	{
	}

	void CDialogoMostrarCrsHorizontal::DoDataExchange(CDataExchange* pDX)
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

	BEGIN_MESSAGE_MAP(CDialogoMostrarCrsHorizontal, CDialog)
		ON_BN_CLICKED(IDOK, &CDialogoMostrarCrsHorizontal::OnBnClickedOk)
	END_MESSAGE_MAP()


	BOOL CDialogoMostrarCrsHorizontal::OnInitDialog()
	{
		CDialog::OnInitDialog();

		m_wndTiposCRS.AddString(_G(IDS_STRING2004));
		m_wndTiposCRS.AddString(_G(IDS_STRING2005));
		m_wndTiposCRS.AddString(_G(IDS_STRING2006));
		m_wndTiposCRS.SetCurSel(0);

		RellenaCamposEnFunciónScrActivo();

		m_wndCRS.EnableWindow(FALSE);
		m_wndTiposCRS.EnableWindow(FALSE);

		return TRUE; // Devuelve TRUE  a menos que establezca el foco en un control
	}


	void CDialogoMostrarCrsHorizontal::RellenaCamposEnFunciónScrActivo()
	{
		if( nullptr != dynamic_pointer_cast<LocalCoordinateSystem>(_sistemaCoordenadas))
			m_wndTiposCRS.SetCurSel(0);
		else if( nullptr != dynamic_pointer_cast<ProjectedCoordinateSystem>(_sistemaCoordenadas))
			m_wndTiposCRS.SetCurSel(1);
		else if( nullptr != dynamic_pointer_cast<GeographicCoordinateSystem>(_sistemaCoordenadas))
			m_wndTiposCRS.SetCurSel(2);
		else
			m_wndTiposCRS.SetCurSel(-1);

		OnCbnSelchangeComboTiposCrs();
		OnCbnSelchangeComboCrs();
	}

	void CDialogoMostrarCrsHorizontal::OnCbnSelchangeComboTiposCrs()
	{
		m_wndCRS.ResetContent();

		if( 0 == m_wndTiposCRS.GetCurSel() ) 
			return;

		if( 0 == _sistemaCoordenadas->GetAuthority().CompareNoCase(L"EPSG")) {
			CStringW tipoSistemaCoordenadas;

			switch(m_wndTiposCRS.GetCurSel())
			{
			case 1:
				tipoSistemaCoordenadas = CoordinateSystemKind::Projected;
				break;
			case 2:
				tipoSistemaCoordenadas = CoordinateSystemKind::Geographic2D;
				break;
			}

			auto const listaSistemasCoordenadas = GetCoordinateSystemAuthorityFactory()->EnumerateCoordinateSystems(tipoSistemaCoordenadas);

			CWaitCursor wc;
			for (auto const& [codigo, nombre] : listaSistemasCoordenadas)
				m_wndCRS.SetItemData(m_wndCRS.AddString(nombre), codigo);

			for (auto i=0; i<m_wndCRS.GetCount(); i++ ) {
				if( static_cast<int>(m_wndCRS.GetItemData(i)) == _sistemaCoordenadas->GetAuthorityCode()) {
					m_wndCRS.SetCurSel(i);
					break;
				}
			}
		} else {
			m_wndCRS.AddString(_sistemaCoordenadas->GetName());
			m_wndCRS.SetCurSel(0);
		}
	}

	void CDialogoMostrarCrsHorizontal::OnCbnSelchangeComboCrs()
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
			m_sMeridiano = fabricaSrc->GetNameOfPrimeMeridian(fabricaSrc->GetCodeOfPrimeMeridianAssociatedWithCrs(códigoDatum));
			m_sAmbitoUso = fabricaSrc->GetDescriptionAreaApplicationDatum(códigoDatum);
			m_sElipsoide = fabricaSrc->GetNameOfEllipsoid(fabricaSrc->GetCodeOfEllipsoidAssociatedWithDatum(códigoDatum));
		} else {
			m_sArea = L"";
			m_sTipoSistemaCoordenadas = L"";
			m_sOrigenDatum = L"";

			auto const geo = dynamic_pointer_cast<GeographicCoordinateSystem>(_sistemaCoordenadas);
			auto const proj = dynamic_pointer_cast<ProjectedCoordinateSystem>(_sistemaCoordenadas);
			if( geo != nullptr ) {
				m_sMeridiano = dynamic_pointer_cast<IInfo>(geo->GetPrimeMeridian())->GetName();
				m_sNombreDatum = dynamic_pointer_cast<IInfo>(geo->GetHorizontalDatum())->GetName();
				m_sElipsoide = dynamic_pointer_cast<IInfo>(_sistemaCoordenadas->GetHorizontalDatum()->GetEllipsoid())->GetName();
			} else {
				m_sMeridiano = dynamic_pointer_cast<IInfo>(proj->GetGeographicCoordinateSystem()->GetPrimeMeridian())->GetName();
				m_sNombreDatum = dynamic_pointer_cast<IInfo>(proj->GetGeographicCoordinateSystem()->GetHorizontalDatum())->GetName();
				m_sElipsoide = dynamic_pointer_cast<IInfo>(proj->GetGeographicCoordinateSystem()->GetHorizontalDatum()->GetEllipsoid())->GetName();
			}
			m_sAmbitoUso = L"";
		}

		UpdateData(FALSE);
	}

	void CDialogoMostrarCrsHorizontal::OnBnClickedOk()
	{
		OnOK();
	}

}
