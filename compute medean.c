#include <stdio.h>
void swap (int*data,int i,int j)
{
     int tmp = data[i];
     data [i] = data[j];
          data[j] = tmp;
     }
     void quick_sort (int*data,int start,int last)
     {
          int i = start+1 ,j = last-1 ,pivot = data[start];
                    if (last-start<2)
                              return;
                                        while (i<=j)
                                                  {
                if (data[i]<pivot) 
                {
                     ++i;
                     continue;
                     }
                     if(data[j]>=pivot)
                                          {
                          --j;
                                                    continue;
                                                                              }
                          swap(data,i,j);
                          }
                          swap(data,i-1,start);
                                                    quick_sort(data,start,i);
                                                    quick_sort(data,i,last);
                                                    }
                          int select_middle(int*data,int start,int last,int nth)
                          {
                              int i = start+1,j = last-1,pivot = data[start];
                              int k;
                              if(last-start<2)
                                                            return data[start];
                                                            while(i<=j)
                              {
                                    if (data[i]<pivot)
                                    {
                                    ++i;
                                                                        continue ;
                                    }
                                    if(data[j]>=pivot) 
                                    {
                                         --j;
                                                                               continue ;
                                                                               }
                                         swap(data,i,j);
                                         }
                                         swap ( data, i - 1 , start ) ;
                                         k = i-start;
                                                  if(k == nth ) 
                                                  return data[i-1];
                                                                            else if(k>nth) 
                                                                            return select_middle(data,start,i,nth);
                                         else return select_middle(data,i,last,nth-k);
                                         }
                                         int main ( )
                                         {
                                             int i,n=0,middle;
                                             int data[256];
                                             while (1)
                                             {
                                                   n = 0;
                                                   while (scanf ("%d",& data[n])!=EOF && data[n]>=0) 
                                                   {
                                                         ++n;
                                                         }
                                                         if(feof (stdin)||ferror(stdin)) 
                                                         break ;
                                                         middle = select_middle (data,0,n,(n+1)/2);
                                                         quick_sort (data,0,n);
                                                         for (i=0;i<n;i++)
                                                                                    printf ("%d",data[i]);
                                                         printf ("\n middle number is %d \n ",middle);
                                                         }
                                                         return 0 ;
                                                         }
