//
//  main.cpp
//  Lab #4
//
//  Created by Erlis Ibraimi on 2.11.22.
//

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct proScheduling {
    
    int pid = 0; //process ID
    int proPriority = 0; //process priority
    int tAt = 0; //turnaround time
    int arrrivalT = 0; //arrival time
    int startT = 0; //start time
    int compT = 0; //completion
    int execT = 0; //execution time
};


void FCFS(queue<proScheduling>);
void SJN(queue<proScheduling>);
void Prioriy(queue<proScheduling>);
queue<proScheduling> shortExec(queue<proScheduling>);
queue<proScheduling> highPr(queue<proScheduling>);
bool compareExecT(proScheduling, proScheduling);
bool comparePr(proScheduling, proScheduling);

int main(int argc, const char * argv[]) {
    
    int arrT, prio, exeT, pro;
    
    queue<proScheduling> processQue;
    
    cout<<"How many Processes would you like: ";
    cin>>pro;
    
    cout<<endl;
    
    for(int i = 0; i < pro; i++) {
        
        proScheduling pro;
        
        pro.pid = i + 1;
        
        cout<<"Enter the arrival, priority and execution time: ";
        cin>>arrT;
        cin>>prio;
        cin>>exeT;
        pro.arrrivalT = arrT;
        pro.proPriority = prio;
        pro.execT = exeT;
        
        processQue.push(pro);
    }
    
    cout<<endl;
    
    FCFS(processQue);
    SJN(processQue);
    Prioriy(processQue);
    
    return 0;
}


//FCFS Scheduling
void FCFS(queue<proScheduling> processQue) {
    
    int Time = 0; //a time unit to simulate time
    double avgTaT = 0.0, qSize = processQue.size(); //both variables are to caluclate the average tat
    
    processQue.front().startT = processQue.front().arrrivalT; //setting the start of the first job to the arrival time
    
    proScheduling temp; //temp struct
    
    queue<proScheduling> readyQue; //the ready queue to process the processes for the time
    
    
    cout<<"------------------------FCFS---------------------------"<<endl;
    cout<<setw(4)<<"PID"<<setw(10)<<"Start"<<setw(20)<<"Completion Time"<<setw(20)<<"Turn Around Time"<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    
    //a loop to do all the calculations until we are done with every process
    while(!processQue.empty() || !readyQue.empty()){
        
        //adding the process' to the ready queue based on the time unit
        if(processQue.front().arrrivalT <= Time){
            while(!processQue.empty() && processQue.front().arrrivalT <= Time){
                temp = processQue.front();
                processQue.pop();
                readyQue.push(temp);
            }
            
            temp = readyQue.front(); //stores the current process as temp for the calculations below
            readyQue.pop(); //pops the current process
            
            Time += temp.execT; //updates the time unit
            
            temp.compT = temp.startT + temp.execT; //calculates completion time
            
            temp.tAt = Time - temp.arrrivalT; //calculates tat
            avgTaT += temp.tAt; //used to calculate average tat
            
            readyQue.front().startT = temp.compT; //setting the next process' start time to the completition time
            
            //output
            cout<<setw(3)<<temp.pid<<setw(9)<<temp.startT<<setw(15)<<temp.compT<<setw(20)<<temp.tAt<<endl;
                        
        }else
            Time++;
    }
    
    cout<<endl;
    cout<<setw(32)<<"Average TaT: "<<(avgTaT/qSize)<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    cout<<endl;
}


//SJN Scheduling
void SJN(queue<proScheduling> processQue) {
    
    int Time = 0; //a time unit to simulate time
    double avgTaT = 0.0, qSize = processQue.size(); //both variables are to caluclate the average tat
    
    processQue.front().startT = processQue.front().arrrivalT; //setting the start of the first job to the arrival time
    
    proScheduling temp; //temp struct
    
    queue<proScheduling> readyQue; //the ready queue to process the processes for the time
    
    cout<<"------------------------SJN----------------------------"<<endl;
    cout<<setw(4)<<"PID"<<setw(10)<<"Start"<<setw(20)<<"Completion Time"<<setw(20)<<"Turn Around Time"<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    
    //a loop to do all the calculations until we are done with every process
    while(!processQue.empty() || !readyQue.empty()){
        
        if(processQue.front().arrrivalT <= Time){
            while(!processQue.empty() && processQue.front().arrrivalT <= Time){
                temp = processQue.front();
                processQue.pop();
                readyQue.push(temp);
            }
            
            readyQue = shortExec(readyQue); //updating the current processes with the one which has the shortest execution time
            
            temp = readyQue.front(); //stores the current process as temp for the calculations below
            readyQue.pop(); //pops the current process
            
            Time += temp.execT; //updates the time unit
            
            temp.compT = temp.startT + temp.execT; //calculates completion time
            
            temp.tAt = Time - temp.arrrivalT; //calculates tat
            avgTaT += temp.tAt; //used to calculate average tat
            
            readyQue.front().startT = temp.compT; //setting the next process' start time to the completition time
                        
            cout<<setw(3)<<temp.pid<<setw(9)<<temp.startT<<setw(15)<<temp.compT<<setw(20)<<temp.tAt<<endl;
            
                        
        }else
            Time++;
    }
    
    cout<<endl;
    cout<<setw(32)<<"Average TaT: "<<(avgTaT/qSize)<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    cout<<endl;
}


//a funtion to sort each process based on the shortest time of execution (used for SJN)
queue<proScheduling> shortExec(queue<proScheduling> q) {
    
    vector<proScheduling> v;
    proScheduling temp = q.front();
    
    //adding every process to a vector
    while(!q.empty()){
        v.push_back(q.front());
        q.pop();
    }
    
    //sorting the vector based on shortest execution time of the process
    sort(v.begin(), v.end(), compareExecT);
    
    //pushing the sorted vector to the queue
    for(int i = 0; i < v.size(); i++){
        q.push(v[i]);
    }
    
    q.front().startT = temp.startT; //used to fix the starting time of the first process in queue after the sorting
    
    return q;
    
}


//a function to compare the shortest execution time between 2 processes (used for SJN)
bool compareExecT(proScheduling a, proScheduling b){
    return a.execT < b.execT;
}


//Priority Scheduling
void Prioriy(queue<proScheduling> processQue) {
    
    int Time = 0; //a time unit to simulate time
    double avgTaT = 0.0, qSize = processQue.size(); //both variables are to caluclate the average tat
    
    processQue.front().startT = processQue.front().arrrivalT; //setting the start of the first job to the arrival time
    
    proScheduling temp; //temp struct
    
    queue<proScheduling> readyQue; //the ready queue to process the processes for the time
    
    cout<<"----------------------Priority-------------------------"<<endl;
    cout<<setw(4)<<"PID"<<setw(10)<<"Start"<<setw(20)<<"Completion Time"<<setw(20)<<"Turn Around Time"<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    
    //a loop to do all the calculations until we are done with every process
    while(!processQue.empty() || !readyQue.empty()){
        
        //adding the process' to the ready queue based on the time unit
        if(processQue.front().arrrivalT <= Time){
            while(!processQue.empty() && processQue.front().arrrivalT <= Time){
                temp = processQue.front();
                processQue.pop();
                readyQue.push(temp);
            }
            
            readyQue = highPr(readyQue); //updating the current processes with the one which has the shortest execution time
            
            temp = readyQue.front(); //stores the current process as temp for the calculations below
            readyQue.pop(); //pops the current process
            
            Time++; //updates the time unit
            
            //checking if the current process is bigger then the others processes on both queues
            if((temp.proPriority <= readyQue.front().proPriority) && (processQue.empty() || temp.proPriority <= processQue.front().proPriority)){
                
                temp.compT = temp.startT + temp.execT; //calculates completion time
                temp.tAt = temp.compT - temp.arrrivalT; //calculating tat here because we know this process will finish and not get interupted
               
            //used for processes that get interupted
            }else {
                if(!readyQue.empty()){
                    temp.execT--;
                    temp.compT = temp.startT + 1;
                    readyQue.push(temp);
                }else {
                    //same as the one above just calculates tat if both queues are empty when there's only 1 value left to calculate (edge case)
                    temp.compT = temp.startT + temp.execT;
                    temp.tAt = temp.compT - temp.arrrivalT;
                }
                    
            }
            
            avgTaT += temp.tAt; //used to calculate average tat

            readyQue.front().startT = temp.compT; //setting the next process' start time to the completition time

            cout<<setw(3)<<temp.pid<<setw(9)<<temp.startT<<setw(15)<<temp.compT<<setw(20)<<temp.tAt<<endl;
                        
        }else
            Time++;
    }
    
    cout<<endl;
    cout<<setw(32)<<"Average TaT: "<<(avgTaT/qSize)<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    cout<<endl;
}


//a funtion to sort each process based on the highest priority (used for Priority)
queue<proScheduling> highPr(queue<proScheduling> q) {
    
    vector<proScheduling> v;
    proScheduling temp = q.front();
    
    //adding every process to a vector
    while(!q.empty()){
        v.push_back(q.front());
        q.pop();
    }
    
    //sorting the vector based on shortest execution time of the process
    sort(v.begin(), v.end(), comparePr);
    
    //pushing the sorted vector to the queue
    for(int i = 0; i < v.size(); i++){
        q.push(v[i]);
    }
    
    q.front().startT = temp.startT; //used to fix the starting time of the first process in queue after the sorting
        
    return q;
    
}


//a function to compare the highest priority between 2 processes (used for Priority)
bool comparePr(proScheduling a, proScheduling b){
    return a.proPriority < b.proPriority;
}
