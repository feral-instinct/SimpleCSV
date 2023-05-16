
typedef struct CsvData {

    char*** table;
    
    int rows_amount;

    int columns_amount;

} CSVDATA;

CSVDATA* scsv_read_csv(char* path);

char** scsv_get_column(CSVDATA* csv_data, int column);

char** scsv_get_row(CSVDATA* csv_data, int row);