#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CPaginaSeleccionScrMemorizados final : public CXTPPropertyPage
	{
		DECLARE_DYNAMIC(CPaginaSeleccionScrMemorizados)

#pragma region Campos privados
		CXTPPropertyPage*& current;

		CStringW m_sWkt;
		CXTPListBox m_wndListaSistemasReferenciaMemorizados;
#pragma endregion

#pragma region Constructores y destructor
	public:
		explicit CPaginaSeleccionScrMemorizados(CXTPPropertyPage*& current);

		// Dialog Data
		enum { IDD = IDD_PAGINA_SELECCION_SCR_MEMORIZADOS };
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

		// Horizontal
		void RellenaListaMemorizados();
		void ActivaControlesSistema(BOOL activar) const;
		afx_msg void OnBnClickedButtonEliminarSistemaCoordenadas();
		afx_msg void OnSelchangeListaSistemasCoordenadasMemorizados();
		void OnBnClickedBotonExportarSistemaCoordenadas();
		DECLARE_MESSAGE_MAP()
#pragma endregion
	};

}
