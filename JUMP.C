#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Note that several variables that are declared as ints should
//be thought of as bools.  Unfortunately, bools aren't in
//standard C.

int Matrix[20][20]; //The adjacency matrix that describes the board
int FinalState[20]; //The target board config
int BoardState[20]; //The current board config
int Path[20][2];    //The moves that are made
int n;              //The number of holes in the board
int SolveDepth;     //The number of moves necessary to solve it

int Solve (int depth)
//The recursive meat of the program
{
 int i, j;
 int Soln;
 int hopover;

 //This if is an optimization that dramatically speeds things up
 //You know the number of moves necessary to reach a particular
 //solution, and that number can't change.  So only check for
 //a solution at that number, and stop recursing there.
 if (depth == SolveDepth)
  {
   Soln = 1;
   for (i=0;i<n;i++)
     if (BoardState[i] != FinalState[i])
       Soln = 0;
   if (Soln)
    {
     for (i=0;i<depth;i++)
       printf ("%6d ->%3d\n", Path[i][0]+1, Path[i][1]+1);
     return 1;
    }
  }
 else
  {
   //All that this problem requires is a brute force solution
   //This chunk of code cycles through every possible move
   //from the current state, and tries em all
   for (i=0;i<n;i++)
     for (j=0;j<n;j++)
      {
       if (BoardState[i] && !BoardState[j] && Matrix[i][j])
        {
         hopover = Matrix[i][j] - 1;
         if (BoardState[hopover])
          {
           //For now, we'll move the pegs and assume that the move is right
           BoardState[i] = BoardState[hopover] = 0;
           BoardState[j] = 1;
           Path[depth][0] = i;
           Path[depth][1] = j;
           //We'll try out our move.  If it works, exit the for loops and
           //stop recursing, cause we've found the answer
           if (Solve(depth+1))
             return 1;
           //If it didn't work, we'll hit this code, which undoes the
           //move we just made
           BoardState[i] = BoardState[hopover] = 1;
           BoardState[j] = 0;
          }
        }
      }
  }
 return 0;  //If we get here, we didn't find the soln this time
}

void main ()
{
 FILE *In;
 int NumSets, m;
 int i, j, k;
 char temp;

 In = fopen ("jump.in", "rt");
 fscanf (In, "%d\n", &NumSets);
 for (i=0;i<NumSets;i++)
  {
   fscanf (In, "%d\n", &n);
   for (j=0;j<n;j++)
     for (k=0;k<n;k++)
       fscanf (In, "%d", &Matrix[j][k]);
   fscanf (In, "%d\n", &m);

   printf ("Board #%d:\n", i+1);
   for (j=0;j<m;j++)
    {
     //Resets the whole array to zero.
     //Be careful with this function - it works byte by byte, so it gets
     //tricky for operation with multi-byte data (like ints)
     memset (BoardState, 0, sizeof(BoardState));
     for (k=0;k<n;k++)
      {
       fscanf (In, "%c", &temp);
       if (temp == '*')
         BoardState[k] = 1;
      }
     fscanf (In, "\n");
     memset (FinalState, 0, sizeof(FinalState));
     for (k=0;k<n;k++)
      {
       fscanf (In, "%c", &temp);
       if (temp == '*')
         FinalState[k] = 1;
      }
     fscanf (In, "\n");
     printf ("  Solution for puzzle #%d:\n", j+1);
     //SolveDepth is the number of moves necessary to get a solution
     //It is the number of pegs in the starting state minus the number
     //of pegs in the final state
     SolveDepth = 0;
     for (k=0;k<n;k++)
      {
       SolveDepth += BoardState[k];
       SolveDepth -= FinalState[k];
      }
     //And start trying solutions.
     Solve(0);
    }
  }
 fclose (In);
}
