/*************************************************************************
	> File Name: list_head.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月11日 星期二 10时38分59秒
 ************************************************************************/
#ifndef		LIST_HEAD_H
#define		LIST_HEAD_H
struct list_head{
	struct 	list_head * prev, * next;
};

#define	LIST_HEAD_INIT(name)	{(&name), (&name)  }
#define	LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)


#define	INIT_LIST_HEAD(ptr) do{ \
	(ptr)->prev = (ptr); \
	(ptr)->next = (ptr); \
}while(0)
static inline void __list_add(struct list_head *new, struct list_head *head, struct list_head *next)
{
	new->next = next;
	next->prev = new;
	head->next = new;
	new->prev = head;
}
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	prev->next = next;
	next->prev = prev;
}
static inline void list_add(struct list_head *new,struct list_head *head)
{
	printf("list_add..");
	__list_add(new,head,head->next);
}
static inline void list_add2()
{
	printf("list_add..");
}
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev,entry->next);
}
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({ \
		const typeof( ((type *)0)->member ) *__mptr = (ptr);\
		(type *)( (char *)__mptr - offsetof(type,member) );})
#define	list_for_each(entry, head)	\
	for(entry=(head)->next;entry!=head;entry=entry->next)

#define	list_entry(ptr,type,member)	container_of(ptr,type,member)
#endif
