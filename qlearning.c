#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Declaring some global variables
int ranVector[1000], top = 0, possibleMoves[6], max_index[6];
float learningRate = 0.9; 

// Function to print the reward matrix
void printAdjMatrix(int m[6][6])
{
    int count = 0;
    printf("\n>> Adjacent Reward Matrix => \n\n");
    printf("# \t0 \t 1\t 2\t 3\t 4\t 5\n");
    printf("%d \t", count);
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            printf("%d \t", m[i][j]);
        }
        printf("\n");
        count++;
        if(count == 6) break;
        printf("%d \t", count);
    }
}

// Function to print the q matrix
void printQMatrix(double q[6][6], double qVal)
{
    int count = 0;
    printf("\n>> Trained Q Matrix => \n\n");
    printf("# \t0 \t\t 1\t\t 2\t\t 3\t\t 4\t\t 5\n");
    printf("%d \t", count);
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            printf("%f \t", q[i][j] / qVal * 100.0);
        }
        printf("\n");
        count++;
        if(count == 6) break;
        printf("%d \t", count);
    }
}

// Initializing a random vector array with values from 0 - 6
void init_a_random_vector_array()
{
    for(int i = 0; i < 1000; i++)
    {
        ranVector[i] = rand() % 6;
    }
}

// Incrementing the random vector array by one so that 
// it will give random values at every itteration
int return_rand_vector()
{
    return rand() % 6;
}

// Function to calculate total number of moves
int check_moves(int rV, int aM[6][6])
{
    int i = 0, count = 0;

    // increment the count if the reward is more than 0
    while (i < 6)
    {
        if (aM[rV][i] >= 0.0)
        {
            possibleMoves[count] = i;
            count++;
        }
        i++;
    }
    return count;
}

// Function to return any possible move
int next_move(int numMoves)
{
    int ranVec = return_rand_vector();
    int nextMove = possibleMoves[ranVec % numMoves];
    return nextMove;
}

// Main function for generating the qmatrix
double get_score(int rV, int move, int aM[6][6], double qM[6][6])
{
    int count = 0, k = 0, index_of_max;
    double temp_max = 0.0, max_value = 0.0, sumA = 0.0;

    // check if the selected index is maximum, if not take 
    // the other index and store it in the max_index
    for (int i = 0; i < 6; i++)
    {
        max_index[i] = 0;

        if (temp_max == qM[move][i])
        {
            max_index[count] = i;
            count++;
        }
        else if (temp_max < qM[move][i])
        {
            count = 0;
            temp_max = qM[move][i];
            max_index[count] = i;
            count++;
        }
    }

    int ranVec = return_rand_vector() % count;
    index_of_max = max_index[ranVec];
    max_value = qM[move][index_of_max];

    // Main QLearning formula
    qM[rV][move] = aM[rV][move] + (learningRate * max_value);

    temp_max = 0.0;

    // searching for the max value in the qmatrix
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (qM[i][j] > temp_max)
            {
                temp_max = qM[i][j];
            }
        }
    }

    // getting the score
    if (temp_max > 0)
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                sumA = sumA + (qM[i][j] / temp_max);
            }
        }
        sumA = sumA * 100;
        return sumA;
    }
    else
    {
        return 0.0;
    }
}

// Function to get the shortest path with the help of the trained qmatrix
void print_shortest_path(int current_state, int final_state, double qMatrix[6][6])
{
    int visited_states_history[6] = {0,0,0,0,0,0}, max_reward_value =0, index_of_max_reward_state, i;
    printf("\n");

    // Find for the max qvalue in the index and break 
    // when we arive aat the final vector end
    while(current_state !=final_state)
    {
        printf("Vector '%d' to ",current_state);
        for(i=0;i<6;i++)
        {
            if(visited_states_history[i]==0)
            {
                if(qMatrix[current_state][i]>max_reward_value)
                {
                    max_reward_value = qMatrix[current_state][i];
                    index_of_max_reward_state = i;
                }
            }
        }
        
        current_state = index_of_max_reward_state;

        //marking state as visited to avoid revisiting
        visited_states_history[index_of_max_reward_state] = 1;

        // break if the max reward value is 0
        if(max_reward_value==0)
        {
            printf("\n[!!] No way ahead\n");
            break;
        }   
    }

    // Print
    printf("Vector '%d' will be the shortest path in the Graph.\n",current_state);
}

int main()
{
    int vectorS = 0, vectorE = 5, i, j, adjMatrix[6][6];
    double QMatrix[6][6], score[100];

    /*
        Initializing the reward matrix
        0: for the connected vectors
        -1: for the not connected vectors
        50: for the vectors adjacent to the final vector, here: 3->4, 2->4, 0->4
        100: for reaching the final vector position, here: 4->5, 5->5
    */

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            adjMatrix[i][j] = -1; // initially initiallizing to -1
            QMatrix[i][j] = 0; // initializing to zero to avoid garbage value

            if((i==0 && j==1) || (i==1 && j==0) || (i==1 && j==3) || (i==3 && j==1) || (i==3 && j==2) || (i==2 && j==3) || (i==0 && j==2) || (i==2 && j==0) || (i==2 && j==4) || (i==4 && j==2) || (i==3 && j==4) || (i==4 && j==3) || (i==4 && j==5) || (i==5 && j==4))
            {
                adjMatrix[i][j] = 0; // gave no reward as it can get some reward for unnecessary steps
            }

            if((i==3 && j==4) || (i==2 && j==4) || (i==0 && j ==4))
            {
                adjMatrix[i][j] = 50;
            }

            if((i==4 && j==5) || (i == 5 && j == 5))
            {
                adjMatrix[i][j] = 100;
            }
        }
    }

    // Calling the initialized functions
    init_a_random_vector_array();
    printAdjMatrix(adjMatrix);

    // Variables to track our training process
    clock_t start, end;
    double time;

    // Starting itteration and clock
    int itter = 1;
    start = clock();

    // Starting our training
    printf("\n>> Training Q Matrix => \n\n");
    
    for(i = 0; i < 100; i++)
    {
        // Get a random vector from 0-5
        int randomVector = return_rand_vector();

        // Check possible number of moves for that random vector 
        int num_of_possible_moves = check_moves(randomVector, adjMatrix);

        // Get a random move selected for therandom vector
        int move = next_move(num_of_possible_moves);

        // Apply qlearning and generate score
        double current_score = get_score(randomVector, move, adjMatrix, QMatrix);
        score[i] = current_score;

        // Print
        printf("Current Score: %f | Num Of Iterations: %d \n", current_score, itter);
        itter++;
    }

    // Ending clock and printing execution time
    end = clock();
    time += ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n  ---Total Time taken for training: %gs---  \n", time);

    double maxQval = 0.0;

    // Getting the max qvalue for normalization
    for(i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            if (maxQval < QMatrix[i][j])
            {
                maxQval = QMatrix[i][j];
            }
        }
    }

    // Printing trained normalized Q matrix
    printQMatrix(QMatrix, maxQval);

    // Getting the shortest path and printing it
    print_shortest_path(vectorS,vectorE,QMatrix);

}