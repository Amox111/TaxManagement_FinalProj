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
        printf("Cannot open %s file\n", FILENAME);
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

//add
void addRecord() {
    if (recordCount >= MAX_RECORDS) { printf("Maximum records reached!\n"); return; }

    printf("Enter Payment ID: ");
    scanf("%s", records[recordCount].paymentID);
    while (getchar() != '\n'); //clear input buffer

    printf("Enter Payer Name: ");
    scanf("%[^\n]", records[recordCount].payerName);
    while (getchar() != '\n'); 

    printf("Enter Tax Type: ");
    scanf("%[^\n]", records[recordCount].taxType); 
    while (getchar() != '\n'); 

    printf("Enter Amount: ");
    scanf("%lf", &records[recordCount].amount);
    while (getchar() != '\n');

    printf("Enter Date (yyyy-mm-dd): ");
    scanf("%s", records[recordCount].paymentDate);
    while (getchar() != '\n'); 

    recordCount++;
    saveCSV();
    printf("Record added!\n");
}

//update
void updateRecord() {
    char searchID[STRING_SIZE];
    printf("Enter Payment ID to update: ");
    scanf("%s", searchID);
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].paymentID, searchID) == 0) {
            printf("Enter new Amount: ");
            scanf("%lf", &records[i].amount);
            printf("Enter new Date (yyyy-mm-dd): ");
            scanf("%s", records[i].paymentDate);
            saveCSV();
            printf("Record updated!\n");
            return;
        }
    }
    printf("Record not found!\n");
}

void print_record(const Payment *rec) {
    printf("Found: %s, %s, %s, %.2f, %s \n",
           rec->paymentID, rec->payerName, rec->taxType, rec->amount, rec->paymentDate);
}

//Delete
void deleteRecord(){
    char searchID[STRING_SIZE];
    int foundID = -1;
    
    while (getchar() != '\n'); 
    printf("\n====== Delete the information ======\n");
    printf("Type the PaymentID that you want to delete: ");
    scanf("%s", searchID);

    for (int i = 0; i < recordCount; i++){
        if (strcmp(records[i].paymentID, searchID) == 0){
            foundID = i;
            break;
        }
    }
    
    if (foundID != -1){
        printf("\nConfirm deletion? (Type 'YES' to proceed): ");
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
    printf("Enter payment ID or payer name to search:: ");

    while (getchar() != '\n'); 
    scanf("%[^\n]", searchTerm);

    for (int i = 0; i < recordCount; i++) {
        int match = 0;
        if (strcmp(records[i].paymentID, searchTerm) == 0) {
            match = 1;
        } 
        if (!match && strstr(records[i].payerName, searchTerm) != NULL) {
            match = 1;
        }
        if (match) {
            print_record(&records[i]);
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("No records found");
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
            choice = -1; //กำหนดค่าไม่ถูกต้องเรียก default case
        }

        switch (choice){
            case 1:
                addRecord();
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