#include <windows.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <conio.h>
#include <math.h>

#define XAXIS 100
#define YAXIS 20
#define UP 72
#define RIGHT 77
#define DOWN 80
#define LEFT 75




int head_direction = 0; // 0 Right, 1 Left, 2 Up, 3 Down;
int score = 0;
int level = 0;
int level_speed = 5000;
void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
typedef struct node
{
    int x;
    int y;
    struct node* next;
    struct node* prev;
}node;

void generate_food();
void write_score();
void clear_memory(node *tail_ptr);

int main(void)
{
    char key;
    system("CLS");
    system("color 0a");
	for(int i = 0; i < XAXIS; i++)
    {
        for (int j = 0; j < YAXIS; j++)
        {
            if(i == 0 || j == 0 || i == (XAXIS - 1) || j == (YAXIS-1))
            {
                gotoxy(i,j);
                printf("*");
            }
        }
    }
    write_score();
    node* head = NULL;
    node* tail = NULL;

    node* n = malloc(sizeof(node));
    if (n == NULL)
    {   
        printf("Null Pointer\n");
        return 1;
    }
    n->prev = NULL;
    n->next = NULL;
    n->x = 23;
    n->y = 10;
    head = n;
    tail = n;

    n = malloc(sizeof(node));
    if (n == NULL)
    {   
        printf("Null Pointer\n");
        return 1;
    }
    n->prev = head;
    n->next = NULL;
    head->next = n;
    n->x = 24;
    n->y = 10;
    head = n;

    n = malloc(sizeof(node));
    if (n == NULL)
    {   
        printf("Null Pointer\n");
        return 1;
    }
    n->prev = head;
    n->next = NULL;
    head->next = n;
    n->x = 25;
    n->y = 10;
    head = n;

    n = malloc(sizeof(node));
    if (n == NULL)
    {   
        printf("Null Pointer\n");
        return 1;
    }
    n->prev = head;
    n->next = NULL;
    head->next = n;
    n->x = 26;
    n->y = 10;
    head = n;

    n = malloc(sizeof(node));
    if (n == NULL)
    {   
        printf("Null Pointer\n");
        return 1;
    }
    n->prev = head;
    n->next = NULL;
    head->next = n;
    n->x = 27;
    n->y = 10;
    head = n;

    n = malloc(sizeof(node));
    if (n == NULL)
    {   
        printf("Null Pointer\n");
        return 1;
    }
    n->prev = head;
    n->next = NULL;
    head->next = n;
    n->x = 28;
    n->y = 10;
    head = n;

    for (node* temp = tail; temp != NULL; temp= temp->next)
    {
        gotoxy(temp->x,temp->y);
        if(temp == head)
        {
            printf(">");
            continue;
        }
        printf("*");
    }
    node* food = malloc(sizeof(node));
    if (food == NULL)
    {   
        printf("Null Pointer\n");
        return 1;
    }
    generate_food(tail,food);
    int k = 0;
    label:
    while(!kbhit())
    {
        
        if(k== level_speed)
        {
            if(head_direction == 0)
            {
                goto labelright;
            }
            if(head_direction == 1)
            {
                goto labelleft;
            }
            if(head_direction == 2)
            {
                goto labelup;
            }
            if(head_direction == 3)
            {
                goto labeldown;
            }
            k = 0;
        }
        k++;
    }
    key = getch();
    
    
    
    node* temporary = NULL;
    if(key == RIGHT)
    {
        if (head_direction == 1)    // 0 Right, 1 Left, 2 Up, 3 Down;
        {
            goto label;
        }
        labelright:
        k=0;
        head_direction = 0;
        n = malloc(sizeof(node));
        if (n == NULL)
        {   
            printf("Null Pointer\n");
            return 1;
        }
        gotoxy(head->x,head->y);
        printf("*");
        n->x = (head->x)+1;
        n->y = head->y;
        head->next = n;
        n->prev = head;
        n->next = NULL;
        head = n;
        gotoxy(head->x,head->y);
        printf(">");
        if ((head->x == food->x) && (head->y == food->y))
        {
            score++;
            write_score();
            goto label1;
        }
        if (head->x == (XAXIS - 1))
        {
            system("CLS");
            printf("YOU LOST!!!\n");
            clear_memory(tail);
            printf("Press Any Key to  kill this window");
            while(!kbhit())
            {

            }
            return 0;
        }
        gotoxy(tail->x,tail->y);
        printf(" ");
        tail->next->prev = NULL;
        temporary = tail->next;
        free(tail);
        tail = temporary;
        goto label;
        label1:
        generate_food(tail,food);
    }
    if (key == UP)
    {   
        if (head_direction == 3)    // 0 Right, 1 Left, 2 Up, 3 Down;
        {
            goto label;
        }
        labelup:
        k=0;
        head_direction = 2;
        n = malloc(sizeof(node));
        if (n == NULL)
        {   
            printf("Null Pointer\n");
            return 1;
        }
        gotoxy(head->x,head->y);
        printf("*");
        head->next = n;
        n->x = head->x;
        n->y = (head->y) - 1;
        n->prev = head;
        n->next = NULL;
        head = n;
        gotoxy(head->x,head->y);
        printf("^");
        if ((head->x == food->x) && (head->y == food->y))
        {
            score++;
            write_score();
            goto label2;
        }
        if (head->y == 0)
        {
            system("CLS");
            printf("YOU LOST!!!\n");
            clear_memory(tail);
            printf("Press Any Key to  kill this window");
            while(!kbhit())
            {

            }
            return 0;
        }
        gotoxy(tail->x,tail->y);
        printf(" ");
        tail->next->prev = NULL;
        temporary = tail->next;
        free(tail);
        tail = temporary;
        goto label; 
        label2:
        generate_food(tail,food);
    }
    if (key == DOWN)
    {
        if (head_direction == 2)    // 0 Right, 1 Left, 2 Up, 3 Down;
        {
            goto label;
        }
        labeldown:
        k=0;
        head_direction = 3;
        n = malloc(sizeof(node));
        if (n == NULL)
        {   
            printf("Null Pointer\n");
            return 1;
        }
        gotoxy(head->x,head->y);
        printf("*");
        head->next = n;
        n->x = head->x;
        n->y = (head->y) + 1;
        n->prev = head;
        n->next = NULL;
        head = n;
        gotoxy(head->x,head->y);
        printf("v");
        if ((head->x == food->x) && (head->y == food->y))
        {
            score++;
            write_score();
            goto label3;
        }
        if (head->y == (YAXIS - 1))
        {
            system("CLS");
            printf("YOU LOST!!!\n");
            clear_memory(tail);
            printf("Press Any Key to  kill this window");
            while(!kbhit())
            {

            }
            return 0;
        }
        gotoxy(tail->x,tail->y);
        printf(" ");
        tail->next->prev = NULL;
        temporary = tail->next;
        free(tail);
        tail = temporary;
        goto label;
        label3:
        generate_food(tail,food);
    }
    if(key == LEFT)
    {
        if (head_direction == 0)    // 0 Right, 1 Left, 2 Up, 3 Down;
        {
            goto label;
        }
        labelleft:
        k=0;
        head_direction = 1;
        n = malloc(sizeof(node));
        if (n == NULL)
        {   
            printf("Null Pointer\n");
            return 1;
        }
        gotoxy(head->x,head->y);
        printf("*");
        n->x = (head->x)-1;
        n->y = head->y;
        head->next = n;
        n->prev = head;
        n->next = NULL;
        head = n;
        gotoxy(head->x,head->y);
        printf("<");
        if ((head->x == food->x) && (head->y == food->y))
        {
            score++;
            write_score();
            goto label4;
        }
        if (head->x == 0)
        {
            system("CLS");
            printf("YOU LOST!!!\n");
            clear_memory(tail);
            printf("Press Any Key to  kill this window");
            while(!kbhit())
            {

            }
            return 0;
        }
        gotoxy(tail->x,tail->y);
        printf(" ");
        tail->next->prev = NULL;
        temporary = tail->next;
        free(tail);
        tail = temporary;
        goto label;
        label4:
        generate_food(tail,food);

    }
    goto label;
    return 0;
}
void generate_food(node* tail,node* f)
{
    unsigned int bayrak = 0;
    srand(time(0));
    
    node* temp = NULL;
    label:
    f->x =(rand() % (XAXIS - 2)) + 1;
    f->y = (rand() % (YAXIS - 2)) + 1;
    f->next = NULL;
    f->prev = NULL;
    for (node* temp = tail; temp != NULL; temp= temp->next)
    {
        
        if((temp->x == f->x) && (temp->y == f->y))
        {
            bayrak = 1;
            break;
        }
    }
    if (bayrak == 1)
    {
        goto label;
    }
    gotoxy(f->x,f->y);
    printf("F");
}
void write_score()
{
    gotoxy(0,YAXIS);
    printf("SCORE: %i",score);
    if((score % 5) == 0)
    {
        level++;
        level_speed = level_speed /((float)level/(1.618));
    }
    gotoxy(10,YAXIS);
    printf("LEVEL: %i",level);
} 
void clear_memory(node* tail_ptr)
{
    node* temp = NULL;
    while(tail_ptr->next != NULL)
    {
        temp = tail_ptr->next;
        free(tail_ptr);
        tail_ptr = temp;
    }
}