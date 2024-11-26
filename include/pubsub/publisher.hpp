#ifndef PUBSUB_PUBLISHER_HPP
#define PUBSUB_PUBLISHER_HPP

#include "subscriber.hpp"
#include <vector>
#include "types/types.hpp"
using namespace std;

class Publisher {
    public:
        virtual ~Publisher() {}; // Deconstructor

        // Three standard pubisher methods
        virtual void subscribe(Subscriber* subscriber, const types::Event& event) = 0;
        virtual void unsubscribe(Subscriber* subscriber, const types::Event& event) = 0;
        virtual void notify(const types::Event& event, const std::string& data) = 0;
        virtual void notify(const types::Event& event, const int id) = 0;
        virtual void notify(const types::Event& event, const int id, const int val) = 0;
        virtual void notify(const types::Event& event, const int id, const types::ErrorType error_type, const bool resolved) = 0;
        virtual void notify(const types::Event& event, const int id, const std::vector<int>& data) = 0;
        virtual void notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) = 0;
        
};

#endif