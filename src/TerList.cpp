#include "stdafx.h"
#include <Windows.h>
#include "TerList.h"
#include <stdlib.h>

int Add(struct TerList *__ptrHeader, int __intValue){
	while (__ptrHeader->Next !=NULL)
	{
		if (__ptrHeader->Value == __intValue)
		{
			return 0;
		}
		__ptrHeader = __ptrHeader->Next;
	}
	struct TerList *_ptrTemp = (struct TerList*) malloc(sizeof(struct TerList));
	_ptrTemp->Next = NULL;
	__ptrHeader->Value = __intValue;
	__ptrHeader->Next = _ptrTemp;
	return 1;
}

struct TerList **InitializeArr( int __size){
	struct TerList **_output=(struct TerList**) malloc(sizeof(struct TerList)*__size); 
	for (int i = 0; i < __size; i++){
		struct TerList *_ptrTemp = (struct TerList*) malloc(sizeof(struct TerList));
		_ptrTemp->Next = NULL;	
		_output[i] = _ptrTemp;
	}
	return _output;
}

void PrintTerList(struct TerList **__ptrHeader, int __TokenCount){
	struct TerList *_ptrTemp;

	for (int i  = 0;i<__TokenCount; i++){
		_ptrTemp = __ptrHeader[i];
		while (_ptrTemp->Next!=NULL){
			printf("%4d", _ptrTemp->Value);
			_ptrTemp = _ptrTemp->Next;
		}
			printf("\n");
	}

}