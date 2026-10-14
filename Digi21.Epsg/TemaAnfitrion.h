#pragma once

#include <optional>

// Colores del tema de Codejock que ha cargado el programa anfitrión con XTPThemeDLL()->SetHandle. ToolkitPro es una DLL
// compartida: este módulo ve el mismo tema que el anfitrión.
struct ColoresTema
{
	COLORREF fondo{};
	COLORREF texto{};
	bool oscuro{};
};

// Sin valor si el anfitrión no ha cargado ningún tema de Codejock.
auto LeeColoresTemaAnfitrion() -> std::optional<ColoresTema>;

// Pone la barra de título oscura de DWM si el tema es oscuro.
auto AplicaBarraTituloTema(HWND ventana, ColoresTema const& tema) -> void;
