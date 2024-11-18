#include "simulation/floorplan.hpp"
#include "spdlog/spdlog.h"

// Return all the floors in the FloorPlan 
std::vector<Floor> FloorPlan::get_all_floor() const {
    std::vector<Floor> floors;
    for (const auto& pair : floorgraph_) {
        floors.push_back(pair.first);
    }
    return floors;
}

// Access a Floor with int
Floor FloorPlan::access_floor(int floor_id) {
    for (const auto& pair : floorgraph_) {
        if (floor_id == pair.first.get_id()) {
            return pair.first;
        }
    }
    spdlog::error("Floor id {} does not exist in the FloorPlan", floor_id);
    throw std::runtime_error("Floor not found in the FloorPlan");
}

// to_string for FloorPlan
std::string FloorPlan::to_string() const {
    std::string out_str = "FloorPlan: \n";
    for (const auto& pair : floorgraph_) {
        out_str += "Id: " + std::to_string(pair.first.get_id()) + ", Adjacent Floors: ";
        std::string neighbors_str = "";
        for (const Floor& neighbor : pair.second) {
            neighbors_str += std::to_string(neighbor.get_id()) + ", ";
        }
        if (neighbors_str.length() >= 2) {
        neighbors_str.erase(neighbors_str.length() - 2, 2); // Remove last two characters
        }
        out_str += neighbors_str + "\n";
    }
    return out_str;
}

// to_string for Floor
std::string FloorPlan::floor_to_string(const Floor& floor) const {
    if (floorgraph_.find(floor) == floorgraph_.end()) {
        spdlog::error("Floor id {} does not exist in the floorplan", floor.get_id());
        throw std::runtime_error("Floor not found in the FloorPlan for to_string operation");
    }

    std::string out_str = floor.to_string();
    std::string neighbors_str = "";
    std::vector<Floor> neighbors = floorgraph_.at(floor);
    for (const auto& neighbor : neighbors) {
        neighbors_str += neighbor.get_name() + ", ";
    }

    if (neighbors_str.length() >= 2) {
        neighbors_str.erase(neighbors_str.length() - 2, 2); // Remove last two characters
    }

    return out_str + "Adjacent Floors: [" + neighbors_str + "]";

}

// Add floor to the FloorPlan
void FloorPlan::add_floor(const Floor& floor, const std::vector<Floor> neighbors) {
    if (floorgraph_.count(floor) != 1) {
        floorgraph_[floor] = neighbors;
        update_neighbors(floor, true); // Update all other neighbors
    } else {
        // Floor already exist in the graph
        spdlog::error("Floor {} already exist in the Floorplan", floor.get_id());
        throw std::runtime_error("Floor already in the FloorPlan");
    }
}

// Remove floor from the FloorPlan
void FloorPlan::remove_floor(const Floor& floor) {
    auto remove = floorgraph_.find(floor);
    if (remove != floorgraph_.end()) {
        update_neighbors(floor, false); // Update all other neighbors
        floorgraph_.erase(remove);
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
        throw std::runtime_error("Floor not found in the FloorPlan");
    }
}

void FloorPlan::update_floor(const Floor& floor) {
    auto the_floor = floorgraph_.find(floor);
    auto neighbors = std::vector<Floor>();
    if (the_floor != floorgraph_.end()) {
        neighbors = the_floor->second; // Set the old neighbors
        remove_floor(floor); // Remove the old floor
        add_floor(floor, neighbors); // Add the new floor
        auto newfloor = floorgraph_.find(floor);
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
        throw std::runtime_error("Floor not found in the FloorPlan");
    }
}

// Return the adjacent floors of the floor
std::vector<Floor> FloorPlan::get_neighbors(const Floor& floor) const {
    return floorgraph_.at(floor);
}

// Modify the neighbors of the Floor
void FloorPlan::update_floor_neighbors(const Floor& floor, const std::vector<Floor> neighbors) {
    auto update = floorgraph_.find(floor);
    if (update != floorgraph_.end()) {
        update_neighbors(floor, false); // Remove floor from old neighbors
        update->second = neighbors;
        update_neighbors(floor, true); // Update new neighbors
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
        throw std::runtime_error("Floor not found in the FloorPlan");
    }
}

// Update the neighbors from either adding or removing floor
void FloorPlan::update_neighbors(const Floor& floor, bool add) {
    std::vector<Floor> floors_to_modify = floorgraph_[floor];
    if (add) {
        // Append floor to every neighbor's vector if not already present
        for (Floor& modify_floor : floors_to_modify) {
            std::vector<Floor>& neighbors = floorgraph_[modify_floor];
            if (std::find(neighbors.begin(), neighbors.end(), floor) == neighbors.end()) {
                neighbors.push_back(floor);
            }
        }

    } else {
        // remove floor from every neighbor's vector
        for (Floor modify_floor : floors_to_modify) {
            int pos = 0;
            std::vector<Floor> modify_neighbors = floorgraph_[modify_floor];
            for (int i = 0; i < modify_neighbors.size(); i++) {
                if (modify_neighbors[pos] == floor) {
                    break;
                }
                pos++;
            }
            if (pos != modify_neighbors.size()) {
                auto it = floorgraph_[modify_floor].begin() + pos; // iterator to that index of pos
                floorgraph_[modify_floor].erase(it);
            }
        }
    }
}

// Get the shortest path between two floors and return a queue of floor ids
std::queue<int> FloorPlan::get_path(int floor_id_start, int floor_id_goal) {
    // Edge Cases
    if (floorgraph_.empty()) {
        throw std::runtime_error("The FloorPlan is empty. Cannot compute a path");
    }

    std::queue<Floor> frontier; // Queue
    std::vector<Floor> visited;
    std::unordered_map<int, int> track_path;

    // Initialization
    track_path[floor_id_start] = -1; // Starting point with -1 as its parent id
    frontier.push(access_floor(floor_id_start)); // Push the start node into the frontier

    while (frontier.size() > 0) {
        int curr_floor_id = frontier.front().get_id();
        if (curr_floor_id == floor_id_goal) {
            return get_shortest_path(track_path, floor_id_goal);
        } else {
            Floor curr_floor = access_floor(curr_floor_id);
            for (const Floor& floor : get_neighbors(curr_floor)) {
                if ((!floor_in_frontier(frontier, floor)) && (!floor_visited(visited, floor))) {
                    // Push all its neighbors nodes onto the frontier
                    frontier.push(floor);
                    track_path[floor.get_id()] = curr_floor_id; // Set parent for all of these floors
                }
            }
        }
        visited.push_back(frontier.front());
        frontier.pop(); // Remove the current floor
    }
    // Path does not exist
    throw std::runtime_error("No path found from floor ID " + std::to_string(floor_id_start) + " to floor ID " + std::to_string(floor_id_goal));
}

bool FloorPlan::floor_in_frontier(std::queue<Floor> queue, const Floor& check_floor) {
    // Check if check_floor is in queue
    while (!queue.empty()) {
        if (check_floor == queue.front()) {
            return true;
        }
        queue.pop();
    }

    return false;
}

bool FloorPlan::floor_visited(std::vector<Floor> visited, const Floor& check_floor) {
    // Check if check_floor is in the visited vector
    for (const auto& floor : visited) {
        if (floor == check_floor) {
            return true;
        }
    }

    return false;
}


std::queue<int> FloorPlan::get_shortest_path(std::unordered_map<int,int> track_path, int floor_id_goal) {
    if (track_path.find(floor_id_goal) == track_path.end()) {
        throw std::runtime_error("Goal floor ID " + std::to_string(floor_id_goal) + " is not reachable");
    }

    // Trace the path
    std::vector<int> path_backward; // stack
    int curr = floor_id_goal;
    while (curr != -1) {
        //spdlog::info("Curr floor {}", curr);
        path_backward.push_back(curr);
        curr = track_path[curr];
    }

    std::queue<int> path_forward;
    while(!path_backward.empty()) {
        path_forward.push(path_backward.back());
        path_backward.pop_back();
    }

    return path_forward;
    
}
