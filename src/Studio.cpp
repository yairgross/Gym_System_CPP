#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Studio.h"
#include "Action.h"
#include <iostream>
#include <fstream>

extern Studio* backup;

// Constructors
Studio::Studio(): open(false), trainers(std::vector<Trainer*>()),
                  workout_options(std::vector<Workout>()), actionsLog(std::vector<BaseAction*>()) {}
Studio::Studio(const std::string &configFilePath): open(false) {
    std::ifstream is_file(configFilePath);
    std::string line ="";
    while(line.empty() || (int)line.find('#') >= 0)
        std::getline(is_file, line);  // Reading the first line which includes the number of trainers
    int numOfTrainers = std::stoi(line);
    trainers = std::vector<Trainer*>();

    std::getline(is_file, line);
    while(line.empty() || (int)line.find('#') >= 0)
        std::getline(is_file, line); // Reading the second line which includes each trainer's capacity
    for (int i = 0; i < numOfTrainers-1; ++i) { // Substring each capacity and constructing a Trainer which is pushed into the vector of trainers
        std::string capacityStr = line.substr(0, (int)line.find(','));
        line = line.substr((int)(line.find(','))+1);
        int capacityInt = std::stoi( capacityStr );
        trainers.push_back(new Trainer(capacityInt));
    }
    int capacityInt = std::stoi( line ); // creating the last trainer whose capacity doesn't have a comma after it
    trainers.push_back(new Trainer(capacityInt));

    std::getline(is_file, line);
    workout_options = std::vector<Workout>();
    int workoutID = 0;
    while(line.empty() || (int)line.find('#') >= 0)
        std::getline(is_file, line); // Jumping over all empty rows and all comments
    while(!line.empty()) { // Constructing the workout from the remaining lines and pushing them into the vector
        // Extracting workout name
        std::string workoutName = line.substr(0,(int)(line.find(',')));
        line = line.substr((int)(line.find(','))+1);

        // Extracting workout type
        std::string wType_str = line.substr(0,(int)(line.find(',')));
        line = line.substr((int)(line.find(','))+1);
        WorkoutType wtype = CARDIO;
        if((int)wType_str.find("Anaerobic") >=0)
            wtype = ANAEROBIC;
        else if((int)(wType_str.find("Mixed")) >=0)
            wtype = MIXED;
        // extracting workout price
        //std::string workoutPriceStr = line;
        int workoutPrice = std::stoi(line);

        workout_options.push_back(Workout(workoutID, workoutName, workoutPrice, wtype));
        workoutID++;
        // Reading the next line
        std::getline(is_file, line);

    }
}

// Destructor
Studio::~Studio() { clear(); }
// Copy Constructor
Studio::Studio(const Studio& other): open(other.open){
    copy(other.open, other.trainers, other.workout_options, other.actionsLog);
}
// Copy Assignment Operator
Studio& Studio::operator=(const Studio& other){
    if(this != &other)
    {
        clear();
        copy(other.open, other.trainers, other.workout_options, other.actionsLog);
    }
    return *this;
}
// Move Constructor
Studio::Studio(Studio&& other): open(other.open), trainers(other.trainers),
                                workout_options(other.workout_options), actionsLog(other.actionsLog)
{
    other.open = false;
    other.trainers = std::vector<Trainer*>();
    other.workout_options = std::vector<Workout>();
    other.actionsLog = std::vector<BaseAction*>();
}
//Move Assignment Operator
Studio& Studio::operator=(Studio&& other){
    if(this != &other)
    {
        clear();
        open = other.open;
        trainers = other.trainers;
        for(Workout w: other.workout_options)
            workout_options.push_back(w);
        actionsLog = other.actionsLog;
        other.open = false;
        other.trainers = std::vector<Trainer*>();
        other.workout_options = std::vector<Workout>();
        other.actionsLog = std::vector<BaseAction*>();
    }
    return *this;
}
void Studio::copy(bool other_open, std::vector<Trainer*> other_trainers, std::vector<Workout> other_workout_options,
                  std::vector<BaseAction*> other_actionsLog){ // Copy method
    open = other_open;
    trainers = std::vector<Trainer*>();
    for(int i = 0; i<(int)other_trainers.size(); i++){
        Trainer* t = new Trainer(*other_trainers[i]);
        trainers.push_back(t);
        t = nullptr;
    }
    workout_options = std::vector<Workout>();
    for(int i = 0; i < (int)other_workout_options.size(); i++){
        workout_options.push_back(Workout(other_workout_options[i]));
    }
    actionsLog = std::vector<BaseAction*>();
    for(int i = 0; i<(int)other_actionsLog.size(); i++)
    {
        actionsLog.push_back((other_actionsLog[i])->copyMe());
    }
}
void Studio::clear() { // Clear method
    open = false;
    if(&trainers) {
        for(Trainer* t: trainers)
            delete t;
        trainers.clear();
    }
    if(&workout_options) {
        workout_options.clear();
    }
    if(&actionsLog) {
        for(BaseAction* action: actionsLog)
            delete action;
        actionsLog.clear();
    }
}

bool Studio::isOpen() { // Returns ths open/close status of the Studio
    return open;
}
const std::vector<Trainer*>& Studio::getTrainers() { // returns a reference to the trainers vector
    return trainers;
}
void Studio::start(){
    std::cout <<"Studio is now open!" << std::endl;
    open = true;
    std::string act = "";
    std::getline(std::cin, act);
    // for every input, Extracting the data, creating the instance of the appropriate action and acting
    int customersID = 0;
    while(act != "closeall") {
        std::string subAct = act.substr(0,(int)act.find(' '));
        act = act.substr((int)(act.find(' '))+1);
        if(subAct == "open") {
            customersID = openTrainer(act, customersID);
        }
        else if(subAct == "order")
        {
            // Extracting the Trainer id which is positioned after the space character to the end of the line
            int trainer_id = std::stoi(act.substr(0,(int)act.find(' ')));
            Order* orderAction = new Order(trainer_id);
            orderAction->act(*this);
            actionsLog.push_back(orderAction);
        }
        else if(subAct == "move"){
            // Extracting IDs of source trainer, destination trainer and ID of customer
            int srcTrainerId = std::stoi(act.substr(0,(int)act.find(' '))) ;
            act = act.substr((int)(act.find(' '))+1);
            int destTrainerId = std::stoi(act.substr(0,(int)act.find(' '))) ;
            act = act.substr((int)(act.find(' '))+1);
            int customerID = std::stoi(act) ;
            MoveCustomer* moveAction = new MoveCustomer(srcTrainerId, destTrainerId, customerID);
            moveAction->act(*this);
            actionsLog.push_back(moveAction);
        }
        else if(subAct == "close"){
            // Extracting the Trainer id which is positioned after the space character to the end of the line
            int trainerID = std::stoi(act.substr(0,(int)act.find(' '))) ;
            act = act.substr((int)(act.find(' '))+1);
            Close* closeAction = new Close(trainerID);
            closeAction->act(*this);
            actionsLog.push_back(closeAction);

        }
        else if (subAct == "workout_options")
        {
            PrintWorkoutOptions* printWorkoutOptionsAction = new PrintWorkoutOptions();
            printWorkoutOptionsAction->act(*this);
            actionsLog.push_back(printWorkoutOptionsAction);
        }
        else if(subAct == "status"){
            // Extracting the Trainer id which is positioned after the space character to the end of the line
            int trainerID = std::stoi(act.substr(0,(int)act.find(' '))) ;
            act = act.substr((int)(act.find(' '))+1);
            PrintTrainerStatus* printTrainerStatusAction = new PrintTrainerStatus(trainerID);
            printTrainerStatusAction->act(*this);
            actionsLog.push_back(printTrainerStatusAction);
        }
        else if(subAct == "log"){
            PrintActionsLog* print = new PrintActionsLog();
            print->act(*this);
            actionsLog.push_back(print);
        }
        else if(subAct == "backup"){
            BackupStudio* backUpAction = new BackupStudio();
            backUpAction->act(*this);
            actionsLog.push_back(backUpAction);
        }
        else if(subAct == "restore"){
            RestoreStudio* restoreAction = new RestoreStudio();
            restoreAction->act(*this);
            actionsLog.push_back(restoreAction);
        }
        else{ // if the input does not match any action syntax
            std::cout << "Invalid input for action" <<std::endl;
        }
        std::getline(std::cin, act);
    }
    CloseAll* closeAllAction = new CloseAll();
    closeAllAction->act(*this);
    actionsLog.push_back(closeAllAction);
    open = false;
    std::cout << "Studio is now closed. Exiting" <<std::endl;
}

int Studio:: openTrainer(std::string act, int customersID){ // Builds the openTrainer action in the method 'start' and returns the id of the latest customer added
    int trainer_id = std::stoi(act.substr(0,(int)act.find(' ')));
    act = act.substr((int)(act.find(' '))+1);
    std::vector<Customer *> customersList = std::vector<Customer*>();
    while ((int)act.length() > 0) {
        // Extracting the customer name and type from the string
        std::string customerStr = "";
        if(act.find(' ') < act.length()) {
            customerStr = act.substr(0, (int)(act.find(' ')));
            act = act.substr((int)(act.find(' '))+1);
        }
        else // if we've reached the last customer
        {
            customerStr = act;
            act = "";
        }
        std::string customerName = customerStr.substr(0,(int)customerStr.find(','));
        customerStr = customerStr.substr((int)(customerStr.find(','))+1);
        std::string customerType = customerStr;
        // Creating the customer according to his type and pushing to the vector of customers
        if (customerType == "swt") {
            SweatyCustomer *c = new SweatyCustomer(customerName, customersID);
            customersList.push_back(c);
        }
        else if (customerType == "chp") {
            CheapCustomer *c = new CheapCustomer(customerName, customersID);
            customersList.push_back(c);
        }
        else if (customerType == "mcl") {
            HeavyMuscleCustomer *c = new HeavyMuscleCustomer(customerName, customersID);
            customersList.push_back(c);
        }
        else if (customerType == "fbd"){
            FullBodyCustomer *c = new FullBodyCustomer(customerName, customersID);
            customersList.push_back(c);
        }
        customersID++;
    }
    // Creating the action
    OpenTrainer* openTrainerAction = new OpenTrainer(trainer_id, customersList);
    openTrainerAction->act(*this);
    actionsLog.push_back(openTrainerAction);
    for(Customer* c: customersList)
        delete c;
    customersList.clear();
    return customersID;

}
int Studio::getNumOfTrainers() const { // Returns the number of trainers in the studio
    return trainers.size();
}
Trainer* Studio::getTrainer(int tid) { // Returns a reference to a specific Trainer by index
    return trainers[tid];
}
const std::vector<BaseAction*>& Studio::getActionsLog() const // Return a reference to the history of actions
{
    return actionsLog;
}
std::vector<Workout>& Studio::getWorkoutOptions() { // Returns a reference to the vector of the workouts
    return workout_options;
}
