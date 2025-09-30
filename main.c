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
    printf("Saved successfully");
}

void readCSV(){
    FILE *file = fopen("record.csv", "r");
    if (file == NULL) {
        printf("Can not open file\n");
        return;
    }
    fclose(file);
}

//add
void addRecord(){
    if (countID > 100) {
        printf("\nEnter name: ");
        scanf(" %[^\n]s", I);


    }
    else {
        printf("\nMaximum numbers of ID is reached\n");
    }

}


void search(){


}



//Display Menu
void displayMenu(){
    printf("      Tax Management System\n");
    printf("===============Menu===============\n");
    printf("[1] Add the information\n");
    printf("[2] Update the information\n");
    printf("[3] Delete the data\n");  
    printf("[4] Search(by paymentID)\n");
    printf("[5] Unit test\n");
    printf("[6] End to end test\n");
    printf("[7] Exit\n");
    printf("==================================\n");
    printf("Enter your choice:\n");
    scanf(%d, );
}

int main(){

    return 0;
}

 
