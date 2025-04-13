#include <iostream> // For input/output
#include <thread>   // For std::this_thread::sleep_for
#include <chrono>   // For std::chrono::seconds
using namespace std;

string PriorityNames[5]; // Array declared for quick access of Priority Names
int PriorityRank[5];     // Array declared for accessing Priority Ranks

struct Stack_Node {
    int priorityrank_stack;
    Stack_Node *next;
};

class Stack {
public:
    Stack_Node *top = NULL;

    void push(int val) {
        Stack_Node *newnode = new Stack_Node;
        newnode->priorityrank_stack = val;
        newnode->next = top;
        top = newnode;
    }

    int pop() {
        if (top == NULL) {
            cout << "Stack Underflow" << endl;
            return -1;
        } else {
            int temp = top->priorityrank_stack;
            top = top->next;
            return temp;
        }
    }

    void display() {
        Stack_Node *ptr;
        if (top == NULL) {
            cout << "History is empty" << endl;
        } else {
            ptr = top;
            cout << "History of accessed nodes (in terms of Priority Rank): ";
            while (ptr != NULL) {
                cout << ptr->priorityrank_stack << ", ";
                ptr = ptr->next;
            }
        }
        cout << endl;
    }
};

class Priority {
public:
    string priorityname;
    int priorityrank, subdivision, division, divisionbudget, subdivisionbudget;
    float prioritybudget;

    Priority() {
        priorityrank = 0;
        division = 0;
        subdivision = 0;
        divisionbudget = 0;
        subdivisionbudget = 0;
    }

    int Calcdivbudget(int prbudget, int d) {
        if (d == 0) {
            divisionbudget = 0;
        } else if (d == 1) {
            divisionbudget = prbudget / 30;
        } else if (d == 2) {
            divisionbudget = prbudget / 4;
        }
        return divisionbudget;
    }

    int Calcsubdivbudget(int d_budget, int s_div) {
        if (s_div == 0) {
            return 0;
        }
        subdivisionbudget = d_budget / s_div;
        return subdivisionbudget;
    }

    void SetValues_Preset(string name, int div, int subdiv, int prrank, int totalbudget) {
        priorityname = name;
        division = div;
        subdivision = subdiv;
        priorityrank = prrank;

        if (priorityrank == 1)
            prioritybudget = totalbudget * 0.4;
        else if (priorityrank == 2)
            prioritybudget = totalbudget * 0.25;
        else if (priorityrank == 3)
            prioritybudget = totalbudget * 0.15;
        else if (priorityrank == 4)
            prioritybudget = totalbudget * 0.10;
        else if (priorityrank == 5)
            prioritybudget = totalbudget * 0.10;

        divisionbudget = Calcdivbudget(prioritybudget, division);
        subdivisionbudget = Calcsubdivbudget(divisionbudget, subdivision);
        PriorityNames[priorityrank - 1] = priorityname;
        PriorityRank[priorityrank] = priorityrank;
    }

    void SetValues_Custom(string name, int div, int subdiv, int prrank, int totalbudget, int prrank_allocation) {
        priorityname = name;
        division = div;
        subdivision = subdiv;
        priorityrank = prrank;

        prioritybudget = totalbudget * (static_cast<double>(prrank_allocation) / 100);
        divisionbudget = Calcdivbudget(prioritybudget, division);
        subdivisionbudget = Calcsubdivbudget(divisionbudget, subdivision);
        PriorityNames[priorityrank - 1] = priorityname;
        PriorityRank[priorityrank] = priorityrank;
    }
};

struct PriorityNode {
    Priority pr;
    PriorityNode *next;
    PriorityNode *prev;
};

class BudgetManager {
public:
    PriorityNode *head;
    PriorityNode *tail;
    Stack History;

    BudgetManager() {
        head = NULL;
        tail = NULL;
    }

    void InsertNode(string name, int div, int subdiv, int prrank, int totalbudget, int budgetopt, int prrank_allocation) {
        PriorityNode *newNode = new PriorityNode;

        if (budgetopt == 1) {
            newNode->pr.SetValues_Preset(name, div, subdiv, prrank, totalbudget);
        } else if (budgetopt == 2) {
            newNode->pr.SetValues_Custom(name, div, subdiv, prrank, totalbudget, prrank_allocation);
        }
        newNode->next = head;
        newNode->prev = NULL;

        if (head != NULL) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }

        head = newNode;
        History.push(prrank);
    }

    void DisplayAllBudget(PriorityNode *node) {
        if (node == NULL)
            return;

        cout << "For " << node->pr.priorityname << ":" << endl;
        cout << "The monthly budget is: " << node->pr.prioritybudget << endl;

        if (node->pr.divisionbudget != 0) {
            cout << "The budget of each division is: " << node->pr.divisionbudget << endl;
        }

        if (node->pr.subdivisionbudget != 0) {
            cout << "The budget of each subdivision is: " << node->pr.subdivisionbudget << endl;
        }

        History.push(node->pr.priorityrank);
        DisplayAllBudget(node->next);
    }

    void SearchDisplaybudget(string searchName) {
        bool found = false;

        for (int i = 0; i < 5; i++) {
            if (searchName == PriorityNames[i]) {
                found = true;
                break;
            }
        }
        if (!found) return;

        PriorityNode *current = head;

        while (current != NULL) {
            if (current->pr.priorityname == searchName) {
                cout << "Priority Name: " << current->pr.priorityname << endl;
                cout << "Priority Rank: " << current->pr.priorityrank << endl;
                cout << "Monthly Budget: " << current->pr.prioritybudget << endl;

                if (current->pr.divisionbudget != 0) {
                    cout << "Division Budget (" << (current->pr.division == 1 ? "Daily" : "Weekly") << "): " << current->pr.divisionbudget << endl;
                }

                if (current->pr.subdivisionbudget != 0) {
                    cout << "Subdivision Budget: " << current->pr.subdivisionbudget << endl;
                }

                History.push(current->pr.priorityrank);
                break;
            }

            current = current->next;
        }
    }
};

void BM_display() {
    BudgetManager B;
    int chosen_budget_option, total_allocated_budget;

    cout << "Welcome to Budget Manager!" << endl;

    do {
        cout << "Please enter your total budget: " << endl;
        cin >> total_allocated_budget;
    } while (total_allocated_budget <= 0);

    cout << "There are 2 options for calculating your budget:" << endl;
    cout << "1- Using a preset (divides the budget into following percentages: 40-25-15-10-10)." << endl;
    cout << "2- Using your own custom divisions." << endl;

    do {
        cout << "Which option would you like to proceed with? (enter either 1 or 2): ";
        cin >> chosen_budget_option;
    } while (chosen_budget_option != 1 && chosen_budget_option != 2);

    if (chosen_budget_option == 1) {
        cout << "You have chosen the Preset Option. You only have to enter 5 priorities. " << endl;
        for (int i = 0; i < 5; i++) {
            string name;
            int chosen_division, chosen_subdivision, chosen_rank;

            cout << endl;
            cout << "Please enter the priority name:" << endl;
            cin >> name;

            cout << endl << "Following, are some ways you can divide your budget." << endl;
            cout << "Enter 1 for getting a daily budget allocated for this priority." << endl;
            cout << "Enter 2 for getting a weekly budget allocated for this priority." << endl;
            cout << "Enter 0 for skipping this feature." << endl;
            do {
                cout << "Please enter a number:" << endl;
                cin >> chosen_division;
            } while (chosen_division != 1 && chosen_division != 2 && chosen_division != 0);

            cout << endl << "Following, you can add any number of subdivisions to your subdivisions to your current priority" << endl;
            cout << "(e.g 3 subdivisions for daily food expenses, or, 2 subdivisions for toll taxes during weekly commute)" << endl;
            do {
                cout << "Please enter any number of subdivisions you want: (or press 0 to skip)" << endl;
                cin >> chosen_subdivision;
            } while (chosen_subdivision != 0 && (chosen_subdivision < 1 || chosen_subdivision > 50));

            cout << "The ranks of priorities are distributed as follows:" << endl;
            cout << "Highest budget percentage is given to rank 1 & so on, with the lowest budget percentage given to rank 5 priority." << endl;
            do {
                cout << "Please enter a rank for this priority (between 1-5): ";
                cin >> chosen_rank;
            } while ((chosen_rank < 1 || chosen_rank > 5) ||
                     (chosen_rank == PriorityRank[0] ||
                      chosen_rank == PriorityRank[1] ||
                      chosen_rank == PriorityRank[2] ||
                      chosen_rank == PriorityRank[3] ||
                      chosen_rank == PriorityRank[4]));

            B.InsertNode(name, chosen_division, chosen_subdivision, chosen_rank, total_allocated_budget, chosen_budget_option, 0);
        }
    }

    if (chosen_budget_option == 2) {
        cout << "You have chosen the Custom Option. " << endl;
        cout << "You have to now enter the budget allocation of each priority in percentages. " << endl;
        cout << "Note that percentages should add up to 100 and each individual percentage must be between 0 and 100. (Don't add percentage signs)" << endl;

        int priority_rank_budgetallocation[5];
        int sum_check = 0;                     
        int count = 0;

        int input;
        do {
            cout << "Please enter the percentage for priority rank " << count + 1 << ": ";
            cin >> input;

            if (input < 0 || input > 100) {
                cout << "Percentage must be between 0 and 100. Please try again." << endl;
            } else if (sum_check + input > 100) {
                cout << "Total allocation exceeds 100%. Please try again." << endl;
            } else {
                priority_rank_budgetallocation[count] = input;
                sum_check += input;
                count++;
            }
        } while (count < 5);

        if (sum_check == 100) {
            cout << "Budget allocation completed successfully!" << endl;
        } else {
            cout << "Error: Total allocation does not equal 100." << endl;
            cout << "Program will terminate in 5 seconds..." << endl;
            this_thread::sleep_for(chrono::seconds(5));
            return;
        }

        for (int i = 0; i < 5; i++) {
            string name;
            int chosen_division, chosen_subdivision;

            cout << endl;
            cout << "Please enter the priority name:" << endl;
            cin >> name;

            cout << "Following, are some ways you can divide your budget." << endl;
            cout << "Enter 1 for getting a daily budget allocated for this priority." << endl;
            cout << "Enter 2 for getting a weekly budget allocated for this priority." << endl;
            cout << "Enter 0 for skipping this feature." << endl;
            do {
                cout << "Please enter a number:" << endl;
                cin >> chosen_division;
            } while (chosen_division != 1 && chosen_division != 2 && chosen_division != 0);

            cout << endl << "Following, you can add any number of subdivisions to your divided budget. " << endl;
            cout << "(e.g 3 subdivisions for daily food expenses, or, 2 subdivisions for toll taxes during weekly commute)" << endl;
            do {
                cout << "Please enter any number of subdivisions you want: (or press 0 to skip)" << endl;
                cin >> chosen_subdivision;
            } while (chosen_subdivision != 0 && (chosen_subdivision < 1 || chosen_subdivision > 50));

            B.InsertNode(name, chosen_division, chosen_subdivision, (i + 1), total_allocated_budget, chosen_budget_option, priority_rank_budgetallocation[i]);
        }
    }

    int finalopt = 0;
    do {
        cout << endl << "Now you can use one of the following features: " << endl;
        cout << "1. Display all budget." << endl;
        cout << "2. Search for a specific priority by its name." << endl;
        cout << "3. Display history of last accessed Priorities. " << endl;
        cout << "4. Exit program." << endl;
        cout << "Please enter your option (1-4): " << endl;
        cin >> finalopt;

        if (finalopt == 1) {
            B.DisplayAllBudget(B.head);
        }

        if (finalopt == 2) {
            string nametosearch;
            cout << "Please enter the name you want to search for:" << endl;
            cin >> nametosearch;
            B.SearchDisplaybudget(nametosearch);
        }

        if (finalopt == 3) {
            B.History.display();
        }
    } while (finalopt != 4);
}

int main() {
    int choice;

    do {
        cout << "                    Student Helper Program                    " << endl;
        cout << "1. Budget Manager" << endl;
        cout << "2. Plagiarism Detector" << endl; // Placeholder for future functionality
        cout << "3. Time Manager" << endl;        // Placeholder for future functionality
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                BM_display();
                break;
            case 2:
                cout << "Plagiarism Detector is currently under development." << endl;
                break;
            case 3:
                cout << "Time Manager is currently under development." << endl;
                break;
            case 4:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}