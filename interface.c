#include <stdio.h>
#include <stdlib.h>

void snake()
{
    printf("\nSnake Game Selected\n");
}

void maze()
{
    printf("\nMaze Runner Game Selected\n");
}

void about()
{
    printf("\nGame Hub using C Programming\nThis game project is developed using the C programming language as part of our course work. It provides a simple and interactive gaming experience through a console-based interface. The project focuses on applying core programming concepts such as logic building, functions, loops, and user input handling. The main objective of this game is to demonstrate problem-solving skills and practical implementation of C programming in an engaging way.");
}

int main()
{
    int choice;

    while (1)
    {
        printf("\n===== GAME HUB =====\n");
        printf("1. Snake Game\n");
        printf("2. Maze Runner Game\n");
        printf("3. About\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                snake();
                break;
            case 2:
                maze();
                break;
            case 3:
                about();
                break;
            case 4:
                exit(0);
            default:
                printf("\nInvalid Choice\n");
        }
    }

    return 0;
}
