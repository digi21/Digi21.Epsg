#pragma once

class CDialogoB�squedaPorC�digoEpsg final : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogoB�squedaPorC�digoEpsg)

public:
	explicit CDialogoB�squedaPorC�digoEpsg(CWnd* pParent = nullptr);

// Dialog Data
	enum { IDD = IDD_DIALOGO_BUSQUEDA_POR_CODIGO_EPSG };

protected:
	void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_nC�digo{};
};
