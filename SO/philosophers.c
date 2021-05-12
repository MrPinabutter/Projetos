#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define N_PHILOSOPHERS 5

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N_PHILOSOPHERS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var[N_PHILOSOPHERS];
time_t t0;

pthread_mutex_t s[N_PHILOSOPHERS]; 
void *philosopher(void* num);
void take_forks(int i);
void put_forks(int i);
void test(int i);

int main () {
  pthread_t thread1, thread2, thread3, thread4, thread5;
  t0 = time(0);
  int philo1 = 0;
  int philo2 = 1;
  int philo3 = 2;
  int philo4 = 3;
  int philo5 = 4;

  int iret1, iret2, iret3, iret4, iret5;

  iret1 = pthread_create(&thread1, NULL, philosopher, &philo1);
  iret2 = pthread_create(&thread2, NULL, philosopher, &philo2);
  iret3 = pthread_create(&thread3, NULL, philosopher, &philo3);
  iret4 = pthread_create(&thread4, NULL, philosopher, &philo4);
  iret5 = pthread_create(&thread5, NULL, philosopher, &philo5);


  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);
  pthread_join(thread5, NULL);
  
  exit(0);
  return 0;
}

void *philosopher(void *num) {
  int *i = (int*) num;
  state[*i] = THINKING;
  while(1) {
    sleep(1);
    take_forks(*i);
    sleep(1);
    put_forks(*i);
  }
}

void take_forks (int i){
  pthread_mutex_lock(&mutex);
  state[i] = HUNGRY;
  test(i);
  if(state[i] != EATING) {
    pthread_cond_wait(&condition_var[i], &mutex);
  }
  pthread_mutex_unlock(&mutex);
}

void put_forks(int i) {
  pthread_mutex_lock(&mutex); 
  state[i] = THINKING;
  test((i-N_PHILOSOPHERS+1)%N_PHILOSOPHERS);
  test((i+1)%N_PHILOSOPHERS);
  sleep(1);
  pthread_mutex_unlock(&mutex); 
}

void test(int i) {
  if(state[i] == HUNGRY 
    && state[(i+1)%N_PHILOSOPHERS] != EATING 
    && state[(i-1+N_PHILOSOPHERS)%N_PHILOSOPHERS] != EATING)
  {
    state[i] = EATING;
    time_t t1 = time(0);
    //system("clear || cls"); // COMENTE SE VOCE QUER DEIXAR O HISTÓRICO

    printf("Tempo decorrido em segundos: %d \n", (int)difftime(t1, t0));
    printf("Estados atuais: \n");
    for(int j=0; j < N_PHILOSOPHERS; j++){
      if(j==i){
        printf("filosofo %d: Hungry -> Eating\n", j);
      }else{
        printf("filosofo %d: ", j);
        if(state[j] == 0) {
          printf("Thinking \n");
        }else if (state[j] == 1){
          printf("Hungry \n");
        }else {
          printf("Eating \n");
        }
      }
    }
    printf("\n\n");
    sleep(2);
    pthread_cond_signal(&condition_var[i]);
  }
}
