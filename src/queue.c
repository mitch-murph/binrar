/*
 * Simple Queue implementation using arrays
 */
#include <stdio.h>

#include <stdlib.h>

#define MAX 5

void insert();
void delete ();
void display();
int queue_array[MAX];
int rear = -1;
int front = -1;

int main2()
{
  int choice;
  while (1)
  {
    printf("\n1.Insert element to queue \n");
    printf("2.Delete element from queue \n");
    printf("3.Display all elements of queue \n");
    printf("4.Quit \n\n");
    printf("Enter your choice : ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      insert();
      break;
    case 2:
      delete ();
      break;
    case 3:
      display();
      break;
    case 4:
      exit(1);
    default:
      printf("Invalid selection \n\n");
    } /* End of switch */
  }   /* End of while */
} /* End of main() */

void insert()
{
  int add_item;
  if (rear == MAX - 1)
    printf("Queue is full \n\n");
  else
  {
    if (front == -1)
      /*If queue is initially empty */
      front = 0;
    printf("Inserting element to queue: ");
    scanf("%d", &add_item);
    rear = rear + 1;
    queue_array[rear] = add_item;
    printf("\n");
  }
} /* End of insert() */

void delete ()
{
  if (front == -1 || front > rear)
  {
    printf("Nothing to delete \n\n");
    return;
  }
  else
  {
    printf("Element deleted from queue is : %d\n\n", queue_array[front]);
    front = front + 1;
  }
} /* End of delete() */

void display()
{
  int i;
  if (front == -1)
    printf("Queue is empty \n\n");
  else
  {
    printf("Queue is : \n\n");
    for (i = front; i <= rear; i++)
      printf("%d ", queue_array[i]);

    printf("\n\n");
  }
} /* End of display() */
