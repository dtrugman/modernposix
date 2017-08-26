/*
 * mutex.hpp
 *
 *  Created on: 05/02/2017
 *      Author: Ben Agai
 */

#ifndef MP_MUTEX_HPP
#define MP_MUTEX_HPP

#include "modernposix.hpp"

#include <stdexcept>
#include <pthread.h>

using namespace std;

namespace mp
{

class mutex
{
public: // C'tors
	inline mutex()
	{
		int retval = pthread_mutex_init(&_mutex, NULL);
		if(retval != 0)
		{
			char * error = "Underlying mutex <init> failed";
			MP_RETURN_OR_THROW_EX(retval, std::runtime_error, error);
		}
	}

public: // D'tors
	inline virtual ~mutex()
	{
		pthread_mutex_destroy(&_mutex);
	}

public: // Methods
	inline void lock()
	{
		int retval = pthread_mutex_lock(&_mutex);
		if(retval != 0)
		{
			char * error = "Underlying mutex <lock> failed";
			MP_RETURN_OR_THROW_EX(retval, std::runtime_error, error);
		}
	}

	inline bool tryLock()
	{
		return (pthread_mutex_trylock(&_mutex) == 0)?true:false;
	}

	inline void unlock()
	{
		int retval = pthread_mutex_unlock(&_mutex);
		if(retval != 0)
		{
			char * error = "Underlying mutex <unlock> failed";
			MP_RETURN_OR_THROW_EX(retval, std::runtime_error, error);
		}
	}

private: // Methods
	mutex(const mutex & mutex); // D'ont allow copy constructor
	mutex & operator=(const mutex & mutex); // D'ont allow copy assignment

private: // Members
	pthread_mutex_t _mutex;
};
}


#endif /* MP_MUTEX_HPP */
