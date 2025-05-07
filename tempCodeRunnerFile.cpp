#include <iostream>
#include "Plagiarism_detector.cpp"
#include "budget_manager.cpp"
#include "Time_Manager_Main.cpp"

using namespace std;

void displayMainMenu()
{
   cout<< "Student Helper Program\n ";
   cout<< "1. Budget Manager\n ";
   cout<< "2. Plagiarism Detector\n ";
   cout<< "3. Time Manager\n";
   cout<< "4. Exit\n ";
   cout<< "Enter your choice: ";
}

int main()
{
    int choice;
    do
    {
        displayMainMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                BM_display();  // Function from budget_manager.cpp
                break;
            case 2:
                PD_menu();  // Function from Plagiarism_detector.cpp
                break;
            case 3:
                TM_menu();  // Function from Time_Manager_Main.cpp
                break;
            case 4:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
