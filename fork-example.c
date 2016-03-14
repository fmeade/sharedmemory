#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("I about to be a parent!\n");
  pid_t result = fork();
  printf("Fork completed with status %d.\n", result);
  if (result<0) {
    fprintf(stderr, "fork failed\n");
    return result;
    }
  else if (result==0) {
    printf("I am the child.  Waaah.\n");
    printf("Often -- though not always -- the child process"
           " replaces itself with a different program:\n");
    execl("/bin/ls","ls",NULL);  // the 2nd and later strings are the child's argv (terminating with NULL)
    perror("oops!");  // print the error message if we reach here.
    printf("This line never reached!");
    printf("`execlp` replaced this process's code with `ls`'s code.");
    }
  else {
    printf("I just had a baby, whose pid is %d.\n", result );
    return 0;
    }
  }

/* Discuss the timing, and how output can be interleaved:
   For example, on my system, I get (in order):
   - 'I am the parent'
   - 'I just had a baby'
   - 'I am the child'
   - 'Often ...'
   - the shell promt system-prompt -- because the parent finished
   - the results of 'ls'
 */
