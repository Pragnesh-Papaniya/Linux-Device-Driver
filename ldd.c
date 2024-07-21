#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENCE("GPL");
MODULE_AUTHOR("PRAGNESH");
MODULE_DESCRIPTION("Basic read loadable kernel module");

static struct proc_dir_entry *custom_proc_node;

static ssize_t read(struct file* file_pointer,
                    char* user_space_buffer,
                    size_t count,
                    loff_t* offset)
                    {
                        char msg[] = "Ack!\n";
                        size_t len = strlen(msg);
                        int result;
                        printk("basic read\n");

                        if(*offset == len){
                            return 0;
                        }

                        result = copy_to_user(user_space_buffer, msg, len);
                        *offset += len;

                        return len;
                    }

struct proc_ops driver_proc_ops={
    .proc_read=read
};

static int module_init(void){
    printk("module_init: entry\n");

    custom_proc_node = proc_create("pdriver",
                                    0,
                                    NULL,
                                    &driver_proc_ops);

    if (custom_proc_node == NULL){
        printk("module_init: error");
        return -1;
    }

    printk("module_init: exit\n");
    return 0;
}

static void module_exit(void){
    printk("module_exit: entry\n");
    proc_remove(custom_proc_node);
    printk("module_exit: exit\n");
}