#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "record.csv"
#define MAX_RECORDS 100
#define STRING_SIZE 100
//unit test and end to end test

typedef struct {
    char paymentID[STRING_SIZE];
    char payerName[STRING_SIZE];
    char taxType[STRING_SIZE];
    double amount;
    char paymentDate[STRING_SIZE];
} Payment;

Payment records[MAX_RECORDS];
int recordCount = 0;

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

//add
void addRecord() {
    if (recordCount >= MAX_RECORDS) { printf("Maximum records reached!\n"); return; }

    Payment newRecord;

    printf("Enter Payment ID:");
    scanf("%s", newRecord.paymentID);
    while (getchar() != '\n'); 

    printf("Enter Payer Name:");
    scanf("%[^\n]", newRecord.payerName);
    while (getchar() != '\n'); 

    printf("Enter Tax Type:");
    scanf("%[^\n]", newRecord.taxType); 
    while (getchar() != '\n'); 

    printf("Enter Amount:");
    if (scanf("%lf", &newRecord.amount) != 1) {
        printf("Invalid amount entered. Addition cancelled.\n");
        while (getchar() != '\n'); 
        return;
    }
    while (getchar() != '\n');

    printf("Enter Date (yyyy-mm-dd):");
    scanf("%s", newRecord.paymentDate);
    while (getchar() != '\n'); 
    
    //Confirm
    printf("This data will be added:");
    print_record(&newRecord);
    printf("Type YES to confirm: ");
    
    char confirmation[10];
    scanf("%s", confirmation);
    while (getchar() != '\n'); 

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

//Display Menu
void displayMenu(){
    printf("\n      Tax Management System\n");
    printf("===============Menu===============\n");
    printf("[1] Add the record\n");
    printf("[2] Update the record\n");
    printf("[3] Delete the record\n");  
    printf("[4] Search(by paymentID or payer name)\n");
    printf("[5] \n");
    printf("[6] \n");
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
                //
                break;           
            case 6:
                //
                break; 
            case 7:
                printf("Have a nice day!\n");
                break;             
            default:
                printf("CHOOSE between 1-7\n");
                break;
        }
    } while (choice !=7);

    return 0;
}