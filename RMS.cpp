#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>
#include<algorithm>
using namespace std;

struct Road {
    int city1, city2;
    double budget;
};

string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}
// Check if the city exists(in lowercase as Kigali and kigali are the same.)
bool cityExists(const vector<string>& cities, const string& city) {
    string target = toLowerCase(city);
    for (const auto& city : cities) {
        if (toLowerCase(city) == target) {
            return true;
        }
    }
    return false;
}

void saveCities(const vector<string>& cities) {
    ofstream outFile("cities.txt");
    for (size_t i = 0; i < cities.size(); i++) {
        outFile << i + 1 << " " << cities[i] << endl;
    }
    outFile.close();
}

void saveRoads(const vector<Road>& roads, const vector<string>& cities) {
    ofstream outFile("roads.txt");
    for (size_t i = 0; i < roads.size(); ++i) {
        outFile << i + 1 << " " << cities[roads[i].city1 - 1]<< "--"
                << cities[roads[i].city2 - 1] << " " << roads[i].budget << endl;
    }
    outFile.close();
}

void addCities(vector<string>& cities, int numCities) {
    string cityName;
    for (int i = 0; i < numCities; ++i) {
        cout << "Enter name for city " << i + 1 << ": ";
        cin >> cityName;
        if (cityExists(cities, cityName)) {
            cout << "City " << cityName << " already exists." << endl;
            --i;
            continue;
        }
        cities.push_back(cityName);
        cout << "City " << cityName << " assigned index " << cities.size() << endl;
    }
    saveCities(cities);
}

int findCityIndex(const vector<string>& cities, const string& city) {
    for (size_t i = 0; i < cities.size(); ++i) {
        if (cities[i] == city) return i + 1;
    }
    return -1;
}

void addRoad(vector<vector<int>>& roads, vector<Road>& roadList, const vector<string>& cities) {
    string city1, city2;
    cout << "Enter the name of the first city : ";
    cin >> city1;
    cout << "Enter the name of the second city : ";
    cin >> city2;

    int idx1 = findCityIndex(cities, city1);
    int idx2 = findCityIndex(cities, city2);

    if (idx1 == -1 || idx2 == -1) {
        cout << "One or both cities not found!\n";
        return;
    }
    if (idx1 == idx2) {
        cout << "Cannot create road between the same city!\n";
        return;
    }

    roads[idx1 - 1][idx2 - 1] = roads[idx2 - 1][idx1 - 1] = 1;
    cout << "Road added between " << city1 << " and " << city2 << endl;
    saveRoads(roadList, cities);
}

void addRoadBudget(vector<vector<int>>& roads, vector<vector<double>>& budgets,
                   vector<Road>& roadList, const vector<string>& cities) {
    string city1, city2;
    cout << "Enter the name of the first city: ";
    cin >> city1;
    cout << "Enter the name of the second city: ";
    cin >> city2;

    int idx1 = findCityIndex(cities, city1);
    int idx2 = findCityIndex(cities, city2);

    if (idx1 == -1 || idx2 == -1) {
        cout << "One or both cities not found!\n";
        return;
    }
    if (roads[idx1 - 1][idx2 - 1] == 0) {
        cout << "No road exists between these cities!\n";
        return;
    }

    double budget;
    cout << "Enter budget for the road (in billions): ";
    cin >> budget;
    budgets[idx1 - 1][idx2 - 1] = budgets[idx2 - 1][idx1 - 1] = budget;

    bool found = false;
    for (auto& road : roadList) {
        if ((road.city1 == idx1 && road.city2 == idx2) ||
            (road.city1 == idx2 && road.city2 == idx1)) {
            road.budget = budget;
            found = true;
            break;
        }
    }
    if (!found) {
        roadList.push_back({idx1, idx2, budget});
    }

    saveRoads(roadList, cities);
    cout << "Budget added for the road between " << city1 << " and " << city2 << endl;
}

void editCity(vector<string>& cities, int index) {
    if (index < 1 || index > static_cast<int>(cities.size())) {
        cout << "Invalid city index!\n";
        return;
    }

    string newName;
    cout << "Enter new name for city: ";
    cin >> newName;

    if (cityExists(cities, newName)) {
        cout << "City " << newName << " already exists.\n";
        return;
    }

    cities[index - 1] = newName;
    saveCities(cities);
    cout << "City updated successfully\n";
}

void searchCity(const vector<string>& cities, int index) {
    if (index < 1 || index > static_cast<int>(cities.size())) {
        cout << "Invalid city index!\n";
        return;
    }
    cout << "City found: Index " << index << ", Name: " << cities[index - 1] << endl;
}

void displayCities(const vector<string>& cities) {
    cout << "Cities displayed:\n";
    for (size_t i = 0; i < cities.size(); ++i) {
        cout << "Index: " << i + 1 << ", Name: " << cities[i] << endl;
    }
}

void displayMatrix(const vector<vector<int>>& matrix, const vector<string>& cities, const string& title) {
    cout << "Cities :\n";
    for (size_t i = 0; i < cities.size(); ++i) {
        cout << i + 1 << "  " << cities[i] << endl;
    }
    cout << "\n" << title << "\n";

    cout << endl;
    for (size_t i = 0; i < cities.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
}

void displayBudgetMatrix(const vector<vector<double>>& matrix, const vector<string>& cities) {
    cout << "Cities :\n";
    for (size_t i = 0; i < cities.size(); ++i) {
        cout << i + 1 << "  " << cities[i] << endl;
    }
    cout << "\nRoad Budget Matrix:\n ";

    cout << endl;
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 0.0) {
                cout << setw(5) << "-";
            } else {
                cout << setw(5) << matrix[i][j];
            }
        }
        cout << endl;
    }
}

void displayAllData(const vector<string>& cities, const vector<vector<int>>& roads,
                    const vector<vector<double>>& budgets) {
    displayCities(cities);
    displayMatrix(roads, cities, "Road Adjacency matrix");
    displayBudgetMatrix(budgets, cities);
}

int main() {
    vector<string> cities;
    vector<Road> roadList;
    int choice, numCities, index;

    vector<vector<int>> roads(cities.size(), vector<int>(cities.size(), 0));
    vector<vector<double>> budgets(cities.size(), vector<double>(cities.size(), 0.0));

    while (true) {
        cout << "\nMenu:\n"
             << "1. Add new city(ies)\n"
             << "2. Add roads between cities\n"
             << "3. Add the budget for roads\n"
             << "4. Edit city\n"
             << "5. Search for a city using its index\n"
             << "6. Display cities\n"
             << "7. Display roads\n"
             << "8. Display recorded data on console\n"
             << "9. Exit\n"
             << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter number of cities to add: ";
                cin >> numCities;
                addCities(cities, numCities);
                roads.resize(cities.size(), vector<int>(cities.size(), 0));
                budgets.resize(cities.size(), vector<double>(cities.size(), 0.0));
                break;
            case 2:
                addRoad(roads, roadList, cities);
                break;
            case 3:
                addRoadBudget(roads, budgets, roadList, cities);
                break;
            case 4:
                cout << "Enter the index of the city to edit: ";
                cin >> index;
                editCity(cities, index);
                break;
            case 5:
                cout << "Enter city index to search: ";
                cin >> index;
                searchCity(cities, index);
                break;
            case 6:
                displayCities(cities);
                break;
            case 7:
                displayMatrix(roads, cities, "Road Adjacency matrix");
                break;
            case 8:
                displayAllData(cities, roads, budgets);
                break;
            case 9:
                cout << "Thank you for using our system. Exiting program ...\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }
}