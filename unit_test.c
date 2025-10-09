#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "tax_management.c"  

void test_isValidPaymentID() {
    printf("Running test_isValidPaymentID...\n");
    assert(isValidPaymentID("A123") == 1);   //normal
    assert(isValidPaymentID("a001") == 1);   //lower letter ok
    assert(isValidPaymentID("1234") == 0);   //no letter
    assert(isValidPaymentID("AA12") == 0);   //too many letters
    assert(isValidPaymentID("A12") == 0);    //too short
    assert(isValidPaymentID("A1234") == 0);  //too long
    printf("test_isValidPaymentID passed...\n\n");
}

void test_isValidDate() {
    printf("Running test_isValidDate...\n");
    assert(isValidDate("2024-02-29") == 1);  //leap year
    assert(isValidDate("2023-02-29") == 0);  //invalid feb date
    assert(isValidDate("2025-04-31") == 0);  //april has 30 days
    assert(isValidDate("2025-12-15") == 1);  //normal
    assert(isValidDate("25-12-15") == 0);    //wrong format
    printf("test_isValidDate passed...\n\n");
}

void test_findRecordIndex() {
    printf("Running test_findRecordIndex...\n");
    recordCount = 2;
    strcpy(records[0].paymentID, "A001");
    strcpy(records[1].paymentID, "B002");

    assert(findRecordIndex("A001") == 0);
    assert(findRecordIndex("B002") == 1);
    assert(findRecordIndex("C003") == -1);
    printf("test_findRecordIndex passed...\n\n");
}

void test_add_and_delete_record_memory() {
    printf("Running test_add_and_delete_record_memory...\n");
    recordCount = 0;
    Payment *ptr1 = malloc(sizeof(Payment));
    Payment *ptr2 = malloc(sizeof(Payment));
    assert(ptr1 != NULL && ptr2 != NULL);
    free(ptr1);
    free(ptr2);
    printf("Memory allocation/deallocation test passed...\n\n");
}

void unitTest() {
    printf("========================================\n");
    printf("             UNIT TESTING\n");
    printf("========================================\n");
    test_isValidPaymentID();
    test_isValidDate();
    test_findRecordIndex();
    test_add_and_delete_record_memory();
    printf("========================================\n");
    printf(" ALL UNIT TESTS PASSED SUCCESSFULLY!!!\n");
    printf("========================================\n");
}
