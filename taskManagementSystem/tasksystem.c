#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxEmail 30
#define maxPassword 12
#define maxUsername 12
#define FILE_NAME "tasks.txt"

typedef struct 
{
    char email[maxEmail];
    char password[maxPassword];
    char username[maxUsername];
} user;

typedef struct  {
    char email[100];
    char task_name[100];
    int priority;
    char category[50];
    char due_date[20];
    char due_time[10];
}Task ;

user USER;
Task task;

void Register();
int login();
void addTask();
void viewTasks();
void displayTask();
void editTask();


int main(){
    int select;
    while (1)
    {
        printf("1. REGISTER\n");
        printf("2. LOGIN\n");
        printf("3. EXIT\n");
        printf("Select option(1-3): ");
        scanf("%i", &select);
        switch (select)
        {
        case 1:
            Register();
            break;
        case 2:
            if(login()){
                while (1) {
                    select = 0;
                    printf("\nTask Management System\n");
                    printf("1. Add Task\n");
                    printf("2. View Tasks\n");
                    printf("3. Edit/Delete Task\n");
                    printf("4. Exit\n");
                    printf("Enter your choice: ");
                    scanf("%d", &select);

                    switch (select) {
                        case 1:
                            addTask();
                            break;
                        case 2:
                            viewTasks(USER.email);
                            break;
                        case 3:
                            editTask(USER.email);
                            break;
                        case 4:
                            return 0;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
            break;
            }else{
                printf("invalid credentials!\n");
            }
            
        case 3:
            return 0;
        default:
            break;
        }
    }
return 0;
}

//REGISTER functiono
void Register(){ 
    while (1)
    {
        int atcount = 0;
        int dotcount = 0;
        printf("username: ");
        scanf("%s", USER.username);
        printf("Email: ");\
        scanf("%s", USER.email);
        printf("Password: ");
        scanf("%s", USER.password);
        for(int i = 0; i < sizeof(USER.email); i++){
            if(USER.email[i] == '@'){
                atcount++;
            } 
            if(USER.email[i] == '.'){
                dotcount++;
            }
        }
        if (atcount != 1 || dotcount < 1){
            printf("Invalid email format. \n");
        }
        else if (strlen(USER.password) < 5 || strlen(USER.password) > 12){
            printf("Password must be between 5-12 characters.\n");
        }else{break;}
    }
    FILE *file;
    file = fopen("register.txt", "a");
    if (file == NULL){
        perror("Failed to open file");
        return;
    }
    printf("%s,%s\n", USER.email, USER.password);
    fprintf(file,"%s, %s, %s\n", USER.email, USER.password, USER.username);
    fclose(file);
    printf("Successfully Registered!\n");
}

//Login function
int login(){
    char loginUsername[maxUsername];
    char line[maxEmail + maxPassword + maxUsername + 3];
    char loginEmail[maxEmail];
    char loginPassword[maxPassword];
    printf("Email: ");
    scanf("%s", loginEmail);
    printf("Password: ");
    scanf("%s", loginPassword);
    FILE* file;
    file = fopen("register.txt", "r");
    if(file == NULL){
        return 1;
    }
    while(fgets(line, sizeof(line), file)){
        sscanf(line, "%[^,], %[^,], %s", USER.email, USER.password, USER.username);
        printf("check: %s, %s, %s\n", USER.email, USER.password, USER.username);
        if(strcmp(loginEmail, USER.email)==0 && strcmp(loginPassword, USER.password)==0){
            return 1;
        }
    }
    fclose(file);
    return 0;
}

//Add task function
void addTask() {
    FILE *file = fopen(FILE_NAME, "a");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    strcpy(task.email, USER.email);
    printf("Enter task name: ");
    scanf("%s", task.task_name);
    printf("Enter priority (1-5): ");
    scanf("%d", &task.priority);
    printf("Enter category: ");
    scanf("%s", task.category);
    printf("Enter due date (YYYY-MM-DD): ");
    scanf("%s", task.due_date);
    printf("Enter due time (HH:MM): ");
    scanf("%s", task.due_time);

    // Save the task in text format
    fprintf(file, "%s,%s,%d,%s,%s,%s\n", task.email, task.task_name, task.priority, task.category, task.due_date, task.due_time);
    fclose(file);
    printf("Task added successfully.\n");
}

// Function to view tasks by email
void viewTasks(char email[30]) {
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    strcpy(email, USER.email);

    printf("\nTasks for %s:\n", USER.username);
    while (fscanf(file, "%99[^,],%99[^,],%d,%49[^,],%19[^,],%9[^\n]\n", task.email, task.task_name, &task.priority, task.category, task.due_date, task.due_time) == 6) {
        if (strcmp(task.email, email) == 0) {
            displayTask(task);
        }
    }
    fclose(file);
}

// Helper function to display a task
void displayTask(Task task) {
    printf("\nTask Name: %s\n", task.task_name);
    printf("Email: %s\n", task.email);
    printf("Priority: %d\n", task.priority);
    printf("Category: %s\n", task.category);
    printf("Due Date: %s\n", task.due_date);
    printf("Due Time: %s\n", task.due_time);
}

// Function to edit or delete a task
void editTask(char email[maxEmail]) {
    char task_name[100];
    int found = 0;

    FILE *file = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter the task name you want to edit/delete: ");
    scanf("%s", task_name);

    while (fscanf(file, "%99[^,],%99[^,],%d,%49[^,],%19[^,],%9[^\n]\n",
                  task.email, task.task_name, &task.priority,
                  task.category, task.due_date, task.due_time) == 6) {
        if (strcmp(task.email, email) == 0 && strcmp(task.task_name, task_name) == 0) {
            found = 1;
            int choice;
            printf("Task found:\n");
            displayTask(task);

            printf("\n1. Edit Task\n2. Delete Task\nEnter choice: ");
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter new task name: ");
                scanf("%s", task.task_name);
                printf("Enter new priority: ");
                scanf("%d", &task.priority);
                printf("Enter new category: ");
                scanf("%s", task.category);
                printf("Enter new due date (YYYY-MM-DD): ");
                scanf("%s", task.due_date);
                printf("Enter new due time (HH:MM): ");
                scanf("%s", task.due_time);
                
                fprintf(tempFile, "%s,%s,%d,%s,%s,%s\n", task.email, task.task_name,
                        task.priority, task.category, task.due_date, task.due_time);
                printf("Task edited successfully.\n");
            } else if (choice == 2) {
                printf("Task deleted successfully.\n");
            }
        } else {
            fprintf(tempFile, "%s,%s,%d,%s,%s,%s\n", task.email, task.task_name,
                    task.priority, task.category, task.due_date, task.due_time);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (!found) {
        printf("No matching task found.\n");
    }
}
