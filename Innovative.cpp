#include<bits/stdc++.h>
using namespace std;

class Vehicle{
protected:
    int id,ratePerDay;
    string brand,model;
    bool isRented;
public:
    virtual ~Vehicle(){}
    Vehicle(int id,string b,string m,int r):id(id),ratePerDay(r),brand(b),model(m),isRented(false){}
    virtual string VName()=0;
    virtual void displayInfo(){
        cout<<"ID: "<<id<<" | "<<VName()<<" | "<<brand<<" "<<model<<" | Rate/day: ₹"<<ratePerDay<<" | Status: "<<(isRented?"Rented":"Available")<<endl;
    }
    virtual int calculateRentalCost(int days){
        return days*ratePerDay;
    }
    int getId(){return id;}
    bool getIsRented(){return isRented;}
    void setRented(bool v){isRented=v;}
};

class Car:public Vehicle{
    int seats;
public:
    Car(int id,string b,string m,int r,int s):Vehicle(id,b,m,r),seats(s){}
    string VName(){return "Car";}
    void displayInfo(){
        cout<<"ID: "<<id<<" | Car | "<<brand<<" "<<model<<" | Seats: "<<seats<<" | Rate: ₹"<<ratePerDay<<" | Status: "<<(isRented?"Rented":"Available")<<endl;
    }
};

class Bike:public Vehicle{
    int cc;
public:
    Bike(int id,string b,string m,int r,int c):Vehicle(id,b,m,r),cc(c){}
    string VName(){return "Bike";}
    void displayInfo(){
        cout<<"ID: "<<id<<" | Bike | "<<brand<<" "<<model<<" | Engine: "<<cc<<"cc | Rate: ₹"<<ratePerDay<<" | Status: "<<(isRented?"Rented":"Available")<<endl;
    }
};

class Customer{
    int id,rVid,rDays;
    string name,sdate;
public:
    Customer():id(0),rVid(-1),rDays(0),name(""),sdate(""){}
    Customer(int id,string n):id(id),rVid(-1),rDays(0),name(n),sdate(""){}
    void rent(int Vid,int d,string s){
        rVid=Vid;rDays=d;sdate=s;
    }
    void returned(){
        rVid=-1;rDays=0;sdate="";
    }
    int getRentedVehicleId(){return rVid;}
    int getId(){return id;}
    string getName(){return name;}
    int getRentalDays(){return rDays;}
    void displayInfo(){
        cout<<"Customer ID: "<<id<<" | Name: "<<name;
        if(rVid!=-1)cout<<" | Rented Vehicle ID: "<<rVid<<" | Days: "<<rDays<<" | Start: "<<sdate;
        else cout<<" | No active rental";
        cout<<endl;
    }
};

class RentalSystem{
    vector<Vehicle*>vehicles;
    map<int,Customer>customers;
    int nextVid,nextCid,latefee;
public:
    RentalSystem():nextVid(1),nextCid(1),latefee(100){}
    ~RentalSystem(){
        for(auto v:vehicles)delete v;
    }
    void addVehicleInteractive(){
        int choice;
        cout<<"Add vehicle -> Enter (1) Car (2) Bike: ";cin>>choice;
        cin.ignore();
        string b,m;
        int rate;
        cout<<"Brand : ";getline(cin,b);
        cout<<"Model : ";getline(cin,m);
        cout<<"Rate per day (₹): ";cin>>rate;
        cin.ignore();
        int x=nextVid++;
        if(choice==1){
            int seats;
            cout<<"Seating capacity : ";cin>>seats;
            cin.ignore();
            vehicles.push_back(new Car(x,b,m,rate,seats));
        }else{
            int cc;
            cout<<"Engine CC : ";cin>>cc;
            cin.ignore();
            vehicles.push_back(new Bike(x,b,m,rate,cc));
        }
        cout<<"Vehicle added successfully!"<<endl;
    }
    void displayAllVehicles(){
        cout<<endl<<"***All Vehicles***"<<endl;
        if(vehicles.empty())cout<<"No vehicles in system."<<endl;
        else for(auto v:vehicles)v->displayInfo();
        cout<<"**********"<<endl<<endl;
    }
    void displayAvailableVehicles(){
        cout<<endl<<"**** Available Vehicles ****"<<endl;
        bool found=false;
        for(auto v:vehicles){
            if(!v->getIsRented()){
                v->displayInfo();
                found=true;
            }
        }
        if(!found)cout<<"No vehicles available."<<endl;
        cout<<"**********"<<endl<<endl;
    }
    Vehicle*findVehicleById(int vid){
        for(auto v:vehicles)if(v->getId()==vid)return v;
        return nullptr;
    }
    void addCustomerInteractive(){
        string name;
        cout<<"Customer name : ";getline(cin,name);
        int x=nextCid++;
        customers[x]=Customer(x,name);
        cout<<"Customer added successfully with ID: "<<x<<endl;
    }
    void listCustomers(){
        cout<<endl<<"**** Customers ****"<<endl;
        if(customers.empty())cout<<"No customers in system."<<endl;
        else for(auto&p:customers)p.second.displayInfo();
        cout<<"**********"<<endl<<endl;
    }
    void rentVehicleInteractive(){
        int cid;
        cout<<"Enter customer ID: ";cin>>cid;
        cin.ignore();
        if(customers.find(cid)==customers.end()){
            cout<<"Customer not found!"<<endl;
            return;
        }
        if(customers[cid].getRentedVehicleId()!=-1){
            cout<<"Customer already has an active rental!"<<endl;
            return;
        }
        displayAvailableVehicles();
        int vid;
        cout<<"Enter vehicle ID to rent: ";cin>>vid;
        cin.ignore();
        Vehicle*v=findVehicleById(vid);
        if(!v||v->getIsRented()){
            cout<<"Vehicle not available!"<<endl;
            return;
        }
        int d;
        cout<<"Enter number of days : ";cin>>d;
        cin.ignore();
        string start;
        cout<<"Enter start date (DD-MM-YYYY) : ";getline(cin,start);
        customers[cid].rent(vid,d,start);
        v->setRented(true);
        cout<<endl<<"*Rental Confirmation*"<<endl<<"Customer: "<<customers[cid].getName()<<endl<<"Vehicle: "<<v->VName()<<" (ID: "<<vid<<")"<<endl<<"Days: "<<d<<endl<<"Estimated cost: ₹"<<v->calculateRentalCost(d)<<endl<<"**********"<<endl<<endl;
    }
    void returnVehicleInteractive(){
        int cid;
        cout<<"Enter customer ID: ";cin>>cid;
        cin.ignore();
        auto it=customers.find(cid);
        if(it==customers.end()){
            cout<<"Customer not found!"<<endl;
            return;
        }
        Customer&c=it->second;
        int vid=c.getRentedVehicleId();
        if(vid==-1){
            cout<<"No active rental for this customer!"<<endl;
            return;
        }
        Vehicle*v=findVehicleById(vid);
        int actualDays;
        cout<<"Enter actual days customer used the vehicle: ";cin>>actualDays;
        cin.ignore();
        int late=max(0,actualDays-c.getRentalDays());
        int baseCost=v->calculateRentalCost(actualDays);
        int lateFee=late*latefee;
        int total=baseCost+lateFee;
        v->setRented(false);
        c.returned();
        cout<<endl<<"** Return Receipt **"<<endl<<"Customer: "<<it->second.getName()<<endl<<"Vehicle ID: "<<vid<<endl<<"Base cost: ₹"<<baseCost<<endl<<"Late days: "<<late<<endl<<"Late fee: ₹"<<lateFee<<endl<<"Total charge: ₹"<<total<<endl<<"**********"<<endl<<endl;
    }
};

int main(){
    cout<<endl<<"****************"<<endl;
    cout<<"    VEHICLE RENTAL MANAGEMENT SYSTEM"<<endl;
    cout<<"****************"<<endl<<endl;
    RentalSystem system;
    while(true){
        cout<<endl<<"**** MAIN MENU ****"<<endl<<"1. Add vehicle"<<endl<<"2. Display all vehicles"<<endl<<"3. Display available vehicles"<<endl<<"4. Add customer"<<endl<<"5. List customers"<<endl<<"6. Rent vehicle"<<endl<<"7. Return vehicle"<<endl<<"8. Exit"<<endl<<"**********"<<endl<<"Choose option (1-8): ";
        int op;
        if(!(cin>>op)){
            cin.clear();
            cin.ignore(10000,'\n');
            cout<<"Invalid input! Please enter a number."<<endl;
            continue;
        }
        cin.ignore();
        switch(op){
            case 1:system.addVehicleInteractive();break;
            case 2:system.displayAllVehicles();break;
            case 3:system.displayAvailableVehicles();break;
            case 4:system.addCustomerInteractive();break;
            case 5:system.listCustomers();break;
            case 6:system.rentVehicleInteractive();break;
            case 7:system.returnVehicleInteractive();break;
            case 8:cout<<endl<<"Thank you for using Vehicle Rental System!"<<endl;return 0;
            default:cout<<"Invalid option! Please choose 1-8."<<endl;
        }
    }
}