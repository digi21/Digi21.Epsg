#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CPaginaSeleccionScrProyectadoYVertical final : public CXTPPropertyPage
	{
		DECLARE_DYNAMIC(CPaginaSeleccionScrProyectadoYVertical)

#pragma region Campos privados
		CXTPPropertyPage*& current;
		CXTPComboBox m_wndCrsHorizontal;
		int m_nScrHorizontal{ 32630 };

		CXTPComboBox m_wndCrsVertical;
		int m_nScrVertical{ 3855 };
		BOOL m_bVerticalDesconocido{};

		int m_nOrientaciónEjes{};
#pragma endregion

#pragma region Constructores y destructor
	public:
		explicit CPaginaSeleccionScrProyectadoYVertical(CXTPPropertyPage*& current);

		// Dialog Data
		enum { IDD = IDD_PAGINA_SELECCION_PROYECTADO_VERTICAL };
#pragma endregion

#pragma region Propiedades
		CStringW GetWkt() const;
		__declspec(property(get=GetWkt)) CStringW Wkt;
#pragma endregion

#pragma region Métodos
	protected:
		void DoDataExchange(CDataExchange* pDX) override;

		BOOL OnInitDialog() override;

		void RellenaScrGeográficos();
		void RellenaScrVerticales();

		BOOL OnSetActive() override;

		void OnCbnSelchangeComboCrsHorizontal();
		void OnBnClickedBuscarHorizontalEpsg();


		void OnCbnSelchangeComboCrsVertical();
		void OnBnClickedBuscarVerticalEpsg();

		void OnBnClickedCheckVerticalDesconocido();
		void SeleccionaVertical( int código );
		void SeleccionaHorizontal( int ódigo);

		void OnBnClickedBotonExportarSistemaCoordenadas();
		void OnBnClickedBotonMemorizarSistemaCoordenadas();

		afx_msg void OnBnClickedBotonCopiar();
		DECLARE_MESSAGE_MAP()
#pragma endregion
	};

}
