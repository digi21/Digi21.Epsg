#include "pch.h"
#include "Digi21.Epsg.h"
#include "DialogoBúsquedaPorCódigoEpsg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogoBúsquedaPorCódigoEpsg, CDialogEx)

CDialogoBúsquedaPorCódigoEpsg::CDialogoBúsquedaPorCódigoEpsg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
}

void CDialogoBúsquedaPorCódigoEpsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nCódigo);
}

BEGIN_MESSAGE_MAP(CDialogoBúsquedaPorCódigoEpsg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogoBúsquedaPorCódigoEpsg::OnBnClickedOk)
END_MESSAGE_MAP()

void CDialogoBúsquedaPorCódigoEpsg::OnBnClickedOk()
{
	UpdateData();
	CDialogEx::OnOK();
}
