#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
#include "inverse.h"

int main(void) {

	int Goppa[4][7] = { { 1,0,0,0,1,1,0 },{ 0,1,0,0,1,0,1 },{ 0,0,1,0,0,1,1 },{ 0,0,0,1,1,1,1 } };  //Goppa Code
	int PublicKey[4][7]; // PublicKey
	int Chiper[7]; //Chiper text
	int text[4]; // Plain text
	int(*P)[7] = MatrixPGeneration(); // Permutation Matrix as a Private Key
	int(*S)[4] = MatrixSGeneration(); // Invertible Matrix as a Private Key
	int(*ParrityCheck)[7] = MakeParrityCheckMatrix(Goppa); // ParrityCheck Matrix of Goppa Matrix for Decryption
	int(*TransPoseMatrix)[3] = Transpose(ParrityCheck);

	//Get InverseMatrix Of S(4X4) for Decryption
	int InverseS[4][4];
	int TempInverseS[25][25];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			TempInverseS[i][j] = S[i][j];
	int(*TempInverseS1)[25] = GenerateInverse(TempInverseS, 4);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			InverseS[i][j] = TempInverseS1[i][j];
		}
	//Get InverseMatrix Of P(7X7) for Decryption
	int InverseP[7][7];
	int TempInverseP[25][25];
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			TempInverseP[i][j] = P[i][j];
	int(*TempInverseP1)[25] = GenerateInverse(TempInverseP, 7);

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++) {
			InverseP[i][j] = TempInverseP1[i][j];
		}

	printf("Input Your Message : ");
	for (int i = 0; i < 4; i++) {
		scanf("%d", &text[i]);
	}

	PublicKeyGeneration(S, Goppa, P, PublicKey); //Generate Publickey using with P and S
	Encryption(text, PublicKey, Chiper); //Encrypt Message with Publickey and intended Error

	printf("\nGenerated Chipertext : ");
	for (int i = 0; i < sizeof(Chiper) / sizeof(int); i++)
		printf("%d ", Chiper[i]);
	printf("\n\n");

	Decryption(Chiper, InverseP, InverseS, TransPoseMatrix); // Decrypt ChiperText 

	return 0;
}

//Generate Random Permutation Matrix(7X7)
int MatrixPGeneration() {
	int Position[7];
	static int P[7][7] = { 0, };
	int check;
	for (int i = 0; i < 7; i++) {
		check = 1;
		int Randomnumber = rand() % 7;
		for (int j = 0; j < i; j++) {
			if (Randomnumber == Position[j])
				check = 0;
		}
		if (check == 1) {
			Position[i] = Randomnumber;
			P[i][Randomnumber] = 1;
		}
		else
			i--;
	}
	return P;
}

//Generate Random Invertible Matrix(4X4)
int MatrixSGeneration() {
	srand(time(NULL));
	static int S[4][4];
	int check = 0;
	while (check == 0) {
		for (int i = 0; i < 16; i++) {
			S[0][i] = rand() % 2;
		}
		check = determinantOfMatrix(S, 4);
	}
	return S;
}

//Generate Publickey with use Goppa Code, P Matrix and S Matrix
PublicKeyGeneration(int S[4][4], int Goppa[4][7], int P[7][7], int PublicKey[4][7]) {
	int Scol = sizeof(S[0]) / sizeof(int);
	int Gcol = sizeof(Goppa[0]) / sizeof(int);
	int Pcol = sizeof(P[0]) / sizeof(int);
	int Srow = 4;
	int Prow = 7;
	int sum;
	int temp[4][7];
	for (int i = 0; i < Srow; i++) {
		for (int j = 0; j < Gcol; j++)	{
			sum = 0;
			for (int p = 0; p < Scol; p++) {
				sum = sum + S[i][p] * Goppa[p][j];
			}
			temp[i][j] = sum % 2;
		}
	}

	int Crow = sizeof(temp) / sizeof(temp[0]);

	for (int i = 0; i < Crow; i++) {
		for (int j = 0; j < Pcol; j++) {
			sum = 0;
			for (int p = 0; p < Prow; p++) {
				sum = sum + temp[i][p] * P[p][j];
			}
			PublicKey[i][j] = sum % 2;
		}
	}
	printf("\nPublicKey\n");
	for (int i = 1; i < 29; i++) {
		printf("%d ", PublicKey[0][i-1]);
		if (i % 7 == 0)
			printf("\n");
	}
}

//Encrypt Message with PublicKey
Encryption(int m[4], int PublicKey[4][7], int Chiper[7]) {
	int sum;
	int error[7] = { 0,0,0,0,1,0,0 };
	for (int i = 0; i < 7; i++) {
		sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += m[j] * PublicKey[j][i];
		}
		Chiper[i] = sum % 2;
	}
	for (int i = 0; i < 7; i++) {
		Chiper[i] = Chiper[i] ^ error[i];
	}
}

//Decrypt ChiperText with PrivateKey
Decryption(int Chiper[7], int InverseP[7][7], int InverseS[4][4], int TransPoseParrityCheck[7][3]) {
	int TempMessage[7];
	int ErrorMatrix[3];
	int ErrorPosition = 0;
	int sum = 0;
	int PlainText[4];

	//Multiplication InverseMatrix of P
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			sum += Chiper[j] * InverseP[j][i];
		}
		TempMessage[i] = sum;
		sum = 0;
	}

	printf("Decoding Procedure 1 -> Chiper * Inverse P \n");
	for (int i = 0; i < 7; i++)
		printf("%d ", TempMessage[i]);
	printf("\n\n");

	//Syndrome Decoding(Find ErrorPosition)
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 7; j++) {
			sum += TempMessage[j] * TransPoseParrityCheck[j][i];
		}
		ErrorMatrix[i] = sum % 2;
		sum = 0;
	}
	ErrorPosition = ErrorMatrix[0] + ErrorMatrix[1] * 2 + ErrorMatrix[2] * 4;
	TempMessage[7 - ErrorPosition] = (TempMessage[7 - ErrorPosition] + 1) % 2; //Error Correction

	printf("Decoding Procedure 2 -> Find Error Postion(Syndrome Decoding) \n");
	for (int i = 0; i < 3; i++)
		printf("%d ", ErrorMatrix[i]);
	printf("\n\nErrorPosition : %d\n\n",ErrorPosition);

	printf("Error Correction ");
	for (int i = 0; i < 7; i++)
		printf("%d ", TempMessage[i]);

	//Multiplication InverseMatrix of S
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sum += TempMessage[j] * InverseS[j][i];
		}
		PlainText[i] = sum % 2;
		sum = 0;
	}
	printf("\n\nGet PlainText by Multiplication InverseMatrix of S....\n\nPlainText :");
	for (int i = 0; i < 4; i++)
		printf(" %d ", PlainText[i]);
}

//Make ParrityCheckMatrix with Goppa Code
int MakeParrityCheckMatrix(int Goppa[4][7]) {
	static int ParrityCheck[3][7] = { 0, };
	for (int i = 0; i < 4; i++) {
		for (int j = 4; j < 7; j++) {
			ParrityCheck[j - 4][i] = Goppa[i][j];
		}
		for (int i = 0; i < 3; i++) {
			ParrityCheck[i][i + 4] = 1;
		}
	}
	return ParrityCheck;
}

//Transpose ParrityCheck Matrix for Multiplication
int Transpose(int(*ParrityCheck)[7]) {
	static int TransPoseMatrix[7][3];
	for (int i = 2; i > -1; i--) {
		for (int j = 0; j < 7; j++) {
			TransPoseMatrix[j][2 - i] = ParrityCheck[i][j];
		}
	}
	return TransPoseMatrix;
}

//Find Inverse Matrix(maximum 25X25)
int GenerateInverse(int S[25][25], int size) {
	int TempInverse[25][25];
	int Inverse[25][25];
	cofactor(S, size, TempInverse);

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			Inverse[i][j] = ((TempInverse[i][j] + 2) % 2);
	return Inverse;
}