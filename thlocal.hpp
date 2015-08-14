#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include <mutex>

using namespace std;
template <class T>
class ThreadLocal {
    pthread_once_t key_once;
    static pthread_key_t thread_local_key;

    //storage of local data
    vector<T*> local_data;
    mutex lock;

public:
    ThreadLocal()
	: key_once{ PTHREAD_ONCE_INIT }
    {
    }

    ~ThreadLocal(){
    	for(T* data : local_data){
		delete data;
	}
    }

    static void create_key(void)
    {
	pthread_key_create(&thread_local_key, NULL);
    }

    T& get_thread_local();
    
    vector<T*> get_all()
    {
	return local_data;
    }
};

