#pragma once

class PaginaManual : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(PaginaManual)

#pragma region Campos privados
	CXTPPropertyPage*& current;
	CString _wkt;
#pragma endregion
public:
	explicit PaginaManual(CXTPPropertyPage*& current);

#pragma region Propiedades
	CStringW GetWkt() const { return _wkt; }
	void SetWkt(CStringW const& wkt) { _wkt = wkt; }
	__declspec(property(get = GetWkt, put = SetWkt)) CStringW Wkt;
#pragma endregion

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;
	void OnBnClickedButtonBuscarArchivo();
	void OnBnClickedBotonMemorizarSistemaCoordenadas();

	DECLARE_MESSAGE_MAP()
};


