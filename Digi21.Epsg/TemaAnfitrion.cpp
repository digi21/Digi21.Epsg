#include "pch.h"
#include "TemaAnfitrion.h"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

auto LeeColoresTemaAnfitrion() -> std::optional<ColoresTema>
{
	// Los mismos colores que Digi3D.NET usa en sus cuadros de diálogo (PaneColorSet).
	auto const fondo = XTPIniColor(L"DockingPane.Inactive", L"CaptionColor", CLR_INVALID);
	auto const texto = XTPIniColor(L"DockingPane.Inactive", L"TextColor", CLR_INVALID);
	if (CLR_INVALID == fondo || CLR_INVALID == texto)
		return std::nullopt;

	// El fondo de las barras de comandos solo es oscuro en el tema oscuro; en el azul el de los paneles también lo es.
	auto const fondoBarras = XTPIniColor(L"CommandBars", L"Back", GetSysColor(COLOR_BTNFACE));
	auto const luminancia = (GetRValue(fondoBarras) * 299 + GetGValue(fondoBarras) * 587 + GetBValue(fondoBarras) * 114) / 1000;

	return ColoresTema{fondo, texto, luminancia < 128};
}

auto AplicaBarraTituloTema(HWND ventana, ColoresTema const& tema) -> void
{
	if (!tema.oscuro)
		return;

	constexpr BOOL valor{TRUE};
	DwmSetWindowAttribute(ventana, DWMWA_USE_IMMERSIVE_DARK_MODE, &valor, sizeof(valor));
}
