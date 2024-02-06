#include "pch.h"
#include "EpsgManager.h"
#include "Resource.h"
#include "DialogoSeleccionScrHorizontal.h"
#include "DialogoSeleccionScrVertical.h"
#include "DialogoMostrarCrsHorizontal.h"
#include "DialogoMostrarScrVertical.h"
#include "DialogoSeleccionTransformacionVertical.h"
#include "DialogoSeleccionScr.h"

using namespace Digi21::OpenGis::CoordinateSystems;
using namespace Digi21::OpenGis::CoordinateTransformations;
using namespace Digi21::Geodesia::EPSG;
using namespace std;

#pragma warning(disable:4947)

namespace Digi21::OpenGis::Epsg
{
	class TransformacionesMemorizadas
	{
	public:
		static void AlmacenaTransformaci�n(CStringW const& origen, CStringW const& destino, CStringW const& wkt)
		{
			CRegistro::SetStringCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"wkt", wkt);
		}

		static std::shared_ptr<IMathTransform> LocalizaTransformaci�n(CStringW const& origen, CStringW const& destino)
		{
			auto const wkt = CRegistro::GetStringCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"wkt");

			if (0 == wkt.GetLength())
				return nullptr;

			try {
				return GetMathTransformFactory()->CreateFromWkt(wkt);
			}
			catch (...)
			{
				return nullptr;
			}
		}

		static void AlmacenaC�digoTransformaci�n(CStringW const& origen, CStringW const& destino, int c�digo)
		{
			CRegistro::SetIntCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"C�digo2", c�digo);
		}

		static int LocalizaC�digoTransformaci�n(CStringW const& origen, CStringW const& destino)
		{
			return CRegistro::GetIntCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"C�digo2");
		}
	};

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogoSeleccionScr::DialogSeleccionaScr(
			title,
			titleUnknownCoordinateSystemCheckbox,
			GetCoordinateSystemFactory()->CreateCompoundUnknown());
	}

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, shared_ptr<CoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogoSeleccionScr::DialogSeleccionaScr(
			title,
			coordinateSystem);
	}

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, int defaultHorizontalCrs, int defaultVerticalCrs)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogSelectCrs(
			title,
			titleUnknownCoordinateSystemCheckbox,
			GetCoordinateSystemAuthorityFactory()->CreateCompoundCoordinateSystem(defaultHorizontalCrs, defaultVerticalCrs));
	}

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, CStringW const& wkt)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogoSeleccionScr::DialogSeleccionaScr(
			title,
			CStringW(titleUnknownCoordinateSystemCheckbox),
			GetCoordinateSystemFactory()->CreateFromWkt(wkt));
	}

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, shared_ptr<CoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogoSeleccionScr::DialogSeleccionaScr(
			title,
			CStringW(titleUnknownCoordinateSystemCheckbox),
			coordinateSystem);
	}

	CStringW EpsgManager::DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, shared_ptr<CoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CDialogoSeleccionScrHorizontal dlg(title, CStringW(titleUnknownCoordinateSystemCheckbox), coordinateSystem);

		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };

		if (IDOK == dlg.DoModal())
			return dlg.m_sWkt;

		return coordinateSystem->GetWkt();
	}

	CStringW EpsgManager::DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox)
	{
		return DialogSelectHorizontalCrs(
			title,
			titleUnknownCoordinateSystemCheckbox,
			_G(IDS_STRING2014));
	}

	CStringW EpsgManager::DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, int defaultCrs)
	{
		return DialogSelectHorizontalCrs(
			title,
			titleUnknownCoordinateSystemCheckbox,
			GetCoordinateSystemAuthorityFactory()->CreateCoordinateSystem(defaultCrs));
	}

	CStringW EpsgManager::DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, CStringW const& wkt)
	{
		return DialogSelectHorizontalCrs(
			title,
			titleUnknownCoordinateSystemCheckbox,
			GetCoordinateSystemFactory()->CreateFromWkt(wkt));
	}

	void EpsgManager::DialogShowCrs(shared_ptr<CoordinateSystem> const& coordinateSystem)
	{
		if (nullptr != dynamic_pointer_cast<HorizontalCoordinateSystem>(coordinateSystem))
			DialogShowCrs(dynamic_pointer_cast<HorizontalCoordinateSystem>(coordinateSystem));
		else if (nullptr != dynamic_pointer_cast<VerticalCoordinateSystem>(coordinateSystem))
			DialogShowCrs(dynamic_pointer_cast<VerticalCoordinateSystem>(coordinateSystem));
		else {
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			AfxMessageBox(_G(IDS_STRING2020));
		}
	}

	void EpsgManager::DialogShowCrs(shared_ptr<HorizontalCoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CDialogoMostrarCrsHorizontal dlg(coordinateSystem);

		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
		dlg.DoModal();
	}

	void EpsgManager::DialogShowCrs(shared_ptr<VerticalCoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CDialogoMostrarScrVertical dlg(coordinateSystem);

		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
		dlg.DoModal();
	}

	CStringW EpsgManager::DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox)
	{
		return DialogSelectVerticalCrs(
			title,
			titleUnknownCoordinateSystemCheckbox,
			_G(IDS_STRING2015));
	}

	CStringW EpsgManager::DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, int defaultCrs)
	{
		return DialogSelectVerticalCrs(
			title,
			titleUnknownCoordinateSystemCheckbox,
			GetCoordinateSystemAuthorityFactory()->CreateCoordinateSystem(defaultCrs));
	}

	CStringW EpsgManager::DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, CStringW const& wkt)
	{
		return DialogSelectVerticalCrs(
			title,
			titleUnknownCoordinateSystemCheckbox,
			GetCoordinateSystemFactory()->CreateFromWkt(wkt));
	}

	CStringW EpsgManager::DialogSelectVerticalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, shared_ptr<CoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CDialogoSeleccionScrVertical dlg(title, CStringW(titleUnknownCoordinateSystemCheckbox), coordinateSystem);

		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
		if (IDOK == dlg.DoModal())
			return dlg.m_sWkt;
		return L"";
	}

	SelectTransformationHelper::SelectTransformationHelper(HWND hwndParent)
		: hWnd(hwndParent)
	{
	}

	int SelectTransformationHelper::DialogSelectTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs, vector<CoordinateOperation> const& locatedOperations) const
	{
		return DialogSelectTransformationWnd(sourceCrs, destinationCrs, locatedOperations, hWnd);
	}

	int SelectTransformationHelper::DialogSelectTransformation(CStringW const& sourceCrs, CStringW const& destinationCrs, vector<CoordinateOperation> const& locatedOperations)
	{
		return DialogSelectTransformationWnd(sourceCrs, destinationCrs, locatedOperations, nullptr);
	}

	int SelectTransformationHelper::DialogSelectTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs, vector<CoordinateOperation> const& locatedOperations, HWND hWnd)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		auto const c�digoTransformaci�n = TransformacionesMemorizadas::LocalizaC�digoTransformaci�n(sourceCrs, destinationCrs);
		if (0 != c�digoTransformaci�n)
			return c�digoTransformaci�n;

		CXTPTaskDialog taskDlg(CWnd::FromHandle(hWnd));
		taskDlg.EnableCommandLinks();
		taskDlg.SetWindowTitle(_G(IDS_STRING2016));
		taskDlg.SetWidth(640);
		taskDlg.SetMainInstruction(_G(IDS_STRING2009));

		CStringW cadena;
		cadena.Format(_G(IDS_STRING2010), static_cast<LPCWSTR>(CStringW(sourceCrs)), static_cast<LPCWSTR>(CStringW(destinationCrs)));
		taskDlg.SetContent(cadena);

		vector<int> opciones;
		auto i = 1;
		for(auto const& operaci�n : locatedOperations) {
			cadena.Format(_G(IDS_STRING2011),
				static_cast<LPCWSTR>(operaci�n.Name),
				static_cast<LPCWSTR>(GetCoordinateSystemAuthorityFactory()->GetNameOfAlgorithm(operaci�n.MethodCode)),
				operaci�n.Version.has_value() ? operaci�n.Version.value() : CStringW{},
				operaci�n.Accuracy.has_value() ? static_cast<LPCWSTR>(Utilidades::UtilidadesString::Format(L"%f", operaci�n.Accuracy.value())) : static_cast<LPCWSTR>(_G(IDS_STRING2012)),
				operaci�n.InformationSource.has_value() ? operaci�n.InformationSource.value() : CStringW{});

			taskDlg.AddButton(cadena, static_cast<int>(i++));
			opciones.push_back(operaci�n.Code);
		}

		taskDlg.SetVerificationText(_G(IDS_STRING2018));

		auto const opci�n = taskDlg.DoModal(CRegistro::GetIntLocalMachine(L"Microsoft", L"Windows NT", L"CurrentVersion", L"CurrentVersion", 1) >= 6);

		if (taskDlg.IsVerificiationChecked())
			TransformacionesMemorizadas::AlmacenaC�digoTransformaci�n(sourceCrs, destinationCrs, opciones[static_cast<int>(opci�n) - 1]);

		return opciones[static_cast<int>(opci�n) - 1];
	}

	CreateVerticalTransformationHelper::CreateVerticalTransformationHelper(HWND hwndParent)
		: hWnd(hwndParent)
	{
	}

	shared_ptr<IMathTransform> CreateVerticalTransformationHelper::DialogCreateVerticalTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs) const
	{
		return DialogCreateVerticalTransformationWnd(sourceCrs, destinationCrs, hWnd);
	}

	shared_ptr<IMathTransform> CreateVerticalTransformationHelper::DialogCreateVerticalTransformation(CStringW const& sourceCrs, CStringW const& destinationCrs)
	{
		return DialogCreateVerticalTransformationWnd(sourceCrs, destinationCrs, nullptr);
	}

	shared_ptr<IMathTransform> CreateVerticalTransformationHelper::DialogCreateVerticalTransformationWnd(CStringW const& sourceCrs, CStringW const& destinationCrs, HWND hWnd)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		auto transformaci�n = TransformacionesMemorizadas::LocalizaTransformaci�n(CStringW(sourceCrs), CStringW(destinationCrs));
		if (nullptr != transformaci�n)
			return transformaci�n;

		CDialogoSeleccionTransformacionVertical dlg(CWnd::FromHandle(hWnd));
		dlg.m_sTituloScrOrigen = CStringW(sourceCrs);
		dlg.m_sTituloScrDestino = CStringW(destinationCrs);

		INT_PTR resultado;
		{
			AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
			resultado = dlg.DoModal();
		}

		if (IDOK == resultado) {
			transformaci�n = GetMathTransformFactory()->CreateParameterizedTransform(L"vertical_offset", { Parameter{L"vertical_offset", dlg.m_dValor } });

			if (dlg.m_bUtilizarSiempreEstaTransf)
				TransformacionesMemorizadas::AlmacenaTransformaci�n(dlg.m_sTituloScrOrigen, dlg.m_sTituloScrDestino, transformaci�n->GetWkt());

			return transformaci�n;
		}

		return nullptr;
	}

}
