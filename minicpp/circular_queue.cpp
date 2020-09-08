/*******************************************************************************
 * A basic test implementation of a circular queue type data structure.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <cstdlib>
#include <cstdio>

constexpr int QUEUE_SIZE = 4;

static int queue[QUEUE_SIZE];

static int front = -1;
static int rear  = -1;

static inline void Insert  ();
static inline void Delete  ();
static inline void Display ();

int main (int _argc, char* _argv[])
{
    while (true) {
        Insert();
        Display();
    }

    return 0;
}

static inline void Insert ()
{
    int value;
    printf("Value to Insert: ");
    scanf("%d", &value);

    // If the queue is full we must remove something first.
    if ((rear == QUEUE_SIZE-1 && front == 0) || (front == rear+1)) {
        Delete();
    }

    printf("Inserted %d!\n", value);
    if ((rear == QUEUE_SIZE-1) && (front != 0)) {
        rear = -1;
    }
    queue[++rear] = value;
    if (front == -1) {
        front = 0;
    }
}

static inline void Delete ()
{
    if (front == -1 && rear == -1) {
        printf("Queue is empty, cannot delete!\n");
    } else {
        printf("Deleted %d!\n", queue[front++]);
        if (front-1 == rear) {
            front = -1;
            rear = -1;
        } else {
            if (front == QUEUE_SIZE) {
                front = 0;
            }
        }
    }
}

static inline void Display ()
{
    if (front == -1) {
        printf("Queue is empty, cannot display!\n");
    } else {
        if (front <= rear) {
            for (int i=front; i<=rear; ++i) {
                printf("%d = %d\n", i, queue[i]);
            }
        } else {
            for (int i=front; i<=QUEUE_SIZE-1; ++i) {
                printf("%d = %d\n", i, queue[i]);
            }
            for (int i=0; i<=rear; ++i) {
                printf("%d = %d\n", i, queue[i]);
            }
        }
    }
}

/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
*******************************************************************************/
