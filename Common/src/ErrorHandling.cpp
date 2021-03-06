#include "ErrorHandling.h"

#include <Windows.h>

namespace Core
{
	void EHandler::ShowErrorDialog(const char* const message, const char* const title)
	{
		MessageBoxA(nullptr, message, title, MB_ICONERROR | MB_OK);
	}
} // end namespace Core