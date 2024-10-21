#ifndef PUBSUB_SUBSCRIBER_HPP
#define PUBSUB_SUBSCRIBER_HPP
#include <string>
#include <vector>
using namespace std;

class Subscriber {
    public:
        virtual ~Subscriber() {};
        virtual void update(vector<string> inputs);
};

#endif