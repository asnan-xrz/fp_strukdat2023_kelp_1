#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>

using namespace std;

class Attraction {
public:
    string name;
    string description;

public:
    Attraction(const string& attractionName, const string& attractionDescription)
        : name(attractionName), description(attractionDescription) {}

    void displayInfo() const {
        cout << "Attraction: " << name << " (" << description << ")" << endl;
    }

    virtual ~Attraction() {}
};

class City {
protected:
    string name;
    string region;
    vector<Attraction*> attractions;

public:
    City(const string& cityName, const string& cityRegion, const vector<Attraction*>& cityAttractions)
        : name(cityName), region(cityRegion), attractions(cityAttractions) {}

    virtual void displayInfo() const {
        cout << "City: " << name << " (Region: " << region << ")" << endl;
        cout << "Attractions:" << endl;
        for (const Attraction* attraction : attractions) {
            attraction->displayInfo();
        }
        cout << endl;
    }

    virtual ~City() {
        for (Attraction* attraction : attractions) {
            delete attraction;
        }
    }
};

class ConcreteCity : public City {
public:
    ConcreteCity(const string& cityName, const string& cityRegion, const vector<Attraction*>& cityAttractions)
        : City(cityName, cityRegion, cityAttractions) {}

    void displayInfo() const override {
        City::displayInfo();
    }

    ~ConcreteCity() {
        for (Attraction* attraction : attractions) {
            delete attraction;
        }
    }
};

class Route {
public:
    City* startCity;
    City* endCity;
    int distanceKm;
    int travelTimeHours;
    int cost;

public:
    Route(City* start, City* end, int distance, int time, int routeCost)
        : startCity(start), endCity(end), distanceKm(distance), travelTimeHours(time), cost(routeCost) {}

    void displayInfo() const {
        cout << endl << "Route from ";
        startCity->displayInfo();
        cout << "to ";
        endCity->displayInfo();
    }
};

class TravelGraph {
private:
    vector<vector<int>> adjacencyList;
    vector<City*> cities;
    vector<Route*> routes;

public:
    TravelGraph() {
        adjacencyList.resize(5, vector<int>(5, 0));
        cities.push_back(new ConcreteCity("Jakarta", "DKI Jakarta", {new Attraction("Monas", "National Monument"), new Attraction("Ancol Dreamland", "Theme park")}));
        cities.push_back(new ConcreteCity("Bandung", "West Java", {new Attraction("Saung Angklung Udjo", "Traditional music performance"), new Attraction("Tangkuban Perahu Volcano", "Natural landmark")}));
        cities.push_back(new ConcreteCity("Yogyakarta", "DIY Yogyakarta", {new Attraction("Prambanan Temple", "Hindu temple complex"), new Attraction("Malioboro Street", "Bustling shopping street")}));
        cities.push_back(new ConcreteCity("Surabaya", "East Java", {new Attraction("Surabaya Zoo", "Zoological garden"), new Attraction("Tanjung Perak Port", "Largest port in Indonesia")}));
        cities.push_back(new ConcreteCity("Denpasar", "Bali", {new Attraction("Pura Tanah Lot", "Temple perched on a rock"), new Attraction("Kuta Beach", "Popular surfing destination")}));

        addConnection(0, 1, 157, 3, 72500);
        addConnection(0, 2, 568, 8, 439500);
        addConnection(0, 3, 788, 11, 728500);
        addConnection(0, 4, 1186, 20, 728500);
        addConnection(1, 2, 484, 7, 455500);
        addConnection(1, 3, 788, 11, 744500);
        addConnection(2, 3, 325, 5, 278500);
        addConnection(3, 4, 425, 10, 90500);
    }

    ~TravelGraph() {
        for (City* city : cities) {
            delete city;
        }
        for (Route* route : routes) {
            delete route;
        }
    }

    void addConnection(int cityIndex1, int cityIndex2, int distance, int time, int routeCost) {
        adjacencyList[cityIndex1][cityIndex2] = 1;
        adjacencyList[cityIndex2][cityIndex1] = 1;

        routes.push_back(new Route(cities[cityIndex1], cities[cityIndex2], distance, time, routeCost));
    }

    vector<int> dijkstra(int startCityIndex, int endCityIndex) const {
        vector<int> distance(5, numeric_limits<int>::max());
        distance[startCityIndex] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, startCityIndex});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (int v = 0; v < 5; ++v) {
                if (adjacencyList[u][v] == 1) {
                    int weight = 1;  
                    if (distance[v] > distance[u] + weight) {
                        distance[v] = distance[u] + weight;
                        pq.push({distance[v], v});
                    }
                }
            }
        }

        return distance;
    }

    void displayShortestPath(int startCityIndex, int endCityIndex) const {
        vector<int> shortestDistances = dijkstra(startCityIndex - 1, endCityIndex - 1);

        cout << endl << "Shortest distance: " << shortestDistances[endCityIndex - 1] << " cities." << endl;

        int currentCityIndex = startCityIndex - 1;
        int nextCityIndex = (currentCityIndex + 1) % 5;

        if (shortestDistances[nextCityIndex] != numeric_limits<int>::max()) {
            cout << endl << "Route from ";
            cities[currentCityIndex]->displayInfo();
            cout << "to ";
            cities[endCityIndex - 1]->displayInfo();

            cout << "Distance: " << routes[endCityIndex - 2]->distanceKm << " KM, "
                << "Travel Time: " << routes[endCityIndex - 2]->travelTimeHours << " hours, "
                << "Cost: " << routes[endCityIndex - 2]->cost << " IDR" << endl;
        }
    }
};

int main() {
    int startCityIndex, endCityIndex;

    do {
        cout << "Select your starting city:" << endl;
        cout << "1. Jakarta" << endl;
        cout << "2. Bandung" << endl;
        cout << "3. Yogyakarta" << endl;
        cout << "4. Surabaya" << endl;
        cout << "5. Denpasar" << endl;
        cout << "Enter your choice (1-5): ";
        cin >> startCityIndex;
    } while (startCityIndex < 1 || startCityIndex > 5);

    do {
        cout << "Select your destination city:" << endl;
        cout << "1. Jakarta" << endl;
        cout << "2. Bandung" << endl;
        cout << "3. Yogyakarta" << endl;
        cout << "4. Surabaya" << endl;
        cout << "5. Denpasar" << endl;
        cout << "Enter your choice (1-5): ";
        cin >> endCityIndex;
    } while (endCityIndex < 1 || endCityIndex > 5);

    TravelGraph travelGraph;

    travelGraph.displayShortestPath(startCityIndex, endCityIndex);

    return 0;
}
