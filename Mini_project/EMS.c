#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for contact information
typedef struct {
    char phone[12]; // Allow for 11 digits plus '\0'
    char email[51]; // Allow for 50 characters plus '\0'
} ContactInfo;

// Define a structure for an employee
typedef struct {
    int empID;
    char name[51]; // Allow for 50 characters plus '\0'
    double salary;
    ContactInfo contact;
} Employee;

// Function to add an employee
void addEmployee(Employee*** employees, int* numEmployees) {
    Employee newEmployee;
    printf("Enter Employee ID: ");
    scanf("%d", &newEmployee.empID);
    printf("Enter Employee Name: ");
    scanf(" %50s", newEmployee.name);
    printf("Enter Salary: ");
    scanf("%lf", &newEmployee.salary);
    printf("Enter Phone Number: ");
    scanf(" %11s", newEmployee.contact.phone);
    printf("Enter Email: ");
    scanf(" %50s", newEmployee.contact.email);

    // Allocate memory for the new employee
    Employee** temp = (Employee**)realloc(*employees, (*numEmployees + 1) * sizeof(Employee*));
    if (temp == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    *employees = temp;

    // Allocate memory for the new employee record
    (*employees)[*numEmployees] = (Employee*)malloc(sizeof(Employee));
    if ((*employees)[*numEmployees] == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Copy the new employee data into the allocated record
    *(*employees)[*numEmployees] = newEmployee;
    (*numEmployees)++;

    printf("Employee added successfully!\n");
}

// Function to search for an employee by ID
void searchEmployee(const Employee** employees, int numEmployees, int targetID) {
    for (int i = 0; i < numEmployees; i++) {
        if (employees[i]->empID == targetID) {
            printf("Employee found:\n");
            printf("Employee ID: %d\n", employees[i]->empID);
            printf("Name: %s\n", employees[i]->name);
            printf("Salary: %.2f\n", employees[i]->salary);
            printf("Phone: %s\n", employees[i]->contact.phone);
            printf("Email: %s\n", employees[i]->contact.email);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", targetID);
}

// Function to display all employees
void displayEmployees(const Employee** employees, int numEmployees) {
    if (numEmployees == 0) {
        printf("No data found.\n");
        return;
    }

    printf("\nAll Employees:\n");
    for (int i = 0; i < numEmployees; i++) {
        printf("Employee ID: %d\n", employees[i]->empID);
        printf("Name: %s\n", employees[i]->name);
        printf("Salary: %.2f\n", employees[i]->salary);
        printf("Phone: %s\n", employees[i]->contact.phone);
        printf("Email: %s\n", employees[i]->contact.email);
        printf("--------------------------\n");
    }
}

// Function to delete an employee by ID
void deleteEmployee(Employee*** employees, int* numEmployees, int targetID) {
    int found = 0;
    for (int i = 0; i < *numEmployees; i++) {
        if ((*employees)[i]->empID == targetID) {
            // Free the memory for the employee record
            free((*employees)[i]);
            
            // Shift remaining employees to cover the deleted position
            for (int j = i; j < *numEmployees - 1; j++) {
                (*employees)[j] = (*employees)[j + 1];
            }
            (*numEmployees)--;
            found = 1;
            printf("Employee with ID %d deleted.\n", targetID);
            break;
        }
    }
    if (!found) {
        printf("Employee with ID %d not found.\n", targetID);
    }

    // Reallocate memory to adjust the array size
    Employee** temp = (Employee**)realloc(*employees, (*numEmployees) * sizeof(Employee*));
    if (temp == NULL && *numEmployees > 0) {
        printf("Memory reallocation failed.\n");
        exit(1);
    }
    *employees = temp;
}

// Function to free memory allocated for employees
void freeEmployees(Employee*** employees, int* numEmployees) {
    for (int i = 0; i < *numEmployees; i++) {
        free((*employees)[i]);
    }
    free(*employees);
    *employees = NULL;
    *numEmployees = 0;
}

int main() {
    Employee** employees = NULL;
    int numEmployees = 0;
    int choice, empID;

    do {
        printf("\nWelcome to Employee Management System:\n");
        printf("1. Add Employee\n");
        printf("2. Search Employee\n");
        printf("3. Display All Employees\n");
        printf("4. Delete Employee\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEmployee(&employees, &numEmployees);
                break;
            case 2:
                printf("Enter Employee ID to search: ");
                scanf("%d", &empID);
                searchEmployee((const Employee**)employees, numEmployees, empID);
                break;
            case 3:
                displayEmployees((const Employee**)employees, numEmployees);
                break;
            case 4:
                printf("Enter Employee ID to delete: ");
                scanf("%d", &empID);
                deleteEmployee(&employees, &numEmployees, empID);
                break;
            case 5:
                freeEmployees(&employees, &numEmployees);
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
