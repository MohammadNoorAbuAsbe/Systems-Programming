#include "Studio.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

///////////////////////////////////////////////
//Rule OF 5
//deconstructor
Studio::~Studio(){
    this->clean();
}

//helper function to clean the memory
void Studio::clean(){
    
    //clear trainers vector
    int size = trainers.size();
    
    for(int i = 0; i < size; i++){
        delete trainers[i];//delete the trainers
        trainers[i] = nullptr;
    }
    trainers.clear();
    
    //clear Actions vector
    size = actionsLog.size();
     
    for(int i = 0; i < size; i++){
        delete actionsLog[i];//delete the action
        actionsLog[i] = nullptr;
    }
    actionsLog.clear();
    
    //clear workout_options Vector
        workout_options.clear();
}


//copy constructor
Studio::Studio(const Studio& other){
    trainers = vector<Trainer*>();
    workout_options = vector<Workout>();
    actionsLog = vector<BaseAction*>();
    this->copy(other);
}

//move constructor
Studio::Studio(Studio&& other){
   //trainers = other.trainers;
   // actionsLog = other.actionsLog;
   // workout_options = other.workout_options;
   // open = other.open;
   // currentId = other.currentId;
    copy(other);
    other.workout_options = vector<Workout>();
    other.actionsLog = vector<BaseAction*>();
    other.trainers = vector<Trainer*>();
}

//helper for copy constructor
void Studio::copy(const Studio& other){
    open = other.open;
    currentId = other.currentId;

    Studio& otherStud = const_cast<Studio&>(other);
    //copy trainers
    int size = other.getNumOfTrainers();
    
    for(int i = 0; i < size; i++){
        trainers.push_back(new Trainer(*otherStud.getTrainer(i)));
    }

    //copy actions
    size = other.getActionsLog().size();

    for(int i = 0; i < size; i++){
        BaseAction* action = const_cast<BaseAction*>(other.getActionsLog()[i]);
        actionsLog.push_back(new PrintActionsLog(*action));
        
    }

    //copy workout_options
    size = otherStud.getWorkoutOptions().size();
    
    for(int i = 0; i < size; i++){
        workout_options.push_back(Workout(otherStud.getWorkoutOptions()[i]));
    }
    
}

//move assignment operator
Studio& Studio::operator=(Studio&& other){
    this->clean();
    /*trainers = other.trainers;
    actionsLog = other.actionsLog;
    workout_options = other.workout_options;
    open = other.open;
    currentId = other.currentId;*/
    copy(other);
    other.workout_options = vector<Workout>();
    other.actionsLog = vector<BaseAction*>();
    other.trainers = vector<Trainer*>();
    return *this;
}
//copy assignment operator
Studio& Studio::operator=(const Studio& other){
    if(this != &other){
        this->clean();
        this->copy(other);
    }
    return *this;
}



////////////////////////////////////////////////

Studio::Studio()
{
    backup = nullptr;
}

Studio::Studio(const std::string &configFilePath){
    currentId = 0;
    std::ifstream myfile; myfile.open(configFilePath);
    std::string myline;
    if ( myfile.is_open() ) {
        int count = 0;
        while ( myfile ) {
            std::getline (myfile, myline);
            if(myline[0] != '#' && myline[0] != NULL){
                if(count == 1)
                {
                    //foreach, if not ',' then create trainer(myline[index]) then push_back
                    int index = 0;
                    while (index < myline.size())
                    {
                        if(myline[index] != ',')
                        {   
                            //come back when you know what to do TTT
                            Trainer* tp = new Trainer(myline[index]-'0');
                            trainers.push_back(tp);
                        }
                        index++;
                    }
                }
                if (count > 1) // the lines of workout options
                {
                    string w_name = "";
                    WorkoutType w_type;
                    string w_price = "";
                    int index = 0; // for the chars in a line
                    int var = 0; // to choose where to store it (name, type or price)
                    bool foundType = false;// is used to declare that we have found the type

                    while (index < myline.size())
                    {
                        if(myline[index] != ',')
                        {   
                            if(var == 0)
                            {
                                w_name += myline[index];
                            }
                            else if(var == 2)
                            {
                                // get the price as a string
                                w_price += myline[index];
                            }
                            else if(!foundType)
                            {
                                if(myline[index] == 'A')
                                {
                                    w_type = ANAEROBIC;
                                }
                                else if(myline[index] == 'C')
                                {
                                    w_type = CARDIO;
                                }
                                else
                                {
                                    w_type = MIXED;
                                }
                                foundType = true;
                            }
                        }
                        else
                        {
                            var ++;
                            index++;
                        }
                        index++;
                    }
                    int price = stoi(w_price);
                    
                    workout_options.push_back(Workout(workout_options.size(),w_name, price, w_type));
                }
                count++;
            }
        }
    }
}

void Studio::start()
{
    open = true;
    cout << "Studio is now open!\n"<< endl;
    std::string line;
    while(open && getline(cin, line)){
    
       
       
        if(line.find("open") == 0){
            vector<Customer*> customers;
            int id;
            //get id
            for(int i = 0; i < line.length();i++ ){
                if(line[5+i] == ' '){
                    id = stoi(line.substr(5,i));
                }
            }
            std::string name ="";
            for (int i = 7; i < line.length() && getTrainer(id)->getCustomers().size()+customers.size() < getTrainer(id)->getCapacity(); i++)
            {   
               
                if(line[i] == ','){
                    if(line.substr(i+1,3)== "swt"){
                        customers.push_back(new SweatyCustomer(name, currentId));
                        currentId++;
                    }
                    else if(line.substr(i+1,3) == "chp"){
                        customers.push_back(new CheapCustomer(name, currentId));
                        currentId++;
                    }
                    else if(line.substr(i+1,3) == "mcl"){
                        customers.push_back(new HeavyMuscleCustomer(name, currentId));
                        currentId++;
                    }
                    else if(line.substr(i+1,3) == "fbd"){
                        customers.push_back(new FullBodyCustomer(name, currentId));
                        currentId++;
                    }
                    i+2;
                }
                else if (line[i] == ' ')
                {
                    name = "";
                }
                else{
                    name+=line[i];
                }
            }
            //the temp customer that will have the inputline for action tostring
            Customer* inputline = new SweatyCustomer(line, -1);
            customers.push_back(inputline);

            actionsLog.push_back(new OpenTrainer(id,customers));
            actionsLog[actionsLog.size()-1]->act(*this);
        }
        else if(line.find("order") == 0){
            
            int id = stoi(line.substr(6));
            actionsLog.push_back(new Order(id));
            actionsLog[actionsLog.size()-1]->act(*this);
            
        }
        else if(line.find("move") == 0){
            int origin_trainer_id ;
            int dest_trainer_id ;
            int customer_id;
            int count = 0;
            std::string num="";
            //get all 3 ids
            for(int i = 5; i < line.length(); i++){
                
                if(count == 0 && line[i] == ' '){
                    origin_trainer_id = stoi(num);
                    num ="";
                    count++;
                }else if(count == 1 && line[i] == ' '){
                    dest_trainer_id = stoi(num);
                    customer_id = stoi(line.substr(i+1));
                }else{
                    num+=line[i];
                }
            }
            actionsLog.push_back(new MoveCustomer(origin_trainer_id, dest_trainer_id, customer_id));
            actionsLog[actionsLog.size()-1]->act(*this);
        }
        //close action
        else if(line.find("close") == 0){
            int id = stoi(line.substr(6));
            actionsLog.push_back(new Close(id));
            actionsLog[actionsLog.size()-1]->act(*this);
        }
        //closeall
        else if(line.find("closeall") == 0){
            actionsLog.push_back(new CloseAll());
            actionsLog[actionsLog.size()-1]->act(*this);
            open = false;
        }
        //print workout options
        else if(line.find("workout_option") == 0){
            actionsLog.push_back(new PrintWorkoutOptions());
            actionsLog[actionsLog.size()-1]->act(*this);
        }
        //print status of trainer
        else if(line.find("status") == 0){
            int id = stoi(line.substr(6));
            actionsLog.push_back(new PrintTrainerStatus(id));
            actionsLog[actionsLog.size()-1]->act(*this);
        }
        //log
        else if(line.find("log") == 0){
            PrintActionsLog* log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);

        }
        //backup
        else if(line == "backup"){
            actionsLog.push_back(new BackupStudio());
            actionsLog[actionsLog.size()-1]->act(*this);
        }
        //restores
        else if(line == "restore"){
            RestoreStudio* res = new RestoreStudio();
            res->act(*this);
            actionsLog.push_back(res);
        }
        
    }
    
}

int Studio::getNumOfTrainers() const
{
	return trainers.size();
}

Trainer* Studio::getTrainer(int tid)
{
	return trainers[tid];
}

const std::vector<BaseAction*>& Studio::getActionsLog() const
{
	return actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions()
{
	return workout_options;
}
