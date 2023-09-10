#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CDialogoMostrarScrVertical final : public CDialog
	{
		enum {IDD = IDD_DIALOGO_MOSTRAR_PARAMETROS_SCR_VERTICAL};

		std::shared_ptr<OpenGis::CoordinateSystems::VerticalCoordinateSystem> _sistemaCoordenadas;
		CXTPComboBox m_wndCRS;
		CStringW m_sArea;
		CStringW m_sTipoSistemaCoordenadas;
		CStringW m_sNombreDatum;
		CStringW m_sOrigenDatum;
		CStringW m_sAmbitoUso;

		// Construcción
	public:
		explicit CDialogoMostrarScrVertical(std::shared_ptr<OpenGis::CoordinateSystems::VerticalCoordinateSystem> const& sistemaCoordenadas, CWnd* pParent = nullptr);

	private:
		void DoDataExchange(CDataExchange* pDX) override; // Compatibilidad con DDX/DDV
		BOOL OnInitDialog() override;
		DECLARE_MESSAGE_MAP()

		afx_msg void OnBnClickedOk();
		afx_msg void OnCbnSelchangeComboCrs();
	};

}
