#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <fstream>
#include <string>

#define WORDLENGTH	(8)
#define PARITYBITS	(5)

using namespace std;

/*
 * Wypełnia tablicę jednowymiarową o wzkazanej długości zerami.
 */
void zeroArray (int *A, int length)
{
	for (int zeros = 0; zeros < length; zeros++)
	{
		A[zeros] = 0;
	}
}

/*
 * Wypisuje na ekran konsoli tablicę jednowymiarową
 * o wskazanej długości.
 */
void printWord(const int *message, int length)
{
	for (int i = 0; i < length; i++)
	{
		cout << message[i] << " ";
	}
	cout << endl;
}

/*
 * Tworzy w słowie błąd na losowym bicie.
 * Dlaczego nie zabezpiecza sam siebie???
 */
void createInterference(int *codeWord)
{
//	int blad = rand() % (WORDLENGTH + PARITYBITS);
	int blad = rand() % (WORDLENGTH);
	if (codeWord[blad] == 1)
	{
		codeWord[blad] = 0;
	}
	else
	{
		codeWord[blad] = 1;
	}
	cout << "Blad jest na bicie: " << blad + 1 << " ale Ciiii! - to tajemnica. " << endl;
}

/*
 * Koduje słowo do postaci 12 bitowej, czyli takiej która po zakłóceniu
 * maksymalnie jednego bita może zostać odtworzona.
 */
void encodeWord(int *codeWord, int* word, const int G[][WORDLENGTH + PARITYBITS])
{
	for (int i = 0; i < WORDLENGTH + PARITYBITS; i++)
	{
		for (int j = 0; j < WORDLENGTH; j++)
		{
			codeWord[i] += word[j] * G[j][i];
		}
		codeWord[i] %= 2;
	}
}

/*
 * Na podstawie iloczynu macierzy zakodowanego słowa (codeWord) i macierzy parzystości H
 * tworzy macierz, która w sposób binarny zakodowuje, w któ©ej kolumnie nastąpiło zakłócenie.
 * Na przykład [1, 0, 0] oznacza, że bład wystąpił w pierszej kolumnie czyli na zerowym bicie.
 *
 * Uwaga! Ta macierz chyba nie potrafi zabezpieczyć samej siebie więc to co jej wyjdzie
 * 		  trzeba liczyć modulo 8. Wtedy działa ;-/
 */
void lookForInterference(int *interference, int *codeWord, const int H[][WORDLENGTH + PARITYBITS])
{
	for (int i = 0; i < PARITYBITS; i++)
	{
		for (int j = 0; j <  WORDLENGTH + PARITYBITS; j++)
		{
			interference[i] += codeWord[j] * H[i][j];
		}
		interference[i] %= 2;
	}
}

/*
 * Zamienia przekłamany bit na przeciwny.
 */
void fixError(int *codeWord, int bit)
{
	if (codeWord[bit] == 1)
	{
		codeWord[bit] = 0;
	}
	else
	{
		codeWord[bit] = 1;
	}
}

/*
 * Zamienia wartość z tablicy interference na liczbę w systemie dziesiętnym % 8 (dlaczego? -
 * czytaj opis lookForInterference) i naprawia błąd.
 */
void analizeInterference(int *interference, int *codeWord)
{
	int bit = 0;
	for (int n = 0; n < PARITYBITS; n++)	//de facto zamiana z liczby binarnej na dziesietna
	{
		if (interference[n] == 1)
		{
			bit += pow(2, n);
		}
	}
	bit -= 1; 								//wynika to z faktu, że tablice indeksujemy od zera, a nie od 1
	bit %= 8;								//no bo niestety bity parzystości nie zabezpieczają samych siebie

	cout << "Skad wniosek, ze przeklamanie znajduje sie na:   " << bit + 1<< " bicie." << endl;

	fixError(codeWord, bit);
}



int main() {

	srand( time( NULL ) );
	int word[8] = { 1, 0, 1, 1, 1, 1, 1, 0};					//slowo
	int codeWord[WORDLENGTH + PARITYBITS];						//slowo kodowe
	int interference[PARITYBITS];								//tablica wskazująca zakłócenie
	int resetMsg[WORDLENGTH];									//tablica, w której znajdzie się naprawione słowo

	//Macierz parzystości H
	const int H [PARITYBITS][WORDLENGTH + PARITYBITS] = { { 1, 0, 1, 0, 	1, 0, 1, 0, 	1, 0, 1, 0, 1 },
														  { 0, 1, 1, 0, 	0, 1, 1, 0, 	0, 1, 1, 0, 0 },
														  { 0, 0, 0, 1, 	1, 1, 1, 0, 	0, 0, 0, 1, 1 },
														  { 0, 0, 0, 0, 	0, 0, 0, 1, 	1, 1, 1, 1, 1 },
														  { 0, 0, 0, 0,     0, 0, 0, 0,     0, 0, 0, 0, 0 }};
	//Macierz generująca słowa kodowe
	const int G[WORDLENGTH][WORDLENGTH + PARITYBITS]  = { { 1, 0, 0, 0, 	0, 0, 0, 0,		1, 0, 0, 0, 0 },
														  { 0, 1, 0, 0, 	0, 0, 0, 0,		0, 1, 0, 0, 0 },
														  { 0, 0, 1, 0, 	0, 0, 0, 0,		1, 1, 0, 0, 0 },
														  { 0, 0, 0, 1, 	0, 0, 0, 0,		0, 0, 1, 0, 0 },
														  { 0, 0, 0, 0, 	1, 0, 0, 0,		1, 0, 1, 0, 0 },
														  { 0, 0, 0, 0, 	0, 1, 0, 0,		0, 1, 1, 0, 0 },
														  { 0, 0, 0, 0, 	0, 0, 1, 0,		1, 1, 1, 0, 0 },
														  { 0, 0, 0, 0, 	0, 0, 0, 1,		0, 0, 0, 1, 0 }};

	//pozbycie się śmieci z tablic
	zeroArray(codeWord, (WORDLENGTH + PARITYBITS));
	zeroArray(interference, PARITYBITS);
	zeroArray(resetMsg, WORDLENGTH);

	//Wypisanie wiadomosci
	cout << "Wiadomosc ma postac:                             ";
	printWord(word, WORDLENGTH);

	//Kodowanie słowa
	encodeWord(codeWord, word, G);
	cout << "Slowo kodowe ma postac:                          ";
	printWord(codeWord, WORDLENGTH + PARITYBITS);

	//Wprowadzanie losowych maksymalnie 2 zakłóceń
	createInterference(codeWord);
	createInterference(codeWord);
	cout << "Po transmisji narazonej na zaklocenia ma postac: ";
	printWord(codeWord, WORDLENGTH + PARITYBITS);

	//Odnalezienie zakłócenia
	lookForInterference(interference, codeWord, H);
	cout << "Macierz wykrywajaca bledy wyglada tak:           ";
	for (int m = PARITYBITS - 1; m >= 0; m--)	//Wypisywana od końca dla czytelności
	{
		cout << interference[m] << " ";
	}
	cout << endl;

	//Poprawienie błędu
	analizeInterference(interference, codeWord);
	cout << "Po poprawieniu slowo kodowe ma postac:           ";
	printWord(codeWord, WORDLENGTH + PARITYBITS);

	//Przywrocenie tresci wiadomosci word
	cout << "W efekcie wiadomosc wyglada nastepujaco          ";
	for (int m = 0; m < WORDLENGTH; m++)
	{
		resetMsg[m] = codeWord[m];
	}
	printWord(codeWord, WORDLENGTH);

	return 0;
}
