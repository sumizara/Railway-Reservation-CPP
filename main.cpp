#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

// Train Class
class Train {
private:
    int trainNumber;
    string trainName;
    string source;
    string destination;
    int totalSeats;
    int availableSeats;
    double fare;
    
public:
    Train() : trainNumber(0), trainName(""), source(""), destination(""), totalSeats(0), availableSeats(0), fare(0.0) {}
    
    Train(int tNo, string tName, string src, string dest, int seats, double tFare) {
        trainNumber = tNo;
        trainName = tName;
        source = src;
        destination = dest;
        totalSeats = seats;
        availableSeats = seats;
        fare = tFare;
    }
    
    // Getters
    int getTrainNumber() { return trainNumber; }
    string getTrainName() { return trainName; }
    string getSource() { return source; }
    string getDestination() { return destination; }
    int getAvailableSeats() { return availableSeats; }
    double getFare() { return fare; }
    
    // Seat operations
    bool bookSeat(int seats) {
        if (seats <= availableSeats) {
            availableSeats -= seats;
            return true;
        }
        return false;
    }
    
    void cancelSeat(int seats) {
        availableSeats += seats;
        if (availableSeats > totalSeats) {
            availableSeats = totalSeats;
        }
    }
    
    void display() {
        cout << left << setw(15) << trainNumber 
             << setw(25) << trainName 
             << setw(20) << source 
             << setw(20) << destination 
             << setw(15) << availableSeats << "/" << totalSeats
             << setw(10) << "$" << fare << endl;
    }
    
    void saveToFile(ofstream &out) {
        out << trainNumber << "," << trainName << "," << source << "," 
            << destination << "," << totalSeats << "," << availableSeats << "," << fare << endl;
    }
    
    void loadFromFile(string line) {
        size_t pos = 0;
        vector<string> tokens;
        
        while ((pos = line.find(',')) != string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        tokens.push_back(line);
        
        if (tokens.size() == 7) {
            trainNumber = stoi(tokens[0]);
            trainName = tokens[1];
            source = tokens[2];
            destination = tokens[3];
            totalSeats = stoi(tokens[4]);
            availableSeats = stoi(tokens[5]);
            fare = stod(tokens[6]);
        }
    }
};

// Passenger Class
class Passenger {
private:
    int pnr;
    int trainNumber;
    string name;
    int age;
    string gender;
    int seats;
    double totalFare;
    string status;
    string bookingDate;
    
public:
    Passenger() {}
    
    Passenger(int p, int tNo, string pname, int page, string pgender, int s, double fare, string stat) {
        pnr = p;
        trainNumber = tNo;
        name = pname;
        age = page;
        gender = pgender;
        seats = s;
        totalFare = fare * s;
        status = stat;
        
        time_t now = time(0);
        bookingDate = ctime(&now);
        bookingDate = bookingDate.substr(0, bookingDate.length()-1);
    }
    
    int getPNR() { return pnr; }
    int getTrainNumber() { return trainNumber; }
    int getSeats() { return seats; }
    string getStatus() { return status; }
    
    void cancelTicket() {
        status = "CANCELLED";
    }
    
    void display() {
        cout << left << setw(15) << pnr 
             << setw(25) << name 
             << setw(10) << age 
             << setw(10) << gender 
             << setw(10) << trainNumber 
             << setw(10) << seats 
             << setw(15) << "$" << totalFare 
             << setw(15) << status << endl;
    }
    
    void saveToFile(ofstream &out) {
        out << pnr << "," << trainNumber << "," << name << "," 
            << age << "," << gender << "," << seats << "," 
            << totalFare << "," << status << "," << bookingDate << endl;
    }
};

// Railway System Class
class RailwaySystem {
private:
    vector<Train> trains;
    vector<Passenger> passengers;
    int nextPNR;
    
public:
    RailwaySystem() {
        nextPNR = 1001;
        initializeTrains();
        loadFromFile();
    }
    
    void initializeTrains() {
        // Add some default trains
        trains.push_back(Train(12001, "Rajdhani Express", "Delhi", "Mumbai", 120, 85.50));
        trains.push_back(Train(12002, "Shatabdi Express", "Delhi", "Chennai", 100, 95.00));
        trains.push_back(Train(12003, "Duronto Express", "Mumbai", "Bangalore", 80, 75.50));
        trains.push_back(Train(12004, "Garib Rath", "Delhi", "Kolkata", 150, 45.00));
        trains.push_back(Train(12005, "Sampark Kranti", "Chennai", "Mumbai", 90, 65.50));
    }
    
    // Display all trains
    void displayAllTrains() {
        cout << "\n🚂 === AVAILABLE TRAINS === 🚂\n";
        cout << left << setw(15) << "Train No" 
             << setw(25) << "Train Name" 
             << setw(20) << "Source" 
             << setw(20) << "Destination" 
             << setw(15) << "Available Seats"
             << setw(10) << "Fare" << endl;
        cout << string(105, '═') << endl;
        
        for (auto &train : trains) {
            train.display();
        }
    }
    
    // Book ticket
    void bookTicket() {
        int trainNo, seats, age;
        string name, gender;
        
        cout << "\n🎫 === BOOK TICKET === 🎫\n";
        displayAllTrains();
        
        cout << "\nEnter Train Number: ";
        cin >> trainNo;
        
        Train* train = findTrain(trainNo);
        if (train == NULL) {
            cout << "❌ Train not found!\n";
            return;
        }
        
        cout << "\nTrain: " << train->getTrainName() << endl;
        cout << "From: " << train->getSource() << " To: " << train->getDestination() << endl;
        cout << "Available Seats: " << train->getAvailableSeats() << endl;
        cout << "Fare per seat: $" << train->getFare() << endl;
        
        cout << "\nEnter Passenger Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Gender (M/F): ";
        cin >> gender;
        cout << "Number of Seats: ";
        cin >> seats;
        
        if (seats <= 0) {
            cout << "❌ Invalid number of seats!\n";
            return;
        }
        
        if (train->bookSeat(seats)) {
            Passenger p(nextPNR, trainNo, name, age, gender, seats, train->getFare(), "CONFIRMED");
            passengers.push_back(p);
            
            cout << "\n✅ Ticket Booked Successfully!\n";
            cout << "═══════════════════════════════════\n";
            cout << "PNR Number: " << nextPNR << endl;
            cout << "Train: " << train->getTrainName() << endl;
            cout << "From: " << train->getSource() << " → To: " << train->getDestination() << endl;
            cout << "Passenger: " << name << endl;
            cout << "Seats: " << seats << endl;
            cout << "Total Fare: $" << train->getFare() * seats << endl;
            cout << "Status: CONFIRMED\n";
            cout << "═══════════════════════════════════\n";
            
            nextPNR++;
            saveToFile();
        } else {
            cout << "❌ Sorry! Only " << train->getAvailableSeats() << " seats available!\n";
        }
    }
    
    // Cancel ticket
    void cancelTicket() {
        int pnr;
        
        cout << "\n❌ === CANCEL TICKET === ❌\n";
        cout << "Enter PNR Number: ";
        cin >> pnr;
        
        for (auto it = passengers.begin(); it != passengers.end(); ++it) {
            if (it->getPNR() == pnr) {
                if (it->getStatus() == "CANCELLED") {
                    cout << "❌ Ticket already cancelled!\n";
                    return;
                }
                
                // Free up seats
                Train* train = findTrain(it->getTrainNumber());
                if (train != NULL) {
                    train->cancelSeat(it->getSeats());
                }
                
                it->cancelTicket();
                cout << "✅ Ticket cancelled successfully!\n";
                cout << "PNR: " << pnr << " has been cancelled.\n";
                saveToFile();
                return;
            }
        }
        cout << "❌ PNR not found!\n";
    }
    
    // Check PNR status
    void checkPNRStatus() {
        int pnr;
        
        cout << "\n🔍 === CHECK PNR STATUS === 🔍\n";
        cout << "Enter PNR Number: ";
        cin >> pnr;
        
        for (auto &p : passengers) {
            if (p.getPNR() == pnr) {
                cout << "\n✅ PNR Found!\n";
                cout << "═══════════════════════════════\n";
                p.display();
                cout << "═══════════════════════════════\n";
                return;
            }
        }
        cout << "❌ PNR not found!\n";
    }
    
    // View all bookings
    void viewAllBookings() {
        if (passengers.empty()) {
            cout << "\n📭 No bookings found.\n";
            return;
        }
        
        cout << "\n📋 === ALL BOOKINGS === 📋\n";
        cout << left << setw(15) << "PNR" 
             << setw(25) << "Passenger Name" 
             << setw(10) << "Age" 
             << setw(10) << "Gender" 
             << setw(10) << "Train No" 
             << setw(10) << "Seats" 
             << setw(15) << "Total Fare" 
             << setw(15) << "Status" << endl;
        cout << string(110, '═') << endl;
        
        for (auto &p : passengers) {
            p.display();
        }
    }
    
    // Search train by route
    void searchTrainByRoute() {
        string src, dest;
        
        cout << "\n🔍 === SEARCH TRAIN BY ROUTE === 🔍\n";
        cout << "Enter Source Station: ";
        cin.ignore();
        getline(cin, src);
        cout << "Enter Destination Station: ";
        getline(cin, dest);
        
        cout << "\n🚂 Available Trains from " << src << " to " << dest << ":\n";
        cout << string(70, '═') << endl;
        
        bool found = false;
        for (auto &train : trains) {
            if (train.getSource() == src && train.getDestination() == dest) {
                train.display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "❌ No trains found on this route!\n";
        }
    }
    
    // Find train helper
    Train* findTrain(int trainNo) {
        for (auto &train : trains) {
            if (train.getTrainNumber() == trainNo) {
                return &train;
            }
        }
        return NULL;
    }
    
    // Save to file
    void saveToFile() {
        ofstream trainFile("trains.txt");
        for (auto &train : trains) {
            train.saveToFile(trainFile);
        }
        trainFile.close();
        
        ofstream passFile("passengers.txt");
        for (auto &p : passengers) {
            p.saveToFile(passFile);
        }
        passFile.close();
    }
    
    // Load from file
    void loadFromFile() {
        ifstream trainFile("trains.txt");
        if (trainFile) {
            trains.clear();
            string line;
            while (getline(trainFile, line)) {
                Train t;
                t.loadFromFile(line);
                trains.push_back(t);
            }
        }
        trainFile.close();
        
        ifstream passFile("passengers.txt");
        if (passFile) {
            passengers.clear();
            string line;
            while (getline(passFile, line)) {
                // Load passenger data
                // Implementation would go here
            }
        }
        passFile.close();
    }
    
    // Main menu
    void menu() {
        int choice;
        do {
            cout << "\n🚂 RAILWAY RESERVATION SYSTEM 🚂\n";
            cout << string(50, '=') << endl;
            cout << "1. 🚂 View All Trains\n";
            cout << "2. 🎫 Book Ticket\n";
            cout << "3. ❌ Cancel Ticket\n";
            cout << "4. 🔍 Check PNR Status\n";
            cout << "5. 📋 View All Bookings\n";
            cout << "6. 🚉 Search Train by Route\n";
            cout << "7. 🚪 Exit\n";
            cout << string(50, '=') << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: displayAllTrains(); break;
                case 2: bookTicket(); break;
                case 3: cancelTicket(); break;
                case 4: checkPNRStatus(); break;
                case 5: viewAllBookings(); break;
                case 6: searchTrainByRoute(); break;
                case 7: 
                    cout << "\n👋 Thank you for using Railway Reservation System!\n";
                    cout << "🚂 Have a safe journey!\n";
                    break;
                default: cout << "\n❌ Invalid choice! Try again.\n";
            }
        } while (choice != 7);
    }
};

// Main function
int main() {
    RailwaySystem railway;
    cout << "\n🚂 WELCOME TO RAILWAY RESERVATION SYSTEM 🚂\n";
    railway.menu();
    return 0;
}
