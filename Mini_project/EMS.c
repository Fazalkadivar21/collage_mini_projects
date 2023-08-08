#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMPLOYEES 100

// Define a union to store different contact information
typedef union {
    char phone[11];
    char email[50];
} ContactInfo;

// Define a structure to represent an employee
typedef struct {
    int empID;
    char name[50];
    double salary;
    ContactInfo contact;
} Employee;

// Function to read employee data from the file
int readEmployeesFromFile(Employee* employees, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    int numEmployees = 0;
    while (fread(&employees[numEmployees], sizeof(Employee), 1, file) == 1) {
        numEmployees++;
    }

    fclose(file);
    return numEmployees;
}

// Function to write employee data to the file
void writeEmployeesToFile(const Employee* employees, int numEmployees, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fwrite(employees, sizeof(Employee), numEmployees, file);
    fclose(file);
}

// Function to print employee information
void printEmployee(const Employee* emp) {
    printf("Employee ID: %d\n", emp->empID);
    printf("Name: %s\n", emp->name);
    printf("Salary: %.2f\n", emp->salary);
    printf("Contact: %s\n", emp->contact.phone[0] != '\0' ? emp->contact.phone : emp->contact.email);
    printf("--------------------------\n");
}

// Function to search for an employee by ID
Employee* searchEmployeeByID(Employee* employees, int numEmployees, int targetID) {
    for (int i = 0; i < numEmployees; i++) {
        if (employees[i].empID == targetID) {
            return &employees[i];
        }
    }
    return NULL;
}

// Function to sort employees by ID using bubble sort
void sortEmployeesByID(Employee* employees, int numEmployees) {
    for (int i = 0; i < numEmployees - 1; i++) {
        for (int j = 0; j < numEmployees - i - 1; j++) {
            if (employees[j].empID > employees[j + 1].empID) {
                Employee temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
        }
    }
}

// Function to check if a given string is a valid phone number
int isValidPhoneNumber(const char* phone) {
    // In this example, we are assuming that a valid phone number
    // should contain only digits and must have 10 characters.
    size_t len = strlen(phone);
    if (len != 10) {
        return 0;
    }

    for (size_t i = 0; i < len; i++) {
        if (!isdigit(phone[i])) {
            return 0;
        }
    }

    return 1;
}

// Function to check if a given string is a valid email address
int isValidEmail(const char* email) {
    // In this example, we are assuming that a valid email address
    // should contain '@' character.
    return strchr(email, '@') != NULL;
}

int main() {
    Employee employees[MAX_EMPLOYEES];
    int numEmployees = 0;

    // Read employees' data from the file (if exists)
    numEmployees = readEmployeesFromFile(employees, "employees.dat");

    int choice;
    do {
        printf("Welcome to FN co. EMS : \n");
        printf("1. Add Employee\n");
        printf("2. Search Employee\n");
        printf("3. Sort Employees\n");
        printf("4. Display All Employees\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume the newline character left by scanf

        switch (choice) {
            case 1: {
                if (numEmployees >= MAX_EMPLOYEES) {
                    printf("Maximum number of employees reached.\n");
                } else {
                    Employee newEmployee;
                    printf("Enter Employee ID: ");
                    if (scanf("%d", &newEmployee.empID) != 1) {
                        printf("Invalid input. Please enter a valid employee ID.\n");
                        getchar(); // consume the remaining characters from the buffer
                        break;
                    }
                    getchar(); // consume the newline character left by scanf

                    printf("Enter Employee Name: ");
                    if (fgets(newEmployee.name, sizeof(newEmployee.name), stdin) == NULL) {
                        printf("Error reading input.\n");
                        exit(1);
                    }
                    newEmployee.name[strcspn(newEmployee.name, "\n")] = '\0'; // remove newline character

                    printf("Enter Salary: ");
                    if (scanf("%lf", &newEmployee.salary) != 1) {
                        printf("Invalid input. Please enter a valid salary.\n");
                        getchar(); // consume the remaining characters from the buffer
                        break;
                    }
                    getchar(); // consume the newline character left by scanf

                    printf("Enter Phone Number (or Email): ");
                    if (fgets(newEmployee.contact.phone, sizeof(newEmployee.contact.phone), stdin) == NULL) {
                        printf("Error reading input.\n");
                        exit(1);
                    }
                    newEmployee.contact.phone[strcspn(newEmployee.contact.phone, "\n")] = '\0'; // remove newline character

                    // Validate phone number or email
                    if (!isValidPhoneNumber(newEmployee.contact.phone) && !isValidEmail(newEmployee.contact.phone)) {
                        printf("Invalid phone number or email address. Please enter a valid contact.\n");
                        break;
                    }

                    employees[numEmployees++] = newEmployee;
                    printf("Employee added successfully!\n");
                }
                break;
            }
            case 2: {
                int searchID;
                printf("Enter Employee ID to search: ");
                scanf("%d", &searchID);
                Employee* result = searchEmployeeByID(employees, numEmployees, searchID);
                if (result != NULL) {
                    printf("Employee found:\n");
                    printEmployee(result);
                } else {
                    printf("Employee not found.\n");
                }
                break;
            }
            case 3: {
                sortEmployeesByID(employees, numEmployees);
                printf("Employees sorted by ID.\n");
                break;
            }
            case 4: {
                printf("All Employees:\n");
                for (int i = 0; i < numEmployees; i++) {
                    printEmployee(&employees[i]);
                }
                break;
            }
            case 5: {
                // Write employees' data to the file before exiting
                writeEmployeesToFile(employees, numEmployees, "employees.dat");
                printf("Exiting the program.\n");
                break;
            }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}