#ifndef THREADQUEUE_H
#define THREADQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
using namespace std;

template <typename T>
class ThreadQueue
{
 public:
  T pop()
  {
    std::unique_lock<std::mutex> mlock(mtx);

    if(que.empty()) return T();
    auto item = que.front();
    que.pop();
    return item;
  }

  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mtx);
    que.push(item);
    mlock.unlock();
  }

  void push(T&& item)
  {
    std::unique_lock<std::mutex> mlock(mtx);
    que.push(std::move(item));
    mlock.unlock();
  }

 private:
  queue<T> que;
  mutex mtx;

};




template <typename T>
class ThreadMap
{
public:
  void insertNewKey(string key) {
      std::unique_lock<mutex> mlock(mtx);
      mapTh.insert(make_pair(key,queue<T>()));
      mlock.unlock();
      cond_.notify_one();
  }

  bool emptyQueues()
  {
      bool empty = true;
      for(pair<string,queue<T>> que : mapTh){
          if (!que.second.empty()) {
              empty = false;
              break;
          }
      }
      return empty;
  }

  void push(string key, T&& value)  //key,value
 {
      std::unique_lock<std::mutex> mlock(mtx);
      mapTh.at(key).push(move(value));
      mlock.unlock();
      cond_.notify_one();
 }
  const map<string,queue<T>>& getMapToRead()
  {
      std::unique_lock<std::mutex> mlock(mtx);
      while (emptyQueues())
            {
              cond_.wait(mlock);
            }
      return mapTh;
  }

  T pop(string key)
  {
      std::unique_lock<std::mutex> mlock(mtx);

      if(mapTh.at(key).empty())
          return T();
      auto item = mapTh.at(key).front();
      mapTh.at(key).pop();
      return item;

  }

private:
 map<string,queue<T>> mapTh;
 mutable mutex mtx;
 condition_variable cond_;
};

#endif // THREADQUEUE_H
