# Requirements
## Customer Interview Document
### Users
- Senior Management: these users care about the efficiency of the robots 
- Building Manager: this user will assign tasks to groups of robot and view the status of robots
- Building Operator: start robots when necessary/assigned and monitor robot status
- Field Engineer: checks/fixes robot errors and add/remove robots to fleet 
### Robots
- The robot must allow for interaction in its mid-run to be able to either cancel the current task or queue the next task
- obot will return to its localization point after finishing the tasks or faced with an error.
- Three types of robots: scrubber, vacuum, and shampoo. Scrubbers work on wood and tile, vacuum cleaners work on all types and shampoos only work on carpets.
- Three sizes of robots: S, M, L. The size determines the battery capacity and thus determines the runtime.
- Should be able to start remotely by someone with limited technical knowledge
- Dirt capacity, water capacity, battery, shampoo capacity are all needed
### Simulation Engine
- The simulation will be discrete-time (integer-time tick)
- The config file should be able to change automatically with the season
### Database
- Information that should be included:
  - Error codes, task start/end time, % of floor each robot completed, battery used, water used, cost/sq ft, expenses (labor, consumable, maintenance)
- Company managers will want to know:
  - A) Are machines being used? B) Do the machines work? C) Are the machines cheaper than the alternatives?
### Management System
- The robot should send notifications regarding its progress to the building manager and/or building operator.
### Rooms/Floors
- There are three types of floors: carpet, tile, and wood and three types of room sizes: small, medium, and large.
- No more than 10 rooms (Includes hallways and elevators)
- Each room will be assigned a value representing the priority
- One floor type per room
- Rooms have restricted access designated by building manager
### Error Handling/Edge Cases
- When a robot “dies,” someone controlling the software should know where the robot was when it died
- If Robot A dies while cleaning Room X, Robot B should know to come finish the incomplete task
- If Robot A is doing Task A, but a user sends a new task, Task B, the robot should switch to doing Task B, before going back to complete Task A
- When faced with error, the robot will report a log consisting of location data, error messages, and/or more specific messages
- Robots should go back to their charging station once they are under X% battery
