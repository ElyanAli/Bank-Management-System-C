#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CUSTOMERS 100
#define MAX_TRANSACTIONS 1000

void create_account();
void see_all_customers();
void deposit();
void withdraw();
void delete_account();

typedef struct {
    char type;
    time_t time;
    int account_number;
    double amount;
} Trans;

typedef struct {
    int account_number;
    char name[50];
    double account_balance;
} Customer;

int num_customers = 0;
Customer customers[MAX_CUSTOMERS];

int num_transactions = 0;
Trans transactions[MAX_TRANSACTIONS];

char password[50] = "bank123"; // password for accessing the system

// function to add a new customer
void create_account()
{
// prompt user for customer details
    int account_number; // declare variable for account number
    char name[50]; // declare variable for customer name
    float account_balance; // declare variable for account balance
invalid_account_number: // define a label to be used in goto statement for invalid account number
    printf("Enter account number: ");
    scanf("%d", &account_number); // get account number from user

// check if account number already exists
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].account_number == account_number) {
            printf("Account number already exists. Please enter a unique account number.\n");
            goto invalid_account_number; // if account number already exists, prompt user to enter a unique number using a goto statement to return to the invalid_account_number label
        }
    }

    printf("Enter the name of the Account Holder: ");
    scanf("%s", name); // get customer name from user
    printf("Enter the Starting Balance: ");
    scanf("%f", &account_balance); // get account balance from user

// add customer to array
    customers[num_customers].account_number = account_number; // add account number to customers array
    strcpy(customers[num_customers].name, name); // add customer name to customers array
    customers[num_customers].account_balance = account_balance; // add account balance to customers array

    num_customers++; // increment number of customers by 1
    printf("\nCustomer added successfully.\n"); // print success message

// Add transaction to file
    time_t t = time(NULL); // get current system time
    struct tm *tm = localtime(&t); // convert system time to local time
    char date[20]; // define character array for storing date
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm); // format date and time

    FILE *fp = fopen("transactions.txt", "a"); // open file for appending
    if (fp == NULL) { // check if file was successfully opened
        printf("Error opening file.\n");
        return;
    }

    fprintf(fp, "\n%s: Account %d created with initial balance $%.2f\n", date, account_number, account_balance); // write transaction to file
    fclose(fp); // close the file
}

// function to display all customer details
void see_all_customers()
{
    printf("%-15s %-30s %s\n", "Account Number", "Name", "Balance"); // print headers for account number, name and balance
    for (int i = 0; i < num_customers; i++) { // iterate through each customer in the customers array
        printf("\n%-15d %-30s $%.2lf\n", customers[i].account_number, customers[i].name, customers[i].account_balance); // print the account number, name and balance of each customer in a formatted string
    }
}

// function to deposit money into a customer's account
void deposit()
{
    int account_number; // declare variable for account number
    printf("Enter the account number you want to deposit money in: ");
    scanf("%d", &account_number); // prompt user for account number
    // search for the account number in the customers array
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].account_number == account_number) {
            double amount; // declare variable for amount to deposit
            printf("Enter amount to deposit: ");
            scanf("%lf", &amount); // prompt user for amount to deposit
            customers[i].account_balance += amount; // add the deposit amount to the customer's account balance
            printf("\nNew balance for account %d: $%.2lf\n", account_number, customers[i].account_balance); // print the updated account balance

            // Add transaction to file
            time_t t = time(NULL); // get current time
            struct tm *tm = localtime(&t); // convert time to local time
            char date[20]; // declare character array to store formatted date
            strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm); // format date string

            FILE *fp = fopen("transactions.txt", "a"); // open transactions file in append mode
            if (fp == NULL) {
                printf("Error opening file.\n"); // print an error message if file cannot be opened
                return;
            }

            fprintf(fp, "\n%s: $%.2lf deposited into account %d\n", date, amount, account_number); // write transaction to file
            fprintf(fp, "Customer name: %s\n", customers[i].name); // include customer name in transaction
            fprintf(fp, "New account balance: $%.2lf\n\n", customers[i].account_balance); // include new account balance in transaction

            fclose(fp); // close file

            return; // exit the function
        }
    }
    printf("\nAccount not found.\n"); // if the account number is not found in the customers array, print an error message
}

// function to withdraw money from a customer's account
void withdraw()
{
    int account_number; // declare variable to hold account number entered by the user
    printf("Enter the account number you want to withdraw money from: ");
    scanf("%d", &account_number); // prompt user to enter account number
    // search for the account number in the customers array
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].account_number == account_number) {
            double amount; // declare variable to hold the amount to be withdrawn
            printf("Enter amount to withdraw: ");
            scanf("%lf", &amount); // prompt user to enter withdrawal amount

            if (customers[i].account_balance < amount) { // check if the customer has sufficient funds to withdraw the amount requested
                printf("Insufficient funds.\n"); // if the customer does not have enough funds, print an error message
                return; // exit the function
            }

            customers[i].account_balance -= amount; // subtract the withdrawal amount from the customer's account balance
            printf("\nNew balance for account %d: $%.2lf\n", account_number, customers[i].account_balance); // print the updated account balance

            // Add transaction to file
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            char date[20];
            strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);

            // Open transactions file and write transaction information
            FILE *fp = fopen("transactions.txt", "a");
            if (fp == NULL) { // Check if file was successfully opened
                printf("Error opening file.\n");
                return;
            }

            // Print the withdrawal transaction details to the transactions file
            fprintf(fp, "\n%s: $%.2lf withdrawn from account %d\n", date, amount, account_number);
            fprintf(fp, "Customer name: %s\n", customers[i].name);
            fprintf(fp, "New account balance: $%.2lf\n\n", customers[i].account_balance);

            fclose(fp); // close transactions file

            return; // exit the function
        }
    }
    printf("Account not found.\n"); // if the account number is not found in the customers array, print an error message
}

// function to delete a customer account
void delete_account()
{
    int account_number;
    printf("Enter the account number you want to delete: ");
    scanf("%d", &account_number);
    // search for the account in the customers array
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].account_number == account_number) {
            // shift all the elements to the left by one position to remove the deleted account
            for (int j = i; j < num_customers - 1; j++) {
                customers[j] = customers[j + 1];
            }
            num_customers--;

            // print success message
            printf("\nAccount %d deleted.\n", account_number);

            // Add transaction to file
            time_t t = time(NULL); // get current time
            struct tm *tm = localtime(&t); // convert time to local time
            char date[20];
            strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm); // format time as a string

            FILE *fp = fopen("transactions.txt", "a"); // open file for appending
            if (fp == NULL) {
                printf("Error opening file.\n");
                return;
            }

            fprintf(fp, "%s: Account %d deleted\n", date, account_number); // write transaction to file
            fclose(fp); // close the file

            return;
        }
    }
// if account is not found, print error message
    printf("Account not found.\n");
}

int main()
{
    int choice;
    char input_password[50];
    printf("Enter the password to continue: ");
    scanf("%s", input_password);
    //enter password: "bank123"

    // Check if the entered password matches the expected password
    if (strcmp(password, input_password) != 0) {
        printf("Incorrect password. Exiting program.\n");
        return 0;
    }

    // Display the menu and process user input until the user chooses to exit
    do {
        printf("\n*********** Welcome to DHA Suffa Bank Management System ***********\n");
        printf("1. Add customer\n");
        printf("2. List customers\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Delete account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        // Get the user's choice, and handle invalid input
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n') {
                continue;
            }
        }

        // Process the user's choice by calling the corresponding function
        switch (choice) {
        case 1:
            printf("\nAccount Creation Initiated.\n");
            create_account();
            break;
        case 2:
            printf("\nList of all customers selected.\n");
            see_all_customers();
            break;
        case 3:
            printf("\nDeposit selected.\n");
            deposit();
            break;
        case 4:
            printf("\nWithdraw selected.\n");
            withdraw();
            break;
        case 5:
            printf("\nDelete account selected.\n");
            delete_account();
            break;
        case 6:
            printf("\nExiting program.\n");
            break;
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}