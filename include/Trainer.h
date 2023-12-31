#ifndef TRAINER_H_
#define TRAINER_H_

//#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    // Constructor
    Trainer(int t_capacity);
    // Destructor
    virtual ~Trainer();
    // Copy Constructor
    Trainer(const Trainer& other);
    // Copy Assignment Operator
    Trainer& operator=(const Trainer& other);
    // Move Constructor
    Trainer(Trainer&& other);
    //Move Assignment Operator
    Trainer& operator=(Trainer&& other);

    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    void removeOrders(int id);
private:
    void clear();
    void copy(int other_capacity, bool other_open, const std::vector<Customer*>& other_customersList,
              const std::vector<OrderPair>& other_orderList);
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif