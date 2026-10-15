#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main() {
    uid_t id;
    id = getuid();
    uid_t eid;
    eid = geteuid();
    printf("Id real es: %d\n",id);
    printf("Id efectivo es: %d\n",eid);

    struct passwd *info;
    info = getpwuid(id);
    if(info != NULL){
        printf("Nombre: %s\n",info->pw_name);
        printf("Directorio home: %s\n",info->pw_dir);
        printf("Descripcion del usuario: %s\n",info->pw_gecos);
    }
    

    return 0;
}
