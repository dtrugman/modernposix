/*
 * mutex.hpp
 *
 *  Created on: 05/05/2017
 *      Author: Ben Agai
 */

#include "catch.hpp"

#include "../include/mutex.hpp"

#include <unistd.h>

TEST_CASE("Mutex locking and unlocking", "[mutex]")
{
	SECTION("Mutex try-locking")
	{
		Mutex * myMutex = new Mutex();

		bool isLocked = myMutex->tryLock();
		if(isLocked)
		{
			myMutex->unlock();
		}

		REQUIRE(isLocked);
	}

	SECTION("Mutex try-locking on locked mutex")
	{
		Mutex * myMutex = new Mutex();

		myMutex->lock();

		bool isLocked = myMutex->tryLock();

		myMutex->unlock();

		REQUIRE(isLocked == false);
	}
}

