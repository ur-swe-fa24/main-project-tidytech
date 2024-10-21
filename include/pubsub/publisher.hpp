#ifndef PUBSUB_PUBLISHER_HPP
#define PUBSUB_PUBLISHER_HPP

#include "subscriber.hpp"
#include <vector>
using namespace std;

class Publisher {
    public:
        virtual ~Publisher() {};
        virtual void notify(vector<string> outputs);
        virtual void add_subs(Subscriber& sub);
    private:
        vector<Subscriber> subscribers_;
        
};

#endif