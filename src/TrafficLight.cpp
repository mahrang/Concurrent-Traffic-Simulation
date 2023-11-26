#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    // to wait for and receive new messages and pull them from the queue using move semantics.
    // The received object should then be returned by the receive function.
  std::unique_lock<std::mutex> lock(_mutex);
  /* The line below means "wait if there are no messages".
     It behaves like:  while (pred()==false) wait(lck);
     where the pred() function in our case is !_queue.empty().
     When we have messages, _queue.empty() == false.
     Therefore, !_queue.empty() == true.
     When the pred() function is true, then the thread is unblocked.
     See https://cplusplus.com/reference/condition_variable/condition_variable/wait/ 
     and https://knowledge.udacity.com/questions/929618 */
  _condition.wait(lock, [this] {return !_queue.empty();});
  T msg = std::move(_queue.back());
  _queue.clear();  // b/c of https://knowledge.udacity.com/questions/586056
  return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
  std::lock_guard<std::mutex> lock(_mutex);
  _queue.emplace_back(std::move(msg));
  _condition.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop
    // runs and repeatedly calls the receive function on the message queue.
    // Once it receives TrafficLightPhase::green, the method returns.
    while(true)
    {
        if(_msg.receive() == TrafficLightPhase::green)
          return;
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
  
/* threads is a vector in the TrafficObject class, which is the parent class of the TrafficLight class.  In the line below, you're calling the member function  cycleThroughPhases on the TrafficLight object.  
The way you call a member function in a thread is like so:
Vehicle v;
std::thread t = std::thread(&Vehicle::addID, v); // calling member function addID on object v    
See file "Part 1 - Intro & Running Threads" under the heading "Starting Threads with Member Functions" for more info.  
When you're running multiple threads, it's best to add them to a vector of threads, then loop over the vector at the end of the main function and call join on all the thread objects inside the vector like so:
// call join on all thread objects using a range-based loop
for (auto &t : threads)
     t.join();
If we use push_back() instead of emplace_back() in the line below, we get a compiler error. The problem is that by pushing the thread object into the vector, we attempt to make a copy of it. However, thread objects do not have a copy constructor and thus can not be duplicated. If this were possible, we would create yet another branch in the flow of execution - which is not what we want. The solution to this problem is to use move semantics, which provide a convenient way for the contents of objects to be 'moved' between objects, rather than copied.  To solve our problem, we can use the function emplace_back() instead of push_back(), which internally uses move semantics to move our thread object into the vector without making a copy.
See file "Part 1 - Intro & Running Threads" under the heading "Running Multiple Threads" for more info.  */
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles
    // and toggles the current phase of the traffic light between red and green and sends an update method
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds.
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

  /* The code in this function is similar to the code in Vehicle::drive() at the bottom of Vehicle.cpp */

  // initalize variables
  std::chrono::time_point<std::chrono::system_clock> lastUpdate;
  // from https://knowledge.udacity.com/questions/708499
  std::random_device rd;
  /* std::mt19937 is a Mersenne Twister random number generator.  
  See https://cplusplus.com/reference/random/mt19937/ */
  std::mt19937 eng(rd());  
  // 4000 - 6000 milliseconds; i.e. cycle duration is between 4 - 6 seconds
  std::uniform_int_distribution<int> dist(4000,6000);
  // first random value for time on/off of TrafficLight
  double cycleDuration = dist(eng);

  // If a == b ? c : d, i.e. if a=b, then c, else d.  This is a ternary operator
  auto toggle = [](TrafficLightPhase &_currentPhase) {
    _currentPhase == TrafficLightPhase::red
        ? _currentPhase = TrafficLightPhase::green
        : _currentPhase = TrafficLightPhase::red;
  };
    
  // init stop watch
  lastUpdate = std::chrono::system_clock::now();

  while (true) {
    // sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
    // compute time difference to stop watch
    long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
    
    if (timeSinceLastUpdate >= cycleDuration){
      cycleDuration = dist(rd); // change random value
      toggle(_currentPhase); // change color of the light
      _msg.send(std::move(_currentPhase));  // from FP.4b
      // reset stop watch for next cycle
      lastUpdate = std::chrono::system_clock::now();
    }
  }
}
