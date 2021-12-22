#include "stdafx.h"

struct TerList{
	struct TerList *Next;
	int Value;
};

struct TerList **InitializeArr(int __size);
int Add(struct TerList *__ptrHeader, int __intValue);
void PrintTerList(struct TerList **__ptrHeader, int __RowCount);
