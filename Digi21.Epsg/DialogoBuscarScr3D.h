#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CDialogoBuscarScr3D final : public CDialog
	{
		enum { IDD = IDD_DIALOGO_BUSCAR_SCR_3D };
		int m_nSCR;
		CXTPComboBox m_wndCRS;
		CStringW m_sArea;
		CStringW m_sTipoSistemaCoordenadas;
		CStringW m_sNombreDatum;
		CStringW m_sOrigenDatum;
		CStringW m_sMeridiano;
		CStringW m_sAmbitoUso;
		CStringW m_sElipsoide;

	public:
		explicit CDialogoBuscarScr3D(int scrSeleccionadoPorDefecto, CWnd* pParent = nullptr);

	public:
		__declspec(property(get=GetSCR)) int SistemaReferenciaCoordenadas;
		int GetSCR() const { return m_nSCR; }

	private:
		BOOL OnInitDialog() override;
		DECLARE_MESSAGE_MAP()

		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedBuscarEpsg();
		afx_msg void OnCbnSelchangeComboCrs();
		void DoDataExchange(CDataExchange* pDX) override;
	};

}
