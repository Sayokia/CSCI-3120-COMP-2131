#include <stdio.h>
#include <string.h>


int main() {
    // initialize input String
    char input[201];

    // initialize the file to be created
    FILE *f;

    // prompt display
    printf("%s", "Please Enter Your Sentence: ");

    // read in user input
    fgets(input, sizeof(input), stdin);

    //create the deliminator
    char delim[] = " ";
    //extract the first token
    char *token = strtok(input, delim);

    // create the file to write in words
    f = fopen("Output.txt", "w");

    // go through the string to extract other tokens
    while (token != NULL) {
        // print each token to the screen
        printf("%s\n", token);
        // then, write the token into created file
        fputs(token, f);
        fputs("\n", f);

        token = strtok(NULL, delim);
    }

    fclose(f);


    return 0;

}