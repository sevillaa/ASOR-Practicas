#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

int main(int argc, char **argv) {
    struct stat sb;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        return EXIT_FAILURE;
    }

    printf("ID of containing device:  [%x,%x]\n",
           major(sb.st_dev), minor(sb.st_dev));
    printf("Inode number:            %ld\n", (long) sb.st_ino);

    printf("File type:               %s\n",
           (sb.st_mode & S_IFMT) == S_IFDIR  ? "directory" :
           (sb.st_mode & S_IFMT) == S_IFREG  ? "regular file" :
           (sb.st_mode & S_IFMT) == S_IFCHR  ? "character device" :
           (sb.st_mode & S_IFMT) == S_IFBLK  ? "block device" :
           (sb.st_mode & S_IFMT) == S_IFLNK  ? "symbolic link" :
           (sb.st_mode & S_IFMT) == S_IFIFO  ? "FIFO/pipe" :
           (sb.st_mode & S_IFMT) == S_IFSOCK ? "socket" :
                                                "unknown");

    printf("Last access time:        %s", ctime(&sb.st_atime));
    printf("Last modification time:  %s", ctime(&sb.st_mtime));
    printf("Last status change time: %s", ctime(&sb.st_ctime));

    return EXIT_SUCCESS;
}
