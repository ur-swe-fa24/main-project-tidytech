# CMSC322 Main Project (to be deleted)

This main project is your repository for the duration of the semester! **Take care of it!**
Your team must communicate and coordinate to ensure that this project repo does not get totally borked!

## Repo Structure
Here are the main directories you need to worry about this semester:
+ `docs` - this [folder](docs/README.md) maintains **all** of your project documentation: `puml` files and `README`s. You will have deliverables that require updating this documentation.
+ `include` - the [include](include/INCLUDE.md) files for the libraries your team makes
+ `src` - the [source](src/SRC.md) files for libraries you decide to split out the implementation
+ `app` - the [main application(s)](app/APP.md) of the project
+ `tests` - a battery of [tests](tests/TESTS.md) tests for your libraries and app

## Code Management

Your development must take place on your `integration` branch and *any* feature branches you make!
Each feature branch must be tied to a **sprintable PBI** that is in your Github Project backlog.
These issues will become a branch from `integration` using the same naming scheme as you did in the Team Contract:
```
<issue_number>-short-descriptive-title
```

When your team is ready to submit new code to me for the SW manager (me!), you will submit a PR from `integration` to `main` and **add me as a reviewer**.
Following this process is critical to earning your assessment mark at each deliverable.

## Documentation

This project will have a bunch of documentation, and your team will write all of it in Markdown in this repository.
For starters, this `README.md` will evolve to become the landing page of your project.
Thus, make sure that you update it with each sprint to include new developments for design and users.
 
---

# Product Overview

At TidyTech, we aim for other companies and offices to use our products, and our goal is for our robots to be really efficient in cleaning, so users can assign different robots and different tasks on different floors. There are three types of robots that accomidate three different floor types. In order for our robots to be efficient, we ensure they have enough charge, enough cleaning product, and enough space in the cleaning compartement to complete their given tasks as efficiently as possible. Our product has direct communication to a field engineer, who will be notified when robots need maintenence to fix errors as quickly as possible. Additionally, senior managers will have access to a wide range of business analytics regarding robot efficiency and cost, through a comprehensive dashboard.

New Features (v0.4):
- Ability to open and close app without losing previously inputted information
- New "login" screen to give a different user experiences to different user types
- Ability to add robots/floors/tasks
- Ability to add multiple tasks to the same robot
- Error handling to ensure users can't assign robots to incompatible rooms
