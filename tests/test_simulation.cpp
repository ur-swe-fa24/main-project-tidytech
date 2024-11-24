#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>

#include "simulation/simulator.hpp"
#include "simulation/floor.hpp"
#include "simulation/floorplan.hpp"
#include "simulation/robot.hpp"
#include "types/types.hpp"

using namespace types;

TEST_CASE("Testing Robot Unit Tests") {
    Robot robo1 = Robot(1, "Jimmy", RobotSize::Large, RobotType::Shampoo, 1, 1, RobotStatus::Available, 100);

    SECTION("Initial Robot Properties") {
        REQUIRE(robo1.get_id() == 1);
        REQUIRE(robo1.get_name() == "Jimmy");
        REQUIRE(robo1.get_size() == RobotSize::Large);
        REQUIRE(robo1.get_curr() == 1);
        REQUIRE(robo1.get_base() == 1);
        REQUIRE(robo1.get_status() == RobotStatus::Available);
        REQUIRE(robo1.get_battery() == 100);
        REQUIRE(robo1.get_remaining_capacity() == 100);
        REQUIRE(robo1.get_curr_path_size() == 0);
        REQUIRE(robo1.get_task_queue().empty());
    }

    SECTION("Robot add tasks to back") {
        std::vector<int> tasks{1,2,3,4,5};
        robo1.add_tasks_to_back(tasks);
        REQUIRE(robo1.get_task_size() == 5);
        REQUIRE(robo1.get_task_queue()[0] == 1);
        REQUIRE(robo1.get_task_queue()[4] == 5);
    }

    SECTION("Robot add tasks to front") {
        std::vector<int> tasks{9,8,7,6};
        std::vector<int> tasks2{1,2,3,4,5};
        std::vector<int> tasks3{11,12,13};
        robo1.add_tasks_to_front(tasks);
        robo1.add_tasks_to_front(tasks2);
        robo1.add_tasks_to_back(tasks3);
        REQUIRE(robo1.get_task_size() == 12);
        REQUIRE(robo1.get_task_queue()[0]==1);
    }

    SECTION("Move to Next Floor") {
        std::queue<int> path;
        path.push(1);
        path.push(2);
        path.push(3);
        robo1.set_curr_path(path);
        robo1.move_to_next_floor(); // At 1
        robo1.move_to_next_floor(); // At 2
        REQUIRE(robo1.get_curr() == 2); // Ensure robot moved
        robo1.move_to_next_floor(); // At 3
        REQUIRE(robo1.get_curr() == 3);
        REQUIRE(robo1.get_curr_path_size() == 0);
    }

    SECTION("Robot Task Management") {
        std::vector<int> tasks{1, 2, 3};
        robo1.add_tasks_to_back(tasks); // Set the task

        std::queue<int> path;
        path.push(2);
        path.push(3);
        robo1.set_curr_path(path); // Set the path

        REQUIRE(!robo1.tasks_empty());
        REQUIRE(robo1.get_first_task() == 1);

        robo1.move_to_next_task(); // Clean room 1
        REQUIRE(robo1.get_task_size() == 2);

        robo1.move_to_next_task(); // Reached room 2
        REQUIRE(robo1.get_status() == RobotStatus::Traveling);
        REQUIRE(robo1.get_task_size() == 2); // Two tasks remain

        robo1.move_to_next_task(); // Clean room 2
        REQUIRE(robo1.get_task_size() == 1); // One task remain
        REQUIRE(robo1.get_status() == RobotStatus::Cleaning);

        robo1.move_to_next_task(); // Reached room 3
        REQUIRE(robo1.get_task_size() == 1); // One task completed

        robo1.move_to_next_task();; // Clean room3
        REQUIRE(robo1.tasks_empty());
    }
    
    SECTION("Battery Consumption and Charging") {
        int initial_battery = robo1.get_battery();
        robo1.consume_power(10);
        REQUIRE(robo1.get_battery() == initial_battery - 10);

        robo1.charge(); // Charge 5 unit power
        robo1.charge();
        REQUIRE(robo1.get_battery() == 100);
    }

    SECTION("Robot Status and Error Fix") {
        // Try to break robot first
        while (robo1.get_status() != RobotStatus::Unavailable) {
            robo1.break_robot();
        }
        
        REQUIRE(robo1.get_status() != RobotStatus::Available);

        robo1.fix_error(); // Fix error
        REQUIRE(robo1.get_status() == RobotStatus::Available);
        REQUIRE(robo1.get_battery() == 100);
    }

    SECTION("Capacity Management") {
        while(!robo1.is_capacity_empty()){
            robo1.clean();
        }

        std::queue<int> path;
        path.push(1);
        robo1.set_curr_path(path); // Set the path

        robo1.go_empty();
        REQUIRE(robo1.get_remaining_capacity() == 0);
        REQUIRE(robo1.get_status() == RobotStatus::NeedEmpty);

        robo1.reset_capacity();
        REQUIRE(robo1.get_remaining_capacity() == 100);
        REQUIRE(robo1.get_status() != RobotStatus::NeedEmpty);
    }

    SECTION("Error Handling For Robot") {
        REQUIRE_THROWS(robo1.move_to_next_task()); // Empty task
        REQUIRE_THROWS(robo1.move_to_next_floor()); // Empty path
        std::queue<int> path;
        path.push(2);
        robo1.set_curr_path(path);
        robo1.move_to_next_floor();
        REQUIRE_THROWS(robo1.charge()); // Cannot charge if not at base
        REQUIRE_THROWS(robo1.consume_power(-10)); // Cannot consume negative power unit
    }
}

TEST_CASE("Testing FloorPlan and Floor Unit Tests") {
    // Create Floor objects
    Floor floor1(1, "Lobby", FloorRoomType::Room, FloorType::Wood, FloorSize::Large, FloorInteraction::High, false, 50);
    Floor floor2(2, "Hallway", FloorRoomType::Hallway, FloorType::Carpet, FloorSize::Medium, FloorInteraction::Moderate, false, 75);
    Floor floor3(3, "Elevator", FloorRoomType::Elevator, FloorType::Tile, FloorSize::Small, FloorInteraction::Low, true, 90);

    // Create FloorPlan
    FloorPlan floorplan;

    SECTION("Add Floors to FloorPlan") {
        floorplan.add_floor(floor1, {});
        floorplan.add_floor(floor2, {floor1});
        floorplan.add_floor(floor3, {floor2});

        auto all_floors = floorplan.get_all_floor();
        REQUIRE(all_floors.size() == 3);

        REQUIRE(floorplan.access_floor(1) == floor1);
        REQUIRE(floorplan.access_floor(2) == floor2);
    }

    SECTION("Remove Floor from FloorPlan") {
        floorplan.add_floor(floor1, {});
        floorplan.add_floor(floor2, {floor1});
        floorplan.add_floor(floor3, {floor2});

        floorplan.remove_floor(floor2);
        auto all_floors = floorplan.get_all_floor();
        REQUIRE(all_floors.size() == 2);
        auto neighbors = floorplan.get_neighbors(floor1);
        REQUIRE(neighbors.size() == 0); // Since floor 2 is removed
    }

    SECTION("Get Neighbors of a Floor") {
        floorplan.add_floor(floor1, {});
        floorplan.add_floor(floor2, {floor1});
        floorplan.add_floor(floor3, {floor2});

        auto neighbors_f2 = floorplan.get_neighbors(floor2);
        REQUIRE(neighbors_f2.size() == 2);
        REQUIRE(neighbors_f2[0] == floor1);
        REQUIRE(neighbors_f2[1] == floor3);

        auto neighbors_f1 = floorplan.get_neighbors(floor1);
        REQUIRE(neighbors_f1.size() == 1);
    }

    SECTION("Update Neighbors of a Floor") {
        floorplan.add_floor(floor1, {});
        floorplan.add_floor(floor2, {floor1});
        floorplan.add_floor(floor3, {floor2});

        auto neighbors = floorplan.get_neighbors(floor1);
        auto neighbors2 = floorplan.get_neighbors(floor2);

        REQUIRE(neighbors.size() == 1);
        REQUIRE(neighbors[0] == floor2); // floor 2 is floor 1's neighbor
        REQUIRE(neighbors2.size() == 2); // floor 2 has two neighbors floor 1 and 3

        floorplan.update_floor_neighbors(floor1, {floor3});
        neighbors = floorplan.get_neighbors(floor1);
        neighbors2 = floorplan.get_neighbors(floor2);

        REQUIRE(neighbors.size() == 1);
        REQUIRE(neighbors[0] == floor3); // floor 3 is floor 1's neighbor
        REQUIRE(neighbors2.size() == 1); // floor 1 is no longer a neighbor of floor 2

    }

    SECTION("Get Shortest Path Between Floors") {
        floorplan.add_floor(floor1, {});
        floorplan.add_floor(floor2, {floor1});
        floorplan.add_floor(floor3, {floor2});

        auto path = floorplan.get_path(1, 3);
        REQUIRE(path.size() == 3);
        REQUIRE(path.front() == 1);
        REQUIRE(path.back() == 3);
    }

    SECTION("Floor Cleaning Behavior") {
        REQUIRE(floor1.get_clean_level() == 50);
        REQUIRE(floor2.get_clean_level() == 75);

        // Simulate cleaning
        floor1.clean(RobotSize::Large);
        floor2.clean(RobotSize::Small);

        REQUIRE(floor1.get_clean_level() == 62); // Large robot cleaning Large room
        REQUIRE(floor2.get_clean_level() == 82); // Small robot cleaning Medium room

        // Floor dirties over time
        floor3.dirty();

        REQUIRE(floor3.get_clean_level()== 89); // Low interaction dirty
    }

    SECTION("Error handling for Floors") {
        floorplan.add_floor(floor1, {});
        floorplan.add_floor(floor2, {floor1});
        floorplan.add_floor(floor3, {floor2});

        REQUIRE_THROWS(floor3.clean(RobotSize::Large)); // Cannot clean restricted floor
        REQUIRE_THROWS(floorplan.add_floor(floor1,{})); // Floor 1 already exists

        floorplan.remove_floor(floor1);
        REQUIRE_THROWS(floorplan.remove_floor(floor1)); // Floor 1 is not in floorplan
        REQUIRE_THROWS(floorplan.update_floor_neighbors(floor1, {}));
        
        REQUIRE_THROWS(floorplan.get_path(2, 1)); // No path

        floorplan.remove_floor(floor2);
        floorplan.remove_floor(floor3);

        REQUIRE_THROWS(floorplan.get_path(1,3)); // Floorplan empty
    }

}

TEST_CASE("Simulator Unit Tests") {
    Simulator sim{};

    SECTION("Add Robot to Simulator") {
        sim.add_robot(1, "Jimmy", RobotSize::Large, RobotType::Shampoo, 1, 1, RobotStatus::Available, 100, {}, {}, 100, 0, 0, 0);
        sim.add_robot(2, "Tommy", RobotSize::Medium, RobotType::Scrubber, 1, 1, RobotStatus::Available, 100, {}, {}, 100, 0, 0, 0);
        REQUIRE(sim.get_num_robots() == 2);
        REQUIRE(sim.get_robot(2).get_id() == 2);
    }

    SECTION("Add Floor to Simulator") {
        sim.add_floor(1, "Lobby", FloorRoomType::Room, FloorType::Wood, FloorSize::Large, FloorInteraction::High, false, 100, {});
        sim.add_floor(2, "Elevator", FloorRoomType::Elevator, FloorType::Tile, FloorSize::Small, FloorInteraction::High, false, 100, {1});

        REQUIRE(sim.get_num_floors() == 2);
        REQUIRE(sim.get_neighbors(1).size() == 1); // One neighbor
    }

    SECTION("Add tasks to robot in Simulator") {
        sim.add_robot(1, "Jimmy", RobotSize::Large, RobotType::Shampoo, 1, 1, RobotStatus::Available, 100, {}, {}, 100, 0, 0, 0);
        sim.add_floor(1, "Lobby", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 100, {});
        sim.add_floor(2, "Office", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 100, {});
        sim.add_floor(3, "Living Room", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 100, {});
        sim.add_floor(4, "Bedroom", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 100, {});


        sim.add_task_to_back(1, {1,2});
        REQUIRE(sim.get_robot(1).get_task_size() == 2);

        sim.add_task_to_front(1, {3,4});
        REQUIRE(sim.get_robot(1).get_task_size() == 4);
        REQUIRE(sim.get_robot(1).get_task_queue()[0] == 3);
    }

    SECTION("Check compatibility") {
        sim.add_robot(1, "Jimmy", RobotSize::Large, RobotType::Shampoo, 1, 1, RobotStatus::Available, 100, {}, {}, 100, 0, 0, 0);
        sim.add_floor(1, "Lobby", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 100, {});

        REQUIRE_THROWS(sim.add_task_to_back(1, {2}));
        REQUIRE_THROWS(sim.add_task_to_front(2, {1}));
        REQUIRE_NOTHROW(sim.add_task_to_back(1, {1}));
    }

    SECTION("Notify testing") {
        REQUIRE_NOTHROW(sim.notify(Event::FiveSecReport, "Example message"));
    }
    

    SECTION("Simulation") {
        sim.add_robot(1, "Jimmy", RobotSize::Large, RobotType::Shampoo, 1, 1, RobotStatus::Available, 100, {}, {}, 100, 0, 0, 0);
        sim.add_robot(2, "Tom", RobotSize::Small, RobotType::Vaccum, 1, 1, RobotStatus::Available, 100, {}, {}, 100, 0, 0, 0);

        sim.add_floor(1, "Lobby", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 90, {});
        sim.add_floor(2, "Office", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 80, {1});
        sim.add_floor(3, "Living Room", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 10, {2});
        sim.add_floor(4, "Bedroom", FloorRoomType::Room, FloorType::Carpet, FloorSize::Large, FloorInteraction::High, false, 100, {});

        sim.add_task_to_back(1, {1,2});
        sim.add_task_to_front(2, {2,3});

        // Simulate for 4 ticks
        for (int i = 0; i < 4; i++) {
            sim.simulate_robots();
            sim.simulate_floors();
        }

        REQUIRE(sim.get_robot(1).get_curr() == 2); // Should be in room2
        REQUIRE(sim.get_robot(2).get_curr() == 2); // Should be in room2
        REQUIRE(sim.get_robot(1).get_status() == RobotStatus::Cleaning);
        REQUIRE(sim.get_robot(2).get_status() == RobotStatus::Cleaning);
        REQUIRE(!sim.get_floor(1).get_getting_clean()); // Not getting clean right now
        REQUIRE(sim.get_floor(2).get_getting_clean()); // floor 2 is getting clean right now

    }
}
