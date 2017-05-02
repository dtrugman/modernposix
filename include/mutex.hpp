/*
 * mutex.hpp
 *
 *  Created on: 05/02/2017
 *      Author: Ben Agai
 */

#ifndef MODERNPOSIX_INCLUDE_MUTEX_HPP_
#define MODERNPOSIX_INCLUDE_MUTEX_HPP_

#include <pthread.h>

class Mutex
{
//Functions
public:
	Mutex()
	{
		pthread_mutex_init(&_mutex, NULL);
	}

	virtual ~Mutex()
	{
		pthread_mutex_destroy(&_mutex);
	}

	void lock()
	{
		pthread_mutex_lock(&_mutex);
	}

	bool tryLock()
	{
		return (pthread_mutex_trylock(&_mutex) == 0)? true:false;
	}

	void unlock()
	{
		pthread_mutex_unlock(&_mutex);
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
