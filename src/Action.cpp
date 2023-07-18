#include <string>
#include <iostream>
#include "Customer.h"
#include "Action.h"
#include "Studio.h"

extern Studio* backup;

BaseAction::BaseAction() {}
BaseAction::BaseAction(std::string error_message) : errorMsg(error_message) {}
BaseAction::BaseAction(const BaseAction& other) : errorMsg(other.errorMsg), status(other.status) {}
BaseAction::~BaseAction() {} // Destructor
ActionStatus BaseAction::getStatus() const {return status;}
void BaseAction::complete() {status = COMPLETED;}
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    std::cout << "Error: " << errorMsg << std::endl;
}
std::string BaseAction::getErrorMsg() const {return errorMsg;}

// Implementing the OpenTrainer class's methods
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList)
    : BaseAction("Trainer does not exist or is not open"), trainerId(id) {
    customers = std::vector<Customer*>();
    for (Customer* c : customersList) {
        customers.push_back(c->copyMe());
    }
}
OpenTrainer::OpenTrainer(const OpenTrainer& other) : BaseAction(other), trainerId(other.trainerId) {
    customers = std::vector<Customer*>();
    for(int i = 0; i < (int)other.customers.size(); i++)
    {
        customers.push_back(other.customers[i]->copyMe());
    }
}
OpenTrainer* OpenTrainer::copyMe() {
    return new OpenTrainer(*this);
}
OpenTrainer::~OpenTrainer(){
    if(&customers) {
        for (Customer *c: customers) {
            delete c;
        }
        customers.clear();
    }
}
void OpenTrainer::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || trainer->isOpen()) // Checking for errors
        error(getErrorMsg());
    else {
        trainer->openTrainer();
        for (Customer *c: customers)
            trainer->addCustomer(c->copyMe());
        complete();
    }
}
std::string OpenTrainer::toString() const {
    std::string s = "open " + std::to_string(trainerId) ;
    for (Customer* c : customers)
        s = s + " " + c->toString();
    if (getStatus() == COMPLETED)
        s = s + " Completed";
    else
        s = s + " Error: " + getErrorMsg();
    return s;
}


// Implementing the Order class's methods
Order::Order(int id)
    : BaseAction("Trainer does not exist or is not open"), trainerId(id) {}
Order::Order(const Order& other) : BaseAction(other), trainerId(other.trainerId) {}
Order* Order::copyMe() {
    return new Order(*this);
}
void Order::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !(trainer->isOpen()))
        error(getErrorMsg());
    else {
        for (Customer *c: trainer->getCustomers()) {
            int customerID = c->getId();
            std::vector<int> customerOrderOptions = c->order(studio.getWorkoutOptions());
            trainer->order(customerID, customerOrderOptions, studio.getWorkoutOptions());
            c = nullptr;
        }
        for (OrderPair op: trainer->getOrders()) {
            std::cout << trainer->getCustomer(op.first)->getName() << " Is Doing " << op.second.getName() << std::endl;
        }
        trainer = nullptr;
        complete();
    }
}
std::string Order::toString() const {
    std::string s = "order " + std::to_string(trainerId);
    if (getStatus() == COMPLETED)
        s = s + " Completed";
    else
        s = s + " Error: " + getErrorMsg();
    return s;
}


// Implementing the MoveCustomer class's methods
MoveCustomer::MoveCustomer(int src, int dst, int customerId)
    : BaseAction("Cannot move customer"), srcTrainer(src), dstTrainer(dst), id(customerId) {}
MoveCustomer::MoveCustomer(const MoveCustomer& other)
    : BaseAction (other), srcTrainer(other.srcTrainer), dstTrainer(other.dstTrainer), id(other.id) {}
MoveCustomer* MoveCustomer::copyMe() {
    return new MoveCustomer(*this);
}
void MoveCustomer::act(Studio &studio) {
    Trainer* src = studio.getTrainer(srcTrainer);
    Trainer* dst = studio.getTrainer(dstTrainer);
    if (src == nullptr || dst == nullptr)
        error(getErrorMsg());
    else if (!src->isOpen() || !dst->isOpen())
        error(getErrorMsg());
    else if (src->getCustomers().empty() || (int) dst->getCustomers().size() >= dst->getCapacity())
        error(getErrorMsg());
    else {
        dst->addCustomer(src->getCustomer(id));
        src->removeCustomer(id);
        if (src->getCustomers().empty())
            src->closeTrainer();
        Customer* c = dst->getCustomer(id);
        std::vector<int> customerOrderOptions = c->order(studio.getWorkoutOptions());
        dst->order(id, customerOrderOptions, studio.getWorkoutOptions());
        src = nullptr;
        dst = nullptr;
        complete();
    }
}
std::string MoveCustomer::toString() const {
    std::string s = "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id);
    if (getStatus() == COMPLETED)
        s = s + " Completed";
    else
        s = s + " Error: " + getErrorMsg();
    return s;
}


// Implementing the Close class's methods
Close::Close(int id) : BaseAction("Trainer does not exist or is not open"), trainerId(id) {}
Close::Close(const Close& other) : BaseAction(other), trainerId(other.trainerId) {}
Close* Close::copyMe() {
    return new Close(*this);
}
void Close::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen())
        error(getErrorMsg());

    else {
        trainer->closeTrainer();
        std::cout << "Trainer " << trainerId << " closed. Salary " << trainer->getSalary() << "NIS" << std::endl;
        trainer = nullptr;
        complete();
    }
}
std::string Close::toString() const {
    std::string s = "close " + std::to_string(trainerId);
    if (getStatus() == COMPLETED)
        s = s + " Completed";
    else
        s = s + " Error: " + getErrorMsg();
    return s;
}


// Implementing the CloseAll class's methods
CloseAll::CloseAll() : BaseAction() {}
CloseAll::CloseAll(const CloseAll& other) : BaseAction(other) {}
CloseAll* CloseAll::copyMe() {
    return new CloseAll(*this);
}
void CloseAll::act(Studio &studio) {
    for (int i = 0; i < (int)studio.getTrainers().size(); i++) {
        if (studio.getTrainer(i)->isOpen()) {
            Close c = Close(i);
            c.act(studio);
        }
    }
    complete();
}
std::string CloseAll::toString() const {return "closeall Completed";}


// Implementing the PrintWorkoutOptions class's methods
PrintWorkoutOptions::PrintWorkoutOptions() : BaseAction() {}
PrintWorkoutOptions::PrintWorkoutOptions(const PrintWorkoutOptions& other) : BaseAction(other) {}
PrintWorkoutOptions* PrintWorkoutOptions::copyMe() {
    return new PrintWorkoutOptions(*this);
}
void PrintWorkoutOptions::act(Studio &studio) {
    for (Workout w : studio.getWorkoutOptions()) {
        WorkoutType wType = w.getType();
        std::string wTypeStr = "Cardio";
        if(wType == ANAEROBIC)
            wTypeStr = "Anaerobic";
        else if(wType == MIXED)
            wTypeStr = "Mixed";
        std::cout << w.getName() << ", " << wTypeStr << ", " << w.getPrice() << std::endl;
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const {return "workout_options Completed";}


// Implementing the PrintTrainerStatus class's methods
PrintTrainerStatus::PrintTrainerStatus(int id) : BaseAction(), trainerId(id) {}
PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus& other) : BaseAction(other), trainerId(other.trainerId) {}
PrintTrainerStatus* PrintTrainerStatus::copyMe() {
    return new PrintTrainerStatus(*this);
}
void PrintTrainerStatus::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer != nullptr) {
        if (trainer->isOpen()) {
            std::cout << "Trainer " << trainerId << " status: open" << std::endl;
            std::cout << "Customers:" << std::endl;
            for (Customer* c : trainer->getCustomers())
                std::cout << c->getId() << " " << c->getName() << "" << std::endl;
            std::cout << "Orders:" << std::endl;
            for (OrderPair op : trainer->getOrders())
                std::cout << op.second.getName() << " " << op.second.getPrice() << "NIS " << op.first << std::endl;
            std::cout << "Current Trainer's Salary: " << trainer->getSalary() << "NIS" << std::endl;
        }
        else {
            std::cout << "Trainer " << trainerId << " status: closed" << std::endl;
        }
    }
    trainer = nullptr;
    complete();
}
std::string PrintTrainerStatus::toString() const {return "status " + std::to_string(trainerId) + " Completed";}

// Implementing the PrintActionsLog class's methods
PrintActionsLog::PrintActionsLog() : BaseAction() {}
PrintActionsLog::PrintActionsLog(const PrintActionsLog& other) : BaseAction(other) {}
PrintActionsLog* PrintActionsLog::copyMe() {
    return new PrintActionsLog(*this);
}
void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction*> actionsLog = studio.getActionsLog();
    std::string s = "";
    for (int i = (int)(actionsLog.size())-1; i >= 0; i--) {
        BaseAction* currAction = actionsLog[i];
        s = currAction->toString();
        std::cout << s << std::endl;
        currAction = nullptr;
    }
    complete();
}
std::string PrintActionsLog::toString() const {return "log Completed";}


// Implementing the BackupStudio class's methods
BackupStudio::BackupStudio() : BaseAction() {}
BackupStudio::BackupStudio(const BackupStudio& other) : BaseAction(other) {}
BackupStudio* BackupStudio::copyMe() {
    return new BackupStudio(*this);
}
void BackupStudio::act(Studio &studio) {
    Studio* tmp = new Studio(studio);
    if(backup != nullptr)
        delete backup;
    backup = tmp;
    tmp = nullptr;
}
std::string BackupStudio::toString() const {return "backup Completed";}


// Implementing the RestoreStudio class's methods
RestoreStudio::RestoreStudio() : BaseAction("No backup available") {}
RestoreStudio::RestoreStudio(const RestoreStudio& other) : BaseAction(other) {}
RestoreStudio* RestoreStudio::copyMe() {
    return new RestoreStudio(*this);
}
void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr)
        error(getErrorMsg());
    else {
        studio = *backup;
        complete();
    }
}
std::string RestoreStudio::toString() const {
    std::string s = "restore";
    if (getStatus() == COMPLETED)
        s = s + " Completed";
    else
        s = s + " Error: " + getErrorMsg();
    return s;
}