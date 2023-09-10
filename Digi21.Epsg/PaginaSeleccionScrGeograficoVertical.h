
#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CPaginaSeleccionScrGeograficoYVertical final : public CXTPPropertyPage
	{
		DECLARE_DYNAMIC(CPaginaSeleccionScrGeograficoYVertical)

#pragma region Campos privados
		CXTPPropertyPage*& current;
		CXTPComboBox m_wndCrsHorizontal;
		int m_nScrHorizontal{ 4326 };
		int m_nOrientaci�nEjes{};

		CXTPComboBox m_wndCrsVertical;
		int m_nScrVertical{ 3855 };
		BOOL m_bVerticalDesconocido{};
#pragma endregion

#pragma region Constructores y destructor
	public:
		explicit CPaginaSeleccionScrGeograficoYVertical(CXTPPropertyPage*& current);

		// Dialog Data
		enum { IDD = IDD_PAGINA_SELECCION_GEOGRAFICO_VERTICAL };
#pragma endregion

#pragma region Propiedades
		CStringW GetWkt() const;
		__declspec(property(get=GetWkt)) CStringW Wkt;
#pragma endregion

#pragma region M�todos
	protected:
		void DoDataExchange(CDataExchange* pDX) override;

		BOOL OnInitDialog() override;

		void RellenaScrGeogr�ficos();
		void RellenaScrVerticales();

		BOOL OnSetActive() override;

		void OnCbnSelchangeComboCrsHorizontal();
		void OnBnClickedBuscarHorizontalEpsg();

		void SeleccionaHorizontal( int c�digo );

		void OnBnClickedCheckVerticalDesconocido();
		void OnCbnSelchangeComboCrsVertical();
		void OnBnClickedBuscarVerticalEpsg();

		void SeleccionaVertical( int c�digo );

		void OnBnClickedBotonExportarSistemaCoordenadas();
		void OnBnClickedBotonMemorizarSistemaCoordenadas();
		afx_msg void OnBnClickedBotonCopiar();

		DECLARE_MESSAGE_MAP()
#pragma endregion
	};

}
