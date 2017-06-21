#include "sharingqueues.h"

namespace SHARING_QUEUE
{

    ThreadQueue<string>& getCmdQueue()
    {
      static ThreadQueue<string> cmdQueue;
      return cmdQueue;
    }

    ThreadMap<string>& getLogMap()
    {
      static ThreadMap<string> logMap;
      return logMap;
    }

   atomic<bool>& getToFisinish()
   {
      static atomic<bool> finishAll;
      return finishAll;
   }


}
