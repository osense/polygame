#ifndef OBJECTUPDATERTHREADED_H_
#define OBJECTUPDATERTHREADED_H_

#include "Object.h"
#include "ObjectManager.h"
#include <irrlicht.h>
#include <thread>
#include <atomic>

class ObjectUpdaterThreaded : public Object
{
public:

	ObjectUpdaterThreaded(SContext* cont);

	virtual ~ObjectUpdaterThreaded();

	void onMessage(SMessage msg);

	void stop();

private:
	void updateFunc();

	std::thread Thread;
	std::atomic<bool> Running;

	static constexpr u32 TargetFPS = 60;
};

#endif /* OBJECTUPDATERTHREADED_H_ */
