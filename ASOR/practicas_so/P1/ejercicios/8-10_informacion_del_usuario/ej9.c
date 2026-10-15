#include <stdio.h>
#include <unistd.h>

int main() {
    uid_t id;
    id = getuid();
    uid_t eid;
    eid = geteuid();
    printf("Id real es: %d\n",id);
    printf("Id efectivo es: %d\n",eid);

    return 0;
}