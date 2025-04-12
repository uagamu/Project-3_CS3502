#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MAX_LEN 4096
#define PROC_NAME "myproc"

static struct proc_dir_entry *proc_entry;
static char *info = NULL;
static size_t info_len = 0;

/* Forward declarations */
static ssize_t read_proc(struct file *file, char __user *user_buf, size_t count, loff_t *off);
static ssize_t write_proc(struct file *file, const char __user *user_buf, size_t count, loff_t *off);

/* Use the older file_operations structure for kernel 5.4 */
static const struct file_operations proc_fops = {
    .read = read_proc,
    .write = write_proc
};

/* Read function implementation */
static ssize_t read_proc(struct file *file, char __user *user_buf, size_t count, loff_t *off) {
    size_t len = 0;
    
    if (*off >= info_len)
        return 0;
    
    len = (count < info_len - *off) ? count : info_len - *off;
    
    if (copy_to_user(user_buf, info + *off, len))
        return -EFAULT;
    
    *off += len;
    return len;
}

/* Write function implementation */
static ssize_t write_proc(struct file *file, const char __user *user_buf, size_t count, loff_t *off) {
    size_t len;
    
    len = (count < MAX_LEN - 1) ? count : MAX_LEN - 1;
    
    if (copy_from_user(info, user_buf, len))
        return -EFAULT;
    
    info[len] = '\0';
    info_len = len;
    
    printk(KERN_INFO "proc_write: %s\n", info);
    
    return len;
}

/* Module initialization */
static int __init init_new_module(void) {
    // Allocate memory for info
    info = kmalloc(MAX_LEN, GFP_KERNEL);
    if (!info) {
        printk(KERN_ALERT "myproc: Memory allocation failed\n");
        return -ENOMEM;
    }
    memset(info, 0, MAX_LEN);
    
    // Create proc entry
    proc_entry = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_entry) {
        printk(KERN_ALERT "Error: Could not create proc entry\n");
        kfree(info);
        return -ENOMEM;
    }
    
    printk(KERN_INFO "test_proc created.\n");
    return 0;
}

/* Module cleanup */
static void __exit exit_new_module(void) {
    // Remove proc entry
    remove_proc_entry(PROC_NAME, NULL);
    
    // Free allocated memory
    if (info)
        kfree(info);
        
    printk(KERN_INFO "test_proc removed.\n");
}

/* Module entry and exit points */
module_init(init_new_module);
module_exit(exit_new_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ugochukwu Agamu");
MODULE_DESCRIPTION("A simple module for /proc file interaction");
