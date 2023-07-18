#include <vector>
#include <iostream>
#include "Customer.h"
#include "Workout.h"
#include "Trainer.h"

typedef std::pair<int, Workout> OrderPair;
// Constructor
Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false),
                                  customersList(std::vector<Customer*>()), orderList(std::vector<OrderPair>()) { // Constructor
}
// Destructor
Trainer::~Trainer() { clear(); }
// Copy Constructor
Trainer::Trainer(const Trainer& other){
    copy(other.capacity, other.open, other.customersList, other.orderList);
}
// Copy Assignment Operator
Trainer& Trainer::operator=(const Trainer& other){
    if((this) != (&other))
    {
        clear();
        copy(other.capacity, other.open, other.customersList, other.orderList);
    }
    return *this;
}
// Move Constructor
Trainer::Trainer(Trainer&& other): capacity(other.capacity), open(other.open),
                                   customersList(other.customersList), orderList(other.orderList)
{
    other.capacity = 0;
    other.open = false;
    other.customersList = std::vector<Customer*>();
    other.orderList = std::vector<OrderPair>();
}
//Move Assignment Operator
Trainer& Trainer::operator=(Trainer&& other){
    if(this != &other)
    {
        clear();
        capacity = other.capacity;
        open = other.open;
        customersList = other.customersList;
        orderList = std::vector<OrderPair>();
        for(OrderPair op: other.orderList){
            Workout w = Workout(op.second);
            orderList.push_back(OrderPair(op.first, w));
        }
        other.capacity = 0;
        other.open = false;
        other.customersList = std::vector<Customer*>();
        other.orderList = std::vector<OrderPair>();
    }
    return *this;
}

//Clear method
void Trainer::clear()
{
    if(&customersList)
    {
        for(Customer* c: customersList)
            delete c;
        customersList.clear();
    }
    if(&orderList)
    {
        orderList.clear();
    }
    open = false;
    capacity = 0;

}
// Copy method
void Trainer::copy(int other_capacity, bool other_open, const std::vector<Customer*>& other_customersList, const std::vector<OrderPair>& other_orderList) {
    capacity = other_capacity;
    open = other_open;
    customersList = std::vector<Customer*>();
    // Copying list of customers
    for(int i = 0; i < (int)other_customersList.size(); i++)
    {
        customersList.push_back(other_customersList[i]->copyMe());
    }
    // Copying list of orders
    orderList = std::vector<OrderPair>();
    for(int i = 0; i< (int)other_orderList.size(); i++)
    {
        orderList.push_back(OrderPair(other_orderList[i].first, Workout(other_orderList[i].second)));
    }
}

int Trainer::getCapacity() const // Returns the Trainer's capacity
{
    return capacity;
}
void Trainer:: addCustomer(Customer* customer) // Adds a new customer to the vector iff the capacity still hasn't been reached
{
    if((int)customersList.size() <= capacity) {
        customersList.push_back(customer);
    }
    else
        std::cout << "This trainer is at full capacity!";
}
void Trainer::removeCustomer(int id) // Removes a customer by ID
{
    bool found = false;
    for(int i = 0; i< (int)customersList.size() && !found; i++)
    {
        if(customersList[i]->getId() == id)
        {
            found = true;
            customersList.erase(customersList.begin()+i,customersList.cbegin()+i+1);
        }
    }
    removeOrders(id);
}

Customer* Trainer::getCustomer(int id) // Returns a pointer to a customer by ID
{
    bool found = false;
    for(int i = 0; i< (int)customersList.size() && !found; i++)
    {
        if(customersList[i]->getId() == id)
        {
            found = true;
            return customersList[i];
        }
    }
    return nullptr;
}
std::vector<Customer*>& Trainer::getCustomers() // Returns a reference to the list of customers
{
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders() // Returns a reference to the list of orders
{
    return orderList;
}
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
// book all workout for a specific customer in the orders list, given that the number of workouts in total will not go over the capacity
{
    for (int i = 0; i < (int) workout_ids.size(); i++) {
        for (int j = 0; j < (int) workout_options.size(); j++) {
            if (workout_options[j].getId() == workout_ids[i]) {
                OrderPair currOrder = OrderPair(customer_id, workout_options[j]);
                orderList.push_back(currOrder);
                j = (int)workout_options.size();
            }
        }
    }
}

void Trainer::removeOrders(int id){
    std::vector<OrderPair> updated = std::vector<OrderPair>();
    for (OrderPair op : orderList) {
        if (op.first != id)
            updated.push_back(op);
    }
    orderList = std::move(updated);
}

void Trainer::openTrainer() // Changes the trainer to open state
    {
        open = true;
    }
    void Trainer::closeTrainer() // changes the trainer to closed state
    {
        open = false;
    }
    int
    Trainer::getSalary() // Sums the prices of all the orders of this trainer and returns the sum as the trainer's salary
    {
        int trainerSalary = 0;
        for (int i = 0; i < (int) orderList.size(); i++) {
            trainerSalary += orderList[i].second.getPrice();
        }
        return trainerSalary;
    }
    bool Trainer::isOpen() // Return the close/open status for the trainer
    {
        return open;
    }

