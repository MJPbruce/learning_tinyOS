#ifndef _T_TASK__H
#define _T_TASK__H


// 前置声明
struct _t_event;


typedef uint32_t tTastStack;

#define TINYOS_TASK_STATE_RDY				0
#define TINYOS_TASK_STATE_DELAYED		(0x1<<1)
#define TINYOS_TASK_STATE_SUSPEND		(0x1<<2)

#define TINYOS_TASK_WAIT_MASK       (0xFF << 16)



typedef struct _tTask
{
	//记录了当前任务的堆栈指针
	tTastStack *stack;
	
	//任务列表
	tnode link_node;
	//延时时间
	uint32_t delay_ticks;
	
	//延时结点， 通过delay_node就可以将tTask加入到延时队列中来
	tnode delay_node;
	
	//任务的优先级
	uint32_t prio;
	
	//任务的状态标志
	uint32_t state;
	
	//时间片 最小单位也是时间片的整数倍
	uint32_t slice;
	
	//任务挂起计数
	uint32_t suspend_count;
	
	//任务被删除时调用的清理函数
	void (*clean)(void * clean_param);
	//清理函数传入的参数
	void * clean_param;
	
	//任务请求删除标志 非0表示请求删除
	uint8_t task_request_delete_flag;
	
	//任务正在等待的事件类型
	struct _t_event * wait_event;
	//等待事件的消息存储位置
	void * event_msg;
	//等待事件的返回结果
	uint32_t wait_event_resalt;
	
}tTask;

typedef struct _task_info
{
	uint32_t delay_ticks;
	uint32_t suspend_count;
	uint32_t slice;
	uint32_t state;
	uint32_t prio;

}task_info;


//获取任务状态信息
void t_get_task_info(tTask * task, task_info * info);
/**********************************************************************************************************
** Function name        :   ttask_sched_ready
** Descriptions         :   将任务设置为就绪状态
** input parameters     :   task    等待设置为就绪状态的任务
** output parameters    :   无
** Returned value       :   无
***********************************************************************************************************/
void t_task_sched_ready (tTask * task);

/**********************************************************************************************************
** Function name        :   tSchedulerUnRdyTask
** Descriptions         :   tTaskSchedUnRdy
** Descriptions         :   将任务从就绪列表中移除
** input parameters     :   task   
** output parameters    :   None
** Returned value       :   None
***********************************************************************************************************/
void t_task_sched_unready (tTask * task);

//任务挂起
void t_task_suspend(tTask * task);
//挂起任务的恢复
void t_suspend_task_wake_up(tTask * task);

//task部分
void tTaskInit (tTask *task, void (*entry)(void *), void *param, uint32_t prio,  tTastStack * stack);

/**********************************************************************************************************
** Function name        :   tTaskForceDelete
** Descriptions         :   强制删除指定的任务
** parameters           :   task  需要删除的任务
** Returned value       :   无
***********************************************************************************************************/
void t_task_force_delete (tTask * task) ;


/**********************************************************************************************************
** Function name        :   tTaskRequestDelete
** Descriptions         :   请求删除某个任务，由任务自己决定是否删除自己
** parameters           :   task  需要删除的任务
** Returned value       :   无
***********************************************************************************************************/
void t_task_request_delete (tTask * task);

/**********************************************************************************************************
** Function name        :   tTaskIsRequestedDelete
** Descriptions         :   是否已经被请求删除自己
** parameters           :   无
** Returned value       :   非0表示请求删除，0表示无请求
***********************************************************************************************************/
uint8_t t_task_is_requested_delete (void);

/**********************************************************************************************************
** Function name        :   tTaskDeleteSelf
** Descriptions         :   删除自己
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void t_task_delete_self (void);



/**********************************************************************************************************
** Function name        :   tTaskSetCleanCallFunc
** Descriptions         :   设置任务被删除时调用的清理函数
** parameters           :   task  待设置的任务
** parameters           :   clean  清理函数入口地址
** parameters           :   param  传递给清理函数的参数
** Returned value       :   无
***********************************************************************************************************/
void t_task_set_clean_call_func (tTask * task, void (*clean)(void * param), void * param);








#endif

