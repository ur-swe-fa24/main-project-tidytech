# Architecture
There are 4 main components that make up the architecture: user interface, fleet manager, simulation, and database. There are sub-components in each that interact with other sub-components from the main components. The function of each is as follows:
- ## User Interface
  - **Screen**: literal monitor the user is using
  - **User type**: refers to the type of user that is using the UI (building manager, senior manager, etc.)
  - **Analytics dashboard**: exclusive to the senior manager. Contains business statistics relevant to managment. Gets the data from the database wrapper component in the fleet manager
  - **Error logs**: exclusive to field engineers. Contains the exceptions the robots throw when there's an error during the simulation. Gets the data from the report generated in the fleet manager
  - **Task initiator**: Component of the user interface that users can use to assign tasks
  - **Robot**: Graphical representation of a robot
  - **Floor**: Graphical representation of the floor layout

- ## Fleet Manager
  - **Database wrapper**: Does all the getting/setting for the database to decrease coupling
  - **Task manager**: Takes the tasks assigned by the user and converts them into the data needed for the simuilation
  - **Simulator Initiator**: Takes the data needed for the simulation and converts it into a configuration file for the simuilator to read
  - **Generate report**: Takes the data about the simulation and converts it into the relevant information for the users to read

- ## Database
  - **Data**: Data needed for the fleet manager (task, robots, etc.)

- ## Simulator
  - **Read config**: Takes the configuration file and initiates all the appropriate variables for the simulator to be able to run
  - **Simulation**: Does the actual simulation itself for the robot fleet
  - **Clock**: Keeps track of discrete time for the simulator
  - **Collect data**: Component to keep track of the relevant data for the users related to the simulation