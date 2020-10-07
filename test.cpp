#include <stdio.h>
#include <windows.h>
#include "TLS_ObjectPool.h"
#include "my_profile.h"
#include <process.h>

struct ST {
	INT64 b[90];
};

TLS_CObjectPool<ST> tlsObjPool;


unsigned int WINAPI WorkerThread2(LPVOID lParam) {
	for (int i = 0; i < 10000; i++) {
		//char** a = (char**)malloc(100000*8);
		ST* a[100000];
		PRO_BEGIN(L"TLSAlloc");
		int j = 0;
		while (j!=100000) {
			a[j] = tlsObjPool.Alloc();
			j++;
		}
		PRO_END(L"TLSAlloc");
		PRO_BEGIN(L"TLSFree");
		while (j != 0) {
			j--;
			tlsObjPool.Free(a[j]);
		}
		PRO_END(L"TLSFree");
	}
	return 0;
}

unsigned int WINAPI WorkerThread3(LPVOID lParam) {
	for (int i = 0; i < 10000; i++) {
		PRO_BEGIN(L"Increment");
		int j = 100000;
		while (j != 0) {
			//int* a = tlsObjPool.Alloc();
			j--;
		}
		PRO_END(L"Increment");
	}
	return 0;
}


unsigned int WINAPI WorkerThread(LPVOID lParam) {
	for (int i = 0; i < 10000; i++) {
		//char** a = (char**)malloc(100000 * 8);
		ST* a[100000];
		PRO_BEGIN(L"New");
		int j = 0;
		while (j != 100000) {
			a[j] = new ST;
			j++;
		}
		PRO_END(L"New");
		PRO_BEGIN(L"Delete");
		while (j != 0) {
			j--;
			delete a[j];
		}
		PRO_END(L"Delete");
	}
	return 0;
}

int wmain() {
	ProfileInit();
	//HANDLE hWorkerThread;
	//WCHAR a = getc(stdin);
	//if (a == L'1') {
	//	
	//	hWorkerThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, NULL, 0, NULL);

	//}
	//else if (a == L'2') {

	//	hWorkerThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThread2, NULL, 0, NULL);
	//	
	//}
	//else {

	//	hWorkerThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThread3, NULL, 0, NULL);
	//	
	//}
	//WaitForSingleObject(hWorkerThread, INFINITE);
	HANDLE hWorkerThread[4];
	WCHAR a = getc(stdin);
	if (a == L'1') {
		for (int i = 0; i < 4; i++) {
			hWorkerThread[i] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, NULL, 0, NULL);
		}
	}
	else if( a== L'2') {
		for (int i = 0; i < 4; i++) {
			hWorkerThread[i] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread2, NULL, 0, NULL);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			hWorkerThread[i] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread3, NULL, 0, NULL);
		}
	}
	
	WaitForMultipleObjects(4, hWorkerThread, TRUE, INFINITE);
	ProfileDataOutText();
}