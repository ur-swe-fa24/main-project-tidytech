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
};

#endif