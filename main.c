#include<stdio.h>
#include<string.h> //strcmp
#include<stdlib.h>
#include<ctype.h> //tolower, toupper

#define FILENAME "record.csv"
#define MAX_RECORDS 100
#define STRING_SIZE 100
//unit test and end to end test

typedef struct{
    char paymentID[STRING_SIZE];
    char payerName[STRING_SIZE];
    char taxType[STRING_SIZE];
    double amount;
    char paymentDate[STRING_SIZE]; //yyy-mm-dd
 } paymentRecord ;

 paymentRecord records[MAX_RECORDS];
 int recordCount = 0;

void saveCSV(){
    FILE *file = fopen("record.csv", "w");
    if (file == NULL) {
        printf("Can not open file\n");
        return;
    }   
    for (int i=0; i<recordCount; i++){
        fprintf(file,"%s,%s,%s,%.2f,%s\n", 
            records[i].paymentID, 
            records[i].payerName, 
            records[i].taxType, 
            records[i].amount, 
            records[i].paymentDate);
    }   
    fclose(file);
    printf("Saved successfully\n");
}

void readCSV(){
    FILE *file = fopen("record.csv", "r");
    if (file == NULL) {
        printf("Can not open %S file\n", file);
        recordCount = 0;
        return;
    }
    char line[512];
    recordCount = 0;

    while (fgets(line, sizeof(line), file) != NULL && recordCount < MAX_RECORDS) {
        // ลบอักขระขึ้นบรรทัดใหม่ (\n)
        line[strcspn(line, "\n")] = 0;

        paymentRecord newRecord;
        char *token;
        int fieldIndex = 0;

        // ใช้ strtok เพื่อแยกข้อมูลด้วยคอมมา (',')
        token = strtok(line, ",");
        while(token != NULL) {
            switch(fieldIndex) {
                case 0: // PaymentID
                    strncpy(newRecord.paymentID, token, STRING_SIZE - 1);
                    newRecord.paymentID[STRING_SIZE - 1] = '\0';
                    break;
                case 1: // PayerName
                    strncpy(newRecord.payerName, token, STRING_SIZE - 1);
                    newRecord.payerName[STRING_SIZE - 1] = '\0';
                    break;
                case 2: // TaxType
                    strncpy(newRecord.taxType, token, STRING_SIZE - 1);
                    newRecord.taxType[STRING_SIZE - 1] = '\0';
                    break;
                case 3: // Amount
                    newRecord.amount = atof(token);
                    break;
                case 4: // PaymentDate
                    strncpy(newRecord.paymentDate, token, STRING_SIZE - 1);
                    newRecord.paymentDate[STRING_SIZE - 1] = '\0';
                    break;
            }
            token = strtok(NULL, ",");
            fieldIndex++;
        }
        
        // ตรวจสอบว่ามีข้อมูลครบ 5 คอลัมน์หรือไม่
        if (fieldIndex == 5) {
            records[recordCount++] = newRecord;
        }
    }

    fclose(file);
}





//Add
void addRecord(){
    if (recordCount >= MAX_RECORDS) {
        printf("\nMaximum numbers of ID is reached\n");
        return;
    }
    paymentRecord newRecord; 
    printf("========Add the information=======\n");
    printf("Type paymentID(by follow this format -Aa001-):");
    scanf("%s", newRecord.paymentID);
    printf("Type payer name:");
    scanf(" %[^\n]", newRecord.payerName); 
    printf("Type tax type:");
    scanf("%s", newRecord.taxType);
    printf("Type the amount:");
    scanf("%lf", &newRecord.amount); 
    printf("Type the date(by follow this format yyyy-mm-dd):"); //might add more(prevent from wrong format)
    scanf("%s", newRecord.paymentDate); 

    //add struct to new record
    records[recordCount++]=newRecord;
    printf("\nAdded successfully\n");
    saveCSV();

}

//Update
void updateRecord(){
    char searchID[STRING_SIZE];
    printf("\n======Update the information======\n");
    printf("Type the payment ID that you want to update:");
    scanf("%s", searchID);

    int foundID = -1;
    for (int i = 0; i < recordCount; i++){
        if (strcmp(records[i].paymentID, searchID) == 0){
            foundID = i;
            break;
        }
    }
    
    if (foundID != -1){
        printf("Found the payment ID\n");

        printf("\nAdd new amount:");
        scanf("%lf", &records[foundID].amount); 
        printf("\nAdd new date(yyyy-mm-dd):");
        scanf("%s",records[foundID].paymentDate);   
        
        printf("\nAdded successfully\n");
        saveCSV();
    } else {
        printf("Not found the %s\n",searchID);

    }
    

}

//Delete
void deleteRecord(){


}


void print_record(const paymentRecord *rec) {
    printf("| %-10s | %-20s | %-12s | %10.2f | %-10s |\n",
           rec->paymentID, rec->payerName, rec->taxType, rec->amount, rec->paymentDate);
}


void searchRecord() {
    char searchTerm[STRING_SIZE];
    int foundCount = 0;

    printf("\n--- ค้นหาข้อมูล ---\n");
    printf("ป้อนหมายเลขการชำระ (PaymentID) หรือ ชื่อผู้ชำระ (PayerName) ที่ต้องการค้นหา: ");
    scanf(" %[^\n]", searchTerm);

    printf("\n--- ผลการค้นหาสำหรับ '%s' ---\n", searchTerm);
    printf("--------------------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-12s | %-10s | %-10s |\n", 
           "PaymentID", "PayerName", "TaxType", "Amount", "Date");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < recordCount; i++) {
        // ค้นหาจาก PaymentID หรือ PayerName
        if (strstr(records[i].paymentID, searchTerm) != NULL ||
            strstr(records[i].payerName, searchTerm) != NULL) {
            
            print_record(&records[i]);
            foundCount++;
        }
    }
}


//Display Menu
void displayMenu(){
    printf("      Tax Management System\n");
    printf("===============Menu===============\n");
    printf("[1] Add the information\n");
    printf("[2] Update the information\n");
    printf("[3] Delete the data\n");  
    printf("[4] Search(by paymentID)\n");
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

 
