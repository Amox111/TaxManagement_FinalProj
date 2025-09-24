#include<stdio.h>
#include<string.h>

//Structure 
    char paymentID[10];
    char payerName[30];
    char taxType[20];
    int amount;
    char paymentDate[11];

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

void readCSV(){
    FILE *file = fopen("record.csv", "r");
    if (file == NULL) {
        printf("Can not open file\n");
        return;
    }
/*  while (fscanf(file,"%S,%s,%s,%d,%d\n",paymentID,payerName,taxType,&amount,paymentData) !== EOF)
    fclose(file);
    return;
}*/
}

void writeCSV(){
    FILE *file = fopen("record.csv", "w");
    if (file == NULL) {
    printf("Can not open file\n");
    return;
    }    
    fclose(file);

}

//add
void addRecord(){
    int paymentID, payerName;
    printf("Type the paymentID: \n");
    scanf("%s", paymentID);
    printf("Type the payer's name: \n");
    scanf("%s", payerName);

}


void search(){


}



int main(){
    readCSV();


        displayMenu();
        addRecord();





    return 0;
}

 
