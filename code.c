#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
typedef struct node
{
    void* datap;
    struct node* next;
}stack_node;

typedef struct
{
    int count;
    stack_node* top;
}STACK;

STACK* create_stack()
{
    STACK*stack = (STACK*) malloc(sizeof(STACK));
    if(stack)
    {
        stack -> count = 0;
        stack -> top = NULL;
    }
    return stack;
}

bool push(void* data, STACK* stack)
{
    stack_node* newnode = (stack_node*)malloc(sizeof(stack_node));
    if(newnode)
    {
        newnode->datap = data;
        newnode->next = stack -> top;
        stack -> top = newnode;
        (stack -> count)++;
        return true;
    }
    else
    return false;
}

void* pop(STACK*stack)
{
    void* dp;
    if(stack -> top == NULL)
    {
        dp = NULL;
    }
    else
    {
        stack_node* temp = stack -> top;
        dp = (stack -> top)-> datap;
        stack -> top = (stack -> top)-> next;
        free(temp);
        (stack -> count)--;
    }
    return dp;

}

void* top(STACK*stack)
{
    void* dptr;
    if(stack -> count == 0){
        dptr = NULL;
    }
    else{
    dptr = (stack -> top) -> datap;
    }

    return dptr;
}

int count(STACK* stack)
{
    return stack->count;
}

STACK* destroy(STACK*stack)
{
    if(stack)
    {
        stack_node* temp;
        while(stack->top != NULL)
        {
            free(stack->top->datap);
            temp = stack->top;
            stack->top = stack->top->next;
            free(temp);

        }
        free(stack);
    }
    return NULL;
}

bool emptyStack (STACK* stack) 
{
    if(stack->count == 0)
    return true;
    else
    return false;
}

bool fullStack (STACK* stack) 
{
    stack_node* temp;
    if (temp = (stack_node*)malloc (sizeof(*(stack->top))))
    {
        free(temp);
        return false;
    }
    return true;
}

typedef struct
{
    int row;
    int col;
} POS;

#define ROW 5
#define COL 8

bool isValidSpot(int maze[][COL], int x, int y)
{
    if (x >= 0 && x < ROW && y >= 0 && y < COL && maze[x][y] == 0)
        return true;
    else
        return false;
}

void mouse_simulation(int maze[][COL], POS* start, POS* end)
{
    STACK* visitStack = create_stack();
    STACK* altStack = create_stack();
    bool trap = false;
    bool goal = false;
    POS* currentSpot = start;
    printf("(%d, %d)\n", currentSpot->row, currentSpot->col);
    while (trap == false && goal == false)
    {
        if (currentSpot == end)
        {
            goal = true;
            break;
        }
        maze[currentSpot->row][currentSpot->col] = 5;
        push(currentSpot, visitStack);
        int c = 0;
        POS* nextSpot;

        if (isValidSpot(maze, currentSpot->row + 1, currentSpot->col))
        {
            nextSpot = (POS*) malloc(sizeof(POS));
            nextSpot->row = currentSpot->row + 1;
            nextSpot->col = currentSpot->col;
            push(nextSpot, altStack);
            c++;
        }
        if (isValidSpot(maze, currentSpot->row, currentSpot->col + 1))
        {
            nextSpot = (POS*) malloc(sizeof(POS));
            nextSpot->row = currentSpot->row;
            nextSpot->col = currentSpot->col + 1;
            push(nextSpot, altStack);
            c++;
        }
        if (isValidSpot(maze, currentSpot->row - 1, currentSpot->col))
        {
            nextSpot = (POS*) malloc(sizeof(POS));
            nextSpot->row = currentSpot->row - 1;
            nextSpot->col = currentSpot->col;
            push(nextSpot, altStack);
            c++;
        }
        if (isValidSpot(maze, currentSpot->row, currentSpot->col - 1))
        {
            nextSpot = (POS*) malloc(sizeof(POS));
            nextSpot->row = currentSpot->row;
            nextSpot->col = currentSpot->col - 1;
            push(nextSpot, altStack);
            c++;
        }
        if (c == 1)
        {
            currentSpot = pop(altStack);
            printf("(%d, %d)\n", currentSpot->row, currentSpot->col);
        }
        else if (c > 1)
        {
            currentSpot->row = -1;
            currentSpot->col = -1;
            push(currentSpot, visitStack);
            currentSpot = pop(altStack);
            printf("*(%d, %d)\n", currentSpot->row, currentSpot->col);
        }
        else
        {
            if (emptyStack(altStack))
            {
                trap = true;
                break;
            }
            else if(currentSpot->col == end->col && currentSpot->row == end->row)
            {
                goal = true;
                printf("Exit found! Mouse is free and receives a piece of cheese as a reward.\n");
                for (int i = 0; i < ROW; i++)
                {
                for (int j = 0; j < COL; j++)
                {
                    if(maze[i][j]==5)
                    printf("* ");
                    else
                    printf("%d ", maze[i][j]);
                }
                printf("\n");
                }
                break;
            }
            else
            {
                currentSpot = pop(visitStack);
                printf("\nBacktracking: *(%d, %d) ", currentSpot->row, currentSpot->col);
                maze[currentSpot->row][currentSpot->col] = 0;
                while (!emptyStack(altStack) && currentSpot->row != -1 && currentSpot->col != -1)
                {
                    currentSpot = pop(visitStack);
                    printf("(%d, %d) ", currentSpot->row, currentSpot->col);
                    maze[currentSpot->row][currentSpot->col] = 0;
                }
                printf("\n");
                currentSpot = pop(altStack);
                printf("(%d, %d)\n", currentSpot->row, currentSpot->col);
            }
        }
    }
    if (trap)
        printf("Mouse is trapped!\n");
    free(currentSpot);
    destroy(visitStack);
    destroy(altStack);
}

int main()
{
    int maze[ROW][COL] = {{0, 1, 1, 0, 1, 0, 0, 1},
                          {0, 0, 0, 0, 0, 1, 1, 0},
                          {1, 1, 1, 0, 0, 1, 1, 1},
                          {0, 0, 0, 1, 0, 0, 0, 1},
                          {0, 1, 0, 0, 1, 1, 0, 0}};
    printf("Original maze:\n");
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
    POS* end = (POS*) malloc(sizeof(POS));
    end->row = 4;
    end->col = 7;
    POS* start = (POS*) malloc(sizeof(POS));
    printf("Enter starting point: ");
    scanf("%d %d", &start->row, &start->col);
    while (!isValidSpot(maze, start->row, start->col))
    {
        printf("Invalid starting spot. Enter new coordinates: ");
        scanf("%d %d", &start->row, &start->col);
    }
    mouse_simulation(maze, start, end);
    free(start);
    free(end);

    return 0;
}

// Output:

// Original maze:
// 0 1 1 0 1 0 0 1 
// 0 0 0 0 0 1 1 0 
// 1 1 1 0 0 1 1 1 
// 0 0 0 1 0 0 0 1 
// 0 1 0 0 1 1 0 0 
// Enter starting point: 0 0
// (0, 0)
// (1, 0)
// (1, 1)
// (1, 2)
// (1, 3)
// *(0, 3)

// Backtracking: *(0, 3) (-1, -1) 
// (1, 4)
// (2, 4)

// *(2, 3)

// Backtracking: *(2, 3) (-1, -1) 
// (3, 4)
// (3, 5)
// (3, 6)
// (4, 6)
// (4, 7)

// Exit found! Mouse is free and receives a piece of cheese as a reward.
// * 1 1 0 1 0 0 1 
// * * * * * 1 1 0 
// 1 1 1 0 * 1 1 1 
// 0 0 0 1 * * * 1 
// 0 1 0 0 1 1 * * 
