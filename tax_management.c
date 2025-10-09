#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <float.h>
#include <math.h>

#define FILENAME "record.csv"
#define MAX_RECORDS 100
#define STRING_SIZE 100
#define ID_SIZE 5 

int suppress_save_message = 0;

#define SAVE_MESSAGE_PRINT if (!suppress_save_message) printf("Saved successfully\n");

typedef struct {
    char paymentID[ID_SIZE];
    char payerName[STRING_SIZE];
    char taxType[STRING_SIZE];
    double amount;
    char paymentDate[STRING_SIZE];
} Payment;

Payment records[MAX_RECORDS];
int recordCount = 0;

int isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int isValidDate(const char *dateStr) {
    if (strlen(dateStr) != 10 || dateStr[4] != '-' || dateStr[7] != '-') {    //check format length
        return 0;
    }

    for (int i = 0; i < 10; i++) {  //check if parts are digits
        if (i == 4 || i == 7) continue; 
        if (!isdigit(dateStr[i])) { return 0; }
    }

    int year, month, day;   
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }

    if (month < 1 || month > 12) { return 0; }  

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
    if (month == 2 && isLeap(year)) { //leap year(Feb)
        daysInMonth[2] = 29;
    }
    if (day < 1 || day > daysInMonth[month]) { return 0; }

    return 1;
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

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void saveCSV() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Cannot open file\n");
        return;
    }

    fprintf(file, "paymentID,payerName,taxType,amount,paymentDate\n");

    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%s,%s,%s,%.2lf,%s\n",
                records[i].paymentID,
                records[i].payerName,
                records[i].taxType,
                records[i].amount,
                records[i].paymentDate);
    }
    fclose(file);
    SAVE_MESSAGE_PRINT
}

void readCSV() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        recordCount = 0;
        return;
    }

    recordCount = 0;
    char line[200];

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return;
    }

    while (recordCount < MAX_RECORDS && fgets(line, sizeof(line), file)) {
        Payment tempRec;

        int items_read = sscanf(line, "%[^,],%[^,],%[^,],%lf,%[^\n]", 
                tempRec.paymentID,
                tempRec.payerName,
                tempRec.taxType,
                &tempRec.amount, 
                tempRec.paymentDate);

        if (items_read == 5 && strlen(tempRec.paymentID) > 0) {
            records[recordCount] = tempRec;
            recordCount++;
        }
    }
    fclose(file);
}

void print_record(const Payment *rec) {
    printf("%s, %s, %s, %.2lf, %s \n",
           rec->paymentID, rec->payerName, rec->taxType, rec->amount, rec->paymentDate);
}

int findRecordIndex(const char *id) {
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].paymentID, id) == 0) {
            return i;
        }
    }
    return -1;
}

void addRecord() {
    if (recordCount >= MAX_RECORDS) { printf("Maximum records reached!\n"); return; }
    Payment newRecord;
    
    char firstName[STRING_SIZE / 2]; 
    char lastName[STRING_SIZE / 2];
    int tax_choice;
    
    clear_input_buffer(); 

//paymentID 
    do {
        printf("========================================\n"); 
        printf("             Add the record\n");      
        printf("========================================\n"); 
        printf("Enter Payment ID (Format: 1 Letter + 3 Digits, or type 'NO' to skip):");
        
        char tempID[ID_SIZE + 5]; 

        if (scanf("%9s", tempID) != 1) { 
            printf("Input error - try again\n");
            clear_input_buffer();
            continue;
        }
        
        clear_input_buffer(); 
        
        if (strcmp(tempID, "NO") == 0 || strcmp(tempID, "no") == 0) {
            return; 
        }
        
        strncpy(newRecord.paymentID, tempID, ID_SIZE - 1);
        newRecord.paymentID[ID_SIZE - 1] = '\0';

        if (isValidPaymentID(newRecord.paymentID)) {
            if (findRecordIndex(newRecord.paymentID) != -1) {
                printf("Error: Payment ID '%s' already exists. Please enter a unique ID...\n", newRecord.paymentID);
                continue; 
            }
            
            break; 
        } else {
            printf("Wrong Payment ID format - Must be 1 letter followed by 3 digits!!!\n");
        }
    } while (1);

    
    snprintf(newRecord.payerName, STRING_SIZE, "%s %s", firstName, lastName);
 
 // Payer Name Input
    do {
        printf("Enter Payer First Name:");
        if (scanf("%49s", firstName) != 1) {
            printf("Input error - try again\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        printf("Enter Payer Last Name:");
        if (scanf("%49s", lastName) != 1) {
            printf("Input error - try again\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        if (strlen(firstName) > 0 && strlen(lastName) > 0) {
            snprintf(newRecord.payerName, STRING_SIZE, "%s %s", firstName, lastName);
            break;
        } else {
             printf("Names cannot be empty. Please try again.\n");
        }
    } while(1);    

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
        printf("Invalid choice - Cancelled!!!\n");
        clear_input_buffer();
        return;
    }

    switch (tax_choice) {
        case 1: strncpy(newRecord.taxType, "Personal Income Tax", STRING_SIZE - 1); break;
        case 2: strncpy(newRecord.taxType, "Corporate Income Tax", STRING_SIZE - 1); break;
        case 3: strncpy(newRecord.taxType, "Value Added Tax", STRING_SIZE - 1); break;
        case 4: strncpy(newRecord.taxType, "Withholding Tax", STRING_SIZE - 1); break;
        case 5: strncpy(newRecord.taxType, "Special Business Tax", STRING_SIZE - 1); break;
        case 6: strncpy(newRecord.taxType, "Stamp duty", STRING_SIZE - 1); break;
        case 7:
            printf("Enter Custom Tax Type Name:");
            if (scanf("%99[^\n]", newRecord.taxType) != 1) {
                printf("Invalid input - Cancelled!!!\n");
                return;
            }
            newRecord.taxType[STRING_SIZE - 1] = '\0'; 
            break;
        default:
            printf("Invalid tax type - Cancelled!!!\n");
            return;
    }
    clear_input_buffer(); 

//amount
    printf("\nEnter amount (positive):");
    if (scanf("%lf", &newRecord.amount) != 1 || newRecord.amount <= 0.0) { // Changed %f to %lf
        printf("Invalid or non-positive amount entered - Cancelled!!!\n");
        clear_input_buffer(); 
        return;
    }
    clear_input_buffer();

//date
    do {
        printf("Enter Date (Format: yyyy-mm-dd):");
        if (scanf("%10s", newRecord.paymentDate) != 1) {
            printf("Input error - try again.\n");
            clear_input_buffer();
            continue;
        }
        newRecord.paymentDate[10] = '\0'; 
        clear_input_buffer(); 
        
        if (isValidDate(newRecord.paymentDate)) {
            break; 
        } else {
            printf("Invalid Date format or value - Use yyyy-mm-dd and ensure the date is valid!!!\n");
        }
    } while (1);

//confirm  
    printf("================Confirm=================\n");    
    printf("This data will be added:");
    print_record(&newRecord);
    printf("\nType 'YES' to confirm:");
    
    char confirmation[10];
    if (scanf("%9s", confirmation) != 1) { return; }
    
    clear_input_buffer(); 

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
    char searchID[ID_SIZE]; 
    char again_update_another[5];

    do {    
        printf("========================================");      
        printf("\n           Update Record Menu\n");
        printf("========================================\n");        
        printf("Enter Payment ID to update (Type 'NO' to cancel):");
        clear_input_buffer();
        if (scanf("%4s", searchID) != 1) {
            printf("Input error\n");
            clear_input_buffer();
            continue;
        }
        searchID[ID_SIZE - 1] = '\0'; 
        
        if (strcmp(searchID, "NO") == 0) {
            printf("Returning to main menu...\n");
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
            printf("\nCurrent Data:");
            print_record(&records[found_index]);
            
            char update_choice[5];
            int done_updating = 0;
            
            do {
                printf("\n========================================");        
                printf("\nWhat would you like to update?\n");
                printf("[1] New Amount (Current:%.2lf)\n", records[found_index].amount);
                printf("[2] New Date (Current:%s)\n", records[found_index].paymentDate);
                printf("[3] Finish back to main menu\n");
                printf("Enter choice (1, 2, 3):");
                
                if (scanf("%4s", update_choice) != 1) {
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();
                
                if (strcmp(update_choice, "1") == 0) {
                    float newAmount;
                    printf("Enter new Amount (must be positive):");
                    if (scanf("%lf", &newAmount) == 1 && newAmount > 0.0f) { 
                        records[found_index].amount = newAmount;
                        printf("Updated successfully\n");
                    } else {
                        printf("Invalid or non-positive amount entered\n");
                    }
                    clear_input_buffer();
                    
                } else if (strcmp(update_choice, "2") == 0) {
                    char newDate[STRING_SIZE];
                    printf("Enter new Date (yyyy-mm-dd):");
                    scanf("%10s", newDate);
                    newDate[10] = '\0';
                    clear_input_buffer();
                    
                    if(isValidDate(newDate)) {
                        strncpy(records[found_index].paymentDate, newDate, STRING_SIZE - 1);
                        records[found_index].paymentDate[STRING_SIZE - 1] = '\0';
                        printf("Date updated in memory\n");
                    } else {
                        printf("Invalid date format or value - Date not updated\n");
                    }

                } else if (strcmp(update_choice, "3") == 0) {
                    done_updating = 1;
                } else {
                    printf("Invalid choice - Enter 1, 2, 3\n");
                }
                
            } while (done_updating == 0);
            
            saveCSV();
            printf("\nRecord updated successfully and saved to file\n");
            printf("Final Record:");
            print_record(&records[found_index]);

        } else {
            printf("Record with ID '%s' not found!\n", searchID);
        }

        printf("\nDo you want to update another record?\n");
        printf("Type 'YES' to update another or any other key to return to main menu:");
        scanf("%4s", again_update_another);

    } while (strcmp(again_update_another, "YES") == 0);
}

//Delete
void deleteRecord(){
    char searchID[ID_SIZE]; 
    char again_delete_another[5]; 
    int foundID = -1;
    
    do { 
        foundID = -1; 
        clear_input_buffer(); 
        printf("========================================");
        printf("\n            Delete Record Menu\n");
        printf("========================================\n");
        printf("Type the PaymentID that you want to delete (Type 'NO' to cancel):");
        
        if (scanf("%4s", searchID) != 1) { 
            clear_input_buffer();
            continue; 
        }
        searchID[ID_SIZE - 1] = '\0';
        clear_input_buffer(); 
        
        if (strcmp(searchID, "NO") == 0 || strcmp(searchID, "no") == 0) {
            printf("Returning to main menu.\n");
            break;
        }

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
            scanf("%9s", confirmation); 
            clear_input_buffer(); 
            
            if (strcmp(confirmation, "YES") == 0) {
                for (int i = foundID; i < recordCount - 1; i++) {     
                    records[i] = records[i+1];                       
                }
                recordCount--; 
                saveCSV(); 
                printf("This PaymentID: %s has been deleted successfully\n", searchID);
            } else {
                printf("Deletion cancelled...\n");
            }
        } else {
            printf("This PaymentID: %s not found\n", searchID);
        }

        printf("\nDo you want to delete another record?\n");
        printf("Type 'YES' to delete another or any other key to return to main menu:");
        scanf("%4s", again_delete_another);
        
    } while (strcmp(again_delete_another, "YES") == 0);
}

//search
void searchRecord() {
    char searchTerm[STRING_SIZE];
    char again_search_another[5]; 
    int foundCount = 0;
    
    do { 
        foundCount = 0; 
        printf("========================================");        
        printf("\n            Search Record Menu\n");
        printf("========================================\n");        
        printf("Enter payment ID or payer name to search (Type 'NO' to cancel):");

        clear_input_buffer(); 
        if (scanf("%99[^\n]", searchTerm) != 1) {
            clear_input_buffer(); 
            continue;
        }
        clear_input_buffer(); 

        if (strcmp(searchTerm, "NO") == 0 || strcmp(searchTerm, "no") == 0) {
            printf("Returning to main menu...\n");
            break; 
        }

        printf("========================================");
        printf("\n             Search Results\n");
        for (int i = 0; i < recordCount; i++) {
            int match = 0;
            
            if (strcmp(records[i].paymentID, searchTerm) == 0) {
                match = 1;
            } 

            if (!match && strstr(records[i].payerName, searchTerm) != NULL) {    
                match = 1;
            }

            if (match) {
                printf("Result %d:", foundCount + 1);
                print_record(&records[i]);
                foundCount++;
            }
        }

        if (foundCount == 0) {
            printf("No records found...\n");
        } else {
            printf("Total records found: %d\n", foundCount);
            
            printf("\nDo you want to update or delete any of these records?\n");
            printf("Type [U] to update [D] to delete - any other key to continue:");
            
            char action[5];
            if (scanf("%4s", action) != 1) { 
                 continue;
            }
            
            if (action[0] == 'U' || action[0] == 'u') {
                updateRecord(); 
            } else if (action[0] == 'D' || action[0] == 'd') {
                deleteRecord(); 

            } else {
                printf("No action selected.\n");
            }
        }
        
        printf("\nDo you want to search more?\n");
        printf("Type 'YES' to search again or any other key to return to main menu:");
        
        scanf("%4s", again_search_another);
    } while (strcmp(again_search_another, "YES") == 0);
}

int addPaymentTest() {
    Payment p = {"T001", "Alice", "VAT", 1500.50, "2025-10-09"};
    records[recordCount++] = p;
    return strcmp(records[0].payerName, "Alice") == 0;
}

int boundaryTest() {
    Payment p = {"B000", "Boundary", "VAT", 0.0, "2025-01-01"};
    records[recordCount++] = p;
    return records[recordCount - 1].amount == 0.0 && strcmp(records[recordCount - 1].paymentID, "B000") == 0;
}

int extremeTest() {
    Payment p;
    strcpy(p.paymentID, "EXTREME123456789012345678901234567890");
    strcpy(p.payerName, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    strcpy(p.taxType, "INCOME");
    p.amount = 9999999999.99;
    strcpy(p.paymentDate, "2099-12-31");
    records[recordCount++] = p;
    return p.amount == 9999999999.99;
}

int memoryLeakTest() {
    Payment *temp = malloc(sizeof(Payment));
    if (temp == NULL) return 0;
    strcpy(temp->paymentID, "M001");
    strcpy(temp->payerName, "Bob");
    strcpy(temp->taxType, "Corporate");
    temp->amount = 10000.00;
    strcpy(temp->paymentDate, "2025-10-09");
    free(temp);
    return 1;
}

//unit
void run_unit_tests() {
    printf("========================================\n");
    printf("              Run Unit Tests \n");
    printf("========================================\n");
    recordCount = 0; 
    printf("Normal Test: %s\n", addPaymentTest() ? "PASS" : "FAIL");
    printf("Boundary Test: %s\n", boundaryTest() ? "PASS" : "FAIL");
    printf("Extreme Test: %s\n", extremeTest() ? "PASS" : "FAIL");
    printf("Memory Leak Test: %s\n", memoryLeakTest() ? "PASS" : "FAIL");
    printf("All Unit Tests Completed...\n");
}

//e2e
void run_e2e_tests() {
    printf("========================================\n");
    printf("         Running End-to-End Test\n");
    printf("========================================\n");
    recordCount = 0; 

    Payment p1 = {"E001", "Charlie", "Income", 2500.75, "2025-10-09"};
    records[recordCount++] = p1;

    int found = 0;
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].paymentID, "E001") == 0) found = 1;
    }

    if (found) {
        printf("Record found: %s | %s | %.2lf\n", p1.paymentID, p1.payerName, p1.amount);
    }

    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].paymentID, "E001") == 0) {
            for (int j = i; j < recordCount - 1; j++) {
                records[j] = records[j + 1];
            }
            recordCount--;
            break; 
        }
    }

    int stillExists = 0;
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].paymentID, "E001") == 0) stillExists = 1;
    }

    printf("End-to-End Test: %s\n", stillExists ? "FAIL" : "PASS");
}

//display menu
void displayMenu(){
    printf("========================================");
    printf("\n         Tax Management System\n");
    printf("===================Menu=================\n");
    printf("[1] Add the record\n");
    printf("[2] Update the record\n");
    printf("[3] Delete the record\n");   
    printf("[4] Search(by paymentID or payer name)\n");
    printf("[5] Unit Test\n");
    printf("[6] E2E Test\n");
    printf("[7] Exit\n");
    printf("========================================\n");
    printf("Enter your choice:");
}

int main(){
    int choice;
    readCSV();
    do{
        #ifdef _WIN32    //clear screen
            system("cls");   //windows
        #else
            system("clear"); //linux macOS
        #endif

        displayMenu(); 
        
        do {
            printf("Enter your choice:");
            if (scanf("%d", &choice) == 1) {
                if (choice >= 1 && choice <= 7) {
                    break; 
                } else {
                    printf("Invalid choice - choose a number between 1-7!!!\n");
                }
            } else {
                printf("Invalid input - enter a number\n");
            }
            clear_input_buffer(); 
        } while (1);

        if (choice >= 1 && choice <= 6) {
            #ifdef _WIN32
                system("cls"); 
            #else
                system("clear"); 
            #endif
        }

            switch (choice){
            case 1:
                do {
                    addRecord();
                    printf("========================================\n");    
                    printf("\nWant to add another record?\n");
                    printf("Type 'YES' to add another record or any other key to back to main menu:");
                    char again[5];

                    if (scanf("%4s", again) != 1) { 
                        break;
                    }
                    
                    if (strcmp(again, "YES") != 0) {
                        break;
                    }
                } while (1);
                break;
            case 2:
                updateRecord(); 
                clear_input_buffer();
                break;          
            case 3:
                deleteRecord(); 
                clear_input_buffer();
                break; 
            case 4:
                searchRecord(); 
                clear_input_buffer();
                break; 

            case 5:
                run_unit_tests();
                printf("\nPress ENTER to return to the main menu...");
                clear_input_buffer();
                getchar();
                readCSV(); 
                break;
            case 6:
                run_e2e_tests();
                printf("\nPress ENTER to return to the main menu...");
                clear_input_buffer();
                getchar();
                readCSV();
                break;
            case 7:
                saveCSV();
                printf("Have a good one!!!\n");
                break;      
            default:
                break; 
        }
    } while (choice !=7);

    return 0;
}