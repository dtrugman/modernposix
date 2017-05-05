/*
 * mutex.hpp
 *
 *  Created on: 05/05/2017
 *      Author: Ben Agai
 */

#include <iostream>

#include "../include/mutex.hpp"

using namespace std;

int main()
{
	Mutex * myMutex = new Mutex();

	myMutex->lock();
	cout << "We locked a mutex";
	myMutex->unlock();
	cout << "We unlocked a mutex";

	if(myMutex->tryLock())
	{
		cout << "We tried and succeeded to lock a mutex";
		myMutex->unlock();
		cout << "We unlocked a mutex";
	}

    return 0;
}
