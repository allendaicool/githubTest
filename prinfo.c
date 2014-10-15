#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
/* the prinfo structure is declared again
 * in the testfile to keep up
 * with prinfo structure in kernel
 */
 
 haha
struct prinfo {
	pid_t parent_pid;
	pid_t pid;
	pid_t first_child_pid;
	pid_t next_sibling_pid;
	long state;
	long uid;
	char comm[64];
};

/* this function print the tree process
 * by using stack structure
 * buf: store the process info
 * stack: stack implemented by array
 * top: pointer to array
 */
void Print(struct prinfo *buf, int *stack, int *top)
{
	int i;
	
	/* print the correct number of tab
	*/
	for (i = 0; i < *top; i++) {
		printf("\t");
		if (buf->parent_pid == stack[i]) {
			*top = i + 1;
			break;
		}
	}
	printf("%s,%d,%ld,%d,%d,%d,%d\n", buf->comm,
	       (int)buf->pid, buf->state,
		(int)buf->parent_pid, (int)buf->first_child_pid,
		(int)buf->next_sibling_pid, (int) buf->uid);
	if (buf->first_child_pid != 0) {
		stack[*top] = buf->pid;
		(*top)++;
	}
}

/* the main function first allocate a buffer
* to hold the prinfo structure and call the 
* syscall to print the process info
*/
int main(void)
{
	int i;
	int nr = 300;
	int *stack;
	int num;
	int top;
	struct prinfo *buf;

	buf = (struct prinfo *)malloc(sizeof(struct prinfo) * nr);
	i = 0;
	stack = (int *)malloc(sizeof(int) * 10);
	top = 0;
	num = syscall(223, buf, &nr);
	/*the loop to print process info*/
	for (i = 0; i < num; i++)
		Print(&buf[i], stack, &top);
	printf("There is %d processes\n", num);
	return 0;
}
