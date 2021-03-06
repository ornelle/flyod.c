
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//#define INFINITY 1000000;

void Read_matrix(int graph[], int n, int my_rank, int p,
      MPI_Comm comm);
void Print_matrix(int graph[], int n, int my_rank, int p,
      MPI_Comm comm);
void Shortest_path_floyd(int graph[], int n, int my_rank, 
      int p, MPI_Comm comm);
int min(int a, int b);
void Print_local_matrix(int matrix[], int rows, int columns, int my_rank);



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
   graph = calloc(n*n/p, sizeof(int));

    if (my_rank == 0){
      printf("Enter the local_matrix\n");
    }
   Read_matrix(graph, n, my_rank, p, comm);

    if (my_rank == 0){
      printf("We got\n");
    }

   //Print_local_matrix(graph, n, n, my_rank); 
   Print_matrix(graph, n, my_rank, p, comm);
   //Shortest_path_floyd(graph, n, my_rank, p, comm);
   int i, j, k;
   for(k=0; k< n-1; k++){
      for(i=0; i< n-1; i++){
         for(j=0; j< n-1; j++){
            graph[i*n+j]= min(graph[i*n+j], (graph[i*n+k] + graph[k*n+j]));
            // printf("%d", graph[i*n+j]);
            // printf("%d", graph[i*n+k]);
            // printf("%d\n", graph[k*n+j]);
         }
      }
      MPI_Allreduce(MPI_IN_PLACE, &graph, 0, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
   }
   //printf("\n");
   Print_matrix(graph, n, my_rank, p, comm);
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
               printf("Enter the edge weight from vertex %d to vertex %d:\n", i, j);
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

      //printf("FROM");
      for (i = 0; i < n; i++) {
         // if(i==0)
         //    printf("FROM");
         //printf("%d", +1);
         for (j = 0; j < n; j++){
             //printf("\nTO:\t%d\t \n", i);
            //printf("TO %d:\t", j);
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

void Print_local_matrix(int matrix[], int rows, int columns, int my_rank) {
        printf("Rank %d ", my_rank);
        int i, j;
        for (i = 0; i < rows; i++){ 
            printf("%4.1d ", matrix[i]);
         }
        printf("\n");
}

// void Shortest_path_floyd(int graph[], int n, int my_rank, int p, MPI_Comm comm){
//    //printf("Rank %d ", my_rank);
//    int i, j, k;
//    for(i=0; i< n-1; i++){
//       for(j=0; j< n-1; j++){
//          for(k=0; k< n-1; k++){
//             graph[j*n +k]= min(graph[i*n+j], graph[i*n+k] + graph[k*n+j]);
//          }
//       }
//    }

//    /* need to gather or reduce? then print matrix with new shortest path */

// }
int min(int a, int b){
   if(a<b){
      //printf("%d\n", a);
      return a;
   }
   else{
      //printf("%d\n", b);
      return b;
   }
}
