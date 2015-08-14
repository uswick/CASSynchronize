#include "thlocal.hpp"

template <class T>
pthread_key_t ThreadLocal<T>::thread_local_key;


template <class T>
T& ThreadLocal::get_thread_local()
    {
	pthread_once(&key_once, &ThreadLocal::create_key);

	void* ptr ;
	if ((ptr = pthread_getspecific(thread_local_key)) == NULL) {
	    ptr = new T();
	    lock.lock();
	    local_data.push_back(static_cast<T*>(ptr));
	    lock.unlock();
	    pthread_setspecific(thread_local_key, ptr);
	}

	return *static_cast<T*>(ptr);
    }


