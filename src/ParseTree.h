#include "stdafx.h"

struct ParseTree{
	struct ParseTree *Parent;
	struct ParseTree *Next;
	struct ParseTree *FirstChild;
	int value;
};

void Add(struct ParseTree *__Node, int *__input, int __countInput);
struct ParseTree* GetNextInStack(struct ParseTree *__Node);
struct ParseTree *CreateParseTree();
void PrintTree(struct ParseTree *_ptrHeader);