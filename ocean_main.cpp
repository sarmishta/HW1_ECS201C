
#include <chrono>
#include <cstdlib>
#include <iostream>

using namespace std;

/* Implement this function in serial_ocean and omp_ocean */
extern void ocean (int** grid, int xdim, int ydim, int timesteps);
//extern void ocean (int** grid, int xdim, int ydim, int timesteps, int num_threads);
void printGrid(int** grid, int xdim, int ydim){
int i, j;
  for(i=0;i<ydim;i++){
    for(j=0;j<xdim;j++)
      printf("%d  ",grid[i][j]);
    printf("\n");
  }

}
int step_i = 0; 
int xdim,ydim,timesteps,num_threads;
int** grid;
void* oceanStaticParallel(void* arg) 
{ 
 int tid = *((int*) arg); 
 int steps,i,j,gridLocation;
    for(steps=0; steps<timesteps;steps++){
        for(int i=(((xdim-2)/num_threads)*tid)+1; i<=(((xdim-2)/num_threads)*(tid+1)); i++){
            //each thread takes care of 1/num_thread th part of grid
            gridLocation = (i+steps)%2;// for updating even locations at even timesteps and odd at odd timesteps
            for(j=1+gridLocation; j<ydim-1; j=j+2) {
	            grid[i][j] = (grid[i][j] + grid[i-1][j] + grid[i+1][j]+ grid[i][j-1] + grid[i][j+1])/5;//avg 
         }
        }
    }
} 
int main(int argc, char* argv[])
{
   
    int i,j,t;

    
    /********************Get the arguments correctly (start) **************************/
    /* 
    Three input Arguments to the program
    1. X Dimension of the grid
    2. Y dimension of the grid
    3. number of timesteps the algorithm is to be performed
    */
    int check=0;
    if(argc==5){
        
        xdim = atoi(argv[1]);
        ydim = atoi(argv[2]);
        timesteps = atoi(argv[3]);
        num_threads = atoi(argv[4]);
        check=1;
    }else{
        xdim = atoi(argv[1]);
        ydim = atoi(argv[2]);
        timesteps = atoi(argv[3]);
        check=2;
    }
    ///////////////////////Get the arguments correctly (end) //////////////////////////


    /*********************create the grid as required (start) ************************/
    /*
    The grid needs to be allocated as per the input arguments and randomly initialized.
    Remember during allocation that we want to guarantee a contiguous block, hence the
    nasty pointer math.

    To test your code for correctness please comment this section of random initialization.
    */
    grid = (int**) malloc(ydim*sizeof(int*));
    int *temp = (int*) malloc(xdim*ydim*sizeof(int));
    for (i=0; i<ydim; i++) {
        grid[i] = &temp[i*xdim];
    }
    for (i=0; i<ydim; i++) {
        for (j=0; j<xdim; j++) {
            grid[i][j] = rand()%100000;
        }
    }
    ///////////////////////create the grid as required (end) //////////////////////////


	chrono::milliseconds total;

  auto start = chrono::high_resolution_clock::now();   // Start the time measurment here before the algorithm starts
    if(check==1){
    
    printf("inside static parallel execution"); 
    pthread_t threads[num_threads]; 
    int* id = (int*)malloc(sizeof(int)*num_threads);
    // Creating four threads, each evaluating its own part 
    for (int i = 0; i < num_threads; i++) {  
        id[i]=i;
        pthread_create(&threads[i], NULL, oceanStaticParallel,  (void*) &id[i]); 
    } 
  
    // joining and waiting for all threads to complete 
    for (int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL); 
    }
   

    }
    else{
    auto start = chrono::high_resolution_clock::now(); 
    printf("inside serial execution");
    ocean(grid, xdim, ydim, timesteps);

    }
    
    total = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start);  // End the time measuremnt here since the algorithm ended
    cout << "Total Execution time: "  << total.count() << "ms" << endl; // print the execution time in ms
   // printGrid(grid,xdim,ydim);
    
    

    // Free the memory we allocated for grid
   free(temp);
   free(grid);

    return EXIT_SUCCESS;
}