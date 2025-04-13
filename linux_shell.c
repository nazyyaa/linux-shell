#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node {
    char name[100];
    char owner[100];
    int type;
    int permission;
    struct node* children[1000]; 
    struct node* parent; 
    int file_count;
};

struct user {
    char name[100];
    int permission; 
};

struct node root;
struct node* current_dir;
struct user users[1];

int is_valid_name(char *arg, int max_len) {
    for (int i = 0; i < 1; i++) {
        if (arg[i] == '\n' || arg[i] == '\0' || arg[i] == ' ') return 0;
    }
    return 1;
}

char* permission_to_text(int permission) {
    switch (permission) {
        case 0: return "---";
        case 1: return "--x";
        case 2: return "-w-";
        case 3: return "-wx";
        case 4: return "r--";
        case 5: return "r-x";
        case 6: return "rw-";
        case 7: return "rwx";
        default: return "null";
    }
}

void ls() {
    if (!current_dir->file_count) {
        printf("no files\n");
    } else {
        for (int i = 0; i < current_dir->file_count; i++) {
            printf("%s %s %s\n", current_dir->children[i]->name, current_dir->children[i]->owner, permission_to_text(current_dir->children[i]->permission));
        }
    }
}

void mkdir(char *name) {
    for (int i = 0; i < current_dir->file_count; i++) {
        if (!strcmp(current_dir->children[i]->name, name)) {
            printf("error\n");
            return;
        }
    }
    for (int i = 0; i < strlen(name); i++) {
        if (!isalnum(name[i])) {
            printf("error\n"); 
            return;
        }
    }
    struct node* dir = malloc(sizeof(struct node));
    strcpy(dir->name, name);
    strcpy(dir->owner, users[0].name);
    dir->permission = users[0].permission;
    dir->type = 0;
    dir->parent = current_dir;
    current_dir->children[current_dir->file_count++] = dir;
}

void touch(char *name) {
    for (int i = 0; i < current_dir->file_count; i++) {
        if (!strcmp(current_dir->children[i]->name, name)) {
            printf("error\n");
            return;
        }
    }
    for (int i = 0; i < strlen(name); i++) {
        if (!isalnum(name[i])) {
            printf("error\n"); 
            return;
        }
    }
    struct node* file = malloc(sizeof(struct node));
    strcpy(file->name, name);
    strcpy(file->owner, users[0].name);
    file->permission = users[0].permission;
    file->type = 1;
    current_dir->children[current_dir->file_count++] = file;
}

void rm(char *name) {
    int found = 0;
    
    for (int i = 0; i < current_dir->file_count; i++) {
        if (strcmp(current_dir->children[i]->name, name) == 0) {
            found = 1;
            if (current_dir->children[i]->permission & 2) {
                current_dir->file_count--;
                break;
            } else {
                printf("permission error\n");
            }
        }
    }
    if (!found) {
        printf("error\n");
    }
}

void chmod(char *perm_str, char *name) {
    int found = 0;
    int perm = atoi(perm_str);
    if (!isdigit(perm_str[0])) perm = -1;
    if (perm >= 0 && perm < 8) {
        for (int i = 0; i < current_dir->file_count; i++) {
            if (!strcmp(current_dir->children[i]->name, name)) {
                found = 1;
                current_dir->children[i]->permission = perm;
                break;
            }
        }
    }
    if (!found) {
        printf("error\n");
    }
}

void chown(char *new_owner, char *name) {
    int found = 0;
    for (int i = 0; i < current_dir->file_count; i++) {
        if (!strcmp(current_dir->children[i]->name, name)) {
            found = 1;
            strcpy(current_dir->children[i]->owner, new_owner);
            break;
        }
    }
    if (!found) {
        printf("error\n");
    }
}

void cd(char *name) {
    if (!strcmp(name, "..")) {
        if (current_dir->parent != NULL) {
            current_dir = current_dir->parent;
        }
    } else {
        int found = 0;
        for (int i = 0; i < current_dir->file_count; i++) {
            if (!strcmp(current_dir->children[i]->name, name) && current_dir->children[i]->type == 0) {
                found = 1;
                if (current_dir->children[i]->permission & 1) {
                    current_dir = current_dir->children[i];
                } else {
                    printf("permission error\n");
                }
                break;
            }
        }
        if (!found) printf("error\n");
    }
}

void read_file(char *name) {
    int ok = 0, found = 0;
    for (int i = 0; i < current_dir->file_count; i++) {
        if (!strcmp(current_dir->children[i]->name, name)) {
            found = 1;
            if (current_dir->children[i]->permission & 4) {
                printf("ok\n");
                ok = 1;
            }
            break;
        }
    }
    if (!ok && found) printf("permission error\n");
    else if (!found) printf("error\n");
}

void write_file(char *name) {
    int ok = 0, found = 0;
    for (int i = 0; i < current_dir->file_count; i++) {
        if (!strcmp(current_dir->children[i]->name, name)) {
            found = 1;
            if (current_dir->children[i]->permission & 2) {
                printf("ok\n");
                ok = 1;
            }
            break;
        }
    }
    if (!ok && found) printf("permission error\n");
    else if (!found) printf("error\n");
}

void execute_file(char *name) {
    int ok = 0, found = 0;
    for (int i = 0; i < current_dir->file_count; i++) {
        if (!strcmp(current_dir->children[i]->name, name)) {
            found = 1;
            if (current_dir->children[i]->permission & 1) {
                printf("ok\n");
                ok = 1;
            }
            break;
        }
    }
    if (!ok && found) printf("permission error\n");
    else if (!found) printf("error\n");
}

int main() {
    current_dir = &root;
    strcpy(users[0].name, "root");
    users[0].permission = 7;

    char input[500], command[10], arg1[245], arg2[245];

    while (1) {
        printf("# ");
        fgets(input, 500, stdin);
        sscanf(input, "%s %s %s", command, arg1, arg2);

        if (!strcmp(command, "ls")) {
            ls();
        } else if (!strcmp(command, "mkdir")) {
            if (is_valid_name(arg1, strlen(arg1))) mkdir(arg1);
            else printf("error\n");
        } else if (!strcmp(command, "touch")) {
            if (is_valid_name(arg1, strlen(arg1))) touch(arg1);
            else printf("error\n");
        } else if (!strcmp(command, "rm")) {
            rm(arg1);
        } else if (!strcmp(command, "chmod")) {
            chmod(arg1, arg2);
        } else if (!strcmp(command, "chown")) {
            chown(arg1, arg2);
        } else if (!strcmp(command, "cd")) {
            cd(arg1);
        } else if (!strcmp(command, "read")) {
            read_file(arg1);
        } else if (!strcmp(command, "write")) {
            write_file(arg1);
        } else if (!strcmp(command, "exec")) {
            execute_file(arg1);
        } else if (!strcmp(command, "quit")) {
            break;
        } else {
            printf("error\n");
        }

        command[0] = '\0'; 
        arg1[0] = '\0';
        arg2[0] = '\0';
    }

    return 0;
}
