#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include<assert.h>

#define FILENAME "record.csv"
#define MAX_RECORDS 100
#define STRING_SIZE 100

typedef struct {
    char paymentID[STRING_SIZE];
    char payerName[STRING_SIZE];
    char taxType[STRING_SIZE];
    float amount;
    char paymentDate[STRING_SIZE];
} Payment;

Payment records[MAX_RECORDS];
int recordCount = 0;

int isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int isValidDate(const char *dateStr) {
    if (strlen(dateStr) != 10 || dateStr[4] != '-' || dateStr[7] != '-') {     //check format length
        return 0;
    }

    for (int i = 0; i < 10; i++) {   //check if parts are digits
        if (i == 4 || i == 7) continue; 
        if (!isdigit(dateStr[i])) { return 0; }
    }

    int year, month, day;     //extract YYYY, MM, DD
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }

    if (month < 1 || month > 12) { return 0; }  

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
    
    if (month == 2 && isLeap(year)) { //leap year(Feb)
        daysInMonth[2] = 29;
    }
    if (day < 1 || day > daysInMonth[month]) { return 0; }

    return 1; //date is valid
}

int isValidPaymentID(const char *id) {
    size_t len = strlen(id);
    if (len != 4) { return 0; }
    if (!isalpha(id[0])) { return 0; }
    for (int i = 1; i < 4; i++) {
        if (!isdigit(id[i])) { return 0; }
    }
    return 1;
}

void saveCSV() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Cannot open file\n");
        return;
    }
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%s\n",
                records[i].paymentID,
                records[i].payerName,
                records[i].taxType,
                records[i].amount,
                records[i].paymentDate);
    }
    fclose(file);
    printf("Saved successfully\n");
}

void readCSV() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        recordCount = 0;
        return;
    }
    recordCount = 0;
    char line[200];
    while (recordCount < MAX_RECORDS && fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%lf,%[^\n]",
               records[recordCount].paymentID,
               records[recordCount].payerName,
               records[recordCount].taxType,
               &records[recordCount].amount,
               records[recordCount].paymentDate);
        recordCount++;
    }
    fclose(file);
}

void print_record(const Payment *rec) {
    printf("%s, %s, %s, %.2f, %s \n",
           rec->paymentID, rec->payerName, rec->taxType, rec->amount, rec->paymentDate);
}

void addRecord() {
    if (recordCount >= MAX_RECORDS) { printf("Maximum records reached!\n"); return; }
    Payment newRecord;
    char firstName[STRING_SIZE / 2];
    char lastName[STRING_SIZE / 2];
    int tax_choice;
    
    while (getchar() != '\n' && getchar() != EOF);
//paymentID
    do {
        printf("Enter Payment ID (Format: 1 Letter + 3 Digits, e.g., A001):");
        if (scanf("%s", newRecord.paymentID) != 1) { 
            printf("Input error. Please try again.\n");
            while (getchar() != '\n' && getchar() != EOF);
            continue;
        }
        while (getchar() != '\n' && getchar() != EOF); 

        if (isValidPaymentID(newRecord.paymentID)) {
            break; 
        } else {
            printf("Invalid Payment ID format. Must be 1 letter followed by 3 digits.\n");
        }
    } while (1);

//pyerName
    printf("Enter Payer's First Name:");
    if (scanf("%[^\n]", firstName) != 1) { return; }
    while (getchar() != '\n' && getchar() != EOF); 

    printf("Enter Payer's Last Name:");
    if (scanf("%[^\n]", lastName) != 1) { return; }
    while (getchar() != '\n' && getchar() != EOF); 
    
    strcpy(newRecord.payerName, firstName);
    strcat(newRecord.payerName, " ");
    strcat(newRecord.payerName, lastName);
    
//taxType
    printf("\nSelect Tax Type:\n");
    printf("[1] PIT (Personal Income Tax)\n");
    printf("[2] CIT (Corporate Income Tax)\n");
    printf("[3] VAT (Value Added Tax)\n");
    printf("[4] WHT (Withholding Tax)\n");
    printf("[5] SBT (Special Business Tax)\n");
    printf("[6] SD (Stamp duty)\n");
    printf("[7] Other (Specify Manually)\n");
    printf("Enter choice (1-7):");
    
    if (scanf("%d", &tax_choice) != 1) {
        printf("Invalid choice entered. Addition cancelled.\n");
        while (getchar() != '\n' && getchar() != EOF);
        return;
    }
    while (getchar() != '\n' && getchar() != EOF); 

    switch (tax_choice) {
        case 1: strcpy(newRecord.taxType, "PIT: Personal Income Tax"); break;
        case 2: strcpy(newRecord.taxType, "CIT: Corporate Income Tax"); break;
        case 3: strcpy(newRecord.taxType, "VAT: Value Added Tax"); break;
        case 4: strcpy(newRecord.taxType, "WHT: Withholding Tax"); break;
        case 5: strcpy(newRecord.taxType, "SBT: Special Business Tax"); break;
        case 6: strcpy(newRecord.taxType, "SD: Stamp duty"); break;
        case 7:
            printf("Enter Custom Tax Type Name:");
            if (scanf("%[^\n]", newRecord.taxType) != 1) {
                printf("Invalid input. Addition cancelled.\n");
                return;
            }
            break;
        default:
            printf("Invalid tax type selection. Addition cancelled.\n");
            return;
    }
    while (getchar() != '\n' && getchar() != EOF); 

//amount
    printf("Enter Amount:");
    if (scanf("%f", &newRecord.amount) != 1) {
        printf("Invalid amount entered. Addition cancelled.\n");
        while (getchar() != '\n' && getchar() != EOF); 
        return;
    }
    while (getchar() != '\n' && getchar() != EOF);

//date
    do {
        printf("Enter Date (Format: yyyy-mm-dd):");
        if (scanf("%s", newRecord.paymentDate) != 1) {
            printf("Input error. Please try again.\n");
            while (getchar() != '\n' && getchar() != EOF);
            continue;
        }
        while (getchar() != '\n' && getchar() != EOF); 
        
        if (isValidDate(newRecord.paymentDate)) {
            break; 
        } else {
            printf("Invalid Date format or value. Please use yyyy-mm-dd and ensure the date is valid.\n");
        }
    } while (1);

//confirm
    printf("This data will be added:");
    print_record(&newRecord);
    printf("Type YES to confirm: ");
    
    char confirmation[10];
    if (scanf("%s", confirmation) != 1) { return; }
    while (getchar() != '\n' && getchar() != EOF); 

    if (strcmp(confirmation, "YES") == 0) {
        records[recordCount] = newRecord;
        recordCount++;
        saveCSV();
        printf("Record added successfully\n");
    } else {
        printf("Cancelled\n");
    }
}


//update
void updateRecord() {
    char searchID[STRING_SIZE];
    char again_update_another[5];

    do {
        printf("\n=======Update Record Menu=======\n");
        printf("Enter Payment ID to update (Type 'NO' to cancel):");
        while (getchar() != '\n');
        scanf("%s", searchID);
        
        if (strcmp(searchID, "NO") == 0) {
            printf("Returning to main menu.\n");
            break;
        }

        int found_index = -1;
        for (int i = 0; i < recordCount; i++) {
            if (strcmp(records[i].paymentID, searchID) == 0) {
                found_index = i;
                break;
            }
        }
        
        if (found_index != -1) {
            printf("\nCurrent Data: ");
            print_record(&records[found_index]);
            
            char update_choice[5];
            int done_updating = 0;
            
            do {
                printf("\nWhat would you like to update?\n");
                printf("[1] New Amount (Current: %.2f)\n", records[found_index].amount);
                printf("[2] New Date (Current: %s)\n", records[found_index].paymentDate);
                printf("[3] Finish Updating this Record\n");
                printf("Enter choice (1, 2, or 3): ");
                
                if (scanf("%s", update_choice) != 1) {
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');
                
                if (strcmp(update_choice, "1") == 0) {
                    printf("Enter new Amount: ");
                    if (scanf("%lf", &records[found_index].amount) == 1) {
                        printf("Updated succesfullyll\n");
                    } else {
                        printf("Invalid amount entered\n");
                    }
                    while (getchar() != '\n');
                    
                } else if (strcmp(update_choice, "2") == 0) {
                    printf("Enter new Date (yyyy-mm-dd):");
                    scanf("%s", records[found_index].paymentDate);
                    while (getchar() != '\n');
                    printf("Date updated in memory.\n");

                } else if (strcmp(update_choice, "3") == 0) {
                    done_updating = 1;
                } else {
                    printf("Invalid choice. Please enter 1, 2, or 3.\n");
                }
                
            } while (done_updating == 0);
            
            saveCSV();
            printf("\nRecord updated successfully and saved to file!\n");
            printf("Final Record: ");
            print_record(&records[found_index]);

        } else {
            printf("Record with ID '%s' not found!\n", searchID);
        }

        printf("\nDo you want to update another record?\n");
        printf("Type YES to update another or any other key to return to main menu: ");
        
        scanf("%s", again_update_another);

    } while (strcmp(again_update_another, "YES") == 0);
}

//Delete
void deleteRecord(){
    char searchID[STRING_SIZE];
    int foundID = -1;
    
    while (getchar() != '\n'); 
    printf("Type the PaymentID that you want to delete:");
    scanf("%s", searchID);

    for (int i = 0; i < recordCount; i++){
        if (strcmp(records[i].paymentID, searchID) == 0){
            foundID = i;
            break;
        }
    }
    
    if (foundID != -1){
        printf("\nRecord found: \n");
        print_record(&records[foundID]); 
        printf("\nConfirm deletion? (Type 'YES' to proceed):");
        char confirmation[10];
        scanf("%s", confirmation);

        if (strcmp(confirmation, "YES") == 0) {
            for (int i = foundID; i < recordCount - 1; i++) {     
                records[i] = records[i+1];                           
            }
            recordCount--; 
            saveCSV(); 
            printf("Record with PaymentID %s has been deleted successfully.\n", searchID);
        } else {
            printf("Deletion cancelled.\n");
        }
    } else {
        printf("Record with PaymentID %s not found.\n", searchID);
    }
}

//search
void searchRecord() {
    char searchTerm[STRING_SIZE];
    int foundCount = 0;
    
    printf("Enter payment ID or payer name to search:");

    while (getchar() != '\n'); 
    scanf("%[^\n]", searchTerm);

    printf("\n========Search Results========\n");
    for (int i = 0; i < recordCount; i++) {
        int match = 0;
        if (strcmp(records[i].paymentID, searchTerm) == 0) {
            match = 1;
        } 
        if (!match && strstr(records[i].payerName, searchTerm) != NULL) {
            match = 1;
        }

        if (match) {
            printf("Result %d: ", foundCount + 1);
            print_record(&records[i]);
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("No records found.\n");
    } else {
        printf("Total records found: %d\n", foundCount);
        
        printf("\nDo you want to update or delete any of these records?\n");
        printf("Type [U] to update, [D] to delete, or any other key to continue: ");
        
        char action[5];
        while (getchar() != '\n'); 
        scanf("%s", action);
        
        if (action[0] == 'U' || action[0] == 'u') {
            updateRecord(); 
        } else if (action[0] == 'D' || action[0] == 'd') {
            deleteRecord(); 
        } else {
            printf("Cancelled\n");
        }
    }
}

/*void test_normal(){
    printf("Running the add function\n");
    char paymentID = 'T000';
    char payerName = 'Tester Tester';
    char taxType = 'Wealth tax';
    float amount = 99999;
    char paymentDate = '2023-01-02';

    assert();
    printf("Test add passed\n");

}*/

//Display Menu
void displayMenu(){
    printf("\n      Tax Management System\n");
    printf("===============Menu===============\n");
    printf("[1] Add the record\n");
    printf("[2] Update the record\n");
    printf("[3] Delete the record\n");  
    printf("[4] Search(by paymentID or payer name)\n");
    printf("[5] Unit Test\n");
    printf("[6] E2E Test\n");
    printf("[7] Exit\n");
    printf("==================================\n");
    printf("Enter your choice:");
}

int main(){
    int choice;
    readCSV();
    do{
        displayMenu();
        if (scanf("%d", &choice) != 1){
            while (getchar() != '\n'); 
            choice = -1; 
        }

        switch (choice){
            case 1:
                do {
                    addRecord();
                    printf("\nWant to add another record?\n");
                    printf("Type YES to add another record\n");
                    printf("Type anything to back to main menu\n");
                    char again[5];
                    printf("Enter:");
                    scanf("%s", again);
                    if (strcmp(again, "YES") != 0) {
                        break;
                    }
                } while (1);
                break; 
            case 2:
                updateRecord();
                break;           
            case 3:
                deleteRecord();
                break; 
            case 4:
                searchRecord();
                break; 
            case 5:
                //Unit test
                break;           
            case 6:
                //E2E test
                break; 
            case 7:
                saveCSV();
                printf("Have a nice day!\n");
                break;             
            default:
                printf("CHOOSE between 1-7\n");
                break;
        }
    } while (choice !=7);

    return 0;
}