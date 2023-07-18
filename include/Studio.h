#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{
public:
    // Constructors
    Studio();
    Studio(const std::string &configFilePath);
    // Destructor
    virtual ~Studio();
    // Copy Constructor
    Studio(const Studio& other);
    // Copy Assignment Operator
    Studio& operator=(const Studio& other);
    // Move Constructor
    Studio(Studio&& other);
    //Move Assignment Operator
    Studio& operator=(Studio&& other);

    bool isOpen();
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<Trainer*>& getTrainers();
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    int openTrainer(std::string act, int customerID);

private:
    void clear();
    void copy(bool other_open, std::vector<Trainer*> other_trainers, std::vector<Workout> other_workout_options,
              std::vector<BaseAction*> other_actionsLog);
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif
