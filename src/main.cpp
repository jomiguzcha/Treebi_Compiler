#include "stdafx.h"
#include "Stack.h"
#include <iostream>
#include "ParseTree.h"
#include "ParsingTable.h"
#include "scanner.h"
#include <stdlib.h>

/* The grammer:
S=A
S=M
S=B
S=func
S=K
S=Q
A=T id = L
A=id.C
A=id%id
T=string
T=int
T=float
T=bool
T=tree
L=number_literal
L=string_literal
C=avl
C=simple
M=RE
E=+RE
E=-RE
E=Eps
R=FD
D=*FD
D=/FD
D=Eps
F=id
F=number_literal
O=F<F
O=F>F
O=F<=F
O=F>=F
I=P==P
I=P!=P
P=id
P=number_literal
P=string_literal
B=PG
G=++PG
G=--PG
G=Eps
H=id===id
H=id!!=id
J=O
J=I
J=H
K=if(J){S}else{S}
Q=for(int id = number_literal ;J;M){S}

The Nonterminals are mapped as follow:
[S, 0]
[A, 1]
[T, 2]
[L, 3]
[C, 4]
[M, 5]
[E, 6]
[R, 7]
[D, 8]
[F, 9]
[O, 10]
[I, 11]
[P, 12]
[B, 13]
[G, 14]
[H, 15]
[J, 16]
[K, 17]
[Q, 18]

The Terminals are mapped as follow:
[func, 19]
[id, 20]
[=, 21]
[., 22]
[%, 23]
[string, 24]
[int, 25]
[float, 26]
[bool, 27]
[tree, 28]
[number_literal, 29]
[string_literal, 30]
[avl, 31]
[simple, 32]
[+, 33]
[-, 34]
[*, 35]
[/, 36]
[<, 37]
[>, 38]
[!, 39]
[(, 40]
[), 41]
[{, 42]
[}, 43]
[if, 44]
[else, 45]
[for, 46]
[;, 47]
[Eps, 48]
[$, 49]

*/

struct ParseTree* Parse(int** __Grammers, int* __Rows, int __countRows, 
	int __countTerminals, int __countNonterminals, int* __intInput, int __CountInput) {

	int** _parseTable;
	struct stack* _ItemStack = (struct stack*)malloc(sizeof(struct stack));
	int i;
	int* _production;
	struct ParseTree* _ptrHeader;
	int* _adjInput = (int*)malloc(sizeof(int) * (__CountInput + 1));
	struct ParseTree* _ptrCurrentNode;

	int M_a;
	int _countProductionOutput;
	_parseTable = Generate(__Grammers, __Rows, __countRows, __countTerminals, __countNonterminals);
	Push(&_ItemStack, __countTerminals + __countNonterminals + 1);
	_ptrCurrentNode = CreateParseTree();
	_ptrCurrentNode->value = 0;
	_ptrHeader = _ptrCurrentNode;

	Push(&_ItemStack, 0);
	i = 0;

	for (int i = 0; i < __CountInput; i++) {
		_adjInput[i] = __intInput[i];
	}
	_adjInput[__CountInput] = __countNonterminals + __countNonterminals + 1;

	while (_ItemStack->value != __countTerminals + __countNonterminals + 1) {
		if (_ItemStack->value == _adjInput[i]) {
			_ptrCurrentNode = GetNextInStack(_ptrCurrentNode);
			Pop(&_ItemStack);

			i++;
		}
		else if (_ItemStack->value == __countNonterminals + __countNonterminals) {
			_ptrCurrentNode = GetNextInStack(_ptrCurrentNode);
			Pop(&_ItemStack);
		}
		else if (_ItemStack->value >= __countNonterminals) {
			printf("Error, Terminals don't match\n");
			abort();
		}
		else if (_parseTable[_ItemStack->value][_adjInput[i] - __countNonterminals] == -1) {
			printf("Error, Error entry in parse table\n");
			abort();
		}
		else {
			if (_adjInput[i] == __countNonterminals + __countTerminals + 1) {
				M_a = _parseTable[_ItemStack->value][_adjInput[i] - __countNonterminals - 1];
			}
			else {
				M_a = _parseTable[_ItemStack->value][_adjInput[i] - __countNonterminals];
			}
			Pop(&_ItemStack);
			_production = GetProduction(__Grammers, M_a, __countTerminals + __countNonterminals + 1, &_countProductionOutput);
			Add(_ptrCurrentNode, _production, _countProductionOutput);
			_ptrCurrentNode = _ptrCurrentNode->FirstChild;
			for (int j = _countProductionOutput - 1; j >= 0; j--) {
				if (_production[j] != -1) {
					Push(&_ItemStack, _production[j]);
				}
			}
		}
	}

	/*for (int i = 0; i < __countNonterminals; ++i) {
		for (int j = 0; j < (__countTerminals+1); ++j) {
			std::cout << _parseTable[i][j] << " ";
		}
		std::cout << std::endl << std::endl;
	}*/

	return _ptrHeader;
}

int** InitializeMatrix(int __RowCount, int __ColCount) {
	int** _matOutput = (int**)malloc(sizeof(int*) * __RowCount);
	for (int i = 0; i < __ColCount; i++) {
		_matOutput[i] = (int*)malloc(sizeof(int) * __ColCount);
	}
	for (int i = 0; i < __RowCount; i++) {
		for (int j = 0; j < __ColCount; j++) {
			_matOutput[i][j] = -1;
		}
	}
	return _matOutput;
}

void PrintMat(int** __matInput, int __RowCount, int __ColCount) {
	char* _strPrint = (char*)malloc(sizeof(char) * 1000);
	char* _num2str = (char*)malloc(sizeof(char) * 10);
	_num2str[0] = '\0';
	_strPrint[0] = '\0';
	for (int i = 0; i < __RowCount; i++) {
		for (int j = 0; j < __ColCount; j++) {

			printf("%4d", __matInput[i][j]);
		}
		printf("\n");
	}
}

int main(){

	std::pair <int*, int> _scanner;
	_scanner = tokens_to_input();

	//The grammer matrix
	int **Grammer = InitializeMatrix(48, 50);

	//The index of the Nonterminal for each row of the grammer matrix
	int *Rows = ((int *)malloc(sizeof(int)*48));
	int* _input = _scanner.first;
	struct ParseTree *_ptrResult;

	//S = A
	Grammer[0][1] = 0;
	Rows[0] = 0;

	//S = M
	Grammer[1][5] = 0;
	Rows[1] = 0;

	//S = B
	Grammer[2][13] = 0;
	Rows[2] = 0;

	//S = func
	Grammer[3][19] = 0;
	Rows[3] = 0;

	//S = K
	Grammer[4][17] = 0;
	Rows[4] = 0;

	//S = Q
	Grammer[5][18] = 0;
	Rows[5] = 0;

	//A = T id = L
	Grammer[6][2] = 0;
	Grammer[6][20] = 1;
	Grammer[6][21] = 2;
	Grammer[6][3] = 3;
	Rows[6] = 1;

	//A = id.C
	Grammer[7][20] = 0;
	Grammer[7][22] = 1;
	Grammer[7][4] = 2;
	Rows[7] = 1;

	//A = id % id
	Grammer[8][20] = 0;
	Grammer[8][23] = 1;
	Grammer[8][20] = 2;
	Rows[8] = 1;

	//T = string
	Grammer[9][24] = 0;
	Rows[9] = 2;

	//T = int
	Grammer[10][25] = 0;
	Rows[10] = 2;

	//T = float
	Grammer[11][26] = 0;
	Rows[11] = 2;

	//T = bool
	Grammer[12][27] = 0;
	Rows[12] = 2;

	//T = tree
	Grammer[13][28] = 0;
	Rows[13] = 2;

	//L = number_literal
	Grammer[14][29] = 0;
	Rows[14] = 3;

	//L = string_literal
	Grammer[15][30] = 0;
	Rows[15] = 3;

	//C = avl
	Grammer[16][31] = 0;
	Rows[16] = 4;

	//C = simple
	Grammer[17][32] = 0;
	Rows[17] = 4;

	//M = RE
	Grammer[18][7] = 0;
	Grammer[18][6] = 1;
	Rows[18] = 5;

	//E = +RE
	Grammer[19][33] = 0;
	Grammer[19][7] = 1;
	Grammer[19][6] = 2;
	Rows[19] = 6;

	//E = -RE
	Grammer[20][34] = 0;
	Grammer[20][7] = 1;
	Grammer[20][6] = 2;
	Rows[20] = 6;

	//E = Eps
	Grammer[21][48] = 0;
	Rows[21] = 6;

	//R = FD
	Grammer[22][9] = 0;
	Grammer[22][8] = 1;
	Rows[22] = 7;

	//D = *FD
	Grammer[23][35] = 0;
	Grammer[23][9] = 1;
	Grammer[23][8] = 2;
	Rows[23] = 8;

	//D = / FD
	Grammer[24][36] = 0;
	Grammer[24][9] = 1;
	Grammer[24][8] = 2;
	Rows[24] = 8;

	//D = Eps
	Grammer[25][48] = 0;
	Rows[25] = 8;

	//F = id
	Grammer[26][20] = 0;
	Rows[26] = 9;

	//F = number_literal
	Grammer[27][29] = 0;
	Rows[27] = 9;

	//O = F < F
	Grammer[28][9] = 0;
	Grammer[28][37] = 1;
	Grammer[28][9] = 2;
	Rows[28] = 10;

	//O = F > F
	Grammer[29][9] = 0;
	Grammer[29][38] = 1;
	Grammer[29][9] = 2;
	Rows[29] = 10;

	//O = F <= F
	Grammer[30][9] = 0;
	Grammer[30][37] = 1;
	Grammer[30][21] = 2;
	Grammer[30][9] = 3;
	Rows[30] = 10;

	//O = F >= F
	Grammer[31][9] = 0;
	Grammer[31][38] = 1;
	Grammer[31][21] = 2;
	Grammer[31][9] = 3;
	Rows[31] = 10;

	//I = P == P
	Grammer[32][12] = 0;
	Grammer[32][21] = 1;
	Grammer[32][21] = 2;
	Grammer[32][12] = 3;
	Rows[32] = 11;

	//I = P != P
	Grammer[33][12] = 0;
	Grammer[33][39] = 1;
	Grammer[33][21] = 2;
	Grammer[33][12] = 3;
	Rows[33] = 11;

	//P = id
	Grammer[34][20] = 0;
	Rows[34] = 12;

	//P = number_literal
	Grammer[35][29] = 0;
	Rows[35] = 12;

	//P = string_literal
	Grammer[36][30] = 0;
	Rows[36] = 12;

	//B = PG
	Grammer[37][12] = 0;
	Grammer[37][14] = 1;
	Rows[37] = 13;

	//G = ++PG
	Grammer[38][33] = 0;
	Grammer[38][33] = 1;
	Grammer[38][12] = 2;
	Grammer[38][14] = 3;
	Rows[38] = 14;

	//G = --PG
	Grammer[39][34] = 0;
	Grammer[39][34] = 1;
	Grammer[39][12] = 2;
	Grammer[39][14] = 3;
	Rows[39] = 14;

	//G = Eps
	Grammer[40][48] = 0;
	Rows[40] = 14;

	//H = id == = id
	Grammer[41][20] = 0;
	Grammer[41][21] = 1;
	Grammer[41][21] = 2;
	Grammer[41][21] = 3;
	Grammer[41][20] = 4;
	Rows[41] = 15;

	//H = id!!= id
	Grammer[42][20] = 0;
	Grammer[42][39] = 1;
	Grammer[42][39] = 2;
	Grammer[42][21] = 3;
	Grammer[42][20] = 4;
	Rows[42] = 15;

	//J = O
	Grammer[43][10] = 0;
	Rows[43] = 16;

	//J = I
	Grammer[44][11] = 0;
	Rows[44] = 16;

	//J = H
	Grammer[45][15] = 0;
	Rows[45] = 16;

	//K = if (J) { S }else{ S }
	Grammer[46][44] = 0;
	Grammer[46][40] = 1;
	Grammer[46][16] = 2;
	Grammer[46][41] = 3;
	Grammer[46][42] = 4;
	Grammer[46][0] = 5;
	Grammer[46][43] = 6;
	Grammer[46][45] = 7;
	Grammer[46][42] = 8;
	Grammer[46][0] = 9;
	Grammer[46][43] = 10;
	Rows[46] = 17;

	//Q = for (T id = number_literal; J; M) { S }
	Grammer[47][46] = 0;
	Grammer[47][40] = 1;
	Grammer[47][2] = 2;
	Grammer[47][20] = 3;
	Grammer[47][21] = 4;
	Grammer[47][29] = 5;
	Grammer[47][47] = 6;
	Grammer[47][16] = 7;
	Grammer[47][47] = 8;
	Grammer[47][5] = 9;
	Grammer[47][41] = 10;
	Grammer[47][42] = 11;
	Grammer[47][0] = 12;
	Grammer[47][43] = 13;
	Rows[47] = 18;

	/*for (int i = 0; i < 48; ++i) {
		for (int j = 0; j < 50; ++j) {
			std::cout << Grammer[i][j]<<" ";
		}
		std::cout << std::endl;
	}*/

	/*for (int i = 0; i < 48; ++i) {
		std::cout << Rows[i]<< " ";
	}*/

	_ptrResult=Parse(Grammer, Rows, 48, 31, 19, _input, _scanner.second);
	PrintTree(_ptrResult);
	return 0;
}
