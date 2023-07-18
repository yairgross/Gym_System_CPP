#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer(const Customer& other); // Copy constructor
    virtual ~Customer(); // pure virtual destructor
    virtual Customer* copyMe()=0;
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
private:
    const std::string name;
    const int id;
};

int findWorkoutID(const std::vector<Workout>& workouts, RelativePrice rel_price, WorkoutType type);
int findWorkoutIdByPrice(const std::vector<Workout>& workouts, RelativePrice rel_price);
bool costsLessThan(Workout* w1_ptr, Workout* w2_ptr); // TODO: remove if removing std::sort (?)


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    SweatyCustomer(const Customer& other);
    SweatyCustomer* copyMe();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    CheapCustomer(const Customer& other);
    CheapCustomer* copyMe();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    HeavyMuscleCustomer(const Customer& other);
    HeavyMuscleCustomer* copyMe();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    FullBodyCustomer(const Customer& other);
    FullBodyCustomer* copyMe();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


#endif