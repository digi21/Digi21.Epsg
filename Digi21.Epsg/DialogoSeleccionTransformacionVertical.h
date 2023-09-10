#pragma once

class CDialogoSeleccionTransformacionVertical final : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogoSeleccionTransformacionVertical)

public:
	explicit CDialogoSeleccionTransformacionVertical(CWnd* pParent = nullptr);

// Dialog Data
	enum { IDD = IDD_DIALOGO_SELECCIONA_TRANSFORMACION_VERTICAL };

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	afx_msg void OnBnClickedRadioScrVerticalIdenticos();
	afx_msg void OnBnClickedRadioDesplazamientoVertical();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
public:
	CStringW m_sTituloScrOrigen;
	CStringW m_sTituloScrDestino;
	double m_dValor{};
	BOOL m_bUtilizarSiempreEstaTransf{};
};
