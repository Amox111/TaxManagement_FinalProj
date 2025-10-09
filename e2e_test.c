#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tax_management.c"

void E2E_Test() {
    printf("========================================\n");
    printf("                E2E TEST \n");
    printf("========================================\n");

    suppress_save_message = 1;
    recordCount = 0;

    Payment newRec = {"A111", "Alice Test", "Personal Income Tax", 1000.50, "2024-12-12"};
    records[recordCount++] = newRec;
    saveCSV();

    recordCount = 0;
    readCSV();
    assert(recordCount == 1);
    assert(strcmp(records[0].paymentID, "A111") == 0);
    assert(fabs(records[0].amount - 1000.50) < 0.001);

    records[0].amount = 2000.00;
    saveCSV();
    readCSV();
    assert(fabs(records[0].amount - 2000.00) < 0.001);

    recordCount = 1;
    for (int i = 0; i < recordCount - 1; i++) {
        records[i] = records[i + 1];
    }
    recordCount--;
    saveCSV();
    assert(recordCount == 0);

    printf("E2E Test passed successfully!!!\n");
    printf("========================================\n");
}
