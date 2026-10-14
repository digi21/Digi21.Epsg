#include "pch.h"
#include "Digi21.Epsg.h"
#include "PaginaManual.h"

using namespace Digi21::Utilidades;
using namespace CrsKit::CoordinateSystems;
using namespace CrsKit;
using namespace std;

IMPLEMENT_DYNAMIC(PaginaManual, PaginaAdaptadaAlTema)

PaginaManual::PaginaManual(CXTPPropertyPage*& current)
	: PaginaAdaptadaAlTema(IDD_PAGINA_MANUAL)
	, current{ current }
{
}

void PaginaManual::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WKT, _wkt);
}

BEGIN_MESSAGE_MAP(PaginaManual, PaginaAdaptadaAlTema)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTAR, &PaginaManual::OnBnClickedButtonBuscarArchivo)
	ON_BN_CLICKED(IDC_BOTON_MEMORIZAR_SISTEMA_COORDENADAS_HORIZONTAL, &PaginaManual::OnBnClickedBotonMemorizarSistemaCoordenadas)
END_MESSAGE_MAP()

BOOL PaginaManual::OnSetActive()
{
	current = this;
	return TRUE;
}

BOOL PaginaManual::OnInitDialog()
{
	PaginaAdaptadaAlTema::OnInitDialog();

	SetResize(IDC_EDIT_WKT, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_BUTTON_IMPORTAR, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_BOTON_MEMORIZAR_SISTEMA_COORDENADAS_HORIZONTAL, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);

	return TRUE;
}

void PaginaManual::OnBnClickedButtonBuscarArchivo()
{
	CFileDialog dlg(TRUE, L"*.prj",
		nullptr,
		0,
		_G(IDS_STRING2007),
		this);
	if (IDOK == dlg.DoModal()) {
		CFileException e;
		CStdioFile is;
		if (!is.Open(dlg.GetPathName(), CFile::modeRead, &e)) {
			wchar_t szError[1024];
			e.GetErrorMessage(szError, 1024);

			AfxMessageBox(UtilidadesString::Format(_G(IDS_STRING2013), static_cast<LPCWSTR>(dlg.GetPathName()), szError));
			return;
		}

		CString wkt;
		is.ReadString(wkt);

		try {
			CWaitCursor wc;
			std::ignore = GetCoordinateSystemFactory()->CreateFromWkt(AUtf8(wkt));
		}
		catch (runtime_error& e)
		{
			AfxMessageBox(DeUtf8(e.what()));
			return;
		}

		_wkt = wkt;
		UpdateData(FALSE);
	}
}

void PaginaManual::OnBnClickedBotonMemorizarSistemaCoordenadas()
{
	UpdateData();

	shared_ptr<CoordinateSystem> sistemaCooordenadas;
	try {
		CWaitCursor wc;
		sistemaCooordenadas = GetCoordinateSystemFactory()->CreateFromWkt(AUtf8(_wkt));
	}
	catch (...)
	{
		return;
	}

	CRegistro::SetStringCurrentUser(
		CARPETA_DIGI21,
		VERSION_DIGI3D,
		CARPETA_REGISTRO_DIGI3D_MEMORIZED_COORDINATE_SYSTEMS,
		DeUtf8(sistemaCooordenadas->GetName()),
		DeUtf8(sistemaCooordenadas->GetWkt()));
}
