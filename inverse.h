#include<stdio.h>
#include<math.h>
#define N 4
int determinant(int[][25], int);
void cofactor(int[][25], int);
void transpose2(int[][25], int[][25], int);
int determinant(int a[25][25], int k)
{
	int s = 1, det = 0, b[25][25];
	int i, j, m, n, c;
	if (k == 1)
	{
		return (a[0][0]);
	}
	else
	{
		det = 0;
		for (c = 0; c < k; c++)
		{
			m = 0;
			n = 0;
			for (i = 0; i < k; i++)
			{
				for (j = 0; j < k; j++)
				{
					b[i][j] = 0;
					if (i != 0 && j != c)
					{
						b[m][n] = a[i][j];
						if (n < (k - 2))
							n++;
						else
						{
							n = 0;
							m++;
						}
					}
				}
			}
			det = det + s * (a[0][c] * determinant(b, k - 1));
			s = -1 * s;
		}
	}

	return (det);
}
void cofactor(int num[25][25], int f, int result[25][25])
{
	int b[25][25], fac[25][25];
	int p, q, m, n, i, j;
	for (q = 0; q < f; q++)
	{
		for (p = 0; p < f; p++)
		{
			m = 0;
			n = 0;
			for (i = 0; i < f; i++)
			{
				for (j = 0; j < f; j++)
				{
					if (i != q && j != p)
					{
						b[m][n] = num[i][j];
						if (n < (f - 2))
							n++;
						else
						{
							n = 0;
							m++;
						}
					}
				}
			}
			fac[q][p] = pow(-1, q + p) * determinant(b, f - 1);
		}
	}
	transpose2(num, fac, f, result);
}
/*Finding transpose of matrix*/
void transpose2(int num[25][25], int fac[25][25], int r, int result[25][25])
{
	int i, j;
	int b[25][25], inverse[25][25], d;

	for (i = 0; i < r; i++)
	{
		for (j = 0; j < r; j++)
		{
			b[i][j] = fac[j][i];
		}
	}
	d = determinant(num, r);
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < r; j++)
		{
			result[i][j] = b[i][j] / d;
		}
	}
}

void getCofactor(int mat[N][N], int temp[N][N], int p, int q, int n)
{
	int i = 0, j = 0;

	// Looping for each element of the matrix 
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			//  Copying into temporary matrix only those element 
			//  which are not in given row and column 
			if (row != p && col != q)
			{
				temp[i][j++] = mat[row][col];

				// Row is filled, so increase row index and 
				// reset col index 
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

int determinantOfMatrix(int mat[N][N], int n)
{
	int D = 0; // Initialize result 

			   //  Base case : if matrix contains single element 
	if (n == 1)
		return mat[0][0];

	int temp[N][N]; // To store cofactors 

	int sign = 1;  // To store sign multiplier 

				   // Iterate for each element of first row 
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of mat[0][f] 
		getCofactor(mat, temp, 0, f, n);
		D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1);

		// terms are to be added with alternate sign
		sign = -sign;
	}

	return D;
}