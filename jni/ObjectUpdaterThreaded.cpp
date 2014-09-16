/*
 * ObjectUpdaterThreaded.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: entity
 */

#include <ObjectUpdaterThreaded.h>

ObjectUpdaterThreaded::ObjectUpdaterThreaded(SContext* cont) : Object(cont)
{
	Name = "ObjectUpdaterThreaded";
	setPersistent(true);
	Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

	Running = true;
	Thread = std::thread(&ObjectUpdaterThreaded::updateFunc, this);
}

ObjectUpdaterThreaded::~ObjectUpdaterThreaded()
{
	if (Running)
	{
		stop();
	}

	Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectUpdaterThreaded::onMessage(SMessage msg)
{

}

void ObjectUpdaterThreaded::stop()
{
	Running = false;
	Thread.join();
}

void ObjectUpdaterThreaded::updateFunc()
{
	constexpr std::chrono::duration<double> targetSecs(1.0 / TargetFPS);
	std::chrono::duration<double> fDelta(0);

	while (Running)
	{
		auto start = std::chrono::system_clock::now();

		if (Context->TimeScale > 0)
		{
			double scaledDelta = fDelta.count() * Context->TimeScale;
			SMessage msg(this, EMT_UPDATE);
			msg.Update.Delta = scaledDelta * 1000;
			msg.Update.fDelta = scaledDelta;
			broadcastMessage(msg);
		}


		std::chrono::duration<double> execTime = std::chrono::system_clock::now() - start;

		std::this_thread::sleep_for(targetSecs - execTime);
		fDelta = std::chrono::system_clock::now() - start;

#ifdef DEBUG_UPDATES_THREADED
		irr::core::stringc debugText = "updated in ";
		debugText += u32(fDelta.count() * 1000);
		debugText += "ms";
		debugLog(debugText);
#endif
	}
}
