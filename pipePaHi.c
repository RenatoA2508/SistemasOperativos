// Pipe sin nombre: Pa (W) -> msg(PIPE) -> Hi (R)

#include <unistd.h>  
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/wait.h>  
      
void sys_err(const char *str) {  
    perror(str);  
    exit(1);  
}  
      
int main(int argc, char *argv[])  {  
    pid_t pid;  
    char buf[1024];  
    int fd[2];  

    if (argc != 2) {printf ("USO: %s <msg>\n", argv[0]); exit(1);} 
          
    if (pipe(fd) == -1)   
        sys_err("pipe");  
      
    pid = fork();  
    if (pid < 0) {  
        sys_err("fork err");  
    }
    else if (pid == 0) {  
        close(fd[1]);  
        printf("child process wait to read:\n");
        int len = read(fd[0], buf, sizeof(buf));
	buf[len] = '\n';
        write(STDOUT_FILENO, buf, len);  
        close(fd[0]);  
    }
    else {  
        close(fd[0]);  
        write(fd[1], argv[1], strlen(argv[1])+1);  
        wait(NULL);  
        close(fd[1]);  
    }  
          
    return 0;  
}
