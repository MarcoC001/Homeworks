#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int Generate_N(int p, int q, int r);
int FFTv3(double *x_r, double *x_i, double *y_r, double *y_i, int N);
int Initial(double *x, double *y, int N);
int Print_Complex_Vector(double *y_r, double *y_i, int N);

int main()
{
	int k, n, p, q, r, N;
	double *y_r, *y_i, *x_r, *x_i, w_r, w_i;
	
	printf("Please input p q r=");
	scanf("%d %d %d", &p, &q, &r);
	N = Generate_N(p, q, r);
	printf("N=2^%d 3^%d 5^%d = %d\n",p,q,r,N);
	
	x_r = (double *) malloc(N*sizeof(double));
	x_i = (double *) malloc(N*sizeof(double));
	y_r = (double *) malloc(N*sizeof(double));
	y_i = (double *) malloc(N*sizeof(double));
	
	Initial(x_r, x_i, N);
	FFTv3(x_r, x_i, y_r, y_i, N);
	Print_Complex_Vector(y_r, y_i, N);
	
	return 0;
} 

int Generate_N(int p, int q, int r)
{
	int N = 1;
	for(;p>0;p--) N*=2;
	for(;q>0;q--) N*=3;
	for(;r>0;r--) N*=5;
	return N;
}

int Initial(double *x, double *y, int N)
{
	int n;
	for(n=0;n<N;++n)
	{
		x[n] = n;
		y[n] = 0.0;
	}
}

int Print_Complex_Vector(double *y_r, double *y_i, int N)
{
	int n;
	for(n=0;n<N;++n)
	{
		if (y_i[n] >=0) printf("%d : %f +%f i\n", n, y_r[n], y_i[n]);
		else printf("%d : %f %f i\n", n, y_r[n], y_i[n]);
	}
	return 0;
}

int FFTv3(double *x_r, double *x_i, double *y_r, double *y_i, int N)
{
	int k, n;
	for(n=0;n<N;++n)
	{
		y_r[n] = x_r[n];
		y_i[n] = x_i[n];
	}
	// Do Bit Reverse
	int p, q, m = N/2;
	double t_r, t_i;	// temp 
	// j: Bit Reverse version
	q = m; 
	for(p=1;p<N-1;++p)
	{
		//printf("%d <-> %d\n", p,q);

		if(p < q)
		{
			t_r = y_r[p]; 
			t_i = y_i[p];
			y_r[p] = y_r[q];
			y_i[p] = y_i[q];
			y_r[q] = t_r;
			y_i[q] = t_i;
		}
		// next q
		k = m;
		while(q >= k & k > 0)
		{
			q -= k;
			k >>= 1;
		}
		q += k;
	}
	// Butterfly structure
	double w_r, w_i, v_r, v_i;
	n = 2;
	while(n <= N)
	{
		for(k=0;k<n/2;k++)
		{
			w_r = cos(-2.0*k*M_PI/n);
			w_i = sin(-2.0*k*M_PI/n);
			for(p=k;p<N;p+=n)
			{
				q = p+n/2;
				v_r = w_r*y_r[q]-w_i*y_i[q];
				v_i = w_r*y_i[q]+w_i*y_r[q];
				y_r[q] = y_r[p] - t_r;
				y_i[q] = y_i[p] - t_i;
				y_r[p] += t_r;
				y_i[p] += t_i;
			}
		}
		n <<= 1;
	}
	
}
