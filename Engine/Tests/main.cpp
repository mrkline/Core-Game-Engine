#include <Core.h>
#pragma comment(lib, "Core.lib")

using namespace Core;

int main()
{
	CoreBase* testBase = CreateCoreBase();
	delete testBase;
	printf("Automatic dll linkage success is successful.\n");
	system("Pause");
}