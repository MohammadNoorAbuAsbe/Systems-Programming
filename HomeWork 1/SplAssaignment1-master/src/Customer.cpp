#include "Customer.h"

Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id){}

Customer::~Customer(){
	
}
std::string Customer::toString() const{
	std::string toString = std::to_string(getId())+" ";
	toString += getName();
	return toString;
}
std::string Customer::getName() const{
	return name;
}
int Customer::getId() const{
	return id;

}

//sweaty
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name, id){
	
}
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){

	std::vector<int> ordered_workout;
	//go over all the workout options and add the id of all cardio workouts
	for(auto it = workout_options.begin(); it != workout_options.end(); ++it){
        
        if(it->getType() == CARDIO){
            ordered_workout.push_back(it->getId());
        }
    }
	return ordered_workout;
}
std::string SweatyCustomer::toString() const{
	return getName();
}

//cheap
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name, id){

}
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
	std::vector<int> ordered_workout;
	//find the cheapest workout
	int price = workout_options[0].getPrice();
	int id =	workout_options[0].getId();
	
	for(auto it = workout_options.begin()+1; it != workout_options.end(); ++it){
        
        if(it->getPrice() < price){
            price = it->getPrice();
			id = it->getId();
        }
    }
	//push back into the vector the cheapest workout id
	ordered_workout.push_back(workout_options[id].getId());

	return ordered_workout;
}
std::string CheapCustomer::toString() const{
	return getName();
}

//heavy
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name, id){

}
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){

	std::vector<int> ordered_workout;
	//go over all the workout options and add the id of all anarobic workouts
	for(auto it = workout_options.begin(); it != workout_options.end(); ++it){
        
        if(it->getType() == ANAEROBIC){
            ordered_workout.push_back(it->getId());
        }
    }
	return ordered_workout;
}
std::string HeavyMuscleCustomer::toString() const{
	return getName();
}

//fullbody	
FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name, id){

	
}
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){

	std::vector<int> ordered_workout;
	//find the cheapest cardio and anarobic workout and most expensive mixed workout
	int cheapestC = -1;
	int idC = -1;
	int cheapestA = -1;
	int idA = -1;
	int expenM = -1;
	int idM = -1;
	
	for(auto it = workout_options.begin(); it != workout_options.end(); ++it){
        
		//cheapest cardio
        if(it->getType() == CARDIO){
           if(it->getPrice() < cheapestC || cheapestC == -1){
			   cheapestC = it->getPrice();
			   idC = it->getId();
		   }
        }
		//cheapest anarobic
		if(it->getType() == ANAEROBIC){
           if(it->getPrice() < cheapestA || cheapestA == -1){
			   cheapestA = it->getPrice();
			   idA = it->getId();
		   }
        }
		//expensive mixed
		if(it->getType() == MIXED){
           if(it->getPrice() > expenM || expenM == -1){
			   expenM = it->getPrice();
			   idM = it->getId();
		   }
        }
    }
	//push back into the vector the of the workout ids if found
	//if one of the workouts isnt available we wont order at all
	if(idA != -1 && idC != -1 && idM != -1){
		ordered_workout.push_back(idA);
		ordered_workout.push_back(idC);
		ordered_workout.push_back(idM);
	}

	return ordered_workout;
}
std::string FullBodyCustomer::toString() const{
	return getName();
}