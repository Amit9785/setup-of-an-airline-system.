
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <fstream> 
#include <sstream> 
#include <cmath> 

using namespace std;

#define M_PI 3.14159265358979323846

enum class SeatType {
    ECONOMY,
    BUSINESS,
    FIRST_CLASS
};

enum class FoodOption {
    VEGETARIAN,
    NON_VEGETARIAN,
    BOTH
};

class Seat {
public:
    SeatType seatType;
    string seatNumber;

    Seat(SeatType type, const string& number) : seatType(type), seatNumber(number) {}
};

class Aircraft {
public:
    string model;
    size_t economySeats;
    size_t businessSeats;
    size_t firstClassSeats;

    Aircraft(const string& mdl, size_t economy, size_t business, size_t firstClass)
        : model(mdl), economySeats(economy), businessSeats(business), firstClassSeats(firstClass) {}
};

class Airport {
public:
    string airportCode;
    string airportName;
    string location;
    int runway; // Changed to integer
    double latitude;
    double longitude;

    Airport(const string& code, const string& name, const string& loc, int rw, double lat, double lon)
        : airportCode(code), airportName(name), location(loc), runway(rw), latitude(lat), longitude(lon) {}

};

class Flight {
public:
    string flightNumber;
    string departureAirport;
    string destinationAirport;
    string departureTime;
    string departureDay;
    string arrivalTime;
    string arrivalDay;
    Aircraft aircraft;
    vector<Seat> availableSeats;
    FoodOption foodOption;
    unordered_map<string, Airport>& airports; 

    Flight(const string& number, const string& departure, const string& destination,
        const string& depTime, const string& depDay, const string& arrTime, const string& arrDay,
        const Aircraft& ac, FoodOption food, unordered_map<string, Airport>& airportMap)
        : flightNumber(number), departureAirport(departure), destinationAirport(destination),
        departureTime(depTime), departureDay(depDay), arrivalTime(arrTime), arrivalDay(arrDay),
        aircraft(ac), foodOption(food), airports(airportMap) {
            initializeSeats();
        }

    void displaySeatDetails() const {
        cout << "Seat Details for " << flightNumber << ":" << endl;

        displaySeatDetailsByClass(SeatType::ECONOMY, "Economy Class");
        displaySeatDetailsByClass(SeatType::BUSINESS, "Business Class");
        displaySeatDetailsByClass(SeatType::FIRST_CLASS, "First Class");

        cout << endl;
    }

    double calculateDistance() const {
        // Get departure and destination airports
        auto departureAirportIter = airports.find(departureAirport);
        auto destinationAirportIter = airports.find(destinationAirport);

        if (departureAirportIter != airports.end() && destinationAirportIter != airports.end()) {
            const Airport& departureAirport = departureAirportIter->second;
            const Airport& destinationAirport = destinationAirportIter->second;

            // Calculate distance using Haversine formula
            double distance = calculateDistance(departureAirport, destinationAirport);
            return distance;
        } else {
            cerr << "Error: Departure or destination airport not found." << endl;
            return 0.0;
        }
    }

private:
    void displaySeatDetailsByClass(SeatType seatClass, const string& className) const {
        cout << "   " << className << ":" << endl;
        for (const auto& seat : availableSeats) {
            if (seat.seatType == seatClass) {
                cout << "      " << seat.seatNumber;
            }
        }
        cout << endl;
    }

    void initializeSeats() {
        for (size_t i = 1; i <= aircraft.economySeats; ++i) {
            availableSeats.push_back(Seat(SeatType::ECONOMY, "E" + to_string(i)));
        }

        for (size_t i = 1; i <= aircraft.businessSeats; ++i) {
            availableSeats.push_back(Seat(SeatType::BUSINESS, "B" + to_string(i)));
        }

        for (size_t i = 1; i <= aircraft.firstClassSeats; ++i) {
            availableSeats.push_back(Seat(SeatType::FIRST_CLASS, "F" + to_string(i)));
        }
    }

    double calculateDistance(const Airport& source, const Airport& destination) const {
        // We change radian to degree
        double lat1 = source.latitude * M_PI / 180.0;
        double lon1 = source.longitude * M_PI / 180.0;
        double lat2 = destination.latitude * M_PI / 180.0;
        double lon2 = destination.longitude * M_PI / 180.0;

        // Earth radius in kilometers
        const double R = 6371.0;

        // Haversine formula
        double dlon = lon2 - lon1;
        double dlat = lat2 - lat1;
        double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        double distance = R * c;

        return distance;
    }
};

class Airline {
private:
    unordered_map<string, Airport> airports;
    unordered_map<string, Flight> flights;
    unordered_map<string, vector<string>> flightGraph;

public:
    void addAirport(const string& code, const string& name, const string& location, int runway, double lat, double lon) {
        airports.emplace(code, Airport(code, name, location, runway, lat, lon));
    }

    void addFlight(const string& number, const string& departure, const string& destination,
                const string& depTime, const string& depDay, const string& arrTime, const string& arrDay,
                const Aircraft& aircraft, FoodOption food) {
        flights.emplace(number, Flight(number, departure, destination, depTime, depDay, arrTime, arrDay, aircraft, food, airports));
        flightGraph[departure].push_back(destination);
    }

    void displayFlightSchedule() const {
        cout << "Flight Schedule:" << "\n";
        for (const auto& flight : flights) {
            cout << "Flight Details:" << endl; // Add this  to mark the start of each flight details
            string airlineName = getAirlineName(flight.first);
            cout << "    Airline: " << airlineName << "\n";
            cout << "    Flight Number: " << flight.second.flightNumber << "\n";
            cout << "    Departure Airport: " << flight.second.departureAirport << "\n";
            cout << "    Destination Airport: " << flight.second.destinationAirport << "\n";
            cout << "    Departure Time: " << flight.second.departureTime << "\n";
            cout << "    Departure Day: " << flight.second.departureDay << "\n";
            cout << "    Arrival Time: " << flight.second.arrivalTime << "\n";
            cout << "    Arrival Day: " << flight.second.arrivalDay << "\n";
            cout << "    Aircraft Model: " << flight.second.aircraft.model << "\n";
            cout << "    Available Seats: " << flight.second.availableSeats.size() << "\n";
            cout << "    Food Option: ";
            if (flight.second.foodOption == FoodOption::VEGETARIAN) {
                cout << "Vegetarian";
            } else if (flight.second.foodOption == FoodOption::NON_VEGETARIAN) {
                cout << "Non-Vegetarian";
            } else if (flight.second.foodOption == FoodOption::BOTH) {
                cout << "Both Vegetarian and Non-Vegetarian";
            }
            cout << endl;
            cout << "Distance: " << flight.second.calculateDistance() << " km" << "\n";
            cout << endl;
        }
    }

    void displayAirportDetails() const {
        cout << "Airport Details:" << endl;
        for (const auto& airport : airports) {
            cout << "Airport Code: " << airport.second.airportCode << endl;
            cout << "   Airport Name: " << airport.second.airportName << endl;
            cout << "   Location: " << airport.second.location << endl;
            cout << "   Latitude: " << airport.second.latitude << endl;
            cout << "   Longitude: " << airport.second.longitude << endl;
            cout << "   Runway: " << airport.second.runway << endl; // Display runway
            cout << endl;
        }
    }

    void displayFlightGraph() const {
        cout << "Flight Graph (Directed):" << endl;
        for (const auto& entry : flightGraph) {
            cout << entry.first << " -> ";

            for (const auto& destination : entry.second) {
                cout << destination << " ";
            }
            cout << endl;
        }
    }

    void displaySeatDetailsForAllFlights() const {
        cout << "Seat Details for All Flights:" << endl;
        for (const auto& flight : flights) {
            flight.second.displaySeatDetails();
        }
    }

private:
    string getAirlineName(const string& flightNumber) const {
        if (flightNumber.substr(0, 2) == "6E") {
            return "Indigo";
        } else if (flightNumber.substr(0, 2) == "SG") {
            return "SpiceJet";
        } else if (flightNumber.substr(0, 2) == "AI") {
            return "Air India";
        } else {
            return "Indigo"; 
        }
    }
};

int main() {
    Airline airline;

    ifstream airportFile("airports.txt");
    string line;
    while (getline(airportFile, line)) {
        stringstream ss(line);
        string code, name, location;
        int runway;
        double lat, lon;
        getline(ss, code, ',');
        getline(ss, name, ',');
        getline(ss, location, ',');
        ss >> runway; 
        ss.ignore(); // Ignore the comma
        ss >> lat;
        ss.ignore(); 
        ss >> lon;
        ss.ignore(); 
        airline.addAirport(code, name, location, runway, lat, lon); // Pass runway as integer
    }
    airportFile.close();

    ifstream flightFile("flights.txt");
    while (getline(flightFile, line)) {
        stringstream ss(line);
        string number, departure, destination, depTime, depDay, arrTime, arrDay, model;
        size_t economy, business, firstClass;
        string food;
        getline(ss, number, ',');
        getline(ss, departure, ',');
        getline(ss, destination, ',');
        getline(ss, depTime, ',');
        getline(ss, depDay, ',');
        getline(ss, arrTime, ',');
        getline(ss, arrDay, ',');
        getline(ss, model, ',');
        ss >> economy;
        ss.ignore(); 
        ss >> business;
        ss.ignore(); 
        ss >> firstClass;
        ss.ignore(); 
        ss >> food;

        FoodOption foodOption;
        if (food == "VEGETARIAN") {
            foodOption = FoodOption::VEGETARIAN;
        } else if (food == "NON_VEGETARIAN") {
            foodOption = FoodOption::NON_VEGETARIAN;
        } else {
            foodOption = FoodOption::BOTH;
        }

        Aircraft aircraft(model, economy, business, firstClass);
        airline.addFlight(number, departure, destination, depTime, depDay, arrTime, arrDay, aircraft, foodOption);
    }
    flightFile.close();

    ofstream outFile("output.txt"); // The Output should be store in output.txt file
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf()); 

    airline.displayFlightSchedule();
    airline.displayAirportDetails();
    airline.displayFlightGraph();
    airline.displaySeatDetailsForAllFlights();
    cout.rdbuf(coutbuf);

    outFile.close(); 

    return 0;
}

