// ParseDexFile.cpp : �������̨Ӧ�ó������ڵ㡣
//
#pragma once
#include <TCHAR.H>
#include "CDexInfo.h"

int main(int argc, char* argv[], char* envp[])
{
	char* pszPath = "E:\\A_Projects\\AndroidProjects\\keepAlive\\bin\\classes.dex";
	if (argc >= 1) {
		pszPath = argv[1];
		//printf(pszPath);
	}
	CDexInfo dexObj = CDexInfo(pszPath);
	if (dexObj.mDexFile == NULL)
	{
		printf("Read File Failed, is path correct?");
	}
	//��get��ʱ��Ὣ��Щ����ͬʱ��ӡ����
	//dexObj.mDexFile->getContString_ids();
	//dexObj.mDexFile->getContType_ids();
	//dexObj.mDexFile->getContProto_ids();
	//dexObj.mDexFile->getContMethod_ids();
	dexObj.mDexFile->getContClass_defs();
    return 0;
}
