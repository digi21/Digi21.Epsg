// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CStringW constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

constexpr auto TITULO_EXCEPCIONES = L"Digi21.Epsg";
#include <Util/LogEventos2.h>

constexpr auto CARPETA_REGISTRO_DIGI3D_MEMORIZED_COORDINATE_SYSTEMS = L"Digi3D\\MemorizedCoordinateSystems\\Global";
constexpr auto CARPETA_REGISTRO_DIGI3D_MEMORIZED_HORIZONTAL_COORDINATE_SYSTEMS = L"Digi3D\\MemorizedCoordinateSystems\\Horizontal";
constexpr auto CARPETA_REGISTRO_DIGI3D_MEMORIZED_VERTICAL_COORDINATE_SYSTEMS = L"Digi3D\\MemorizedCoordinateSystems\\Vertical";
constexpr auto CARPETA_REGISTRO_DIGI3D_MEMORIZED_3D_COORDINATE_SYSTEMS = L"Digi3D\\MemorizedCoordinateSystems\\3D";
constexpr auto CARPETA_REGISTRO_TRANSFORMACIONES_MEMORIZADAS = L"App\\TransformacionesMemorizadas\\";

constexpr auto CARPETA_DIGI21 = L"Digi21" ;
constexpr auto VERSION_DIGI3D = L"Digi3D.NET" ;
constexpr auto CARPETA_REGISTRO_APP_CONFIGURATION = L"App\\Configuration" ;

#include <vector>
#include <tuple>
#include <memory>
#pragma warning(disable: 5257)
#include <XTToolkitPro.h>
#include <afxcontrolbars.h>

constexpr auto M_E = 2.71828182845904523536;
constexpr auto M_PI = 3.14159265358979323846;
constexpr auto M_PI_2 = M_PI / 2.0;

template <typename T>
constexpr auto SEXA_A_RADIANES(T grados) -> double { return static_cast<double>(grados) * M_PI / 180.0; }

template <typename T>
constexpr auto RADIANES_A_SEXA(T x) -> double { return static_cast<double>(x) * 180.0 / M_PI; }

template <typename T>
constexpr auto CENTE_A_RADIANES(T grados) -> double { return static_cast<double>(grados) * M_PI / 200.0; }

template <typename T>
constexpr auto RADIANES_A_CENTE(T x) -> double { return static_cast<double>(x) * 200.0 / M_PI; }

template <typename T>
constexpr auto SEXA_A_CENTE(T x) -> double { return x * 200.0 / 180.0; }

template <typename T>
constexpr auto CENTE_A_SEXA(T x) -> double { return x * 180.0 / 200.0; }

template <typename T>
constexpr auto SIGN(T x) -> double { return (x > 0 ? 1.0 : -1.0); }

#include <Digi21.OpenGis/Digi21OpenGis.h>

constexpr auto CARPETA_REGISTRO_APP_CONFIGURATION_DIALOGLAYOUT = L"HKCU\\Software\\Digi21\\Digi3D.NET\\App\\Configuration\\DialogLayout\\Digi21.Epsg";
constexpr auto CARPETA_REGISTRO_APP_TRANSFORMACIONES_MEMORIZADAS = L"App\\TransformacionesMemorizadas";

#include <Util/Registro.h>
#include <Util/ErroresWindows.h>
#include <SoporteMUI.h>
#include <Util/String2.h>
#include <afxcontrolbars.h>
