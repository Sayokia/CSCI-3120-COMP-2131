#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Use char for all variables in case of usage of alphabet in ID
typedef struct Student
{
    char id[10];
    char name[10];
    char emailId[10];
    char courseId[10];
    char grade[10];
} Std;


int main() {
    // initialize the file to be created
    FILE *f;
    // create a temporary file to apply changes
    FILE *ftemp;
    // option represents the options provided to user in the menu
    int option;

    // initialize student record
    Std std;

    // a int for case 3, 4 and 5, 0 represents not found, 1 represents found
    int found;


    // read in user input id as search key for seek and update functionality, and name for delete functionality
    char id[10],name[10];


    while(option != 6){

        // display the menu information to provide options for user to choose
        printf("\n\nMAIN MENU\n");
        printf("1. Create the Binary File\n");
        printf("2. Display the content of the file \n");
        printf("3. Seek a specific record\n");
        printf("4. Update the content of a record\n");
        printf("5. Delete a record for the specific name\n");
        printf("6. Exit\n");
        printf("       Please enter your choice .... ");

        // read in user input for the option chosen by the user
        scanf("%d",&option);

        switch (option){
            // option 1: create the Binary File
            case 1:
                // open the binary file, set mode to all to create the file if it is not exist
                f = fopen("student_record.bin","ab");

                // create student record by user input

                printf("Enter Student ID: ");
                scanf("%s", std.id);
                printf("Enter Student Name: ");
                scanf("%s", std.name);
                printf("Enter Student Email ID: ");
                scanf("%s", std.emailId);
                printf("Enter Student Course ID: ");
                scanf("%s", std.courseId);
                printf("Enter Student Grade: ");
                scanf("%s", std.grade);

                //write the record into the file
                fwrite(&std, sizeof(std),1,f);
                fclose(f);
                break;

            case 2:
                //option2: Display the content of the file
                //open the binary file with read mode
                f = fopen("student_record.bin","rb");
                while (fread(&std, sizeof(std),1,f))
                {
                    printf("\n\nStudent ID: %s", std.id);
                    printf("\nStudent Name: %s", std.name);
                    printf("\nStudent Email ID: %s", std.emailId);
                    printf("\nStudent Course ID: %s", std.courseId);
                    printf("\nStudent Grade: %s", std.grade);
                }
                fclose(f);
                break;

            case 3:
                //option3: Seek a specific record
                found = 0;
                printf("\nEnter Student ID to search for the record: ");
                scanf("%s", id);

                //open the binary file with read mode
                f = fopen("student_record.bin","rb");
                while (fread(&std, sizeof(std),1,f))
                {
                    if(strcmp(std.id,id)==0 )
                    {
                        printf("\n\nStudent ID: %s", std.id);
                        printf("\nStudent Name: %s", std.name);
                        printf("\nStudent Email ID: %s", std.emailId);
                        printf("\nStudent Course ID: %s", std.courseId);
                        printf("\nStudent Grade: %s", std.grade);
                        found = 1;
                    }
                }
                fclose(f);
                if (found ==0){
                    printf("\n\n\tRecord not found!");
                }
                break;

            case 4:
                //option 4: Update a content of a record

                found = 0;

                //read in the ID of the student that his/her record is going to be changed
                printf("\nEnter Student ID to change his/her record:");
                scanf("%s",id);

                // open both student record file and temp file
                f = fopen("student_record.bin","rb");
                // set temp file mode to w to make sure it is empty for each time of use
                ftemp = fopen("temp.bin","wb");

                // go through the student_record.bin file, if the record's id is the required one, make change to this record and write it into the temp file
                //otherwise, directly copy content in student_record.bin file into temp file
                while (fread(&std, sizeof(std),1,f))
                {
                    if(strcmp(std.id,id)==0 )
                    {
                        printf("Enter New Student ID: ");
                        scanf("%s",std.id);
                        printf("Enter New Student Name: ");
                        scanf("%s",std.name);
                        printf("Enter New Student Email ID: ");
                        scanf("%s",std.emailId);
                        printf("Enter New Student Course ID: ");
                        scanf("%s",std.courseId);
                        printf("Enter New Student Grade: ");
                        scanf("%s",std.grade);
                        fwrite(&std, sizeof(std),1,ftemp);
                        found = 1;
                    }
                    else{
                        fwrite(&std, sizeof(std),1,ftemp);
                    }
                }

                fclose(f);
                fclose(ftemp);

                // remove the old version of student record file, and rename temp to replace the old one
                remove("student_record.bin");
                rename("temp.bin","student_record.bin");

                //display the new version of file
                f = fopen("student_record.bin","rb");
                while (fread(&std, sizeof(std),1,f))
                {
                    printf("\n\nStudent ID: %s", std.id);
                    printf("\nStudent Name: %s", std.name);
                    printf("\nStudent Email ID: %s", std.emailId);
                    printf("\nStudent Course ID: %s", std.courseId);
                    printf("\nStudent Grade: %s", std.grade);
                }
                fclose(f);

                if (found ==0){
                    printf("\n\n\tRecord not found!");
                }
                break;

            case 5:
                //option5: Delete a record for the specific name
                found = 0;
                printf("\nEnter the name of the student you want delete his/her record: ");
                scanf("%s",name);

                //repeat the similar process of case 4 update function

                // open both student record file and temp file
                f = fopen("student_record.bin","rb");
                // set temp file mode to w to make sure it is empty for each time of use
                ftemp = fopen("temp.bin","wb");

                while (fread(&std, sizeof(std),1,f))
                {
                    if(strcmp(std.name,name) != 0){
                        fwrite(&std, sizeof(std),1,ftemp);
                    }
                    else{
                        found =1;
                    }
                }

                fclose(f);
                fclose(ftemp);

                // remove the old version of student record file, and rename temp to replace the old one
                remove("student_record.bin");
                rename("temp.bin","student_record.bin");

                //display the new version of file
                f = fopen("student_record.bin","rb");
                while (fread(&std, sizeof(std),1,f))
                {
                    printf("\n\nStudent ID: %s", std.id);
                    printf("\nStudent Name: %s", std.name);
                    printf("\nStudent Email ID: %s", std.emailId);
                    printf("\nStudent Course ID: %s", std.courseId);
                    printf("\nStudent Grade: %s", std.grade);
                }
                fclose(f);

                if (found ==0){
                    printf("\n\n\tRecord not found!");
                }
                break;

            case 6:
                //option 6: exit the program
                printf("Exit the program!");
                exit(0);
        }
    }

}