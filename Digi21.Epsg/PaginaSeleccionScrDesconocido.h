#pragma once

class CPaginaSeleccionScrDesconocido final : public CXTPPropertyPage
{
#pragma region Campos privados
	DECLARE_DYNAMIC(CPaginaSeleccionScrDesconocido)
	CXTPPropertyPage*& current;
#pragma endregion

#pragma region Constructores y destructor
public:
	CPaginaSeleccionScrDesconocido(CXTPPropertyPage*& current, CStringW const& título);
#pragma endregion

#pragma region Métodos
protected:
	BOOL OnSetActive() override;

	// Dialog Data
	enum { IDD = IDD_PAGINA_SCR_DESCONOCIDO };

protected:
	void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
#pragma endregion
public:
	CStringW m_sTituloDesconocido;
};
