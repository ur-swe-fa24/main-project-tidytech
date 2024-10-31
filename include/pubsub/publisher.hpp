#ifndef PUBSUB_PUBLISHER_HPP
#define PUBSUB_PUBLISHER_HPP

#include "subscriber.hpp"
#include <vector>
using namespace std;

class Publisher {
    public:
        virtual ~Publisher() {}; // Deconstructor

        // Three standard pubisher methods
        virtual void subscribe(Subscriber* subscriber, const std::string& event) = 0;
        virtual void unsubscribe(Subscriber* subscriber, const std::string& event) = 0;
        virtual void notify(const std::string& event, const std::string& data) = 0;
        
};

#endif