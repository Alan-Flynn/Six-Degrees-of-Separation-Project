#include <iostream>
#include <stdlib.h>

#include "Vector08.h"
#include "Matrix08.h"
#include "Point.h"
#include "Triplet.h"
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

//Floyd algorithm determines degrees of separation between each node
void floyd(Matrix &Graph, int Elements) {
    for(unsigned int i=0; i<Elements; i++){
        for(unsigned int j=0; j<Elements; j++){
                for(unsigned int k=0; k<Elements; k++){

                    //If there exists a value k that the nodes i and j are both connected to set the value of i,j = i,k + k,j
                    if(i != j && Graph.getij(i,k) >= 1 && Graph.getij(k,j) >= 1){

                        //first clause of if statement is necessary as 0 was being taken as min value in else statement
                        if(Graph.getij(i,j) == 0){
                            int new_val = Graph.getij(i,k) + Graph.getij(k,j);
                            Graph.setij(i,j, new_val);
                            Graph.setij(j,i, new_val);
                        }
                        else{
                            int new_val2 = min(Graph.getij(i,j), Graph.getij(i,k) + Graph.getij(k,j));
                            Graph.setij(i,j, new_val2);
                            Graph.setij(j,i, new_val2);
                        }
                    }
                }
            }
    }
}

int main()
{
    int N;
    std::cout << "Enter a triangular number N (1,3,6 etc.) to create an NxN triangular grid graph: ";
    std::cin >> N;

    //positionsX vector holds x positions for each element in the triangular grid graph
    Vector positionsX(N);

    //positionsY vector holds y positions for each element in the triangular grid graph
    Vector positionsY(N);

    double x_value = 0;

    int count=0;
    for(unsigned int i=0; i<10; i++){
         x_value = x_value + 1;

         for(double y_value=1; y_value<=x_value; y_value++){
            if(count < N){
                //sets positionsX and positionsY such that they will correspond to {(0,0),(1,0),(1,1),(2,0),(2,1),(2,2) .. etc.
                positionsX.seti(count,x_value);
                positionsY.seti(count, y_value);
                count = count+1;
            }
            else{
                break;
            }
         }
    }

    Matrix Connections(N);
    //nnz_max stores no. of non zero entries
    int nnz_max = 0;

    for(unsigned int i=0; i<N; i++){
         for(unsigned int j=0; j<N; j++){
            //set diagonal equal to 0
            if(i == j){
                Connections.setij(i,j,0);
            }
            //Checks to see if there are other nodes to the left, right, above and below each node and sets the appropriate connections
            if((positionsX.geti(j) == positionsX.geti(i)-1 && positionsY.geti(j) == positionsY.geti(i)) ||
               (positionsX.geti(j) == positionsX.geti(i)+1 && positionsY.geti(j) == positionsY.geti(i)) ||
               (positionsX.geti(j) == positionsX.geti(i) && positionsY.geti(j) == positionsY.geti(i)-1) ||
               (positionsX.geti(j) == positionsX.geti(i) && positionsY.geti(j) == positionsY.geti(i)+1) ||
               (positionsX.geti(j) == positionsX.geti(i)-1 && positionsY.geti(j) == positionsY.geti(i)-1)||
               (positionsX.geti(j) == positionsX.geti(i)+1 && positionsY.geti(j) == positionsY.geti(i)+1))
            {
                Connections.setij(i,j,1);
                nnz_max = nnz_max + 1;
            }
         }
    }

    Triplet Sparse_Connections(N, nnz_max);
    for(unsigned int i=0; i<N; i++){
         for(unsigned int j=0; j<N; j++){
            if((positionsX.geti(j) == positionsX.geti(i)-1 && positionsY.geti(j) == positionsY.geti(i)) ||
               (positionsX.geti(j) == positionsX.geti(i)+1 && positionsY.geti(j) == positionsY.geti(i)) ||
               (positionsX.geti(j) == positionsX.geti(i) && positionsY.geti(j) == positionsY.geti(i)-1) ||
               (positionsX.geti(j) == positionsX.geti(i) && positionsY.geti(j) == positionsY.geti(i)+1) ||
               (positionsX.geti(j) == positionsX.geti(i)-1 && positionsY.geti(j) == positionsY.geti(i)-1)||
               (positionsX.geti(j) == positionsX.geti(i)+1 && positionsY.geti(j) == positionsY.geti(i)+1))
            {
                Sparse_Connections.setij(i,j,1);
            }
         }
    }
    std::cout << "Triplet showing conenctions of triangular grid: " << "\n";
    Sparse_Connections.print();
    std::cout << "\n\n";

    std::cout << "Intial matrix showing conenctions of triangular grid: " << "\n";
    Connections.print();
    std::cout << "\n\n";

    //Updates connections using floyd algorithm to inclue degrees of separation for each node
    floyd(Connections, N);
    std::cout << "Matrix updated with degrees of separation: " << "\n";
    Connections.print();
    std::cout << "\n\n";

    //Takes default value of all nodes being within 6 degress of separation of one another
    bool allwithin6 = true;

    for(unsigned int i=0; i<N; i++){
        for(unsigned int j=0; j<N; j++){
            if(Connections.getij(i,j) >= 6){
                allwithin6 = false;
            }
        }
    }

    int edges_added = 0;
    srand (time(NULL));
    //Keeps adding in edges until all nodes are within 6 degrees of separation
    while(allwithin6 == false){

            //sets notallwithin6 to default false value and will change it later if any degrees of separation are greater than 6
            bool notallwithin6 = false;

            //adds in connection between two random nodes
            int r1, r2;
            r1 = std::rand() % N;
            r2 = std::rand() % N;
            Connections.setij(r1,r2,1);

            //edges_added keeps track of the number of random nodes added in
            edges_added = edges_added+1;

            //updates graph to include new connection
            floyd(Connections, N);

            //Checks to see if any of the degrees of separation are still greater than 6 and if so sets notallwithin6 to true
            for(unsigned int i=0; i<N; i++){
                for(unsigned int j=0; j<N; j++){
                    if(Connections.getij(i,j) >= 6){
                        notallwithin6 = true;
                    }
                }
            }

            //if notallwithin6 is still false then they are all within 6 degrees and the loop is broken
            if(notallwithin6 == false){
                allwithin6 = true;
            }
    }

    std::cout << "Connections matrix after random edges were added: " << "\n";
    Connections.print();
    std::cout << "\n\n";

    std::cout << "All nodes are within 6 degrees of separation after " << edges_added << " random edges were added.";

    return 0;
    }
