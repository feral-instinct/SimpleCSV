#include <stdio.h>
#include <stdlib.h>
#include "simplecsv.h"

typedef enum scsv_errors {

    ERR_FILE_NOT_FOUND,

} err;

char* cast_error(err error) {

    switch(error) {

        case ERR_FILE_NOT_FOUND:
            return "ERR_FILE_NOT_FOUND";

        default:
            return "ERR_WHAT?";

    }

}

int get_str_len(char* source) {

    char* data = source;

    int len = 0;

    while(*data != '\0') {

        len++;
        data++;
        
    }

    return len;

}

int mycpy(char* source, char* dest, char delim) {

    if(source == NULL || dest == NULL) {

        return -1;

    }

    while(1) {

        if(!source || *source == delim) { break; }

        *dest++ = *source++;

    }

    return 0;

}

void throw_scsv_error(err error) {

    printf("SimpleCsv error: %s\n", cast_error(error));

}

void scsv_parse_csv(CSVDATA* data_container, char* raw_str) {

    char* rs = raw_str;
    char* start = rs;

    int cell_content_len = 0;
    int row = 0;
    int col = 0;

    char* cell_start = rs;

    while(1) {

        if(*rs == ',' || *rs == '\n' || *rs == '\0') {
            
            char delim = *rs;

            data_container->table[row][col] = malloc(sizeof(char) * (cell_content_len + 1));
            data_container->table[row][col][cell_content_len] = '\0';
            mycpy(cell_start, data_container->table[row][col], delim);

            if (delim == '\0') {

                rs = start;
                break;

            }

            rs++;
            cell_start = rs;
            cell_content_len = 0;

            if(delim == '\n') {

                col = 0;
                row++;
                continue;

            } else {

                col++;

            }

            continue;

        }

        cell_content_len++;
        rs++;

    }

}

CSVDATA* scsv_get_rows_cols(char* rawString) {

    int rows = 0;
    int columns = 0;

    char* rs = rawString;
    char* start = rs;

    while(*rs != '\0') {

        if(rows == 0 && *rs == ',')  { columns++; }
        if(*rs == '\n')              { rows++;    }
        rs++;

    }
    rows++;
    columns++;

    rs = start;

    CSVDATA* data = malloc(sizeof(CSVDATA));

    data->rows_amount = rows;
    data->columns_amount = columns;

    data->table = malloc(sizeof(char*) * data->rows_amount);

    for(int i = 0; i < data->rows_amount; i++) {

        data->table[i] = malloc(sizeof(char*) * data->columns_amount);

    }

    return data;

}

CSVDATA* scsv_read_csv(char* path) {

    FILE* csvFile = fopen(path, "r");

    if(csvFile == NULL) {

        throw_scsv_error(ERR_FILE_NOT_FOUND);
        exit(1);

    }

    fseek(csvFile, 0, SEEK_END);
    int fileSize = ftell(csvFile);
    rewind(csvFile);

    char* csvRawString = malloc(sizeof(char) * (fileSize + 1));
    size_t finalSz = fread(csvRawString, sizeof(char), fileSize, csvFile);
    fclose(csvFile);
    csvRawString[finalSz] = '\0';
    
    CSVDATA* data_container = scsv_get_rows_cols(csvRawString);
    scsv_parse_csv(data_container, csvRawString);

    return data_container;

}

char** scsv_get_column(CSVDATA* csv_data, int column) {

    char** new_column = malloc(sizeof(char*) * csv_data->rows_amount);

    for (int i = 0; i < csv_data->rows_amount; i++) {

        new_column[i] = csv_data->table[i][column];

    }

    return new_column;

}

char** scsv_get_row(CSVDATA* csv_data, int row) {

    char** new_row = malloc(sizeof(char*) * csv_data->columns_amount);

    for (int i = 0; i < csv_data->columns_amount; i++) {

        new_row[i] = csv_data->table[row][i];

    }

    return new_row;

}

/* int main() {

    CSVDATA* data = scsv_read_csv("test.csv");

    for(int i = 0; i < data->rows_amount; i++) {

        for(int j = 0; j < data->rows_amount; j++) {

            printf("%s", data->table[i][j]);

        }

    }

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

} */