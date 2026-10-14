#include "pch.h"
#include "EpsgManager.h"
#include "Resource.h"
#include "DialogoBuscarScrHorizontal.h"
#include "DialogoSeleccionScrHorizontal.h"
#include "DialogoSeleccionScrVertical.h"
#include "DialogoMostrarCrsHorizontal.h"
#include "DialogoMostrarScrVertical.h"
#include "DialogoSeleccionTransformacionVertical.h"
#include "DialogoSeleccionScr.h"

using namespace CrsKit;
using namespace CrsKit::CoordinateSystems;
using namespace CrsKit::CoordinateTransformations;
using namespace Digi21::Geodesia::EPSG;
using namespace std;

#pragma warning(disable:4947)

namespace Digi21::OpenGis::Epsg
{
	class TransformacionesMemorizadas
	{
	public:
		static void AlmacenaTransformación(CStringW const& origen, CStringW const& destino, CStringW const& wkt)
		{
			CRegistro::SetStringCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"wkt", wkt);
		}

		static std::shared_ptr<IMathTransform> LocalizaTransformación(CStringW const& origen, CStringW const& destino)
		{
			auto const wkt = CRegistro::GetStringCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"wkt");

			if (0 == wkt.GetLength())
				return nullptr;

			try {
				return GetMathTransformFactory()->CreateFromWkt(AUtf8(wkt));
			}
			catch (...)
			{
				return nullptr;
			}
		}

		static void AlmacenaCódigoTransformación(CStringW const& origen, CStringW const& destino, int código)
		{
			CRegistro::SetIntCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"Código2", código);
		}

		static int LocalizaCódigoTransformación(CStringW const& origen, CStringW const& destino)
		{
			return CRegistro::GetIntCurrentUser(CARPETA_DIGI21, VERSION_DIGI3D, CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS + origen + L"\\" + destino, L"Código2");
		}
	};

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogoSeleccionScr::DialogSeleccionaScr(
			title,
			titleUnknownCoordinateSystemCheckbox);
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
			GetCoordinateSystemFactory()->CreateFromWkt(AUtf8(wkt)));
	}

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, shared_ptr<CoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogoSeleccionScr::DialogSeleccionaScr(
			title,
			CStringW(titleUnknownCoordinateSystemCheckbox),
			coordinateSystem);
	}

	CStringW EpsgManager::DialogSelectCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, std::shared_ptr<CoordinateSystems::CoordinateSystem> const& ventanaDibujo, std::shared_ptr<CoordinateSystems::CoordinateSystem> const& ventanaFotogrametrica)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		return DialogoSeleccionScr::DialogSeleccionaScr(
			title,
			CStringW(titleUnknownCoordinateSystemCheckbox),
			ventanaDibujo,
			ventanaFotogrametrica);
	}

	std::optional<int> EpsgManager::DialogSearchHorizontalCrs(CStringW const& title, int defaultCrs)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };

		CDialogoBuscarScrHorizontal dlg{ defaultCrs };
		if (IDOK == dlg.DoModal())
			return dlg.GetSCR();

		return {};
	}


	CStringW EpsgManager::DialogSelectHorizontalCrs(CStringW const& title, CStringW const& titleUnknownCoordinateSystemCheckbox, shared_ptr<CoordinateSystem> const& coordinateSystem)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CDialogoSeleccionScrHorizontal dlg(title, CStringW(titleUnknownCoordinateSystemCheckbox), coordinateSystem);

		AsignadorRecursosDLL ar{ G_HINSTRECURSOS };

		if (IDOK == dlg.DoModal())
			return dlg.m_sWkt;

		return DeUtf8(coordinateSystem->GetWkt());
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
			GetCoordinateSystemFactory()->CreateFromWkt(AUtf8(wkt)));
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
			GetCoordinateSystemFactory()->CreateFromWkt(AUtf8(wkt)));
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

	auto EpsgManager::CreaOpcionesTransformacion(HWND hWndPadre) -> CoordinateTransformationOptions
	{
		CoordinateTransformationOptions opciones{};

		opciones.selectOperation = [hWndPadre](string const& origen, string const& destino, vector<CoordinateOperation> const& operaciones) {
			return SelectTransformationHelper::DialogSelectTransformationWnd(DeUtf8(origen), DeUtf8(destino), operaciones, hWndPadre);
		};

		opciones.resolveTransform = [hWndPadre](shared_ptr<CoordinateSystem> const& origen, shared_ptr<CoordinateSystem> const& destino) -> shared_ptr<IMathTransform> {
			if (nullptr != dynamic_pointer_cast<LocalCoordinateSystem>(origen) || nullptr != dynamic_pointer_cast<LocalCoordinateSystem>(destino))
				return nullptr;

			// crskit también llama aquí con sistemas desconocidos; solo los casos con un sistema vertical abren el cuadro.
			if (nullptr == dynamic_pointer_cast<VerticalCoordinateSystem>(origen) && nullptr == dynamic_pointer_cast<VerticalCoordinateSystem>(destino))
				return nullptr;

			return CreateVerticalTransformationHelper::DialogCreateVerticalTransformationWnd(DeUtf8(origen->GetName()), DeUtf8(destino->GetName()), hWndPadre);
		};

		return opciones;
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

		auto const códigoTransformación = TransformacionesMemorizadas::LocalizaCódigoTransformación(sourceCrs, destinationCrs);
		if (0 != códigoTransformación)
			return códigoTransformación;

		CXTPTaskDialog taskDlg(CWnd::FromHandle(hWnd));
		taskDlg.EnableCommandLinks();
		taskDlg.SetWindowTitle(_G(IDS_STRING2016));
		taskDlg.SetWidth(640);
		taskDlg.SetMainInstruction(_G(IDS_STRING2009));

		CStringW cadena;
		cadena.Format(_G(IDS_STRING2010), static_cast<LPCWSTR>(CStringW(sourceCrs)), static_cast<LPCWSTR>(CStringW(destinationCrs)));
		taskDlg.SetContent(cadena);

		taskDlg.EnableCancellation();

		// Los IDs de los botones empiezan en 100 para no coincidir con IDOK, IDCANCEL y el resto de IDs de botones comunes.
		constexpr int primerIdBotón{100};
		vector<int> opciones;
		auto i = primerIdBotón;
		for(auto const& operación : locatedOperations) {
			cadena.Format(_G(IDS_STRING2011),
				static_cast<LPCWSTR>(DeUtf8(operación.Name)),
				static_cast<LPCWSTR>(DeUtf8(GetCoordinateSystemAuthorityFactory()->GetNameOfAlgorithm(operación.MethodCode))),
				static_cast<LPCWSTR>(DeUtf8(operación.Version.value_or(""))),
				operación.Accuracy.has_value() ? static_cast<LPCWSTR>(Utilidades::UtilidadesString::Format(L"%f", operación.Accuracy.value())) : static_cast<LPCWSTR>(_G(IDS_STRING2012)),
				static_cast<LPCWSTR>(DeUtf8(operación.InformationSource.value_or(""))));

			taskDlg.AddButton(cadena, static_cast<int>(i++));
			opciones.push_back(operación.Code);
		}

		taskDlg.SetVerificationText(_G(IDS_STRING2018));

		auto const opción = taskDlg.DoModal(CRegistro::GetIntLocalMachine(L"Microsoft", L"Windows NT", L"CurrentVersion", L"CurrentVersion", 1) >= 6);

		// 0 (comctl32) o -1 (implementación de Codejock) si el cuadro no se pudo mostrar; IDCANCEL con Escape, Alt+F4 o el botón de cerrar.
		auto const índice = static_cast<int>(opción) - primerIdBotón;
		if (índice < 0 || índice >= static_cast<int>(opciones.size()))
			throw runtime_error{CStringA{Utilidades::UtilidadesString::Format(_G(IDS_STRING2035), static_cast<LPCWSTR>(sourceCrs), static_cast<LPCWSTR>(destinationCrs))}};

		if (taskDlg.IsVerificiationChecked())
			TransformacionesMemorizadas::AlmacenaCódigoTransformación(sourceCrs, destinationCrs, opciones[índice]);

		return opciones[índice];
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

		auto transformación = TransformacionesMemorizadas::LocalizaTransformación(CStringW(sourceCrs), CStringW(destinationCrs));
		if (nullptr != transformación)
			return transformación;

		CDialogoSeleccionTransformacionVertical dlg(CWnd::FromHandle(hWnd));
		dlg.m_sTituloScrOrigen = CStringW(sourceCrs);
		dlg.m_sTituloScrDestino = CStringW(destinationCrs);

		INT_PTR resultado;
		{
			AsignadorRecursosDLL ar{ G_HINSTRECURSOS };
			resultado = dlg.DoModal();
		}

		if (IDOK == resultado) {
			transformación = GetMathTransformFactory()->CreateParameterizedTransform("vertical_offset", { Parameter{"vertical_offset", dlg.m_dValor} });

			if (dlg.m_bUtilizarSiempreEstaTransf)
				TransformacionesMemorizadas::AlmacenaTransformación(dlg.m_sTituloScrOrigen, dlg.m_sTituloScrDestino, DeUtf8(transformación->GetWkt()));

			return transformación;
		}

		return nullptr;
	}

}
