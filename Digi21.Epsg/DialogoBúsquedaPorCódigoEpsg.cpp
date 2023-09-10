#include "pch.h"
#include "Digi21.Epsg.h"
#include "DialogoB�squedaPorC�digoEpsg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogoB�squedaPorC�digoEpsg, CDialogEx)

CDialogoB�squedaPorC�digoEpsg::CDialogoB�squedaPorC�digoEpsg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
}

void CDialogoB�squedaPorC�digoEpsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nC�digo);
}

BEGIN_MESSAGE_MAP(CDialogoB�squedaPorC�digoEpsg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogoB�squedaPorC�digoEpsg::OnBnClickedOk)
END_MESSAGE_MAP()

void CDialogoB�squedaPorC�digoEpsg::OnBnClickedOk()
{
	UpdateData();
	CDialogEx::OnOK();
}
