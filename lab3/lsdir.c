#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int lsdir(char * path, int depth);

int main() {
    lsdir("/home/fog/Dev/sysLab", 0);
    return 0;
}

int lsdir(char * path, int depth) {
    DIR * dp;
    struct dirent * entry;
    struct stat statbuf;

    if ((dp = opendir(path)) == NULL) {
        // 打开文件失败
        printf("Open file failed!\n");
        return -1;
    }
    chdir(path);
    while ((entry = readdir(dp)) != NULL) {
        
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if ((strlen(entry->d_name) == 2 && strcmp(entry->d_name, "..") == 0) || (strlen(entry->d_name) == 1 && entry->d_name[0] == '.')) {
                continue;
            }
            printf("%s\n", entry->d_name);
            lsdir(entry->d_name, depth);
        }
        else {
            printf("%s\n", entry->d_name);
        }
        chdir("../");
    }
    return 0;
}
