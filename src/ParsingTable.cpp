#include "stdafx.h"
#include "TerList.h"
#include <Windows.h>

void SetFirstAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas);
void TestPrint(int *__Output, int __RowCount, char *header);

void SetFollowAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas, struct TerList **__FollowAlphas);
int GetItemInProduction(int **__Grammer, int __RowIndex, int __ColumnCount, int __ItemIndex);

int **Generate(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount){
	struct TerList **_FirstAlphas = InitializeArr( __TerminalCount+__NonTerminalCount+1);
	struct TerList **_FollowAs = InitializeArr(__NonTerminalCount);
	int **_output = (int **) malloc(sizeof(int *)*__NonTerminalCount);

	int _ItemInProduction;
	struct TerList * _ptrFirstTemp;
	struct TerList * _ptrFollowTemp;
	int _flagEpsFound;
	for (int i = 0; i <__NonTerminalCount;i++){
		_output[i] = (int *) malloc(sizeof(int)*(__TerminalCount+1));
	}

	for (int i = 0; i <__NonTerminalCount;i++){
		for (int j =0; j<(__TerminalCount+1);j++){
			_output[i][j] = -1;
		}
	}

	SetFirstAs(__Grammer, __Rows, __RowCount, __TerminalCount, __NonTerminalCount, _FirstAlphas);
	SetFollowAs(__Grammer, __Rows, __RowCount, __TerminalCount, __NonTerminalCount, _FirstAlphas, _FollowAs);

	for (int j = 0; j < __RowCount;j ++){
		for (int i = 0; i<__NonTerminalCount;i++){
			if (i==__Rows[j]){
				for (int k=0; k <__NonTerminalCount+__TerminalCount+1;k++){
					_ItemInProduction =  GetItemInProduction(__Grammer, j, __NonTerminalCount+__TerminalCount+1, k);
					if (_ItemInProduction == -1){

						_ptrFollowTemp = _FollowAs[i];
						while(_ptrFollowTemp->Next !=NULL){
							if (_ptrFollowTemp->Value==__NonTerminalCount+__TerminalCount +1){
								_output[i][_ptrFollowTemp->Value - __NonTerminalCount-1] = j;
							}
							else
							{
								_output[i][_ptrFollowTemp->Value - __NonTerminalCount] = j;
							}

							_ptrFollowTemp = _ptrFollowTemp->Next;							
						}
						k = __NonTerminalCount+__TerminalCount+1;
						i = __NonTerminalCount;
					}
					else{
						_flagEpsFound = 0;
						_ptrFirstTemp = _FirstAlphas[_ItemInProduction];
						while (_ptrFirstTemp->Next!=NULL){
							if (_ptrFirstTemp->Value !=__NonTerminalCount + __TerminalCount){
								_output[i][_ptrFirstTemp->Value-__NonTerminalCount] = j;
							}
							else{
								_flagEpsFound =1;
							}
							_ptrFirstTemp = _ptrFirstTemp->Next;
						}
						if (_flagEpsFound ==0){
							k = __NonTerminalCount+__TerminalCount+1;
							i = __NonTerminalCount;
						}
					}
					
				}
			}
		}
	}
	return _output;
}

int GetItemInProduction(int **__Grammer, int __RowIndex, int __ColumnCount, int __ItemIndex){
	for (int i =0; i <__ColumnCount; i++){
		if (__Grammer[__RowIndex][i] == __ItemIndex){
			return i;
		}
	}
	return -1;
}

void SetFollowAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas, struct TerList **__FollowAlphas){
	int _flagChanged;
	int j;
	struct TerList *_ptrFirst;
	int _itemIndex;
	int _itemFollowIndex;
	int _flagEpsFound;
	Add(__FollowAlphas[0], __TerminalCount + __NonTerminalCount+1);
	_flagChanged = 1;
	while (_flagChanged !=0){
		_flagChanged =0;
		for (int m = 0; m<__RowCount;m++){
			for (int i = 0;  i< __TerminalCount+__NonTerminalCount+1;i++){
				_itemIndex = GetItemInProduction(__Grammer, m, __TerminalCount+__NonTerminalCount+1, i);
				if (_itemIndex ==-1){
					i = __TerminalCount+__NonTerminalCount+1;			
				}
				else if (_itemIndex < __NonTerminalCount){
					for ( j = i+1; j <  __TerminalCount+__NonTerminalCount+1; j++){
						_flagEpsFound = 0;
						_itemFollowIndex = GetItemInProduction(__Grammer, m, __TerminalCount+__NonTerminalCount+1, j);
						if (_itemFollowIndex==-1){
							for (int k = 0; k < __NonTerminalCount; k++){
								if (__Rows[m] ==k){
									_ptrFirst = __FollowAlphas[k];
									while (_ptrFirst->Next !=NULL){		
										_flagChanged+=Add(__FollowAlphas[_itemIndex], _ptrFirst->Value);		
										_ptrFirst = _ptrFirst->Next;
									}
									if (j == i+1){
										_ptrFirst = __FollowAlphas[_itemIndex];
										while (_ptrFirst->Next !=NULL){		
											_flagChanged+=Add(__FollowAlphas[k], _ptrFirst->Value);		
											_ptrFirst = _ptrFirst->Next;
										}
									}
								}
							}
						}
						else{
							_ptrFirst = __FirstAlphas[_itemFollowIndex];
							while (_ptrFirst->Next !=NULL){
								if ( _ptrFirst->Value!=__TerminalCount+__NonTerminalCount){
									_flagChanged+=Add(__FollowAlphas[_itemIndex], _ptrFirst->Value);					
								}
								else{
									_flagEpsFound =1;
								}
								_ptrFirst = _ptrFirst->Next;
							}
							if (_flagEpsFound==0){
								j = __TerminalCount+__NonTerminalCount+1;
							}
						}
					}
				}
			}
		}
	}
	
}

void SetFirstAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas){
	int _flagEpsFound;
	int _itemIndex;
	int _flagChanged;
	struct TerList *_ptrFirstTemp;

	for (int i = __NonTerminalCount ; i < __TerminalCount+ __NonTerminalCount+1;i++){
		Add(__FirstAlphas[i], i);
	}

	_flagChanged = 1;
	while (_flagChanged !=0){
		_flagChanged =0;
		for (int i = 0 ; i < __RowCount; i ++){
			for (int j= 0; j < __NonTerminalCount; j ++){
				if (__Rows[i] == j){
					for (int k = 0; k < __NonTerminalCount + __TerminalCount + 1;k ++){
						_itemIndex = GetItemInProduction(__Grammer, i, __NonTerminalCount + __TerminalCount+1, k);
						if (_itemIndex == -1){
							_flagChanged+=Add(__FirstAlphas[j],  __NonTerminalCount + __TerminalCount);
							k= __NonTerminalCount + __TerminalCount + 1;
							j = __NonTerminalCount;
						}
						else{
							_ptrFirstTemp = __FirstAlphas[_itemIndex];
							_flagEpsFound = 0;
							while (_ptrFirstTemp->Next !=NULL){
								if (_ptrFirstTemp->Value != __NonTerminalCount+__TerminalCount){
									_flagChanged+=Add(__FirstAlphas[j], _ptrFirstTemp->Value);
								}
								else{
									_flagEpsFound =1;
								}
								_ptrFirstTemp = _ptrFirstTemp->Next;
							}
							if (_flagEpsFound ==0){
								k= __NonTerminalCount + __TerminalCount + 1;
								j = __NonTerminalCount;
							}
						}
						
					}
				}
			}
		}
	
	}
	
		
}

int * GetProduction(int **__Grammer, int __rowIndex, int __countColumns, int *__countOutput){
	int *_output = (int *)malloc(sizeof(int)*__countColumns);
	for (int i = 0; i < __countColumns; i++){
		_output[i] = GetItemInProduction(__Grammer, __rowIndex, __countColumns, i);
		if (_output[i] ==-1){
			*__countOutput = i;
			break;
		}
	}

	return _output;
}

void TestPrint(int *__Output, int __RowCount, char *header){
	printf(header);
	for (int i = 0; i<__RowCount; i++){
		printf("%d\n", __Output[i]);
	}

}