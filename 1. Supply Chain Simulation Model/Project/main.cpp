#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm> // For std::isspace

using namespace std;
namespace fs = std::filesystem; // Alias for convenience

// Helper function to trim whitespace from the beginning and end of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

int main() {
    char selection {};
    do {
        cout << "\n---------------" << endl;
        cout << "1. New Project" << endl;
        cout << "2. Open Existing Project" << endl;
        cout << "3. View All Inventories" << endl;
        cout << "Q. Exit/Quit" << endl;
        cout << "\nEnter your selection: ";
        cin >> selection;
        
        if (selection == '1') {
            cout << "You selected to add a new project" << endl;
            
            string newProjectName;
            cout << "What is the project name? ";
            cin >> newProjectName;
            
            // Create a file with the entered project name
            ofstream file(newProjectName + ".txt");
            if (file) {  // Check if file opened successfully
                file << "Project Name: " << newProjectName << endl;
                file << "Inventory:\n";
                cout << "Project saved as '" << newProjectName << ".txt'." << endl;
            } else {
                cout << "Error creating the file." << endl;
            }
            file.close();

        } else if (selection == '2') {
            cout << "Listing all '.txt' project files:" << endl;
            
            bool foundFile = false;
            for (const auto& entry : fs::directory_iterator(fs::current_path())) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    cout << entry.path().filename().string() << endl;
                    foundFile = true;
                }
            }
            
            if (!foundFile) {
                cout << "No '.txt' files found in the current directory." << endl;
            } else {
                // Prompt user to select a file to view or update its inventory
                string fileName;
                cout << "\nEnter the name of the project file you want to open (without '.txt'): ";
                cin >> fileName;
                fileName += ".txt";  // Append the .txt extension
                
                ifstream file(fileName);
                if (file) {
                    cout << "\nContents of " << fileName << ":" << endl;
                    string line;
                    while (getline(file, line)) {
                        cout << line << endl;  // Display each line in the file
                    }
                    file.close();
                    
                    // Prompt for updating inventory levels
                    char updateChoice;
                    cout << "\nWould you like to update the inventory for this project? (y/n): ";
                    cin >> updateChoice;
                    
                    if (updateChoice == 'y' || updateChoice == 'Y') {
                        ofstream fileOut(fileName, ios::app);  // Open in append mode
                        if (fileOut) {
                            string itemName;
                            int quantity;
                            cout << "Enter the item name: ";
                            cin >> itemName;
                            cout << "Enter the quantity: ";
                            cin >> quantity;
                            
                            // Write new inventory data to the file
                            fileOut << itemName << ": " << quantity << endl;
                            cout << "Inventory updated for '" << fileName << "'." << endl;
                        } else {
                            cout << "Error opening the file for writing." << endl;
                        }
                        fileOut.close();
                    }
                    
                } else {
                    cout << "Error: File '" << fileName << "' could not be opened." << endl;
                }
            }

        } else if (selection == '3') {
            cout << "\nViewing all inventories from each project file:\n" << endl;
            
            bool foundInventory = false;
            for (const auto& entry : fs::directory_iterator(fs::current_path())) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    ifstream file(entry.path());
                    if (file) {
                        cout << "Inventory for " << entry.path().filename().string() << ":" << endl;
                        string line;
                        bool inventorySection = false;
                        
                        // Look for "Inventory:" section
                        while (getline(file, line)) {
                            // Trim whitespace to ensure proper detection of "Inventory:" line
                            line = trim(line);
                            if (line == "Inventory:") {
                                inventorySection = true;
                                continue;
                            }
                            if (inventorySection && !line.empty()) {
                                cout << line << endl; // Display inventory items
                                foundInventory = true;
                            }
                        }
                        file.close();
                        cout << "-----------------------------------" << endl;
                    }
                }
            }
            
            if (!foundInventory) {
                cout << "No inventory found in any project files." << endl;
            }

        } else if (selection == 'q' || selection == 'Q') {
            cout << "Goodbye!" << endl;

        } else {
            cout << "Invalid selection! Try again!" << endl;
        }
        
    } while (selection != 'q' && selection != 'Q');
    
    return 0;
}