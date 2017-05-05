/*
 * mutex.hpp
 *
 *  Created on: 05/02/2017
 *      Author: Ben Agai
 */

#ifndef MODERNPOSIX_INCLUDE_MUTEX_HPP_
#define MODERNPOSIX_INCLUDE_MUTEX_HPP_

#include <stdexcept>
#include <pthread.h>

using namespace std;

class Mutex
{
//Functions
public:
	Mutex()
	{
		if(pthread_mutex_init(&_mutex, NULL) < 0)
		{
			throw runtime_error("Mutex-error: Failed at initialization");
		}
	}

	virtual ~Mutex()
	{
		if(pthread_mutex_destroy(&_mutex) < 0)
		{
			throw runtime_error("Mutex-error: Failed at destruction");
		}
	}

	void lock()
	{
		if(pthread_mutex_lock(&_mutex) < 0)
		{
			throw runtime_error("Mutex-error: Failed to lock");
		}
	}

	bool tryLock()
	{
		return (pthread_mutex_trylock(&_mutex) == 0)?true:false;
	}

	void unlock()
	{
		if(pthread_mutex_unlock(&_mutex) < 0)
		{
			throw runtime_error("Mutex-error: Failed to unlock");
		}
	}

private:
	//D'ont allow copy constructor/assignment
	Mutex(const Mutex & mutex);
	Mutex & operator=(const Mutex & mutex);

//Members
private:
	pthread_mutex_t _mutex;
};


#endif /* MODERNPOSIX_INCLUDE_MUTEX_HPP_ */
