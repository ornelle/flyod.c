
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define INFINITY 1000000;

void Read_matrix(int graph[], int n, int my_rank, int p,
      MPI_Comm comm);
void Print_matrix(int graph[], int n, int my_rank, int p,
      MPI_Comm comm);
void Shortest_path_floyd(int graph[], int n, int my_rank, 
      int p, MPI_Comm comm);
int min(int a, int b);


   int main(void) {
   int  n;
   int* graph;
   MPI_Comm comm;
   int p, my_rank;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   if (my_rank == 0) {
      printf("How many vertices?\n");
      scanf("%d", &n);
   }

   MPI_Bcast(&n, 1, MPI_INT, 0, comm);
   graph = malloc(n*n/p*sizeof(int));

    if (my_rank == 0){
      printf("Enter the local_matrix\n");
    }
   Read_matrix(graph, n, my_rank, p, comm);

    if (my_rank == 0){
      printf("We got\n");
    }
   Print_matrix(graph, n, my_rank, p, comm);
   Shortest_path_floyd(graph, n, my_rank, p, comm);

   free(graph);
   MPI_Finalize();

   return 0;
}

void Read_matrix(int graph[], int n, int my_rank, int p, MPI_Comm comm) {
   int i, j;
   int* temp_mat = NULL;

   if (my_rank == 0) {
      temp_mat = malloc(n*n*sizeof(int));
      for (i = 0; i < n; i++){
         for (j = 0; j < n; j++){
            if(i==j)
               temp_mat[i*n+j]=0;
            else{
               scanf("%d", &temp_mat[i*n+j]);
            }
         }
      }
      MPI_Scatter(temp_mat, n*n/p, MPI_INT,graph, n*n/p, MPI_INT, 0, comm);
      free(temp_mat);
   }
   else {
      MPI_Scatter(temp_mat, n*n/p, MPI_INT,graph, n*n/p, MPI_INT, 0, comm);
   }

}

void Print_matrix(int graph[], int n, int my_rank, int p, MPI_Comm comm) {
   int i, j;
   int* temp_mat = NULL;
   if (my_rank == 0) {
      temp_mat = malloc(n*n*sizeof(int));
      MPI_Gather(graph, n*n/p, MPI_INT,temp_mat, n*n/p, MPI_INT, 0, comm);

      for (i = 0; i < n; i++) {

         for (j = 0; j < n; j++){
             printf("%d ", temp_mat[i*n+j]);
         }
      printf("\n");
      }
      free(temp_mat);
   }
   else {
      MPI_Gather(graph, n*n/p, MPI_INT,temp_mat, n*n/p, MPI_INT, 0, comm);
   }
}

void Shortest_path_floyd(int graph[], int n, int my_rank, int p, MPI_Comm comm){

   int i, j, k;
   for(i=0; i< n-1; i++){
      for(j=0; j< n-1; j++){
         for(k=0; k< n-1; k++){
            graph[j*n +k]= min(graph[i*n+j], graph[i*n+k] + graph[k*n+j]);
         }
      }
   }

   /* need to gather or reduce? then print matrix with new shortest path */

}
int min(int a, int b){
   if(a<b)
      return a;
   else
      return b;
}
