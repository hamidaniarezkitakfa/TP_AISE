/*utiliser ptrace pour trouver tous les appels système qui appellent par certains processus */
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/reg.h>
int main(int argc, char *argv[]) {
printf("ensemble des appels systeme \n");
printf("pid est : %d \n" , getpid());
// crie un processus enfant
pid_t pid = fork();

if (pid < 0) {

printf("fork failed");
exit(-1);
} else if (pid == 0) {


// définir l'état du processus enfant sur PTRACE
ptrace(PTRACE_TRACEME,0,NULL,NULL);
// l'enfant passera à l'état arrêté lors de l'appel execve, puis enverra le signal au parent
char **args = (char **)malloc(sizeof(char *) * argc);
for (int i = 1; i < argc; i++) {

args[i - 1] = argv[i];
}
args[argc - 1] = NULL;
execve(argv[1], args, NULL);
free(args);
} else {

int status;
int bit = 1;
long entre;
long ret;
// attendre l'enfant
wait(&status);
if(WIFEXITED(status))
return 0;
entre = ptrace(PTRACE_PEEKUSER, pid, ORIG_RAX * 8, NULL);
printf("system call num = %ld \n", entre);
ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
while(1) {

wait(&status);
if(WIFEXITED(status))
return 0;
// pour entrer l'appel système
if(bit) {

entre = ptrace(PTRACE_PEEKUSER, pid, ORIG_RAX * 8, NULL);
printf("system call num = %ld------", entre);

bit = 0;
} else {
 // pour le retour de l'appel système
ret = ptrace(PTRACE_PEEKUSER, pid, RAX*8, NULL);
printf("system call return = %ld \n", ret);
bit = 1;
}
// laissez ce processus enfant continuer à s'exécuter jusqu'au prochain appel système
ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
}
}
}
