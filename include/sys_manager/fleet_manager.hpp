#ifndef SYS_MANAGER_FLEET_MANAGER_HPP
#define SYS_MANAGER_FLEET_MANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>

#include "../simulation/simulator.hpp"
#include "../database/database.hpp"
#include "pubsub/publisher.hpp"
#include "pubsub/subscriber.hpp"

class Fleet_manager : public Publisher, public Subscriber{
    public:
        Fleet_manager(Database db);
        enum class UserType { SM, BM, BO, FE };
        void read_ui_input(std::string filepath);
        void write_output(std::string filepath, std::string message); // outputs to a file
        void notify(vector<string> outputs) override;
        void add_subs(Subscriber& sub) override;
        void update(vector<string> inputs) override;
    private:
        // Simulator simulator_;
        UserType user_type_;
        Database database_;
        vector<Subscriber> subscribers_;
};

#endif
