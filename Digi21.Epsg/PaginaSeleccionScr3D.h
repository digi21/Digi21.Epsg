#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CPaginaSeleccionScr3D final : public CXTPPropertyPage
	{
		DECLARE_DYNAMIC(CPaginaSeleccionScr3D)

#pragma region Campos privados
		CXTPPropertyPage*& current;
		CXTPComboBox m_wndCRS;
		int m_nSCR{4979};
		int m_nOrientaciónEjes{};
#pragma endregion

#pragma region Constructores y destructor
	public:
		explicit CPaginaSeleccionScr3D(CXTPPropertyPage*& current);

		// Dialog Data
		enum { IDD = IDD_PAGINA_SELECCION_SCR_3D };
#pragma endregion

#pragma region Propiedades
		CStringW GetWkt() const;
		__declspec(property(get=GetWkt)) CStringW Wkt;
#pragma endregion

#pragma region Métodos
	protected:
		void DoDataExchange(CDataExchange* pDX) override;

		BOOL OnInitDialog() override;
		BOOL OnSetActive() override;

		void OnCbnSelchangeComboCrs();
		void OnBnClickedBuscarEpsg();

		void SeleccionaCódigo( int código );

		void OnBnClickedBotonExportarSistemaCoordenadas();
		void OnBnClickedBotonMemorizarSistemaCoordenadas();
		afx_msg void OnBnClickedBotonCopiar();
		DECLARE_MESSAGE_MAP()
#pragma endregion
	};

}
