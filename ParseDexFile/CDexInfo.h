#pragma once
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "DexFile.h"

class CDexInfo
{
public:
	CDexInfo();
	CDexInfo(char * pszDexPathName);
	virtual ~CDexInfo();
private:
	char* readDexCont(char * pszDexPathName);

public:
	unsigned int mDexSize;
	DexFile* mDexFile;
	//DexHeader* mDexHeader;
	u1* mPszCont;
};
