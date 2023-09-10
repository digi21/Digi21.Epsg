#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CDialogoBuscarScrVertical final : public CDialog
	{
		enum {IDD = IDD_DIALOGO_BUSCAR_SCR_VERTICAL};

		CXTPComboBox m_wndCRS;
		CStringW m_sArea;
		CStringW m_sTipoSistemaCoordenadas;
		CStringW m_sNombreDatum;
		CStringW m_sOrigenDatum;
		CStringW m_sAmbitoUso;
		int m_nSCR;

		// Construcción
	public:
		explicit CDialogoBuscarScrVertical(int scrSeleccionadoPorDefecto, CWnd* pParent = nullptr);

	public:
		__declspec(property(get=GetSCR)) int SistemaReferenciaCoordenadas;
		int GetSCR() const { return m_nSCR; }

	private:
		void DoDataExchange(CDataExchange* pDX) override; // Compatibilidad con DDX/DDV
		BOOL OnInitDialog() override;
		DECLARE_MESSAGE_MAP()

		afx_msg void OnBnClickedOk();
		afx_msg void OnCbnSelchangeComboCrs();

		void RellenaCamposEnFunciónScrActivo();
	};

}
