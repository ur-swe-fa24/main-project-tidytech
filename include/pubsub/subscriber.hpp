#ifndef PUBSUB_SUBSCRIBER_HPP
#define PUBSUB_SUBSCRIBER_HPP
#include <string>
#include <vector>
#include "types/types.hpp"
using namespace std;

class Subscriber {
    public:
        virtual ~Subscriber() {}; // Deconstructor

        // Update method to send data to subscribers of a particular event
        virtual void update(const types::Event& event, const std::string& data) = 0;
        virtual void update(const types::Event& event, const int id) = 0;
        virtual void update(const types::Event& event, const int id, const types::ErrorType error_type, const bool resolved) = 0;
        virtual void update(const types::Event& event, const int id, const std::vector<int>& data) = 0;
        virtual void update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) = 0; // For Robot update
};

#endif