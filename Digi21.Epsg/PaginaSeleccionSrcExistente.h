#pragma once

class PaginaSeleccionSrcExistente final : public CXTPPropertyPage
{
#pragma region Campos privados
	DECLARE_DYNAMIC(PaginaSeleccionSrcExistente)
	CXTPPropertyPage*& _current;
	CStringW _nombreSistema;
#pragma endregion

#pragma region Constructores y destructor
public:
	PaginaSeleccionSrcExistente(CXTPPropertyPage*& current, int idTemplate, CStringW const& nombreSistema);
#pragma endregion

#pragma region Métodos
protected:
	BOOL OnSetActive() override;

protected:
	void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
#pragma endregion
};
