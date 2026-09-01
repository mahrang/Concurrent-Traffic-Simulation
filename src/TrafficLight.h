#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


// Defined a class "MessageQueue" which has the public methods send and receive.
// Send takes an rvalue reference of type TrafficLightPhase whereas receive returns this type.
// Also, the class defines an std::dequeue called _queue, which stores objects of type TrafficLightPhase.
// Also, there is an std::condition_variable as well as an std::mutex as private members.

// T represents TrafficLightPhase
template <class T>
class MessageQueue
{
public:
  void send(T &&msg);
  T receive();

private:
  std::deque<T> _queue;
  std::condition_variable _condition;
  std::mutex _mutex;
};

// Defined a class "TrafficLight“ which is a child class of TrafficObject.
// The class has the public methods "void waitForGreen()" and "void simulate()"
// as well as "TrafficLightPhase getCurrentPhase()", where TrafficLightPhase is an enum that
// can be either "red" or "green". Also, I added the private method "void cycleThroughPhases()".
// Furthermore, there is the private member _currentPhase which can take "red" or "green" as its value.

// TrafficLightPhase is an enum that can be either red or green
enum TrafficLightPhase {red, green};

// TrafficLight class is a child class of TrafficObject
class TrafficLight : public TrafficObject
{
public:
    // constructor / destructor
  TrafficLight();
    // getters / setters
  TrafficLightPhase getCurrentPhase();
    // typical behaviour methods
  void waitForGreen();
  void simulate();
  
private:
    // typical behaviour methods
  void cycleThroughPhases();
  TrafficLightPhase _currentPhase;
    // Created a private member of type MessageQueue for messages of type TrafficLightPhase
    // and used it within the infinite loop to push each new TrafficLightPhase into it by calling
    // send in conjunction with move semantics.
  MessageQueue<TrafficLightPhase> _msg;
};

#endif
