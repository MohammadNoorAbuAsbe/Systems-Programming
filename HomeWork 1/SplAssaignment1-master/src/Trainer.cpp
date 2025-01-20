#include "Trainer.h"
#include "algorithm"
#include <iostream>

using namespace std;

///////////////////////////////////////////
//Rule OF 5

//deconstructor
Trainer::~Trainer(){
    this->clean();
}

//helper function to clean the memory
void Trainer::clean(){
    
    //clear customersList vector
    int size = customersList.size();
    
    for(int i = 0; i < size; i++){
        delete customersList[i];//delete the customers
        customersList[i] = nullptr;
    }
    customersList.clear();
    
    //clear orderList Vector
        orderList.clear();
}

//copy constructor
Trainer::Trainer(const Trainer& other){
    customersList = vector<Customer*>();
    orderList = vector<OrderPair>();
    this->copy(other);
}

//helper for copy constructor
void Trainer::copy(const Trainer& other){
    open = other.open;
    salary = other.salary;
    capacity = other.capacity;

    Trainer& otherTrain = const_cast<Trainer&>(other);
    //copy customersList
    int size = otherTrain.getCustomers().size();
    
    for(int i = 0; i < size; i++){
        std::string name = otherTrain.getCustomers()[i]->getName();
        int id = otherTrain.getCustomers()[i]->getId();
        if(dynamic_cast<SweatyCustomer*>(otherTrain.getCustomers()[i]) != nullptr){
            customersList.push_back(new SweatyCustomer(name, id));
        }
        else if(dynamic_cast<CheapCustomer*>(otherTrain.getCustomers()[i]) != nullptr){
            customersList.push_back(new CheapCustomer(name, id));
        }
        else if(dynamic_cast<HeavyMuscleCustomer*>(otherTrain.getCustomers()[i]) != nullptr){
            customersList.push_back(new HeavyMuscleCustomer(name, id));
        }
        else if(dynamic_cast<FullBodyCustomer*>(otherTrain.getCustomers()[i]) != nullptr){
            customersList.push_back(new FullBodyCustomer(name, id));
        }
    }

    //copy order list
    size = otherTrain.getOrders().size();

    for(int i = 0; i < size; i++){
        int customer_id = otherTrain.getOrders()[i].first;
        orderList.push_back(OrderPair(customer_id, *new Workout(otherTrain.getOrders()[i].second)));
    }  
}

Trainer::Trainer(Trainer&& other){
    /*customersList = other.customersList;
    orderList = other.orderList;
    open = other.open;
    capacity = other.capacity;
    salary = other.salary;*/
    copy(other);
    other.customersList = vector<Customer*>();
    other.orderList = vector<OrderPair>();
}

Trainer& Trainer::operator=(const Trainer& other){
    if(this != &other){
        this->clean();
        this->copy(other);
    }
    return *this;
}

Trainer& Trainer::operator=(Trainer&& other){
    this->clean();
    /*customersList = other.customersList;
    orderList = other.orderList;
    open = other.open;
    capacity = other.capacity;
    salary = other.salary;*/
    copy(other);
    other.customersList = vector<Customer*>();
    other.orderList = vector<OrderPair>();
    return *this;
}

///////////////////////////////////////////

Trainer::Trainer(int t_capacity): capacity(t_capacity){
    open = false;
}

int Trainer::getCapacity() const{
    return capacity;
}

void Trainer::addCustomer(Customer* customer){
    //adds customer to the end of the vector
    customersList.push_back(customer);
}
void Trainer::removeCustomer(int id){
    std::vector<OrderPair> tempList;
    //find if returns a pointer to the customer in the customersList with the given id 
    //remove the orders
    for(auto it = orderList.begin(); it != orderList.end(); ++it){
        if(it->first == id){
            int currentp = orderList.size()-1;
            while(orderList[currentp].first != it->first){
                
                tempList.push_back(OrderPair(orderList[currentp].first, orderList[currentp].second));
                //push what we Need
                
                orderList.pop_back();
                //pop what we Need
                currentp--;
            }

            while(orderList[orderList.size()-1].first == id){
                //we removed an order so we decrease from salary
                salary -= orderList[orderList.size()-1].second.getPrice();
                orderList.pop_back();
                //pop what we Dont need
            }
            

            //push back all the other orders
            for(int i = tempList.size()-1; i >= 0; i--){
                orderList.push_back(OrderPair(tempList[i].first, tempList[i].second));
                //push back what we need
                tempList.pop_back();
                //pop from temp end Not Needed
            }
            
            break;
        }
    }
    //erase removes the customer using the pointer from remove_if
    for(auto it = customersList.begin(); it != customersList.end(); ++it){
       

        if(static_cast<Customer*>(*it)->getId() == id){

            customersList.erase(it);
            return;
        }
    }
}
    
Customer* Trainer::getCustomer(int id){

    //iterator that looks or customer with the id and returns pointer to it
    for(auto it = customersList.begin(); it != customersList.end(); it++){

        if(static_cast<Customer*>(*it)->getId() == id){
            //found
            return *it;
        }
    }
    //not found
    return nullptr;
}
std::vector<Customer*>& Trainer::getCustomers()
{
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders()
{
   return orderList;
}
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    //go over the workout ids and make a order pair and add it to the orderList
    //add to salary the price of the workout
    for(auto it = workout_ids.begin(); it != workout_ids.end(); ++it){
        
        //if order already exists no need to add it
        if (!containsOrder(customer_id, *it))
        {
            orderList.push_back(OrderPair(customer_id, workout_options[*it]));
            //update salary
            salary += workout_options[*it].getPrice();  
        }
        
    }
}

bool Trainer::containsOrder(int id, int workoutId){
    for(auto it = orderList.begin(); it != orderList.end(); ++it){
        
        if(it->first == id && it->second.getId() == workoutId){
            return true;
        }
    }
    return false;
}

void Trainer::openTrainer(){
    open = true;
}

void Trainer::closeTrainer(){
    open = false;
}

int Trainer::getSalary(){
    return salary;
}
bool Trainer::isOpen(){
    return open;
}