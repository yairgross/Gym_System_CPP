//#include <vector>
//#include <string>
#include "Workout.h"
#include "Customer.h"
#include <iostream>

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}
Customer::Customer(const Customer& other) : name(other.getName()), id(other.getId()) {}
Customer::~Customer() {}
std::string Customer::getName() const {return name;}
int Customer::getId() const {return id;}

// Implementing the SweatyCustomer class's functions
SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}
SweatyCustomer::SweatyCustomer(const Customer& other) : Customer(other) {}
SweatyCustomer* SweatyCustomer::copyMe() {
    return new SweatyCustomer(*this);
}
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> plan = std::vector<int>();
    // Adding all the anaerobic workouts form workout_options to the plan
    for (Workout w : workout_options) {
        if (w.getType() == CARDIO)
            plan.push_back(w.getId());
    }
    return plan;
}
std::string SweatyCustomer::toString() const {return getName() + ",swt";}

// Implementing the CheapCustomer class's functions
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}
CheapCustomer::CheapCustomer(const Customer& other) : Customer(other) {}
CheapCustomer* CheapCustomer::copyMe() {
    return new CheapCustomer(*this);
}
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> plan = std::vector<int>();
    plan.push_back(findWorkoutIdByPrice(workout_options, CHEAP));
    return plan;
}
std::string CheapCustomer::toString() const {return getName() + ",chp";}

// Implementing the HeavyMuscleCustomer class's functions
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}
HeavyMuscleCustomer::HeavyMuscleCustomer(const Customer& other) : Customer(other) {}
HeavyMuscleCustomer* HeavyMuscleCustomer::copyMe() {
    return new HeavyMuscleCustomer(*this);
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<Workout> anaerobic_workouts = std::vector<Workout>();
    std::vector<int> plan = std::vector<int>();
    for (int i = 0; (i < (int)workout_options.size()) && (workout_options[i].getType()==ANAEROBIC); i++) {
        anaerobic_workouts.push_back(workout_options[i]);
    }
    if ((int)anaerobic_workouts.size() > 0) {
        std::vector<bool> added = std::vector<bool>();
        for (int i = 0; i < (int)(anaerobic_workouts.size()); i++) {
            added.push_back(false);
        }
        int p = 0;
        for (int i = 0; i < (int)(anaerobic_workouts.size()); i++) {
            p = 0;
            while (added[p] && p < (int)added.size())
                p++;
            for (int j = 0; (j < (int)anaerobic_workouts.size()) && (!added[j]); j++) {
                if (anaerobic_workouts[j].getPrice() > anaerobic_workouts[p].getPrice()) {
                    p = j;
                }
                else if ((anaerobic_workouts[j].getPrice() == anaerobic_workouts[p].getPrice()) && anaerobic_workouts[j].getId() < anaerobic_workouts[p].getId()) {
                    p = j;
                }
            }
            plan.push_back(anaerobic_workouts[p].getId());
            added[p] = true;
        }
    }
    return plan;
}
std::string HeavyMuscleCustomer::toString() const {return getName() + ",mcl";}

// Implementing the FullBodyCustomer class's functions
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {}
FullBodyCustomer::FullBodyCustomer(const Customer& other) : Customer(other) {}
FullBodyCustomer* FullBodyCustomer::copyMe() {
    return new FullBodyCustomer(*this);
}
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> plan = std::vector<int>();
    plan.push_back(findWorkoutID(workout_options, CHEAP, CARDIO));
    plan.push_back(findWorkoutID(workout_options, EXPENSIVE, MIXED));
    plan.push_back(findWorkoutID(workout_options, CHEAP, ANAEROBIC));
    return plan;
}
std::string FullBodyCustomer::toString() const {return getName() + ",fbd";}

// Returns the ID of the most expensive/cheap workout of a certain type from a given vector
// If the function returns (-1) it means that there is no workout of the specified type in the vector
int findWorkoutID(const std::vector<Workout>& workouts, RelativePrice rel_price, WorkoutType type) {
    int id(-1);
    std::vector<Workout> suited_type = std::vector<Workout>();
    // Adding to suited_type only the workouts of the relevant type
    for (Workout w : workouts) {
        if (w.getType() == type)
            suited_type.push_back(w);
    }
    // Finding the ID of the most expensive/cheap workout in suited_type according to the requested rel_price
    if (!suited_type.empty())
        id = findWorkoutIdByPrice(suited_type, rel_price);
    return id;
}

// Returns the ID of the cheapest/priciest workout in the given vector
// If two workouts have the same price, it returns the one with the smaller ID
int findWorkoutIdByPrice(const std::vector<Workout>& workouts, RelativePrice rel_price) {
    const Workout* priciest = &workouts[0];
    const Workout* cheapest = &workouts[0];
    int id(-1);
    for (int i = 1; i < (int)(workouts.size()); i++) {
        if ((workouts[i].getPrice() <= cheapest->getPrice())){
            if (workouts[i].getPrice() < cheapest->getPrice()) {
                cheapest = &workouts[i];
            }
            else if (workouts[i].getId() < cheapest->getId()) {
                cheapest = &workouts[i];
            }
        }
        if ((workouts[i].getPrice() >= priciest->getPrice())) {
            if (workouts[i].getPrice() > priciest->getPrice()) {
                priciest = &workouts[i];
            }
            else if (workouts[i].getId() < priciest->getId()) {
                priciest = &workouts[i];
            }
        }
    }
    if (rel_price == CHEAP)
        id = cheapest->getId();
    if (rel_price == EXPENSIVE)
        id = priciest->getId();
    cheapest = nullptr;
    priciest = nullptr;
    return id;
}

// Returns true iff the price of the first workout < the price of the second workout
bool costsLessThan(Workout* w1_ptr, Workout* w2_ptr) {
    if (w1_ptr->getPrice() < w2_ptr->getPrice())
        return true;
    else if (w1_ptr->getPrice() == w2_ptr->getPrice())
        return (w1_ptr->getId() < w2_ptr->getId());
    return false;
}