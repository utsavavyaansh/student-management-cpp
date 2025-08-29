#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <sstream>

struct Student {
    int roll{};
    std::string name;
    double marks{};
};

const std::string DB_FILE = "students.csv";

int findIndexByRoll(const std::vector<Student>& v, int roll) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i].roll == roll) return static_cast<int>(i);
    }
    return -1;
}

bool readInt(const std::string& prompt, int& out) {
    std::cout << prompt;
    while (true) {
        if (std::cin >> out) return true;
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

bool readDouble(const std::string& prompt, double& out) {
    std::cout << prompt;
    while (true) {
        if (std::cin >> out) return true;
        std::cout << "Invalid input. Please enter a valid number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void pressEnterToContinue() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void saveToFile(const std::vector<Student>& v) {
    std::ofstream out(DB_FILE, std::ios::trunc);
    if (!out) {
        std::cout << "Error: could not open database file for writing.\n";
        return;
    }
    // CSV Header
    out << "roll,name,marks\n";
    for (const auto& s : v) {
        // Note: keep names without commas to keep CSV simple
        out << s.roll << "," << s.name << "," << s.marks << "\n";
    }
}

std::vector<Student> loadFromFile() {
    std::vector<Student> v;
    std::ifstream in(DB_FILE);
    if (!in) {
        // No file yet, that's fine.
        return v;
    }
    std::string line;
    bool headerSkipped = false;
    while (std::getline(in, line)) {
        if (!headerSkipped) { headerSkipped = true; continue; } // skip header
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string rollStr, name, marksStr;
        if (!std::getline(ss, rollStr, ',')) continue;
        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, marksStr, ',')) continue;
        Student s{};
        try {
            s.roll = std::stoi(rollStr);
            s.name = name;
            s.marks = std::stod(marksStr);
            v.push_back(s);
        } catch (...) {
            // skip malformed line
        }
    }
    return v;
}

void displayAll(const std::vector<Student>& v) {
    if (v.empty()) {
        std::cout << "No students found.\n";
        return;
    }
    std::cout << "\n"
              << std::left << std::setw(10) << "Roll"
              << std::left << std::setw(25) << "Name"
              << std::left << std::setw(10) << "Marks"
              << "\n";
    std::cout << std::string(45, '-') << "\n";
    for (const auto& s : v) {
        std::cout << std::left << std::setw(10) << s.roll
                  << std::left << std::setw(25) << s.name
                  << std::left << std::setw(10) << s.marks
                  << "\n";
    }
}

void addStudent(std::vector<Student>& v) {
    Student s{};
    int roll;
    if (!readInt("Enter roll number: ", roll)) return;
    if (findIndexByRoll(v, roll) != -1) {
        std::cout << "Error: roll number already exists.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter name (no commas): ";
    std::getline(std::cin, s.name);
    double marks;
    if (!readDouble("Enter marks (0-100): ", marks)) return;
    if (marks < 0 || marks > 100) {
        std::cout << "Error: marks must be between 0 and 100.\n";
        return;
    }
    s.roll = roll;
    s.marks = marks;
    v.push_back(s);
    std::cout << "Student added successfully.\n";
}

void searchStudent(const std::vector<Student>& v) {
    int roll;
    if (!readInt("Enter roll number to search: ", roll)) return;
    int idx = findIndexByRoll(v, roll);
    if (idx == -1) {
        std::cout << "No student found with roll " << roll << ".\n";
        return;
    }
    const auto& s = v[idx];
    std::cout << "Found: Roll=" << s.roll << ", Name=" << s.name << ", Marks=" << s.marks << "\n";
}

void updateMarks(std::vector<Student>& v) {
    int roll;
    if (!readInt("Enter roll number to update: ", roll)) return;
    int idx = findIndexByRoll(v, roll);
    if (idx == -1) {
        std::cout << "No student found with roll " << roll << ".\n";
        return;
    }
    double marks;
    if (!readDouble("Enter new marks (0-100): ", marks)) return;
    if (marks < 0 || marks > 100) {
        std::cout << "Error: marks must be between 0 and 100.\n";
        return;
    }
    v[idx].marks = marks;
    std::cout << "Marks updated.\n";
}

void deleteStudent(std::vector<Student>& v) {
    int roll;
    if (!readInt("Enter roll number to delete: ", roll)) return;
    int idx = findIndexByRoll(v, roll);
    if (idx == -1) {
        std::cout << "No student found with roll " << roll << ".\n";
        return;
    }
    v.erase(v.begin() + idx);
    std::cout << "Student deleted.\n";
}

void menu() {
    std::vector<Student> students = loadFromFile();
    while (true) {
        std::cout << "\n===== Student Management System (C++) =====\n"
                  << "1) Add Student\n"
                  << "2) Display All\n"
                  << "3) Search by Roll\n"
                  << "4) Update Marks\n"
                  << "5) Delete Student\n"
                  << "6) Save\n"
                  << "7) Exit\n"
                  << "Choose an option (1-7): ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch (choice) {
            case 1: addStudent(students); break;
            case 2: displayAll(students); break;
            case 3: searchStudent(students); break;
            case 4: updateMarks(students); break;
            case 5: deleteStudent(students); break;
            case 6: saveToFile(students); std::cout << "Saved to " << DB_FILE << ".\n"; break;
            case 7: saveToFile(students); std::cout << "Goodbye!\n"; return;
            default: std::cout << "Invalid choice. Try again.\n"; break;
        }
        pressEnterToContinue();
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    menu();
    return 0;
}
