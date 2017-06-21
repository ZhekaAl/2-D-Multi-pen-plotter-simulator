#ifndef SHARINGQUEUES_H
#define SHARINGQUEUES_H

#include <atomic>
#include "threadqueue.h"


namespace SHARING_QUEUE
{

 ThreadQueue<string>& getCmdQueue();
 ThreadMap<string>& getLogMap();
 atomic<bool>& getToFisinish();

}
#endif // SHARINGQUEUES_H
