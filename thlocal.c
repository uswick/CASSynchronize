#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// thlocal obj declaration
typedef struct thlocal_desc{
  pthread_once_t key_once;
  pthread_mutex_t lock;
  pthread_key_t thread_local_key;
  void  (*create_key)(void);
  void* (*create_obj)(void*);
} thlocal_desc_t;

extern thlocal_desc_t local_testq;

typedef struct foo_q{
  int a;
  int b;  
} foo_q_t;

static void* create_foo_q(void * args){
  foo_q_t* f = (foo_q_t*)malloc(sizeof(foo_q_t));
  f->a = 0;
  f->b = 1;
  return f;
}

// thlocal definitions  for local q
static void local_testq_create_key(void) {
  pthread_key_create(&local_testq.thread_local_key, NULL);
}


thlocal_desc_t local_testq = {
  .key_once = PTHREAD_ONCE_INIT,
  .lock = PTHREAD_MUTEX_INITIALIZER,
  .create_key = local_testq_create_key,
  .create_obj = create_foo_q
};

void* get_thread_local(thlocal_desc_t *thdesc){
  pthread_once(&thdesc->key_once, thdesc->create_key);

  void* ptr ;
  if ((ptr = pthread_getspecific(thdesc->thread_local_key)) == NULL) {
    ptr = thdesc->create_obj(NULL);
    pthread_setspecific(thdesc->thread_local_key, ptr);
    /*pthread_mutex_lock(&thdesc->lock);*/
    /*local_data.push_back(static_cast<T*>(ptr));*/
    /*pthread_mutex_lock(&thdesc->lock);*/ 
    /*pthread_setspecific(thdesc->thread_local_key, obj);*/
  }
  return ptr;
}


/**************************************************************
 * Application code
 * ************************************************************/

#define NUM_THREADS 8
#define ITERS 100

void *progress_thread(void *args) {
  int i;
  for (i = 0; i < ITERS; ++i) {
    foo_q_t *f = (foo_q_t*) get_thread_local(&local_testq);	
    f->a += i ;
  }
  
  foo_q_t *foo = (foo_q_t*) get_thread_local(&local_testq);	
  printf("foo q value : %d \n", foo->a);
  
  return 0;
}


int main(int argc, char *argv[])
{
  int i;
  pthread_t th[NUM_THREADS];  
// create progress thread
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&th[i], NULL, progress_thread, NULL);
  }
	
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_join(th[i], NULL);
  }
  
  return 0;
}
