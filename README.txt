Name: Brandon Brien
ID: 10079883
CPSC457 Assignment Two

Notes:
	- No locking functionality was implement
		- decided to spend time elsewhere, working out bugs
	- No copy on fork was implemented
	- I believe all other functionality is present
	- You have to make sure you install the LKM before anything
		make -> sudo insmod ptag.ko
	

Question #1
A)

- tsk->state is of type volatlie long and it's defined in sched.h @1216.
- TASK_DEAD is defined in sched.h @195

B)

- The purpose of explicitly calling schedule is to tell the kernel
  that it needs to pick a new process to put on the cpu. This is
  because this process is going to die and will no longer need
  the cpu. If we didnt' call it, the CPU would be doing nothing
  while it was giving this dead process it's alloted time.

C)

- The purpose of BUG in this context is to make sure that 
  the current process dies, because BUG will kill it's 
  calling process

D)

- The purpose of this is is to keep telling the CPU that nothing is
  happening and that we're not going to be doing much with it.

E)

- BUG is related to reboot, by going from reboot.c @521 -> boot.c @ 233 -> boot.c @ 210 -> BUG

Question #2

	If someone wanted to add this functionality the would have
to traverse through the pid map before assigning a pid and check
each pid against the one that's been requested. If no process existed
with the requested pid, the function would have to instead of increasing 
the previous PID that was allocated by 1, it would have to just directly
enter the requested pid.
	 If the PID already taken by some other process
it should return a suitable messages/return value to tell the calling
fucntion that the pid is taken and it should either send another request
with a different PID, or just let pid_alloc take care of assigning the 
namespace with it's PID.

Question #3

D)
	Adding onto syscall: 
		Pro: less syscalls in the kernel, more utility in one call
		Con: could end up having a loaded system call, with too much stuff in it

	New System call:
		Pro:  Seperates and increases readability
		Con:  Funtionally is very closely related to our other sys call
		      and it might make logical sense to keep them together

	Pseudo-device:
		Pro:  Adds usability because many different things like shell scripts
		      can read from them
		Con:  Can only return a few types	

G)
	I think it's better to implement it in kernel space. When it's implemented in kernel
	space you have the ablility to protect access to it and it lets you add a lot more
	functionality because you have access to a lot of protected information.

H)
	No beacuse kernel programming is insanely hard.







