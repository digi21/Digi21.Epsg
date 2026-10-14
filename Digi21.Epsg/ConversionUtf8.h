#pragma once

#include <string>

// Conversión entre las cadenas UTF-8 de crskit y las CStringW (UTF-16) de MFC.
inline auto DeUtf8(std::string const& utf8) -> CStringW
{
	if (utf8.empty())
		return {};

	return Digi21::Utilidades::UtilidadesString::UTF82String(CStringA{utf8.c_str(), static_cast<int>(utf8.size())});
}

inline auto AUtf8(CStringW const& texto) -> std::string
{
	if (texto.IsEmpty())
		return {};

	auto const utf8 = Digi21::Utilidades::UtilidadesString::AUTF8(texto);
	return {utf8.GetString(), static_cast<std::size_t>(utf8.GetLength())};
}
