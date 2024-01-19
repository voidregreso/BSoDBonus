#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <TlHelp32.h>

#pragma comment(lib,"ntdll.lib")
 
EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(ULONG,BOOLEAN,BOOLEAN,PBOOLEAN);
EXTERN_C NTSTATUS NTAPI NtSetInformationProcess(HANDLE,ULONG,PVOID,ULONG);

void MannerOne()
{
	HANDLE hProcess = NULL;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, "csrss.exe") == 0)
		{
			hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

void MannerTwo(bool enabled)
{
	BOOLEAN bl;
	ULONG status;
	if (RtlAdjustPrivilege(20, TRUE, FALSE, &bl) != 0)
	{
		printf("Unable to enable SeDebugPrivilege. Make sure you are running this program as administrator.");
		return;
	}
	status = NtSetInformationProcess((HANDLE)-1, 0x1d, &enabled, sizeof(ULONG));
	if (status != 0)
	{
		printf("Error: Unable to set the current process as critical process. NtSetInformationProcess failed with status %#x\n\n", status);
	}
	else
	{
		printf("Successfully set the current process as critical process.\n\n");
	}
}

int main(int argc, char **argv)
{
	int d;
	printf("Choose your option: 1 or 2. 1 for terminating csrss, 2 for set myself as critical process.\r\n");
	scanf("%d", &d);
	switch(d) {
		case 1:
			MannerOne();
			break;
		case 2:
			MannerTwo(true);
			break;
		default:
			break;
	}
	system("pause");
	return 0;
}