#include "sharingqueues.h"

namespace SHARING_QUEUE
{

    SynchroQueue<string>& getCmdQueue()
    {
      static SynchroQueue<string> cmdQueue;
      return cmdQueue;
    }

    SynchroMap<string>& getLogMap()
    {
      static SynchroMap<string> logMap;
      return logMap;
    }

   atomic<bool>& getToFisinish()
   {
      static atomic<bool> finishAll;
      return finishAll;
   }


}
