#include<stdio.h>
#include<stdlib.h>

int main(int argc, char ** argv) {
    // Open the code file
    FILE * code_f = fopen("code.txt", "r");
    // if file not read
    if (code_f == NULL) {fputs("File error",stderr); exit(1);}
    
    // seek to the end and find the total size
    fseek(code_f, 0, SEEK_END);
    long code_size = ftell(code_f);
    rewind(code_f);

    // allocate memory for the code
    char * buffer = (char*) malloc (code_size);
    // if buffer is not set
    if (buffer == NULL) {fputs("Memory error",stderr); exit(2);}
    // load content into buffer
    int read = fread(buffer, 1, code_size, code_f);
    // if less than 16k read into buffer and not end-of-file
    if (read != code_size && feof(code_f) == 0) {fputs("Error while reading file", stderr); exit(3);}
    // close file
    fclose(code_f);
    
    // use system call to make terminal send all keystrokes directly to stdin
    system ("/bin/stty raw");
    
    long seek = 0;
    char c;
    puts("\033[1;32m");
    // type a period to break out of the loop, since CTRL-D won't work raw
    while((c = getchar()) != '.') {
        putchar('\b');
        if (buffer[seek] == '\n') {
            putchar(' ');
        }
        // print a char from the code
        putchar(buffer[seek]);
        if (buffer[seek] == '\n') {
            putchar('\r');
        }
        // move the seek to the nexr char
        seek = (seek+1)%code_size;
    }
    puts("\n\r\033[0m");
    // free memory
    free(buffer);
    // use system call to set terminal behaviour to more normal behaviour
    system ("/bin/stty cooked");
    return 0;
}
