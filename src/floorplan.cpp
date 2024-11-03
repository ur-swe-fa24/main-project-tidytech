#include "simulation/floorplan.hpp"
#include "spdlog/spdlog.h"

void FloorPlan::add_floor(const Floor& floor, const std::vector<Floor> neighbors) {
    if (floorgraph_.count(floor) != 1) {
        floorgraph_[floor] = neighbors;
        update_neighbors(floor, true); // Update all other neighbors
    } else {
        // Floor already exist in the graph
        spdlog::error("Floor {} already exist in the Floorplan", floor.get_id());
    }
}

void FloorPlan::remove_floor(const Floor& floor) {
    auto remove = floorgraph_.find(floor);
    if (remove != floorgraph_.end()) {
        update_neighbors(floor, false); // Update all other neighbors
        floorgraph_.erase(remove);
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
    }
}

std::vector<Floor> FloorPlan::get_neighbors(const Floor& floor) const {
    if (floorgraph_.count(floor) == 1) {
        return floorgraph_.at(floor);
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
    }
}

void FloorPlan::update_floor_neighbors(const Floor& floor, const std::vector<Floor> neighbors) {
    auto update = floorgraph_.find(floor);
    if (update != floorgraph_.end()) {
        update_neighbors(floor, false); // Update all other neighbors
        update->second = neighbors;
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
    }
}

void FloorPlan::update_neighbors(const Floor floor, bool add) {
    std::vector<Floor> floors_to_modify = floorgraph_[floor];
    if (add) {
        // append floor to every neighbor's vector
        for (Floor modify_floor : floors_to_modify) {
            floorgraph_[modify_floor].push_back(floor);
        }

    } else {
        // remove floor from every neighbor's vector
        for (Floor modify_floor : floors_to_modify) {
            int pos = 0;
            std::vector<Floor> modify_neighbors = floorgraph_[modify_floor];
            for (pos; pos < modify_neighbors.size(); pos++) {
                if (modify_neighbors[pos] == floor) {
                    break;
                }
            }
            if (pos != modify_neighbors.size()) {
                auto it = floorgraph_[modify_floor].begin() + pos; // iterator to that index of pos
                floorgraph_[modify_floor].erase(it);
            }
        }
    }
}