/*
    Thread:
        1 main thread
        3 sub-threads: 
            - Thread 1: get time
            - Thread 2: generate temperature value
            - Thread 3: generate humidity value
        1 queue to store the data
    The main thread will create 3 sub-threads to get the time, generate temperature value, and generate humidity value.
    
    If the queue is empty, the main thread will wait for the sub-threads to finish and then print the data in the queue.
    
    Using mutex and condition variable to synchronize the threads.
    When the queue is full, the sub-threads will wait until the main thread dequeues the data.
    When the queue is empty, the main thread will wait until the sub-threads enqueue the data.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 3
#define MAX_MSG_SIZE 1024
#define QUEUE_SIZE 5

// Queue structure 
typedef struct {
    char *data[QUEUE_SIZE];
    int front, rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;


// Global queue variable
Queue queue;

// Initialize the queue
void initQueue(Queue *q) {
    q->front = q->rear = -1;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
}

// Check if the queue is full
int isFull(Queue *q) {
    return (q->rear + 1) % QUEUE_SIZE == q->front;
}

// Check if the queue is empty
int isEmpty(Queue *q) {
    return q->front == -1;
}

// Clear the queue
void clearQueue(Queue *q) {
    q->front = q->rear = -1;
}

// Enqueue data to the queue
void enQueue(Queue *q, char *data) {
    pthread_mutex_lock(&q->mutex);
    
    while (isFull(q)) {
        pthread_cond_wait(&q->cond, &q->mutex); // Wait if the queue is full
    }
    
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->data[q->rear] = data;

    // Signal other threads that the queue has data
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
}

// Dequeue data from the queue
char* deQueue(Queue *q) {
    pthread_mutex_lock(&q->mutex);
    
    while (isEmpty(q)) {
        pthread_cond_wait(&q->cond, &q->mutex); // Wait if the queue is empty
    }
    
    char *data = q->data[q->front];
    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % QUEUE_SIZE;
    }

    pthread_mutex_unlock(&q->mutex);
    return data;
}

// get time function
void *getTime(void *arg) {
    time_t rawtime;
    struct tm *timeinfo;
    char *timeStr = (char *)malloc(MAX_MSG_SIZE * sizeof(char));
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeStr, MAX_MSG_SIZE, "%Y-%m-%d %H:%M:%S", timeinfo);
    
    enQueue(&queue, timeStr);
}

// Generate temperature value function
void *genTemp(void *arg) {
    srand(time(NULL));
    
    char *tempStr = (char *)malloc(MAX_MSG_SIZE * sizeof(char));
    int temp = rand() % 89 + 10;
    sprintf(tempStr, "Temperature: %d°C", temp);
    
    enQueue(&queue, tempStr);

    return NULL;
}

// Generate humidity value function
void *genHumidity(void *arg) {

    char *humidityStr = (char *)malloc(MAX_MSG_SIZE * sizeof(char));
    int humidity = rand() % 89 + 10;
    sprintf(humidityStr, "Humidity: %d%%", humidity);
    
    enQueue(&queue, humidityStr);

    return NULL;
}

// Thread task function
void *threadTask(void *arg) {
    
    pthread_t threads[NUM_THREADS];
    char *data ;
    
    while (1) {
        pthread_create(&threads[0], NULL, getTime, NULL);
        pthread_join(threads[0], NULL);
        pthread_create(&threads[1], NULL, genTemp, NULL);
        pthread_join(threads[1], NULL);
        pthread_create(&threads[2], NULL, genHumidity, NULL);
        pthread_join(threads[2], NULL);

        while(!isEmpty(&queue)) {
            data = deQueue(&queue);
            printf("%s | ", data);
            free(data);
        }
        clearQueue(&queue);
        printf("\n");
        sleep(1);
    }
    return NULL;
}


int main() {
    // Initialize the queue
    initQueue(&queue);
    
    // Create threads
    pthread_t mainThread;
    pthread_create(&mainThread, NULL, threadTask, NULL);

    // Wait for the threads to finish
    pthread_join(mainThread, NULL);

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.cond);
    
    clearQueue(&queue);
    return 0;
}
