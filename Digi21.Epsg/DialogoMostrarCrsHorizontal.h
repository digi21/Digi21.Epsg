#pragma once

namespace Digi21::Geodesia::EPSG
{

	// Cuadro de diálogo de CDialogoMostrarCrsHorizontal
	class CDialogoMostrarCrsHorizontal final : public CDialog
	{
		enum { IDD = IDD_DIALOGO_MOSTRAR_PARAMETROS_SCR_HORIZONTAL };
		std::shared_ptr<OpenGis::CoordinateSystems::HorizontalCoordinateSystem> _sistemaCoordenadas;
		CXTPComboBox m_wndTiposCRS;
		CXTPComboBox m_wndCRS;
		CStringW m_sArea;
		CStringW m_sTipoSistemaCoordenadas;
		CStringW m_sNombreDatum;
		CStringW m_sOrigenDatum;
		CStringW m_sMeridiano;
		CStringW m_sAmbitoUso;
		CStringW m_sElipsoide;

	public:
		explicit CDialogoMostrarCrsHorizontal(std::shared_ptr<OpenGis::CoordinateSystems::HorizontalCoordinateSystem> const& sistemaCoordenadas, CWnd* pParent = nullptr);

	private:
		void DoDataExchange(CDataExchange* pDX) override;

		BOOL OnInitDialog() override;
		DECLARE_MESSAGE_MAP()

		afx_msg void OnBnClickedOk();
		void RellenaCamposEnFunciónScrActivo();
		void OnCbnSelchangeComboTiposCrs();
		void OnCbnSelchangeComboCrs();
	};

}
