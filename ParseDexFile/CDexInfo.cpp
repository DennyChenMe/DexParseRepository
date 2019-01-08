#include "CDexInfo.h"


CDexInfo::CDexInfo()
{
}

CDexInfo::CDexInfo(char * pszDexPathName)
{
	mPszCont = (u1*)readDexCont(pszDexPathName);
	if (mPszCont != NULL)
	{
		mDexFile = new DexFile(mPszCont);
	}
	else {
		mDexFile = NULL;
	}
// 	if (mDexFile != NULL) {
// 		mDexFile->baseAddr = 
// 	}
	
}

char* CDexInfo::readDexCont(char * pszDexPathName) {
	FILE *fp;
	fopen_s(&fp, pszDexPathName, (const char*)"rb+");
	if (fp == NULL)
	{
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	//mDexSize = nFileSize;
	char* buffer = new char[nFileSize];
	if (buffer == NULL)
	{
		goto EXIT;
	}
	fseek(fp, SEEK_SET, 0);
	mDexSize = fread(buffer, 1, nFileSize, fp);
	
EXIT:
	fclose(fp);
	return buffer;
}

CDexInfo::~CDexInfo()
{
	if (mPszCont != NULL) {
		delete mPszCont;
		mPszCont = NULL;
	}
	if (mDexFile != NULL) {
		delete mDexFile;
		mDexFile = NULL;
	}
}
