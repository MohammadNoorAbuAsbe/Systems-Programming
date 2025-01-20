#include "Action.h"
#include "Studio.h"

using namespace std;

//base action
BaseAction::BaseAction(){
    tostring ="";
}

ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete(){
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg){
   this->errorMsg = errorMsg;
   status = ERROR;
}

 std::string BaseAction::getErrorMsg() const{
     return errorMsg;
 }

 //OpenTrainer

 OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id), customers(customersList){
     //to get the input line because i cant tell what type each customer is
     inputline = customers[customers.size()-1]->getName();
     customers.pop_back();
 }
 

 OpenTrainer::~OpenTrainer(){
     int size = customers.size();
    
    for(int i = 0; i < size; i++){
        delete customers[i];//delete the customer
        customers[i] = nullptr;
    }
    customers.clear();
 }
 

 void OpenTrainer::act(Studio &studio){
     
    //range and input check
    if(trainerId >= studio.getNumOfTrainers() || trainerId < 0){
        error("Workout session does not exist or is already open");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    if(studio.getTrainer(trainerId)->isOpen()){
        error("Workout session does not exist or is already open");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
     
    //add the customers to the trainers customerlist and open the trainer
    for(auto it = customers.begin(); it != customers.end(); ++it){
        
        studio.getTrainer(trainerId)->addCustomer(*it);
    }
    studio.getTrainer(trainerId)->openTrainer();

    complete();
    tostring = toString();
 }

std::string OpenTrainer::toString() const{
    if(tostring != ""){
        return tostring;
    }
    std::string tostring = inputline;

    if(getStatus() == COMPLETED){
        tostring+= " Completed";
    }else{
        tostring+= " ERROR: "+getErrorMsg();
    }
    return tostring;
}

// Order

Order::Order(int id):trainerId(id){}

void Order::act(Studio &studio){
   //range and input check
    if(trainerId >= studio.getNumOfTrainers() || trainerId < 0){
        error("Trainer does not exist or is not open");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    if(!studio.getTrainer(trainerId)->isOpen()){
        error("Trainer does not exist or is not open");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }

    std::string line = "";
    std::vector<Customer *> customers = studio.getTrainer(trainerId)->getCustomers();
    Trainer* trainer = studio.getTrainer(trainerId);

    for(auto it = customers.begin(); it != customers.end(); ++it){
        //trainer will make order pair for customer it with the right workoutids that he got from customer.order which returns which workout ids the customer gets
        int customerId = static_cast<Customer*>(*it)->getId();
        std::vector<int> workout_ids = static_cast<Customer*>(*it)->order(studio.getWorkoutOptions());
        //order will add the orders update salary and print the orders
        studio.getTrainer(trainerId)->order(customerId, workout_ids, studio.getWorkoutOptions());
    }
    
    for(auto it = trainer->getOrders().begin(); it != trainer->getOrders().end(); ++it){
        
        //printing
        line += trainer->getCustomer(it->first)->getName();
        line += " Is Doing ";
        line += it->second.getName();
        line += "\n";

    }
    cout << line;
    complete();
    tostring = toString();
}

std::string Order::toString() const{
    if(tostring != ""){
        return tostring;
    }
     std::string tostring= "Order "+to_string(trainerId);
    if(getStatus() == COMPLETED){
        tostring+= " Completed";
    }else{
        tostring+= " ERROR: "+getErrorMsg();
    }
    return tostring;
}

//move
//move customer by moving the pointer
 MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src), dstTrainer(dst), id(customerId){}

void MoveCustomer::act(Studio &studio){
    //input check
    //illegal trainer id
    if(srcTrainer >= studio.getNumOfTrainers() || srcTrainer < 0 ||dstTrainer >= studio.getNumOfTrainers() || dstTrainer < 0){
        error("Cannot move customer");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    //one of the trainers is closed
    if(!studio.getTrainer(srcTrainer)->isOpen()||!studio.getTrainer(dstTrainer)->isOpen()){
        error("Cannot move customer");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    //no such customer exists
    if(studio.getTrainer(srcTrainer)->getCustomer(id)== nullptr){
        error("Cannot move customer");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    //no more place or the customer
    if(studio.getTrainer(dstTrainer)->getCustomers().size() == studio.getTrainer(dstTrainer)->getCapacity()){
        error("Cannot move customer");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    //move the customer
    studio.getTrainer(dstTrainer)->addCustomer(studio.getTrainer(srcTrainer)->getCustomer(id));

    //delete the customer
    studio.getTrainer(srcTrainer)->removeCustomer(id);
    
    //close trainer if no more customers
    if(studio.getTrainer(srcTrainer)->getCustomers().size() == 0){
        Close* ctrainer = new Close(srcTrainer);
        ctrainer->act(studio);
    }
    complete();
    tostring = toString();
}

std::string MoveCustomer::toString() const{
    if(tostring !=""){
        return tostring;
    }
    std::string line= "MoveCustomer "+to_string(srcTrainer)+" "+to_string(dstTrainer)+" "+to_string(id);
    if(getStatus() == COMPLETED){
        line+= " Completed";
    }else{
        line+= " ERROR: "+getErrorMsg();
    }
    return line;
}

//close
Close::Close(int id):trainerId(id){}

void Close::act(Studio &studio){
    //range and input check
    if(trainerId >= studio.getNumOfTrainers() || trainerId < 0){
        error("Trainer does not exist or is not open");
        cout<< getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    if(!studio.getTrainer(trainerId)->isOpen()){
        error("Trainer does not exist or is not open");
        cout<< getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    studio.getTrainer(trainerId)->closeTrainer();
    int salary = studio.getTrainer(trainerId)->getSalary();
    std::string line = "Trainer "+std::to_string(trainerId)+" closed. Salary "+std::to_string(salary)+"NIS";

    cout << line;
    cout << "\n";
    complete();
    tostring = toString();
}

std::string Close::toString() const{
    if(tostring != ""){
        return tostring;
    }
    std::string line= "Close "+to_string(trainerId);
    if(getStatus() == COMPLETED){
        line+= " Completed";
    }else{
        line+= " ERROR: "+getErrorMsg();
    }
    return line;
}

//CloseAll

CloseAll::CloseAll(){}

void CloseAll::act(Studio &studio){

    for(int i = 0; i < studio.getNumOfTrainers(); i++){
        if(studio.getTrainer(i)->isOpen()){
            Close* c = new Close(i);
            c->act(studio);
        }
    }
    complete();
    tostring = toString();
}
std::string CloseAll::toString() const{
    if(tostring != ""){
        return tostring;
    }
    return "CloseAll Completed";
}

//PrintWorkoutOptions

PrintWorkoutOptions::PrintWorkoutOptions(){

}
void PrintWorkoutOptions::act(Studio &studio){
    for(Workout e: studio.getWorkoutOptions()){
        std::string line = e.getName() +", ";
        if(e.getType() == ANAEROBIC){
             line += "ANAEROBIC, ";
        }else if(e.getType() == CARDIO){
            line += "CARDIO, ";
        }else{
            line += "MIXED, ";
        }
        line += to_string(e.getPrice());
        cout << line << endl;
    }
    complete();
    tostring = toString();
}
std::string PrintWorkoutOptions::toString() const{

    return "workout_options Completed";
    
}

//PrintTrainerStatus
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id){}

void PrintTrainerStatus::act(Studio &studio){

    if(trainerId >= studio.getNumOfTrainers() || trainerId < 0){
        error("Trainer does not exist");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }

    if(!studio.getTrainer(trainerId)->isOpen()){
        cout << "Trainer "<< trainerId <<" status: closed" <<endl;
    }else{
        cout << "Trainer "<< trainerId <<" status: open" <<endl;
        cout << "Customers:"<<endl;
        for(Customer* c: studio.getTrainer(trainerId)->getCustomers()){
            cout<< to_string(c->getId())+" " << c->getName() << endl;
        }
        cout << "Orders:"<<endl;
        for(OrderPair o: studio.getTrainer(trainerId)->getOrders()){
            cout<< o.second.getName()+" "<< to_string(o.second.getPrice())+"NIS " << to_string(o.first)<< endl;
        }
        cout << "Current Trainer’s Salary: "<< to_string(studio.getTrainer(trainerId)->getSalary())<<"NIS"<<endl;
    }
    complete();
    tostring = toString();
}
std::string PrintTrainerStatus::toString() const{
    if(tostring != ""){
        return tostring;
    }
    std::string line= "status "+to_string(trainerId);
    if(getStatus() == COMPLETED){
        line+= " Completed";
    }else{
        line+= " ERROR: "+getErrorMsg();
    }
    return line;
   
}

//PrintActionsLog
PrintActionsLog::PrintActionsLog(){

}
PrintActionsLog::PrintActionsLog(const BaseAction& other){
    tostring = other.toString();
}
void PrintActionsLog::act(Studio &studio){
    for(BaseAction* a: studio.getActionsLog()){
        cout << a->toString() <<endl;
    } 
    complete();
}
std::string PrintActionsLog::toString() const{
    if(tostring != ""){
        return tostring;
    }
    return "log Completed";
}

//backup
BackupStudio::BackupStudio(){

}
void BackupStudio::act(Studio &studio){
    delete backup;
    backup = new Studio(studio);
    complete();
}
std::string BackupStudio::toString() const{
    return "backup Completed";
}

//restore studio
RestoreStudio::RestoreStudio(){

}
void RestoreStudio::act(Studio &studio){
    if(backup == nullptr){
        error("No backup available");
        cout<< "ERROR: "+getErrorMsg()<<endl;
        tostring = toString();
        return;
    }
    
    studio = *backup;

    complete();
    tostring = toString();
}
std::string RestoreStudio::toString() const{
    if(tostring != ""){
        return tostring;
    }
    std::string line= "restore ";
    if(getStatus() == COMPLETED){
        line+= " Completed";
    }else{
        line+= " ERROR: "+getErrorMsg();
    }
    return line;
}