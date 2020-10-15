#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ranVector[1000], top = 0, possibleMoves[6], max_index[6];
double learningRate = 0.9; 

void printAdjMatrix(int m[6][6])
{
    int count = 0;
    printf("Adjacent Matrix => \n");
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

void init_a_random_vector_array()
{
    for(int i = 0; i < 1000; i++)
    {
        ranVector[i] = rand() % 6;
    }
}

int return_rand_vector()
{
    return ranVector[top++];
}

int check_moves(int rV, int aM[6][6])
{
    int i = 0, count = 0;

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

int next_move(int numMoves)
{
    int ranVec = return_rand_vector();
    int nextMove = possibleMoves[ranVec % numMoves];
    return nextMove;
}

double get_score(int rV, int move, int aM[6][6], double qM[6][6])
{
    int count = 0, k = 0, index_of_max, temp_max = 0, max_value = 0, sumA = 0;
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

    //Main updation

    qM[rV][move] = aM[rV][move] + (learningRate * max_value);

    temp_max = 0.0;

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

int main()
{
    int vectorS = 0, vectorE = 5, i, j, adjMatrix[6][6];
    double QMatrix[6][6], score[100];

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            adjMatrix[i][j] = -1;
            QMatrix[i][j] = 0;

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

    init_a_random_vector_array();

    printAdjMatrix(adjMatrix);

    int itter = 0;
    double diff = 0.0;
    time_t start;
    time_t stop;

    for(i = 0; i < 100; i++)
    {
        time(&start);

        int randomVector = return_rand_vector();
        int num_of_possible_moves = check_moves(randomVector, adjMatrix);
        int move = next_move(num_of_possible_moves);
        double current_score = get_score(randomVector, move, adjMatrix, QMatrix);
        score[i] = current_score;

        time(&stop);
        diff += difftime(stop, start);

        printf("Score: %d | Iterations: %d | Time: %gs \n", current_score, itter, diff);
        itter++;
    }

    double maxQval = 0.0;

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

    printf("\n\nTrained Q Matrix: \n");
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            printf("%f\t", QMatrix[i][j] / maxQval * 100.0);
        }
        printf("\n");
    }

    int curr_state=vectorS, visited[6]={0,0,0,0,0,0}, no_way=0, row_max=0,max_ind=0;

    printf("Path: \n");

    while (visited[vectorE]!=1)
    {
        printf("%d-> ",curr_state);

        row_max=0;
        max_ind=0;    

        for(int i=0;i<6;i++)
        {
            if(visited[i]==0)
            {
                if(QMatrix[curr_state][i]>row_max)
                {
                    max_ind=i;
                    row_max=QMatrix[curr_state][i];
                }
            }
        }

        curr_state=max_ind;
        visited[max_ind]=1;

        if(row_max==0)
        {
            no_way=1;
            break;
        }

        if(curr_state==vectorE)
        {
            break;
        }
    }

    if(no_way==1)
    {
        printf("%d | There's no way after this\n");
    }
    else
    {
        printf("%d is the shortest path\n",curr_state);
    }
        
}