
#include <string.h>
#include <stdio.h>
#include "DexFile.h"
#include "utils.h"

void DexFile::initall() {
	if (baseAddr == 0) {
		return;
	}
	pHeader = (const DexHeader*)baseAddr;
	try
	{
		pMapList = (DexMapList*)(pHeader->mapOff + baseAddr);
		pStringIds = (DexStringId*)(pHeader->stringIdsOff + baseAddr);
		pTypeIds = (DexTypeId*)(pHeader->typeIdsOff + baseAddr);
		pFieldIds = (DexFieldId*)(pHeader->fieldIdsOff + baseAddr);
		pMethodIds = (DexMethodId*)(pHeader->methodIdsOff + baseAddr);
		pProtoIds = (DexProtoId*)(pHeader->protoIdsOff + baseAddr);
		pClassDefs = (DexClassDef*)(pHeader->classDefsOff + baseAddr);
		pLinkData = (DexLink*)(pHeader->linkOff + baseAddr);
		//pStringIds->stringDataOff;
	}
	catch (...)
	{
		char * psz = "Exception Occur";
		printf_s("%s", psz);
	}
}


void DexFile::getContMapLists() {
}


void DexFile::getContString_ids() {
	int nStringCount = pHeader->stringIdsSize;
	u4* string_data_items = (u4*)pStringIds;
	for (int i = 0; i < nStringCount; i ++)
	{
		const u1* pstr = baseAddr + string_data_items[i];
		int nLen = readUnsignedLeb128(&pstr);
		printf("%d %d %s\r\n", i, nLen, pstr);
	}
}

const u1* DexFile::getDexString(int nIndex) {
	int nStringCount = pHeader->stringIdsSize;
	if (nIndex >= nStringCount) {
		return nullptr;
	}
	u4* string_data_items = (u4*)pStringIds;
	const u1* pstr = baseAddr + string_data_items[nIndex];
	int nLen = readUnsignedLeb128(&pstr);
	//printf("%d %d %s\r\n", nIndex, nLen, pstr);
	return pstr;
}

void DexFile::getContType_ids() {
	int nTypeCount = pHeader->typeIdsSize;
	u4* type_data_items = (u4*)pTypeIds;
	for (int i = 0; i < nTypeCount; i++)
	{
		const u1* strType = getDexString(type_data_items[i]);
		printf("%d %s\r\n", i, strType);
	}
}

const u1* DexFile::getDexType(int nIndex) {
	int nTypeCount = pHeader->typeIdsSize;
	if (nIndex >= nTypeCount) {
		return nullptr;
	}
	u4* type_data_items = (u4*)pTypeIds;
	const u1* strType = getDexString(type_data_items[nIndex]);
	return strType;
}


void DexFile::getContProto_ids() {
	int nProtoCount = pHeader->protoIdsSize;
	for (int i = 0; i < nProtoCount; i++)
	{
		DexProtoId proto_item = pProtoIds[i];
		const u1* strShort = getDexString(proto_item.shortyIdx);
		const u1* strReturn = getDexString(proto_item.returnTypeIdx);
		u1 strArgu[256 * 5] = {'\0'};
		if (proto_item.parametersOff == 0) {
			sprintf_s((char*)strArgu, 256 * 5, "%s", "VOID");
		}
		else {
			DexTypeList* pTypLst = (DexTypeList*)(baseAddr + proto_item.parametersOff);
			int nIndex = 0;
			for (u4 j = 0; j < pTypLst->size; j ++) {
				const u1* pszType = getDexType(pTypLst->list[j].typeIdx);
				sprintf_s((char*)strArgu + nIndex, 256*5 - nIndex, "%s ", pszType);
				nIndex = nIndex + strlen((char*)pszType) + 1;
			}
		}
		printf("%d ret:%s methodDesc:%s argu:%s\r\n", i, strReturn, strShort, strArgu);
	}
}

void DexFile::printDexProto(int nIndex) {
	int nProtoCount = pHeader->protoIdsSize;
	if (nIndex >= nProtoCount) {
		return;
	}
	DexProtoId proto_item = pProtoIds[nIndex];
	const u1* strShort = getDexString(proto_item.shortyIdx);
	const u1* strReturn = getDexString(proto_item.returnTypeIdx);
	u1 strArgu[256 * 5] = { '\0' };
	if (proto_item.parametersOff == 0) {
		sprintf_s((char*)strArgu, 256 * 5, "%s", "VOID");
	}
	else {
		DexTypeList* pTypLst = (DexTypeList*)(baseAddr + proto_item.parametersOff);
		int nIndex = 0;
		for (u4 j = 0; j < pTypLst->size; j++) {
			const u1* pszType = getDexType(pTypLst->list[j].typeIdx);
			sprintf_s((char*)strArgu + nIndex, 256 * 5 - nIndex, "%s ", pszType);
			nIndex = nIndex + strlen((char*)pszType) + 1;
		}
	}
	printf("ret:%s methodDesc:%s argu:%s\r\n", strReturn, strShort, strArgu);
}

void DexFile::printDexProto(int nIndex, char pszBuf[1024]) {
	int nProtoCount = pHeader->protoIdsSize;
	if (nIndex >= nProtoCount) {
		return ;
	}
	DexProtoId proto_item = pProtoIds[nIndex];
	const u1* strShort = getDexString(proto_item.shortyIdx);
	const u1* strReturn = getDexString(proto_item.returnTypeIdx);
	u1 strArgu[256 * 5] = { '\0' };
	if (proto_item.parametersOff == 0) {
		sprintf_s((char*)strArgu, 256 * 5, "%s", "VOID");
	}
	else {
		DexTypeList* pTypLst = (DexTypeList*)(baseAddr + proto_item.parametersOff);
		int nIndex = 0;
		for (u4 j = 0; j < pTypLst->size; j++) {
			const u1* pszType = getDexType(pTypLst->list[j].typeIdx);
			sprintf_s((char*)strArgu + nIndex, 256 * 5 - nIndex, "%s ", pszType);
			nIndex = nIndex + strlen((char*)pszType) + 1;
		}
	}
	int nLenRet = strlen((char*)strReturn);
	int nLenShort = strlen((char*)strShort);
	int nLenArgu = strlen((char*)strArgu);
	int nLenTotal = nLenRet + nLenShort + nLenArgu;
	char* pszTmp = pszBuf;
	if (nLenTotal + 2 <= 1024) {
		memmove(pszTmp, strReturn, nLenRet);   pszTmp += nLenRet;
		memmove(pszTmp, " ", 1);  pszTmp++;
		memmove(pszTmp, strShort, nLenShort); pszTmp += nLenShort;
		memmove(pszTmp, " ", 1);  pszTmp++;
		memmove(pszTmp, strArgu, nLenArgu); pszTmp += nLenArgu;
	}
	else {
		memset(pszTmp, 0, sizeof(pszTmp));
		pszTmp = "String Length not enough";
		memmove(pszBuf, pszTmp, strlen(pszTmp));
	}
		//printf("%d ret:%s methodDesc:%s argu:%s\r\n", i, strReturn, strShort, strArgu);
}


void DexFile::getContField_ids() {
	int nFieldCount = pHeader->fieldIdsSize;
	//u4* field_data_items = (u4*)pFieldIds;
	for (int i = 0; i < nFieldCount; i++)
	{
		const u1* classType = getDexType(pFieldIds[i].classIdx);
		const u1* selfType = getDexType(pFieldIds[i].typeIdx);
		const u1* name = getDexString(pFieldIds[i].nameIdx);
		printf("%d %s %s %s\r\n", i, classType, selfType, name);
	}
}

void DexFile::printField_ids(int nIndex) {
	int nFieldCount = pHeader->fieldIdsSize;
	if (nIndex >= nFieldCount)
	{
		return;
	}
	DexFieldId dexFieldId = pFieldIds[nIndex];
	const u1* classType = getDexType(dexFieldId.classIdx);
	const u1* selfType = getDexType(dexFieldId.typeIdx);
	const u1* name = getDexString(dexFieldId.nameIdx);
	printf("clsType:%s selfType:%s name:%s\r\n", classType, selfType, name);
}

// u2  classIdx;           /* index into typeIds list for defining class */
// u2  protoIdx;           /* index into protoIds for method prototype */
// u4  nameIdx;
void DexFile::getContMethod_ids() {
	int nMethodCount = pHeader->methodIdsSize;
	for (int i = 0; i < nMethodCount; i++)
	{
		const u1* classType = getDexType(pMethodIds[i].classIdx);
		printDexProto(pMethodIds[i].protoIdx);
		const u1* name = getDexString(pMethodIds[i].nameIdx);
		printf("%d %s %s\r\n", i, classType, name);
	}
}

void DexFile::printMethod_ids(int nIndex) {
	int nMethodCount = pHeader->methodIdsSize;
	if (nIndex >= nMethodCount)
	{
		return;
	}
	DexMethodId dexMethodId = pMethodIds[nIndex];
	const u1* classType = getDexType(dexMethodId.classIdx);
	printDexProto(dexMethodId.protoIdx);
	const u1* name = getDexString(dexMethodId.nameIdx);
	printf("claType:%s name:%s\r\n", classType, name);
}

void DexFile::getContClass_defs() {
	int nContClassCount = pHeader->classDefsSize;
	for (int i = 0; i < nContClassCount; i++)
	{
		const u1* classType = getDexType(pClassDefs[i].classIdx);
		u4 access = pClassDefs[i].accessFlags;
		const u1* superClassType = getDexType(pClassDefs[i].superclassIdx);
		u4 nInterfaceOff = pClassDefs[i].interfacesOff;
		if (nInterfaceOff != 0) {
			DexTypeList* pTypeList = (DexTypeList*)(baseAddr + nInterfaceOff);
			parseTypelist(pTypeList);
		}
		const u1* srcFileName = getDexString(pClassDefs[i].sourceFileIdx);
		pClassDefs[i].annotationsOff;//注释所在偏移

		u4 clsDataOff = pClassDefs[i].classDataOff;//类所在偏移，其中还有内容解析
		if (clsDataOff != 0) {
			parseClass_data_item(baseAddr + clsDataOff);
		}
		
		pClassDefs[i].staticValuesOff;//

		printf("%d %s %s %s\r\n", i, classType, superClassType, srcFileName);
	}
}

void DexFile::parseTypelist(DexTypeList* pTypelist){
	if (pTypelist == nullptr) {
		return;
	}
	int nLength = 0;
	for (u4 j = 0; j < pTypelist->size; j++) {
		const u1* pszType = getDexType(pTypelist->list[j].typeIdx);
		if (pszType != NULL)
		{
			nLength = nLength + strlen((char*)pszType) + 1;
		}
	}
	char* pszResult = new char[nLength + 5];
	if (pszResult == NULL) {
		return;
	}
	memset(pszResult, 0, nLength);
	int nIndex = 0;
	for (u4 j = 0; j < pTypelist->size; j++) {
		const u1* pszType = getDexType(pTypelist->list[j].typeIdx);
		memmove(pszResult + nIndex, pszType, strlen((char*)pszType));
		nIndex = nIndex + strlen((char*)pszType);
	}
	printf(pszResult);
	printf("\r\n");
	if (pszResult != NULL) {
		delete pszResult;
		pszResult = NULL;
	}
}

// struct encoded_field
// {
// 	uleb128  filed_idx_diff; // index into filed_ids for ID of this filed
// 	uleb128  access_flags; // access flags like public, static etc.
// };
// 
// //encoded_method 的结构如下 ：
// struct encoded_method
// {
// 	uleb128 method_idx_diff;
// 	uleb128  access_flags;
// 	uleb128 code_off;
// 
void DexFile::parseEncoded_field(u4 filed_idx, u4 access_flags) {
	printField_ids(filed_idx);
	printf("filed access_flags:%08x\r\n", access_flags);
}

void DexFile::parseEncoded_method(u4 method_idx, u4 access_flags, u4 code_off) {
	printMethod_ids(method_idx);
	printf("method_idx access_flags:%08x\r\n", access_flags);
	if (code_off != 0) {
		DexCode* pDexCode = (DexCode*)(baseAddr + code_off);
		parseDexCode(pDexCode);
	}
}

void DexFile::parseDexCode(DexCode* pDexCode) {
	printf("registersSize: %d "
		"insSize: %d "
		"outsSize: %d "
		"triesSize: %d "
		"debugInfoOff: %d "
		"insnsSize: %d \r\n", pDexCode->registersSize,
		pDexCode->insSize, pDexCode->outsSize, pDexCode->triesSize,
		pDexCode->debugInfoOff, pDexCode->insnsSize);
	printf("MethodDexByte: ");
	for (int i = 0; i < pDexCode->insnsSize; i ++)
	{
		printf("%02x ", pDexCode->insns[i]);
	}
	printf("\r\n");
}
//0x2e3a29 
void DexFile::parseClass_data_item(const u1* pDataItemStart) {
	u4 static_fields_size = readUnsignedLeb128(&pDataItemStart);
	u4 instance_fields_size = readUnsignedLeb128(&pDataItemStart);
	u4 direct_methods_size = readUnsignedLeb128(&pDataItemStart);
	u4 virtual_methods_size = readUnsignedLeb128(&pDataItemStart);
	printf("%d %d %d %d\r\n", static_fields_size, instance_fields_size, direct_methods_size, virtual_methods_size);
	int nTotalUleb128 = (static_fields_size + instance_fields_size) * 2 + 
		(direct_methods_size + virtual_methods_size) * 3;
	//parse one by one, it's not solid length,
	u4* uLebAry = new u4[nTotalUleb128];
	if (uLebAry == NULL) { return; } 
	for (int i = 0; i < nTotalUleb128; i ++)
	{
		uLebAry[i] = readUnsignedLeb128(&pDataItemStart);
		//printf("%08x ", uLebAry[i]);
	}
	//return;
	//read Corresponding data,then parse
	int nField_idx = 0;
	for (int i = 0; i < static_fields_size + instance_fields_size; i++) {
		if (i == 0 || i == static_fields_size) {
			nField_idx = uLebAry[2 * i];
		}
		else {
			nField_idx += uLebAry[2 * i];
		}
		//printf("field_idx_off:%d\r\n", nField_idx);
		parseEncoded_field(nField_idx, uLebAry[2 * i+1]);
	}
	int nStart = (static_fields_size + instance_fields_size) * 2;
	int nTotalMethodSize = direct_methods_size + virtual_methods_size;
	int nMethod_idx = 0;
	for (int i = 0; i < nTotalMethodSize; i++) {
		int nNumFirst = 3 * i + nStart;
		if (i == 0 || i == direct_methods_size) {
			nMethod_idx = uLebAry[nNumFirst];
		}
		else {
			nMethod_idx += uLebAry[nNumFirst];
		}
		//printf("method_idx_off:%d\r\n", nMethod_idx);
		parseEncoded_method(nMethod_idx, uLebAry[nNumFirst + 1], uLebAry[nNumFirst + 2]);
	}
	if (uLebAry != NULL)
	{
		delete uLebAry;
		uLebAry = NULL;
	}
}

void DexFile::parseAnnotations_directory_item(DexTypeList* pTypelist) {

}

void DexFile::parseDexEncodedArray(DexTypeList* pTypelist) {

}
