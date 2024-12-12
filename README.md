# CMSC322 Main Project

## Repo Structure
Here are the main directories:
+ `docs` - this [folder](docs/README.md) maintains **all** of the project documentation: `puml` files and `README`s.
+ `include` - the [include](include/INCLUDE.md) files for the libraries
+ `src` - the [source](src/SRC.md) files for libraries we decide to split out the implementation
+ `app` - the [main application(s)](app/APP.md) of the project
+ `tests` - a battery of [tests](tests/TESTS.md) for our libraries and app

## Code Management

Our development take place on the `integration` branch and *any* feature branches we make.
Each feature branch is tied to a **sprintable PBI** that is in our Github Project backlog.
These issues will become a branch from `integration` using the same naming scheme as we did in the Team Contract:
```
<issue_number>-short-descriptive-title
```

When we are ready to submit new code to the SW manager, submit a PR from `integration` to `main` and **add the SW manager as a reviewer**.
 
---

# Product Overview

At TidyTech, we aim for other companies and offices to use our products, and our goal is for our robots to be really efficient in cleaning, so users can assign different robots and different tasks on different floors. There are three types of robots that accomidate three different floor types. In order for our robots to be efficient, we ensure they have enough charge, enough cleaning product, and enough space in the cleaning compartement to complete their given tasks as efficiently as possible. Our product has direct communication to a field engineer, who will be notified when robots need maintenence to fix errors as quickly as possible. Additionally, senior managers will have access to a wide range of business analytics regarding robot efficiency and cost, through a comprehensive dashboard.

#### New Features (v0.4):
- Ability to open and close app without losing previously inputted information
- New "login" screen to give a different user experiences to different user types
- Ability to add robots/floors/tasks
- Ability to add multiple tasks to the same robot
- Error handling to ensure users can't assign robots to incompatible rooms

#### New Features (v0.5):
- Field engineers can fix errors and reset robot capacity.  
- Fully functional building operator page.
- Senior management dashboard with full robot data reports.  
- Improved UI error handling for edge cases.  
- Simulation runs in the background.  
