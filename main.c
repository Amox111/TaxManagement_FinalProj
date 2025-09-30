#include<stdio.h>
#include<string.h>

#define MAX_RECORDS 100
//unit test and end to end test

typedef struct{
    char paymentID[10];
    char payerName[30];
    char taxType[20];
    double amount;
    char paymentDate[11]; //yyy-mm-dd
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
        fprintf(file,"%S,%S,%S,%.2f,%s\n", records[i].paymentID, records[i].payerName, records[i].taxType, records[i].amount, records[i].paymentDate);
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
    fclose(file);
}

//Add
void addRecord(){
    if (recordCount >= MAX_RECORDS) {
        printf("\nMaximum numbers of ID is reached\n");
        return;
    }
    paymentRecord newRecord; 
    printf("=====Add the information=====\n");
    printf("Type paymentID(by follow this format -Aa001-):");
    scanf("%s", newRecord.paymentID);
    printf("Type payer name:");
    scanf("%s", newRecord.payerName);
    printf("Type tax type:");
    scanf("%s", newRecord.taxType);
    printf("Type the amount:");
    scanf("%lf", newRecord.amount); 
    printf("Type the date(by follow this format yyyy-mm-dd):"); //might add more(prevent from wrong format)
    scanf("%s", newRecord.paymentDate); 

    //add struct to new record
    records[recordCount++]=newRecord;
    printf("\nAdded successfully\n\n");
    saveCSV();

}

//Update
void updateRecord(){


}

//Delete
void deleteRecord(){


}

//Search
void searchRecord(){


}






//Display Menu
void displayMenu(){
    printf("\n      Tax Management System\n");
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
        if (scanf("%d\n", &choice) !=1){
            while (getchar() !='\n')
            choice = -1;
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
                printf("ARE YOU DUMD?!?! CHOOSE between 1-7\n");
                break;
        }
    } while (choice !=0);

    return 0;
}

 
