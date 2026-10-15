#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main() {
    
    if (setuid(0) == -1) {
        perror("setuid");
        return 1;
    }
    return 0;
}