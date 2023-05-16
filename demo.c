#include <stdio.h>
#include "simplecsv.h"

int main() {

    CSVDATA* data = scsv_read_csv("test.csv");

    char** row1 = scsv_get_row(data, 0);
    char** col1 = scsv_get_column(data, 1);

    for(int i = 0; i < data->columns_amount; i++) {

        printf("%s", row1[i]);

    }

    printf("\n");

    for(int i = 0; i < data->rows_amount; i++) {

        printf("%s", col1[i]);

    }

    return 0;

}