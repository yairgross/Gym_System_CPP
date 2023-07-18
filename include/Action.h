#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    BaseAction(std::string error_message);
    BaseAction(const BaseAction& other);
    virtual ~BaseAction();
    virtual BaseAction* copyMe()=0;
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    OpenTrainer(const OpenTrainer& other);
    OpenTrainer* copyMe();
    ~OpenTrainer();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(const Order& other);
    Order* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(const MoveCustomer& other);
    MoveCustomer* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(const Close& other);
    Close* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    CloseAll(const CloseAll&);
    CloseAll* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    PrintWorkoutOptions(const PrintWorkoutOptions& other);
    PrintWorkoutOptions* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(const PrintTrainerStatus& other);
    PrintTrainerStatus* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog(const PrintActionsLog& other);
    PrintActionsLog* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    BackupStudio(const BackupStudio& other);
    BackupStudio* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    RestoreStudio(const RestoreStudio& other);
    RestoreStudio* copyMe();
    void act(Studio &studio);
    std::string toString() const;
private:
};


#endif