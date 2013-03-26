#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>


static struct tasklet_struct tasklet;
static struct cdev chardev;
struct timer_list timer;

static struct file_operations fops = {
	.open = copen,
	.release = cclose,
};

static void timer_handler(unsigned long data)
{
	printk("[%s:%d]\n",__func__,__LINE__);
}

static void tasklet_handle(unsigned long data)
{
	printk("[%s:%d]\n",__func__,__LINE__);
}

static void work_handle(struct work_struct *work)
{
	printk("[%s:%d]\n",__func__,__LINE__);
}


static __init int preempt_module_init()
{
	struct task_struct *get;		//������Ϣ get =  current
	struct thread_info *tinfo;	//�ں��߳���Ϣ
	unsigned long data = 0;		//tasklet data
	

	
	printk("HZ = %d\n", HZ);	//  HZ = 1s;HZ������һ��
	printk("jiffies = %d\n", jiffies); //jiffies = ���������ڵ�HZ��

	int min = jiffies / (60 * HZ);
	int sec = (jiffies % (60 * HZ)) / HZ;

	memset(&chardev,0,sizeof(chardev));
	memset(&fops,0,sizeof(fops));
	memset(&tasklet,0,sizeof(tasklet));

	cdev_init(&chardev,&fops);
	cdev_add(&chardev,MKDEV(99,0),1);
	
	/*thread_info*/
	tinfo = current_thread_info();
	get = tinfo->task;
	//get->pid,get->comm
	
	//lock����ʽ
	tinfo->preempt_count = 1;	//�����ں��̼߳���  ���� preempt_disable();
	//....				//�����߼�
	tinfo->preempt_count = 0;	//����	����		preempt_enable();
	
	spin_lock_t lock;

	spin_lock_init(&lock);
	spin_lock(&lock);
	spin_unlock(&lock);
	
	spin_lock_irq();		// = spin_lock() + local_irq_disable();
	spin_unlock_irq();		// = spin_unlock() + local_irq_enable();
	
	/*��ֹ�ж�*/
	local_irq_disable();
	local_irq_enable();

	local_irq_save(flag);
	local_irq_restore(flag);

	/*tasklet*/
	tasklet_init(&tasklet,tasklet_handle,data);
	
	tasklet_schedule(&tasklet);		//һ�����ж���ִ�и����
	
	/**timer*/
	init_timer(&timer);
	mytimer.expires = jiffies + 1 * HZ;	//һ���ִ��
	mytimer.function = timer_handler;
	mytimer.data = 456;

	add_timer(&timer);

	/*schedule */
	set_current_state(TASK_INTERRUPTIBLE);	//current->state = TASK_INTERRUPTIBLE;
	schedule_timeout( 3 * HZ);		//�ӳ�3��
	
	if (timer_before(jiffies, jiffies + 3 * HZ)){
		schedule();			//�ó�CPU�����ں������߳�ʹ��
	}
	
	/*��������*/
	struct work_struct mywork;
	INIT_WORK(&mywork, work_handler);
	
	schedule_work(&mywork);
	//schedule_delayed_work(&mywork,jiffies + 1 * HZ);
	
	/*�ź�����ʹ��*/
	struct semaphore sema;

	sema_init(&sema,1);

	down(&sema);		//�൱�����
	while(1){
		...;
		break;
	}
	up(&sema);
	down_interruptible(&sema);
	down_trylock(&sema);

	/*�����*/
	struct completion done;
	init_completion(&done);
	
	wait_for_completion(&done);
	wait_for_completion_interruptible(&done);
	completion(&done);	//����ж���ȴ��ģ���������һ��
	completion_all(&done);	//�������еȴ������

	//ԭ�Ӳ���
	atomic_t a;
	int tmp;

	atomic_set(&a, 100); //a = 100;
	atomic_add(5, &a);//a += 5;
	atomic_sub(10, &a);//a -= 10;
	tmp = atomic_add_return(5, &a); // tmp = (a += 5); 
	tmp = atomic_sub_return(10, &a);// tmp = (a -= 10);
	atomic_inc(&a); //a += 1;
	atomic_dec(&a); //a -= 1;

	//�ȴ�����
	wait_queue_head_t wqh;
	int flag = 0;
	
	init_wait_queue_head(&wqh);
	wait_event(&wqh, flag == 1);
	
	flag = 1;
	wake_up(&wqh);
	
	/*�ڴ����*/
	char *p = NULL;

	p = kzalloc(SZ_4K,GFP_KERNEL);	//GFP_KERNEL:�ں��ڴ����������. ����˯��. //GFP_ATOMIC:�������жϴ���ͽ���������֮������������з����ڴ�. �Ӳ�˯��.
	//kmalloc�ܹ��������С����ʱ32��64�ֽڣ�ͬʱ���ܷ������128K���ڴ����
	kfree(p);

	p = vmalloc(512 * 1024);	//�������������ڴ棬kmalloc�������������ڴ�
	vfree(p);
	
	
	
	/*kemem*/
	kmem_cache_t *testc_cache;
	struct file_operations *fp;

	ctest_cache = kmem_cache_create("shawn", sizeof(file_operations),SLAB_HWCACHE_ALIGN,0, NULL);//���һ������ctor��void *�� Ϊ�ṹ���ʼ������
	fp = kmem_cache_alloc(ctest_cache,GFP_KERNEL);
	fp.open = ...;
	fp.release = ...;
	...;
	kmem_cache_free(ctest_cache, fp);
	kmem_cache_destroy(ctest_cache);
	
	/*mempool*/
	mempool_t *pool;

	pool = mempool_create(10,mempool_alloc_slab, mempool_free_slab,testc_cache);
	fp = mempool_alloc(pool, GFP_KERNEL);
	mempool_free(fp,pool);
	mempool_destroy(pool);

	
	/*alloc_page*/
	struct page *p;
	char *buf = NULL;

	p = alloc_page(GFP_KERNEL);
	buf = page_address(p);

	__free_page(p);
	
	p = alloc_pages(GFP_KERNEL,1);
	buf = page_address(p);
	__free_pages(p,1);

	buf = __get_free_pages(GFP_KERNEL, 1);
	free_pages(buf,1);
	
	/*cdev �����÷�*/
	struct class *test_class;
	int gmajor	 = 0;

	test_class = class_create(THIS_MODULE, "classcdev");
	gmajor = register_chrdev(0, "cdev", &fps);
	device_create(test_class,NULL,MKDEV(gmajor,0),NULL,"testcdev");
	//�����������豸������/dev������ʾ



	printk("[%s:%d]\n",__func__,__LINE__);
}

static __exit void module_release()
{

	printk("[%s:%d]\n",__func__,__LINE__);

}

module_init(preempt_module_init);
module_exit(module_release);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shawn Zhang");
