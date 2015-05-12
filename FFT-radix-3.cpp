#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int Generate_N(int p, int q, int r);
int FFTv3(double *x_r, double *x_i, double *y_r, double *y_i, int N);
int Initial(double *x_r, double *x_i, int N);
int Print_Complex_Vector(double *y_r, double *y_i, int N);

int main(){
	int k, n, N, p, q, r;
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
	/*
	for(int i=0;i<N;++i){
		printf("%f\n",y_r[i]);
	}
	*/
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
	int a, b, c;
	double t_r, t_i;	// temp 
	// j: Bit Reverse version
	a = b = 0; 
	while(a < N)
	{
		//printf("%d <-> %d\n", p,q);
		if(a < b)
		{
			t_r = y_r[a]; 
			t_i = y_i[a];
			y_r[a] = y_r[b];
			y_i[a] = y_i[b];
			y_r[b] = t_r;
			y_i[b] = t_i;
		}
		// next q
		c = N/3;
		while(b >= 2*c & c > 0)
		{
			b -= 2*c;
			c = c/3;
		}
		b += c;
		a += 1;
	}
	// Butterfly structure
 	
	double theta, w1_r, w1_i, w2_r, w2_i, u_r, u_i, e, a_r,a_i;
	int d;
	n = 3;
	e=sqrt(3)/2;
	while(n <= N)
	{
		for(k=0;k<n/3;k++)
		{
			
			w1_r=cos(-2.0*k*M_PI/n);
			w1_i=sin(-2.0*k*M_PI/n);
			w2_r=cos(-2.0*(2.0*k)*M_PI/n);
			w2_i=sin(-2.0*(2.0*k)*M_PI/n);
			
			for(a=k;a<N;a+=n)
			{
				b=a+n/3;
				d=b+n/3;
				
				a_r=y_r[a];
				a_i=y_i[a];
				
				t_r=w1_r*y_r[b]-w1_i*y_i[b];
				t_i=w1_r*y_i[b]+w1_i*y_r[b];
				u_r=w2_r*y_r[d]-w2_i*y_i[d];
				u_i=w2_r*y_i[d]+w2_i*y_r[d];
				
			    y_r[a]=a_r+t_r+u_r;
			    y_i[a]=a_i+t_i+u_i;
			    y_r[b]=a_r+(-1/2.0)*(t_r+u_r)+e*(t_i-u_i);
			    y_i[b]=a_i+(-1/2.0)*(t_i+u_i)-e*(t_r-u_r);
			    y_r[d]=a_r+(-1/2.0)*(t_r+u_r)-e*(t_i-u_i);
			    y_i[d]=a_i+(-1/2.0)*(t_i+u_i)+e*(t_r-u_r);
			    
				
			}
		}
		n = n * 3;
	}
	 
}
