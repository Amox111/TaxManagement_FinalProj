#include<stdio.h>
#include<string.h>

//Structure - use struct bc contain differnt date type
struct Payment
{
    char paymentID[10];
    char payerName[30];
    char taxType[20];
    int amount;
    char paymentDate[11];
};

//Display Menu
void displayMenu(){
    printf("====Tax Mangenment====\n");
    printf("1.Add the information\n");
    printf("2.Update the information\n");
    printf("3.Delete \n");
    printf("4.Search(by paymentID)\n");
    printf("5.Exit\n");
    printf("Enter your choice:");
}

void loadCSV(){
    FILE *file = fopen("record.csv", "w")
    if (file == NULL) {
        print("Can not open file\n");
        return;
    }
    fscanf(file,"paymentID,payerName,taxType,amount,paymentData");
    




int main(){
    displayMenu();





    return 0;
}

 
