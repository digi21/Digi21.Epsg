#include "pch.h"
#include "Digi21.Epsg.h"
#include "DialogoSeleccionTransformacionVertical.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogoSeleccionTransformacionVertical, CDialogEx)

CDialogoSeleccionTransformacionVertical::CDialogoSeleccionTransformacionVertical(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
}

void CDialogoSeleccionTransformacionVertical::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SISTEMA_COORDENADAS_ORIGEN, m_sTituloScrOrigen);
	DDX_Text(pDX, IDC_EDIT_SISTEMA_COORDENADAS_ORIGEN2, m_sTituloScrDestino);
	DDX_Text(pDX, IDC_EDIT_DESPLAZAMIENTO_VERTICAL, m_dValor);
	DDX_Check(pDX, IDC_CHECK_UTILIZAR_SIEMPRE_ESTA_TRANSFORMACION, m_bUtilizarSiempreEstaTransf);
}

BEGIN_MESSAGE_MAP(CDialogoSeleccionTransformacionVertical, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_SCR_VERTICAL_IDENTICOS, &CDialogoSeleccionTransformacionVertical::OnBnClickedRadioScrVerticalIdenticos)
	ON_BN_CLICKED(IDC_RADIO_DESPLAZAMIENTO_VERTICAL, &CDialogoSeleccionTransformacionVertical::OnBnClickedRadioDesplazamientoVertical)
	ON_BN_CLICKED(IDOK, &CDialogoSeleccionTransformacionVertical::OnBnClickedOk)
END_MESSAGE_MAP()

void CDialogoSeleccionTransformacionVertical::OnBnClickedRadioScrVerticalIdenticos()
{
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_VALOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DESPLAZAMIENTO_VERTICAL)->EnableWindow(FALSE);
}

void CDialogoSeleccionTransformacionVertical::OnBnClickedRadioDesplazamientoVertical()
{
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_VALOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DESPLAZAMIENTO_VERTICAL)->EnableWindow(TRUE);
}

void CDialogoSeleccionTransformacionVertical::OnBnClickedOk()
{
	UpdateData();

	if( static_cast<CButton*>(GetDlgItem(IDC_RADIO_SCR_VERTICAL_IDENTICOS))->GetCheck() ) {
		m_dValor = 0.0;
		UpdateData(FALSE);
	}

	CDialogEx::OnOK();
}
