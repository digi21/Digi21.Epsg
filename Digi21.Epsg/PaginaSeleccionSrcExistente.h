#pragma once

class PaginaSeleccionSrcExistente final : public CXTPPropertyPage
{
#pragma region Campos privados
	DECLARE_DYNAMIC(PaginaSeleccionSrcExistente)
	CXTPPropertyPage*& current;
	CStringW _titulo;
#pragma endregion

#pragma region Constructores y destructor
public:
	PaginaSeleccionSrcExistente(CXTPPropertyPage*& current, CStringW const& título);
#pragma endregion

#pragma region Métodos
protected:
	BOOL OnSetActive() override;

	// Dialog Data
	enum { IDD = IDD_PAGINA_SCR_EXISTENTE};

protected:
	void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
#pragma endregion
};
