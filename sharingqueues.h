#ifndef SHARINGQUEUES_H
#define SHARINGQUEUES_H

#include <atomic>
#include "synchrocontainer.h"


namespace SHARING_QUEUE
{

 SynchroQueue<string>& getCmdQueue();
 SynchroMap<string>& getLogMap();
 atomic<bool>& getToFisinish();

}
#endif // SHARINGQUEUES_H
