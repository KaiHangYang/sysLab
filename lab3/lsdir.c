#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h> // 获取用户名
#include <grp.h> // 获取组名
#include <time.h> // 获取时间

int lsdir(char * path, int depth);
void printstat(struct stat *s, struct dirent *e);

int main(int argc, char ** argv) {
    if (argc <= 1) {
        printf("Please provide path!\n");
        return 0;
    }
    lsdir(argv[1], 0);
    return 0;
}

int lsdir(char * path, int depth) {
    DIR * dp;
    struct dirent * entry;
    struct stat statbuf;

    if ((dp = opendir(path)) == NULL) {
        // 打开文件失败
        printf("Open file failed! %s\n", path);
        return -1;
    }
    chdir(path);
    while ((entry = readdir(dp)) != NULL) {
        char tmpTab[256];
        memset(tmpTab, 0, sizeof(tmpTab));
        memset(tmpTab, '\t', depth);
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if ((strlen(entry->d_name) == 2 && strcmp(entry->d_name, "..") == 0) || (strlen(entry->d_name) == 1 && entry->d_name[0] == '.')) {
                continue;
            }
            printf("%s|%d|", tmpTab, depth);
            printstat(&statbuf, entry);
            if (lsdir(entry->d_name, depth+1)) {
                return -1;
            }
        }
        else {
            printf("%s|%d|", tmpTab, depth);
            printstat(&statbuf, entry);
        }
    }
    chdir("../");
    closedir(dp);
    return 0;
}

void printstat(struct stat * s, struct dirent * e) {
    unsigned short mode = s->st_mode;

    if (S_ISDIR(mode)) {
        printf("d");
    }
    else if (S_ISLNK(mode)) {
        printf("l");
    }
    else {
        printf("-");
    }
    // mode 的高4位描述文件的类型
    // 低9位描述了文件的权限
    int i;
    for (i = 2; i >= 0; --i) {
        unsigned char tmpbit = (mode >> (i*3)) & 7;
        char symbol[4];
        memset(symbol, '-', sizeof(symbol));
        if ((tmpbit & 4) != 0) {
            symbol[0] = 'r';
        }
        if ((tmpbit & 2) != 0) {
            symbol[1] = 'w';
        }
        if ((tmpbit & 1) != 0) {
            symbol[2] = 'x';
        }
        symbol[3] = '\0';
        printf("%s", symbol);
    }

    printf(" %d ", s->st_nlink);
    struct passwd * pw;
    struct group * grp;
    char timeBuf[256];
    pw = getpwuid(s->st_uid);
    grp = getgrgid(s->st_gid);
    strftime(timeBuf, sizeof(timeBuf), "%h %d %H:%M", localtime(&s->st_mtime));
    printf("%s\t%s\t%d\t%s\t%s\n", pw->pw_name, grp->gr_name, s->st_size, timeBuf, e->d_name); //显示文件用户名 和组名信息
}
