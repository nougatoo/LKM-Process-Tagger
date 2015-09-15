/*
 * this ptag.c is an LKM that, upon read, 
 * add all the processes with a tag to the buffer
 * with their respective ID
 *
 * Usage:
 *	make -> sudo insmod ptag.ko
 *
 * Output Formate: Pid : tag state
 *
 * Assumption: the total max characters of all pids and tags is 1000
 * 
 *
 * This LKM was based off my A1 and my A1 was based off the LKM given in 
 * tutorial
 */
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <linux/sched.h>


int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
#define SUCCESS 0
#define DEVICE_NAME "chardev"	/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 1000		/* Max length of the message from the device */

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Device_Open = 0;	/* Is device open?  
				 * Used to prevent multiple access to device */
static char msg[BUF_LEN];	/* The msg the device will give when asked */
static char *msg_Ptr;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};


/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	sprintf(msg, "No tags\n");
//	sprintf(msg, "%d", counter++);
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */

	/* 
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module. 
	 */
	module_put(THIS_MODULE);

	return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{


	int bytes_read = 0;
	int i;
	struct task_struct *aTask;		
	struct tag_list *test;
	struct tag_list *entry = kmalloc(sizeof *entry, GFP_USER);
	i = 0;
	
	/*
	 * for each task, check if it has a ptag list, then add
	 * it's tags and pid to buffer
	 */
	for_each_process(aTask)
	{
		pid_t x = current->pid;
		task_lock(aTask);
		if(aTask->is_init == 1 && (aTask->pid != x))
		{			
			
			list_for_each_entry(test, &(aTask->myList.list), list )
		 	{
				/*
				 * adding all the data byte by byte
				 */
				sprintf(msg+i, "%d : ",aTask->pid);
				i = i + 7;
			
				if(entry->tag_name == NULL){
					printk("Error\n");
				}
				else{			
				entry = list_entry(&(test->list), struct tag_list, list);	
				sprintf(msg+i, "%s", entry->tag_name);
				i = (i + (strlen(entry->tag_name)));
				}
				
				sprintf(msg+i, " %ld\n", aTask->state);
				i = i + 3;
		  	}
								
			
		}//end of if
		
		task_unlock(aTask);

	}//end of for loop
	
	/* All the work should be done by this point, all this is printing */
	
	while (length && *msg_Ptr) {
	put_user(*(msg_Ptr++), buffer++);
	length--;
	bytes_read++;} //End of while loop

	memset(msg, 0, 700);
	
	return bytes_read;
}




static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	printk(KERN_ALERT "Read only.\n");
	return -EINVAL;
}
static int __init hello_proc_init(void) {
  proc_create("ptag", 0, NULL, &fops);
  return 0;
}

static void __exit hello_proc_exit(void) {
  remove_proc_entry("ptag", NULL);
}

MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
