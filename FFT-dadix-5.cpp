#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int Generate_N(int p, int q, int r);
int FFTv3(double *x_r, double *x_i, double *y_r, double *y_i, int N);
int Print_Complex_Vector(double *y_r, double *y_i, int N);
int Initial(double *x_r, double *x_i, int N);


int main()
{
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
	    system("PAUSE");
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
	int p, q, m;
	double t_r, t_i;	// temp 
	// j: Bit Reverse version
	p = q = 0; 
	while(p < N)
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
		m = N/5;
		while(q >= 4*m & m > 0)
		{
			q -= 4*m;
			m = m/5;
		}
		q += m;
		p += 1;
	}
	// Butterfly structure
 		double w_r,w_i,w2_r,w2_i,w3_r,w3_i,w4_r,w4_i,a_r,a_i,b_r,b_i,c_r,c_i,d_r,d_i,aa_r,aa_i,s_r,s_i,u_r,u_i,v_r,v_i;
	int a,b,c,d;
	n = 5;
	w_r=cos(-2.0*M_PI/5);
	w_i=sin(-2.0*M_PI/5);
	w2_r=cos(-4.0*M_PI/5);
	w2_i=sin(-4.0*M_PI/5);
	w3_r=cos(-6.0*M_PI/5);
	w3_i=sin(-6.0*M_PI/5);
	w4_r=cos(-8.0*M_PI/5);
	w4_i=sin(-8.0*M_PI/5);
	while(n <= N)
	{
		for(k=0;k<n/5;k++)
		{
			
			s_r=cos(-2.0*k*M_PI/n);
			s_i=sin(-2.0*k*M_PI/n);
			t_r=cos(-2.0*(2.0*k)*M_PI/n);
			t_i=sin(-2.0*(2.0*k)*M_PI/n);
			u_r=cos(-2.0*(3.0*k)*M_PI/n);
			u_i=sin(-2.0*(3.0*k)*M_PI/n);
			d_r=cos(-2.0*(4.0*k)*M_PI/n);
			d_i=sin(-2.0*(4.0*k)*M_PI/n);
			
			for(p=k;p<N;p+=n)
			{
				a=p+n/5;
				b=a+n/5;
				c=b+n/5;
				d=c+n/5;
				
				aa_r=y_r[p];
				aa_i=y_i[p];
				
				a_r=s_r*y_r[a]-s_i*y_i[a];
				a_i=s_r*y_i[a]+s_i*y_r[a];
				b_r=t_r*y_r[b]-t_i*y_i[b];
				b_i=t_r*y_i[b]+t_i*y_r[b];
				c_r=c_r*y_r[c]-u_i*y_i[c];
				c_i=c_r*y_i[c]+u_i*y_r[c];
				v_r=d_r*y_r[d]-d_i*y_i[d];
				v_i=d_r*y_i[d]+d_i*y_r[d];
				
				y_r[p]=aa_r+a_r+b_r+c_r+v_r;
				y_i[p]=aa_i+a_i+b_i+c_i+v_i;
				
				y_r[a]=aa_r+w_r*a_r-w_i*a_i+w2_r*b_r-w2_i*b_i+w3_r*c_r-w3_i*c_i+w4_r*v_r-w4_i*v_i;
				y_i[a]=aa_i+w_r*a_i+w_i*a_r+w2_r*b_i+w2_i*b_r+w3_r*c_i+w3_i*c_r+w4_r*v_i+w4_i*v_r;
				
				y_r[b]=aa_r+w2_r*a_r-w2_i*a_i+w4_r*b_r-w4_i*b_i+w_r*c_r-w_i*c_i+w3_r*v_r-w3_i*v_i;
				y_i[b]=aa_i+w2_r*a_i+w2_i*a_r+w4_r*b_i+w4_i*b_r+w_r*c_i+w_i*c_r+w3_r*v_i+w3_i*v_r;
				
				y_r[c]=aa_r+w3_r*a_r-w3_i*a_i+w_r*b_r-w_i*b_i+w4_r*c_r-w4_i*c_i+w2_r*v_r-w2_i*v_i;
				y_i[c]=aa_i+w3_r*a_i+w3_i*a_r+w_r*b_i+w_i*b_r+w4_r*c_i+w4_i*c_r+w2_r*v_i+w2_i*v_r;
				
				y_r[d]=aa_r+w4_r*a_r-w4_i*a_i+w3_r*b_r-w3_i*b_i+w2_r*c_r-w2_i*c_i+w_r*v_r-w_i*v_i;
				y_i[d]=aa_i+w4_r*a_i+w4_i*a_r+w3_r*b_i+w3_i*b_r+w2_r*c_i+w2_i*c_r+w_r*v_i+w_i*v_r;
				
				
			}
		}
		n = n * 5;
	}
	 

}
