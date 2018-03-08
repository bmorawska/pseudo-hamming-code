#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;

void zeroArray (int *A, int length)
{
	for (int zeros = 0; zeros < length; zeros++)
	{
		A[zeros] = 0;
	}
}

void printWord(int wordLength, int parityBits, const int *codeWord)
{
	for (int i = 0; i < wordLength + parityBits; i++)
	{
		cout << codeWord[i] << " ";
	}
	cout << endl;
}

int main() {
	int wordLength = 4;											//Długość słowa
	int parityBits = 3;											//Bity parzystości
	int word[4] = { 1, 0, 1, 1};								//Slowo
	int codeWord[wordLength + parityBits];						//Slowo kodowe
	int interference[3];
	srand( time( NULL ) );

	//pozbycie się śmieci z tablic
	zeroArray(codeWord, (wordLength + parityBits));
	zeroArray(interference, 3);

	int H[parityBits][wordLength + parityBits];					//Macierz parzystości
	H[0][0] = 1;	H[0][1] = 0;	H[0][2] = 1;	H[0][3] = 0;	H[0][4] = 1;	H[0][5] = 0;	H[0][6] = 1;
	H[1][0] = 0;	H[1][1] = 1;	H[1][2] = 1;	H[1][3] = 0;	H[1][4] = 0;	H[1][5] = 1;	H[1][6] = 1;
	H[2][0] = 0;	H[2][1] = 0;	H[2][2] = 0;	H[2][3] = 1;	H[2][4] = 1;	H[2][5] = 1;	H[2][6] = 1;

	int G[wordLength][wordLength + parityBits];					//Macierz generująca słowa kodowe
	G[0][0] = 1;	G[0][1] = 0;	G[0][2] = 0;	G[0][3] = 0;	G[0][4] = 1;	G[0][5] = 1;	G[0][6] = 0;
	G[1][0] = 0;	G[1][1] = 1;	G[1][2] = 0;	G[1][3] = 0;	G[1][4] = 1;	G[1][5] = 0;	G[1][6] = 1;
	G[2][0] = 0;	G[2][1] = 0;	G[2][2] = 1;	G[2][3] = 0;	G[2][4] = 0;	G[2][5] = 1;	G[2][6] = 1;
	G[3][0] = 0;	G[3][1] = 0;	G[3][2] = 0;	G[3][3] = 1;	G[3][4] = 1;	G[3][5] = 1;	G[3][6] = 1;

	//Wypisanie wiadomosci
	cout << "Wiadomosc ma postac:                             ";
	for (int m = 0; m < 4; m++)
	{
		cout << word[m] << " ";
	}
	cout << endl;

	//Kodowanie słowa
	for (int i = 0; i < wordLength + parityBits; i++)
	{
		for (int j = 0; j < wordLength; j++)
		{
			codeWord[i] += word[j] * G[j][i];
		}
		codeWord[i] %= 2;
	}
	cout << "Slowo kodowe ma postac:                          ";
	printWord(wordLength, parityBits, codeWord);

	//Wprowadzanie losowych zakłóceń
	//int blad = rand() % (wordLength + parityBits);
	int blad = rand() % (wordLength);
	if (codeWord[blad] == 1)
	{
		codeWord[blad] = 0;
	}
	else
	{
		codeWord[blad] = 1;
	}

	cout << "Po transmisji narazonej na zaklocenia ma postac: ";
	printWord(wordLength, parityBits, codeWord);

	//Odnalezienie zakłócenia

	for (int i = 0; i < parityBits; i++)
	{
		for (int j = 0; j <  wordLength + parityBits; j++)
		{
			interference[i] += H[i][j] * codeWord[j];
		}
		interference[i] %= 2;
	}
	cout << "Macierz wykrywajaca bledy wyglada tak:           ";
	for (int m = parityBits - 1; m >= 0; m--)
	{
		cout << interference[m] << " ";
	}

	cout << endl;

	//Analiza zakłócenia
	int bit = 0;
	for (int n = 0; n < parityBits; n++)	//de facto zamiana z liczby binarnej na dziesietna
	{
		if (interference[n] == 1)
		{
			bit += pow(2, n);
		}
	}
	bit -= 1; 								//wynika to z faktu, że tablice indeksujemy od zera, a nie od 1

	cout << "Skad wniosek, ze przeklamanie znajduje sie na:   " << bit + 1<< " bicie." << endl;

	//Poprawiamy zakłócenie
	if (codeWord[bit] == 1)
	{
		codeWord[bit] = 0;
	}
	else
	{
		codeWord[bit] = 1;
	}

	cout << "Po poprawieniu slowo kodowe ma postac:           ";
	printWord(wordLength, parityBits, codeWord);

	//Przywrocenie tresci wiadomosci word
	cout << "W efekcie wiadomosc wyglada nastepujaco          ";
	int resetMsg[4];
	for (int m = 0; m < 4; m++)
	{
		resetMsg[m] = codeWord[m];
		cout << resetMsg[m] << " ";
	}
	cout << endl;

}
