#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];

void* philosopher(void* num) {
    int id = *(int*)num;

    // Choose left and right chopsticks
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    // Deadlock avoidance: last philosopher picks right first
    if (id == NUM_PHILOSOPHERS - 1) {
        int temp = left;
        left = right;
        right = temp;
    }

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", id);
        sleep(1);

        // Pick up chopsticks
        pthread_mutex_lock(&chopsticks[left]);
        printf("Philosopher %d picked up chopstick %d (left).\n", id, left);
        pthread_mutex_lock(&chopsticks[right]);
        printf("Philosopher %d picked up chopstick %d (right).\n", id, right);

        // Eating
        printf("Philosopher %d is eating.\n", id);
        sleep(2);

        // Put down chopsticks
        pthread_mutex_unlock(&chopsticks[right]);
        pthread_mutex_unlock(&chopsticks[left]);
        printf("Philosopher %d put down chopsticks.\n", id);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Join threads (they never finish in this example)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
