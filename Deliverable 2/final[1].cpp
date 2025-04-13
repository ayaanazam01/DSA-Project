#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;

struct Priority {
    string name;
    int rank;
    vector<string> subdivisions;
    int percentage;
};

bool isNumber(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

int getValidatedNumberInput(const string& prompt, int min = 0, int max = INT32_MAX) {
    string input;
    int num;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!isNumber(input)) {
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        num = stoi(input);
        if (num < min || num > max) {
            cout << "Please enter a number between " << min << " and " << max << ".\n";
            continue;
        }
        break;
    }
    return num;
}

int main() {
    int totalPriorities = getValidatedNumberInput("Enter the number of priorities: ", 1, 100);
    vector<Priority> priorities;

    for (int i = 0; i < totalPriorities; ++i) {
        Priority p;
        cout << "Enter name for priority " << (i + 1) << ": ";
        getline(cin, p.name);

        // Ensure unique rank
        while (true) {
            p.rank = getValidatedNumberInput("Enter unique rank for this priority: ", 1, totalPriorities);
            bool duplicate = false;
            for (const auto& existing : priorities) {
                if (existing.rank == p.rank) {
                    cout << "This rank is already taken. Please enter a different rank.\n";
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) break;
        }

        // Subdivisions
        while (true) {
            int numSubdivisions = getValidatedNumberInput("Please enter any number of subdivisions you want: (or press 0 to skip): ", 0, 100);
            if (numSubdivisions == 0) break;

            for (int j = 0; j < numSubdivisions; ++j) {
                cout << "Enter name for subdivision " << (j + 1) << ": ";
                string sub;
                getline(cin, sub);
                p.subdivisions.push_back(sub);
            }
            break;
        }

        priorities.push_back(p);
    }

    cout << "\nYou have chosen the Custom Option.\nYou have to now enter the budget allocation of each priority in percentages.\n";

    int totalPercent = 0;
    for (int i = 1; i <= totalPriorities; ++i) {
        int percent;
        while (true) {
            percent = getValidatedNumberInput("Please enter the percentage for priority rank " + to_string(i) + ": ", 0, 100);
            if (totalPercent + percent > 100) {
                cout << "Error: Total allocation would exceed 100. Try again.\n";
                continue;
            }
            break;
        }
        totalPercent += percent;
        for (auto& p : priorities) {
            if (p.rank == i) {
                p.percentage = percent;
                break;
            }
        }
    }

    if (totalPercent != 100) {
        cout << "Error: Total allocation does not equal 100.\nProgram will terminate in 5 seconds...\n";
        this_thread::sleep_for(chrono::seconds(5));
        return 1;
    }

    // Final Output
    cout << "\n--- Final Priority List ---\n";
    for (const auto& p : priorities) {
        cout << "Priority: " << p.name << ", Rank: " << p.rank << ", Allocation: " << p.percentage << "%\n";
        if (!p.subdivisions.empty()) {
            cout << "  Subdivisions:\n";
            for (const auto& sub : p.subdivisions)
                cout << "    - " << sub << "\n";
        }
    }

    return 0;
}
