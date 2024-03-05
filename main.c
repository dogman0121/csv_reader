#include <stdio.h>
#include <stdlib.h>


typedef struct {
    FILE* file;
    char sep;
} CSV_file;

CSV_file csv_open(char* path, char sep){
    CSV_file csv_file;
    csv_file.file = fopen(path, "r");
    csv_file.sep = sep;
    return csv_file;
}

void csv_close(CSV_file csv_file){
    fclose(csv_file.file);
}

char* get_string_from_file(FILE* file){
    char* string;
    char c;
    char* p;
    int string_len = 4;
    string = calloc(string_len, sizeof(char));
    p = string;

    fscanf(file, "%c", &c);
    while (c != '\n' && !feof(file)){
        if ((p - string) + 1 >= string_len) {
            string_len *= 2;
            string = realloc(string, string_len * sizeof(char));
            p = string + string_len / 2 - 1;
        }
        *p = c;
        p++;
        fscanf(file, "%c", &c);
    }
    *(p+1) = '\0';
    return string;
}

char **read_line(CSV_file csv_file) {
    char* string;
    char** cols_list;
    char** col_p;
    int cols_cnt = 1;
    char* p;
    string = get_string_from_file(csv_file.file);
    p = string;

    // Finding count of cols
    while (*p != '\0'){
        if (*p == csv_file.sep)
            cols_cnt++;
        p++;
    }

    cols_list = calloc(cols_cnt+1, sizeof(char*));
    // Adding end-pointer
    *(cols_list + cols_cnt) = NULL;
    col_p = cols_list;

    char* temp_string;
    int temp_string_len = 4;
    char* temp_p;
    temp_string = calloc(temp_string_len, sizeof(char));
    temp_p = temp_string;
    p = string;
    while (*p != '\0'){
        if (*p != csv_file.sep){
            if ((temp_p - temp_string) + 1 >= temp_string_len){
                temp_string_len *= 2;
                temp_string = realloc(temp_string, temp_string_len * sizeof(char));
                temp_p = temp_string + temp_string_len / 2 - 1;
            }
            *temp_p = *p;
            temp_p++;
        }
        else {
            *col_p = temp_string;
            col_p++;
            temp_string_len = 4;
            temp_string = calloc(temp_string_len, sizeof(char));
            temp_p = temp_string;
        }
        p++;
    }
    *col_p = temp_string;
    return cols_list;
}

char*** read_lines(CSV_file csv_file) {
    char*** lines;
    int lines_cnt = 4;
    char*** p;
    lines = calloc(lines_cnt, sizeof(char**));
    p = lines;
    while (!feof(csv_file.file)) {
        if ((p - lines + 1) == lines_cnt){
            lines_cnt *= 2;
            lines = realloc(lines, sizeof(char**) * lines_cnt);
            p = lines + lines_cnt / 2;
        }
        *p = read_line(csv_file);
        p++;
    }
    *p = NULL;
    return lines;
}


int main() {

}
