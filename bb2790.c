#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>



void printWorld(int N, int world[N][N]);
int countNeighbors(int row, int column,int N, int world[N][N]);
void nextGen(int minR, int maxR, int minC, int maxC, int N, int world[N][N],int countMap[N][N]);
void splitGen(int N, int world[N][N],int threads);
void copyGen(int minR, int maxR, int minC, int maxC, int N, int world[N][N],int countMap[N][N]);
void inFile(char file[],int N,int world[N][N]);
void outFile(char file[],int N,int world[N][N]);

int main(int argc,char *argv[]){
  	// double start_time = omp_get_wtime();
  	// double total_time = omp_get_wtime() - start_time;

	int threads = atoi(argv[3]);

	int N = atoi(argv[2]);
	int world[N][N];

	inFile(argv[4],N,world);
	int gen = atoi(argv[1]);


	double start_time = omp_get_wtime();
	for (int i = 0; i<gen; i++){
		// printf("Generation:%d\n",i);
		// printWorld(N,world);
		// printf("\n");
		splitGen(N,world,threads);
	}
	double total_time = omp_get_wtime() - start_time;

	printf("\ntotal time: %f\n", total_time);
	// printWorld(N,world);
	outFile(argv[4],N,world);
}


int countNeighbors(int row, int column,int N, int world[N][N]){
	// int N = sizeof(world[0])/sizeof(int);
	int c =0;
	for(int i = row-1; i <= row+1; i++){
		for(int j = column-1; j <= column+1; j++){
			if(i>=0 && j>=0 && i<N && j<N && (i!=row || j!=column)){
				if(world[i][j]==1){
					c++;
				}
			}

		}

	}
	return c;
}

void nextGen(int minR, int maxR, int minC, int maxC, int N, int world[N][N],int countMap[N][N]){
	// int countMap[N][N];
	// for (int i = minR; i<= maxR;i++){
	// 	for (int j = minC; j<= maxC;j++){
	// 		countMap[i][j]=countNeighbors(i,j,N,world);
	// 	}
	// }
	for (int i = minR; i<= maxR;i++){
		for (int j = minC; j<= maxC;j++){
			if(world[i][j]==0){
				if(countMap[i][j]==3){
					world[i][j]=1;
				}
			}else{
				if(countMap[i][j]<2 || countMap[i][j]>3){
					world[i][j]=0;
				}
			}
		}
	}
}

void printWorld(int N, int world[N][N]){
	for (int i = 0; i<N;i++){
		for (int j=0; j<N; j++){
			printf("%d ",world[i][j]);
		}
		printf("\n");
	}
}

void inFile(char file[],int N,int world[N][N]){
  FILE *myfile;
  myfile = fopen(file,"r"); //TODO

  char *line = malloc(10*N*sizeof(char));

  for(int i=0; i<N; i++){
    fgets(line,10*N,myfile);
    char vc = *strtok(line," ");
    int v = atoi(&vc);
    world[i][0] = v;

    for(int j=1; j<N; j++){
      vc = *strtok(NULL," ");
      int v = atoi(&vc);
      world[i][j] = v;

    }
  }
   fclose(myfile);
}

void outFile(char file[],int N,int world[N][N]){
	char s[20];
    sprintf(s,"%s.out",file) ;
    FILE *myfile = fopen(s,"w");
  	char *line = malloc(10*N*sizeof(char));
    for(int i = 0;i<N;i++){
    	sprintf(line,"%d ",world[i][0]) ;
    	for(int j = 1;j<N;j++){
    		sprintf(s,"%d ",world[i][j]);
    		strcat(line,s);
    	}
      strcat(line,"\n");
      fprintf(myfile,"%s",line);
    }
    fclose(myfile);
}

void splitGen(int N, int world[N][N],int threads){
	int countMap[N][N];
	int q = N/threads;
	int i,minR,maxR;

	#pragma omp parallel num_threads(threads) private(i,minR,maxR)
	{
		i = omp_get_thread_num();
		minR = i*q;
		maxR = i*q+(q-1);
		if(i == threads-1){
			maxR = N-1;
			// printf("Thread: %d\nmaxR: %d\n",i, maxR);

		}
		copyGen(minR,maxR,0,N-1,N,world,countMap);
	}
	// printWorld(N,countMap);
	#pragma omp parallel num_threads(threads) private(i,minR,maxR)
	{
		i = omp_get_thread_num();
		// printf("%d\n",i);

		minR = i*q;
		maxR = i*q+(q-1);
		if(i == threads-1){
			maxR = N-1;
			// printf("Thread: %d\nmaxR: %d\n",i, maxR);
		}
		nextGen(minR,maxR,0,N-1,N,world,countMap);
	}
}
void copyGen(int minR, int maxR, int minC, int maxC, int N, int world[N][N],int countMap[N][N]){
	// int countMap[N][N];
	for (int i = minR; i<= maxR;i++){
		for (int j = minC; j<= maxC;j++){
			countMap[i][j]=countNeighbors(i,j,N,world);
		}
	}
	// printf("minR:%d maxR:%d\n", minR,maxR);
}









