#ifndef _THLOCAL_H_
#define _THLOCAL_H_
// thlocal obj declaration
typedef struct thlocal_desc{
  pthread_once_t key_once;
  pthread_mutex_t lock;
  pthread_key_t thread_local_key;
  void  (*create_key)(void);
  void* (*create_obj)(void*);
} thlocal_desc_t;

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

#endif
