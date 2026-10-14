#pragma once

#include "DialogoAdaptadoAlTema.h"

// Página de propiedades que sigue el tema de Codejock del anfitrión. Sin tema se comporta como CXTPPropertyPage.
class PaginaAdaptadaAlTema : public DialogoAdaptadoAlTema<CXTPPropertyPage>
{
	DECLARE_DYNAMIC(PaginaAdaptadaAlTema)

public:
	explicit PaginaAdaptadaAlTema(UINT idPlantilla);
};
