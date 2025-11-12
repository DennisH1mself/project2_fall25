#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;
// hi test
//make directory
void mkdir(char pathName[]){
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if (parent == NULL) {
        return;
    }

    struct NODE* current = parent->childPtr;
    while (current) {
        if (strcmp(current->name, baseName) == 0 && current->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        current = current->siblingPtr;
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    if (parent->childPtr == NULL) {
        parent->childPtr = newDir;
    } else {
        struct NODE* sibling = parent->childPtr;
        while (sibling->siblingPtr) {
            sibling = sibling->siblingPtr;
        }
        sibling->siblingPtr = newDir;
    }
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    char* lastSlash = strrchr(pathName, '/');
    if (!lastSlash) {
        strcpy(baseName, pathName);
        strcpy(dirName, "");
        return cwd;
    } else if (lastSlash == pathName) {
        if (strlen(pathName) == 1) {
            strcpy(baseName, "");
            strcpy(dirName, "/");
            return root;
        } else {
            strcpy(baseName, lastSlash + 1);
            strcpy(dirName, "/");
            return root;
        }
    } 
    strcpy(baseName, lastSlash + 1);
    strncpy(dirName, pathName, lastSlash - pathName);
    dirName[lastSlash - pathName] = '\0';

    struct NODE* current;
    current = (dirName[0] == '/') ? root : cwd;
    
    if (strcmp(dirName, "/") == 0) {
        return root;
    }
    if (strlen(dirName) == 0) {
        return cwd;
    }
    
    char* token = strtok(dirName, "/");
    while (token) {
        struct NODE* child = current->childPtr;
        while (child) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                current = child;
                break;
            }
            child = child->siblingPtr;
        }

        if (!child) {
            printf("ERROR: directory %s does not exist", token);
            return NULL;
        }

        token = strtok(NULL, "/");
    }

    return current;
}
