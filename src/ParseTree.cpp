#include "stdafx.h"
#include <Windows.h>
#include "ParseTree.h"

void PrintTreeAux(struct ParseTree *__ptrHeader, int __Recurse);

void Add(struct ParseTree *__Node, int *__input, int __countInput){
	struct ParseTree *_ptrTemp;
	struct ParseTree *_ptrNew;
	for (int i = 0; i < __countInput;i++){
		_ptrNew = (struct ParseTree*)malloc(sizeof(struct ParseTree));
		_ptrNew->value = __input[i];
		_ptrNew->FirstChild =NULL;
		_ptrNew->Next = NULL;
		if (__Node->FirstChild == NULL){
			__Node->FirstChild = _ptrNew;
			_ptrNew->Parent = __Node;
		}
		else{
			_ptrTemp = __Node->FirstChild;
			while (_ptrTemp->Next !=NULL){
				_ptrTemp= _ptrTemp->Next;
			}
			_ptrTemp->Next = _ptrNew;
			_ptrNew->Parent =_ptrTemp->Parent;
		}
	}
}

struct ParseTree* GetNextInStack(struct ParseTree *__Node){
	int _flag =1;
	while (_flag ==1){
		if (__Node->Next!=NULL){
			__Node = __Node->Next;
			return __Node;
		}
		else if(__Node->Parent !=NULL){
			return  GetNextInStack(__Node->Parent);
		}
		else{
			return NULL;
		}
	}
	
}

struct ParseTree *CreateParseTree(){
	struct ParseTree *_ptrOutput = (struct ParseTree *)malloc(sizeof(struct ParseTree));
	_ptrOutput->Parent = NULL;
	_ptrOutput->FirstChild = NULL;
	_ptrOutput->Next = NULL;
	_ptrOutput->value =-1;
	return _ptrOutput;
}

void PrintTree(struct ParseTree *__ptrHeader){
	printf("Arbol AST Generado\n");
	PrintTreeAux(__ptrHeader, 0);
}

void PrintTreeAux(struct ParseTree *__ptrHeader, int __Recurse){
	struct ParseTree *_ptrTemp;
	char *_strSlash = (char *)malloc(sizeof(char)*100);

	_ptrTemp = __ptrHeader;
	for (int i =0; i < __Recurse; i ++){
		_strSlash[i] ='\\';
	}
	_strSlash[__Recurse] = '\0';
	printf(_strSlash);
	printf("%d\n",_ptrTemp->value);
	if (__ptrHeader->FirstChild!=NULL){
		__Recurse ++;
		PrintTreeAux(__ptrHeader->FirstChild, __Recurse);
		__Recurse--;
	}
	if (__ptrHeader->Next!=NULL){
		
		PrintTreeAux(__ptrHeader->Next, __Recurse);
	}
	
}
