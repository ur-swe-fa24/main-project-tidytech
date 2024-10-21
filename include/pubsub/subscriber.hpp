#ifndef PUBSUB_SUBSCRIBER_HPP
#define PUBSUB_SUBSCRIBER_HPP
#include <string>
#include <vector>
using namespace std;

class Subscriber {
    public:
        virtual ~Subscriber() {}
        virtual void update(const std::string& event, const std::string& data) = 0;
};

#endif