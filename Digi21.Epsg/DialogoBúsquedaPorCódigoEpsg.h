#pragma once

#include "DialogoAdaptadoAlTema.h"

class CDialogoBúsquedaPorCódigoEpsg final : public DialogoAdaptadoAlTema<CDialogEx>
{
	DECLARE_DYNAMIC(CDialogoBúsquedaPorCódigoEpsg)

public:
	explicit CDialogoBúsquedaPorCódigoEpsg(CWnd* pParent = nullptr);

// Dialog Data
	enum { IDD = IDD_DIALOGO_BUSQUEDA_POR_CODIGO_EPSG };

protected:
	void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_nCódigo{};
};
