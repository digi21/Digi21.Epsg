#pragma once

namespace Digi21::Geodesia::EPSG
{

	class CDialogoSeleccionScrVertical final : public CXTPResizeDialog
	{
		DECLARE_DYNAMIC(CDialogoSeleccionScrVertical)

		CStringW m_sWkt;
		CXTPListBox m_wndListaSistemasReferenciaMemorizados;
		BOOL m_bDesconocido;
		CStringW título;
		CStringW títuloCheckboxDesconocido;

	public:
		explicit CDialogoSeleccionScrVertical(CStringW const& título, CStringW const& títuloCheckboxDesconocido, std::shared_ptr<OpenGis::CoordinateSystems::CoordinateSystem> const& sistemaCoordenadasDefecto, CWnd* pParent = nullptr);

		// Dialog Data
		enum { IDD = IDD_DIALOGO_SELECCION_SCR_VERTICAL };

	protected:
		void DoDataExchange(CDataExchange* pDX) override;

		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedBotonGuardarSistemaCoordenadas();
		afx_msg void OnBnClickedButtonEliminarSistemaCoordenadas();
		afx_msg void OnBnClickedButtonImportar();
		afx_msg void OnBnClickedButtonExportar();
		afx_msg void OnBnClickedOk();
		afx_msg void OnSelchangeListaSistemasCoordenadasMemorizados();
		afx_msg void OnBnClickedBotonBuscarSistemaCoordenadas();
		afx_msg void OnBnClickedCheckDesconocido();
		afx_msg void OnDestroy();
		BOOL OnInitDialog() override;

		void ActivaControlesSistemaConocido(BOOL activar) const;
	};

}
