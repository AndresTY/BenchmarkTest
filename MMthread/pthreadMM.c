# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif

# define DATA_SZ (1024*1024*64*3)

static double  MEM_CHUNK[DATA_SZ];
int 	N;		 
int 	num_threads;
double  *a, *b, *c;

void Matrix_Init_col(int SZ, double *a, double *b, double *c) {
	int j,k;

  	for (k=0; k<SZ; k++) 
    		for (j=0; j<SZ; j++) {
      			a[j+k*SZ] = 2.2*(j-k);  
			b[j+k*SZ] = 3.7*(j+k);   
			c[j+k*SZ] = 1.0;
    		}
}

/*
 * La idea del thread es que se divida la MatrizA en tantas 
 * partes como hilos se dispongan, y se envia esa porción
 * junto con la MatrizB completa. La idea es que el hilo
 * calcule la porción que corresponda a la multiplicación
 * de Matrices
 */

void * matrixMultiThread(void *arg){
  	int     i, j, k;
	int 	idT;    	
	int 	porcion;		
	int   	start_row, end_row;	 
 	
	idT       = *(int *)(arg);
	porcion   = N/num_threads;
	start_row = idT*porcion;
	end_row   = (idT+1)*porcion; 
	
	
	for (i=start_row; i<end_row; i++)
    		for (j=0; j<N; j++) {
      			double *pA, *pB, S;
      			S=0.0; 
      			pA = a+(i*N); 
			pB = b+j;
      			for (k=N; k>0; k--, pA++, pB+=N) 
        			S += (*pA * *pB);
      			c[i*N+j]= S;
    		}
}


int main (int argc, char **argv) {
	int   i;
	pthread_t * threads;
	struct timeval t_start, t_end;
	double totalTime;

  	if (argc < 2) {
    		printf("MM1c <MatrixSize> <Num_Threads> \n"); 
    		return -1;
  	}

  	N  	     = (int) atof(argv[1]); argc--; argv++;
  	num_threads  = (int) atof(argv[1]); argc--; argv++;

  	a = MEM_CHUNK;
  	b = a + N*N;
  	c = b + N*N;

  	Matrix_Init_col(N, a, b, c); 
	
	gettimeofday(&t_start, NULL);
	threads = (pthread_t *)malloc(num_threads*sizeof(pthread_t));
	for(i=0; i<num_threads;i++){
		int *tid; 
		tid = (int *)malloc(sizeof(int));
		*tid = i; 
		pthread_create(&threads[i], NULL, matrixMultiThread, (void *)tid);
	}

	for(i=0; i<num_threads;i++){
		pthread_join(threads[i], NULL);
	}
	gettimeofday(&t_end, NULL);
	
	totalTime = (t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec);  

	printf("%f \n", totalTime/1000000);

	return 0;


}
