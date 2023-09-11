# Fitness Studio Management System
In this document, we'll provide an overview of the fitness studio management system, designed and implemented for Systems Programming course at Ben-Gurion University.

## Overview
The system simulates the operations of a fitness studio, including trainers, customers, workout plans, and actions. It was developed in C++ and is designed to be run on a UNIX-based machine.

## Features
**Studio**: Manages trainers and workout plans.
**Trainer**: Represents trainers with available spots and a list of customers.
**Workout**: Defines workouts with an ID, name, price, and type.
**Customer**: Abstract class with various ordering strategies.
**BaseAction**: Abstract class for actions with logging capabilities.


## Input Configuration
The fitness studio configuration is loaded from a text-based config file. It includes details such as the number of trainers, trainer capacities, and workout options.
Here's an example configuration:

### Number of trainers
4
### Trainer capacities
6,6,8,4
### Workout options
Yoga, Anaerobic, 90
Pilates, Anaerobic, 110
Spinning, Mixed, 120
Zumba, Cardio, 100
Rope Jumps, Cardio, 70
CrossFit, Mixed, 140


## Actions
You can interact with the fitness studio through various actions, including opening trainer sessions, ordering workouts, moving customers, and more. Each action has a specific syntax and purpose.
Here are a few example actions:

**OpenTrainer**: Open a trainer's workout session.
open <trainer_id> <customer1>,<customer1_strategy> <customer2>,<customer2_strategy> ...

**Order**: Customers order workouts during a trainer's session.
order <trainer_id>

**Close**: Close a trainer's session and print their salary.
close <trainer_id>

**Backup and Restore**: The system allows you to create backups and restore the fitness studio's state. You can use these features to save and load your studio's progress.

BackupStudio: Save all studio information.

backup

RestoreStudio: Restore the last saved studio state.

restore


## Implementation Notes
We followed the Rule of 5 for proper resource management.
