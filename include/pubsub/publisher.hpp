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
        virtual void notify(const types::Event& event, const int id, const std::vector<int>& data) = 0;
        
};

#endif