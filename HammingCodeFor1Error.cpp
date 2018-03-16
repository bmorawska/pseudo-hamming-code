#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

#define WORDLENGTH	(8)
#define PARITYBITS	(4)


void zeroArray (int A[], int length)
{
	for (int zeros = 0; zeros < length; zeros++)
	{
		A[zeros] = 0;
	}
}

void addParityBits(const int H[][WORDLENGTH + PARITYBITS], const int word[], int codeWord[])
{
	for(int i = 0; i < WORDLENGTH; i++)
	{
		codeWord[i] = word[i];
	}
	for (int i = WORDLENGTH; i < WORDLENGTH + PARITYBITS; i++)
	{
		codeWord[i] = 0;
		for (int j = 0; j < WORDLENGTH; j++)
		{
			codeWord[i] ^= word[j] & H[i-WORDLENGTH][j];
		}
	}
}

void createInterference(int codeWord[])
{
	int blad = rand() % WORDLENGTH;

	if (codeWord[blad] == 1)
		codeWord[blad] = 0;
	else
		codeWord[blad] = 1;

	cout << "Blad jest na bicie: " << blad + 1 << endl;
}

void printWord(const string co, const int message[], int length)
{
	cout << co << ": ";
	for (int i = 0; i < length; i++)
		cout << message[i] << " ";

	cout << endl;
}


bool isItZeroMatrix(int interference[])
{
	for(int i = 0; i < WORDLENGTH + PARITYBITS; i++)
	{
		if(interference[i] != 0)
			return (false);
	}
	return (true);
}

bool isError(const int codeWord[], const int H[][WORDLENGTH + PARITYBITS], int interference[])
{
	for (int i = 0; i < PARITYBITS; i++)
	{
		for (int j = 0; j <  WORDLENGTH + PARITYBITS; j++)
		{
			interference[i] += codeWord[j] * H[i][j];
		}
		interference[i] %= 2;
	}

	printWord("suma ctrl", interference, PARITYBITS);

	if (isItZeroMatrix(interference))
		return (false);
	else
		return (true);
}

int binaryToDecimal (const int interference[])
{
	int decimal = 0;
	for (int n = 0; n < PARITYBITS; n++)
	{
		if (interference[n] == 1)
			decimal += pow(2, n);
	}
	if (decimal > WORDLENGTH) {
		decimal %= WORDLENGTH;
	}
	cout << "Decimal: " << decimal << "\n";
	return (decimal - 1);
}


void fixIt(int codeWord[], const int interference[])
{
	int whereIsError = binaryToDecimal(interference);
	if(codeWord[whereIsError] == 0)
		codeWord[whereIsError] = 1;
	else
		codeWord[whereIsError] = 0;
}

int main(int argc, char * argv[]) {


	//Macierz do korekty 1 błędu (8 bitów informacji + 4 bity kontrolne)
	const int H1[4][12] =
						{{ 1, 0, 1, 0, 	1, 0, 1, 0, 	1, 0, 0, 0 },
						 { 0, 1, 1, 0, 	0, 1, 1, 0, 	0, 1, 0, 0 },
			    		 { 0, 0, 0, 1, 	1, 1, 1, 0, 	0, 0, 1, 0 },
						 { 0, 0, 0, 0, 	0, 0, 0, 1, 	0, 0, 0, 1 } };
	//Macierz do korekty 2 błędów (8 bitów informacji + 9 bitów kontrolnych)
	const int H2[9][17] =
						{{0, 0, 1, 1,	 0, 0, 0, 1,	1, 0, 0, 0, 	0, 0, 0, 0, 	0},
						 {1, 1, 1, 1, 	 0, 1, 0, 0,	0, 1, 0, 0, 	0, 0, 0, 0, 	0},
						 {1, 1, 0, 0,	 1, 0, 0, 1,	0, 0, 1, 0,	    0, 0, 0, 0, 	0},
						 {0, 0, 1, 0,	 1, 0, 1, 0,	0, 0, 0, 1,	    0, 0, 0, 0, 	0},
						 {1, 0, 1, 1,	 1, 1, 0, 1,	0, 0, 0, 0,	    1, 0, 0, 0, 	0},
						 {0, 1, 0, 1,	 1, 1, 1, 1,	0, 0, 0, 0,		0, 1, 0, 0, 	0},
						 {1, 1, 1, 1,	 1, 1, 1, 0,	0, 0, 0, 0,		0, 0, 1, 0, 	0},
						 {0, 0, 1, 0,	 1, 1, 0, 1,	0, 0, 0, 0,		0, 0, 0, 1, 	0},
						 {0, 1, 0, 1,	 1, 0, 1, 0,	0, 0, 0, 0,		0, 0, 0, 0, 	1}};

	srand( time( NULL ) );
	int word[8] = { 1, 0, 1, 1, 1, 1, 1, 0};					//slowo
	int codeWord[WORDLENGTH + PARITYBITS];						//slowo kodowe
	int interference[PARITYBITS];								//tablica wskazująca zakłócenie

	//pozbycie się śmieci z tablic
	zeroArray(codeWord, (WORDLENGTH + PARITYBITS));
	zeroArray(interference, PARITYBITS);

	addParityBits(H1, word, codeWord);
	//encodeWord(codeWord,word,G1);
	printWord("zakodowane", codeWord, WORDLENGTH + PARITYBITS);
	createInterference(codeWord);
	printWord("zepsute   ", codeWord, WORDLENGTH + PARITYBITS);

	if(isError(codeWord, H1, interference))
	{
		fixIt(codeWord, interference);
	}
	printWord("poprawione", codeWord, WORDLENGTH + PARITYBITS);

	return 0;
}

