#ifndef SIMULATION_FLOORPLAN_HPP
#define SIMULATION_FLOORPLAN_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "floor.hpp"
#include "types/types.hpp"

using namespace types;

class FloorPlan {
    public:
        FloorPlan() {}; // Default Constructor
        ~FloorPlan() {}; // Destructor

        std::vector<Floor> get_all_floor();
        void add_floor(const Floor& floor, const std::vector<Floor> neighbors);
        void remove_floor(const Floor& floor);
        std::vector<Floor> get_neighbors(const Floor& floor) const;
        void update_floor_neighbors(const Floor& floor, const std::vector<Floor> neighbors);


    private:
        std::unordered_map<Floor, std::vector<Floor>> floorgraph_;
        void update_neighbors(const Floor floor, bool add);
};

#endif