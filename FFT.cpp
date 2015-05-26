#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
int FFT(double *x_r,double *x_i,double *y_r,double *y_i,int p,int q,int r);

int main()
{
	int p,q,r,N=1;
	printf("p=");
	scanf("%d",&p);
	printf("q=");
	scanf("%d",&q);
	printf("r=");
	scanf("%d",&r);
	for(int i=0;i<p;i++) N=N*2;
	for(int i=0;i<q;i++) N=N*3;
	for(int i=0;i<r;i++) N=N*5;
	printf("N=%d\n",N);
	double *x_r,*x_i,*y_r,*y_i;
	x_r=(double *)malloc(N*sizeof(double));
	x_i=(double *)malloc(N*sizeof(double));
	y_r=(double *)malloc(N*sizeof(double));
	y_i=(double *)malloc(N*sizeof(double));
	clock_t t1,t2;
	
	for(int i=0;i<N;i++)
	{
		x_r[i]=i;
		x_i[i]=0;
	}
	t1=clock();
	FFT(x_r,x_i,y_r,y_i,p,q,r);
	t2=clock();
	
	printf("time=%f\n",1.0*(t2-t1)/CLOCKS_PER_SEC);
	
	free(x_r);
	free(x_i);
	free(y_r);
	free(y_i);
	system("pause");
	return 0;
}

int FFT(double *x_r,double *x_i,double *y_r,double *y_i,int p,int q,int r)
{
	int N=1;
	for(int i=0;i<p;i++) N=N*2;
	for(int i=0;i<q;i++) N=N*3;
	for(int i=0;i<r;i++) N=N*5;
	if(p==0)
	{
		if(q==0)
		{
			if(r==0)
			{
				y_r[0]=x_r[0];
				y_i[0]=x_i[0];
				return 0;
			}
			else
			{
				int k,n,i,j;
				double *u_r,*u_i,*v_r,*v_i,w_r,w_i,t_r,t_i,s_r,s_i,q_r,q_i,tmp;
				u_r=(double *)malloc(N*sizeof(double));
				u_i=(double *)malloc(N*sizeof(double));
				v_r=(double *)malloc(N*sizeof(double));
				v_i=(double *)malloc(N*sizeof(double));
				for(i=0;i<5;i++)
				{
					for(j=0;j<N/5;j++)
					{
						u_r[j+i*N/5]=x_r[j*5+i];
						u_i[j+i*N/5]=x_i[j*5+i];
					}
				}
				
				for(i=0;i<5;i++) FFT(u_r+i*N/5,u_i+i*N/5,v_r+i*N/5,v_i+i*N/5,0,0,r-1);
				q_r=cos(-2.0*M_PI/N); 
				q_i=sin(-2.0*M_PI/N);
				s_r=1.0;
				s_i=0.0;
				y_r[0]=v_r[0];
				y_i[0]=v_i[0];
				for(int n=1;n<5;n++)
				{
					y_r[0]=y_r[0]+v_r[n*N/5];
					y_i[0]=y_i[0]+v_i[n*N/5];
				}
				for(int i=1;i<5;i++)
				{
					y_r[i*N/5]=v_r[0];
					y_i[i*N/5]=v_i[0];
					t_r=cos(-2.0*i*M_PI/5);
					t_i=sin(-2.0*i*M_PI/5);
					w_r=1.0;
					w_i=0.0;
					for(int n=1;n<5;n++)
					{
						tmp=w_r;
						w_r=w_r*t_r-w_i*t_i;
						w_i=tmp*t_i+w_i*t_r;
						y_r[i*N/5]=y_r[i*N/5]+w_r*v_r[n*N/5]-w_i*v_i[n*N/5];
						y_i[i*N/5]=y_i[i*N/5]+w_r*v_i[n*N/5]+w_i*v_r[n*N/5];
					}
				}
				
				for(int k=1;k<N/5;k++)
				{
					tmp=s_r;
					s_r=s_r*q_r-s_i*q_i;
					s_i=tmp*q_i+s_i*q_r;
					y_r[k]=v_r[k];
					y_i[k]=v_i[k];
					w_r=1;
					w_i=0;
					for(int n=1;n<5;n++)
					{
						tmp=w_r;
						w_r=w_r*s_r-w_i*s_i;
						w_i=tmp*s_i+w_i*s_r;
						y_r[k]=y_r[k]+w_r*v_r[k+n*N/5]-w_i*v_i[k+n*N/5];
						y_i[k]=y_i[k]+w_r*v_i[k+n*N/5]+w_i*v_r[k+n*N/5];
					}
					for(int i=1;i<5;i++)
					{
						y_r[k+i*N/5]=v_r[k];
						y_i[k+i*N/5]=v_i[k];
						t_r=cos(-2.0*i*M_PI/5);
						t_i=sin(-2.0*i*M_PI/5);
						w_r=1;
						w_i=0;
						for(int n=1;n<5;n++)
						{
							tmp=w_r;
							w_r=w_r*s_r-w_i*s_i;
							w_i=tmp*s_i+w_i*s_r;
							tmp=w_r;
							w_r=w_r*t_r-w_i*t_i;
							w_i=tmp*t_i+w_i*t_r;
							y_r[k+i*N/5]=y_r[k+i*N/5]+w_r*v_r[k+n*N/5]-w_i*v_i[k+n*N/5];
							y_i[k+i*N/5]=y_i[k+i*N/5]+w_r*v_i[k+n*N/5]+w_i*v_r[k+n*N/5];
						}
					}
				}
				free(u_r);
				free(u_i);
				free(v_r);
				free(v_i);
			}
		}
		else
		{
			int k,n,i,j;
			double *u_r,*u_i,*v_r,*v_i,w_r,w_i,t_r,t_i,s_r,s_i,q_r,q_i,tmp;
			u_r=(double *)malloc(N*sizeof(double));
			u_i=(double *)malloc(N*sizeof(double));
			v_r=(double *)malloc(N*sizeof(double));
			v_i=(double *)malloc(N*sizeof(double));
			for(i=0;i<3;i++)
			{
				for(j=0;j<N/3;j++)
				{
					u_r[j+i*N/3]=x_r[j*3+i];
					u_i[j+i*N/3]=x_i[j*3+i];
				}
			}
			
			for(i=0;i<3;i++) FFT(u_r+i*N/3,u_i+i*N/3,v_r+i*N/3,v_i+i*N/3,0,q-1,r);
			q_r=cos(-2.0*M_PI/N); 
			q_i=sin(-2.0*M_PI/N);
			s_r=1.0;
			s_i=0.0;
			y_r[0]=v_r[0];
			y_i[0]=v_i[0];
			for(int n=1;n<3;n++)
			{
				y_r[0]=y_r[0]+v_r[n*N/3];
				y_i[0]=y_i[0]+v_i[n*N/3];
			}
			for(int i=1;i<3;i++)
			{
				y_r[i*N/3]=v_r[0];
				y_i[i*N/3]=v_i[0];
				t_r=cos(-2.0*i*M_PI/3);
				t_i=sin(-2.0*i*M_PI/3);
				w_r=1.0;
				w_i=0.0;
				for(int n=1;n<3;n++)
				{
					tmp=w_r;
					w_r=w_r*t_r-w_i*t_i;
					w_i=tmp*t_i+w_i*t_r;
					y_r[i*N/3]=y_r[i*N/3]+w_r*v_r[n*N/3]-w_i*v_i[n*N/3];
					y_i[i*N/3]=y_i[i*N/3]+w_r*v_i[n*N/3]+w_i*v_r[n*N/3];
				}
			}
			
			for(int k=1;k<N/3;k++)
			{
				tmp=s_r;
				s_r=s_r*q_r-s_i*q_i;
				s_i=tmp*q_i+s_i*q_r;
				y_r[k]=v_r[k];
				y_i[k]=v_i[k];
				w_r=1;
				w_i=0;
				for(int n=1;n<3;n++)
				{
					tmp=w_r;
					w_r=w_r*s_r-w_i*s_i;
					w_i=tmp*s_i+w_i*s_r;
					y_r[k]=y_r[k]+w_r*v_r[k+n*N/3]-w_i*v_i[k+n*N/3];
					y_i[k]=y_i[k]+w_r*v_i[k+n*N/3]+w_i*v_r[k+n*N/3];
				}
				for(int i=1;i<3;i++)
				{
					y_r[k+i*N/3]=v_r[k];
					y_i[k+i*N/3]=v_i[k];
					t_r=cos(-2.0*i*M_PI/3);
					t_i=sin(-2.0*i*M_PI/3);
					w_r=1;
					w_i=0;
					for(int n=1;n<3;n++)
					{
						tmp=w_r;
						w_r=w_r*s_r-w_i*s_i;
						w_i=tmp*s_i+w_i*s_r;
						tmp=w_r;
						w_r=w_r*t_r-w_i*t_i;
						w_i=tmp*t_i+w_i*t_r;
						y_r[k+i*N/3]=y_r[k+i*N/3]+w_r*v_r[k+n*N/3]-w_i*v_i[k+n*N/3];
						y_i[k+i*N/3]=y_i[k+i*N/3]+w_r*v_i[k+n*N/3]+w_i*v_r[k+n*N/3];
					}
				}
			}
			free(u_r);
			free(u_i);
			free(v_r);
			free(v_i);
		}
	}
	else
	{
		int k,n,i,j;
		double *u_r,*u_i,*v_r,*v_i,w_r,w_i,q_r,q_i,tmp;
		u_r=(double *)malloc(N*sizeof(double));
		u_i=(double *)malloc(N*sizeof(double));
		v_r=(double *)malloc(N*sizeof(double));
		v_i=(double *)malloc(N*sizeof(double));
		for(i=0;i<2;i++)
		{
			for(j=0;j<N/2;j++)
			{
				u_r[j+i*N/2]=x_r[j*2+i];
				u_i[j+i*N/2]=x_i[j*2+i];
			}
		}
		
		for(i=0;i<2;i++) FFT(u_r+i*N/2,u_i+i*N/2,v_r+i*N/2,v_i+i*N/2,p-1,q,r);
		q_r=cos(-2.0*M_PI/N); 
		q_i=sin(-2.0*M_PI/N);
		w_r=1.0;
		w_i=0.0;
		y_r[0]=v_r[0]+v_r[N/2];
		y_i[0]=v_i[0]+v_i[N/2];
		y_r[N/2]=v_r[0]-v_r[N/2];
		y_i[N/2]=v_i[0]-v_i[N/2];
		for(int k=1;k<N/2;k++)
		{
			tmp=w_r;
			w_r=w_r*q_r-w_i*q_i;
			w_i=tmp*q_i+w_i*q_r;
			y_r[k]=v_r[k]+w_r*v_r[k+N/2]-w_i*v_i[k+N/2];
			y_i[k]=v_i[k]+w_r*v_i[k+N/2]+w_i*v_r[k+N/2];
			y_r[k+N/2]=v_r[k]-w_r*v_r[k+N/2]+w_i*v_i[k+N/2];
			y_i[k+N/2]=v_i[k]-w_r*v_i[k+N/2]-w_i*v_r[k+N/2];
		}
		free(u_r);
		free(u_i);
		free(v_r);
		free(v_i);
	}
	return 0;
}
