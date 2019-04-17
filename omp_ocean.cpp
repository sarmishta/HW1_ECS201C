void ocean (int **grid, int xdim, int ydim, int timesteps)
{
    /********************* the red-black algortihm (start)************************/
    /*
    In odd timesteps, calculate indeces with - and in even timesteps, calculate indeces with * 
    See the example of 6x6 matrix, A represents the corner elements. 
        A A A A A A
        A - * - * A
        A * - * - A
        A - * - * A
        A * - * - A
        A A A A A A 
    */

    // PUT YOUR CODE HERE
    int steps,i,j,gridLocation;
    
    for(steps=0; steps<timesteps;steps++){
        #pragma omp parallel for shared(grid,xdim,ydim) private(i,j) schedule(dynamic)
        for(i=1; i<xdim-1; i++){
            gridLocation = (i+steps)%2;// for updating even locations at even timesteps and odd at odd timesteps
            for(j=1+gridLocation; j<ydim-1; j=j+2) {
	            grid[i][j] = (grid[i][j] + grid[i-1][j] + grid[i+1][j]+ grid[i][j-1] + grid[i][j+1])/5;//avg 
         }
        }
    }
	
    /////////////////////// the red-black algortihm (end) ///////////////////////////
}
