#ifndef LIST_H
#define LIST_H

struct list_head {
    struct list_head *next, *prev;
};


/****************************************************************/
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

//这种宏定义的方法，将节点定义的同时，并且初始化，如果定义好了，就不适合这种方法了

/****************************************************************/
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}
//这种方法是节点在外部定义完成后，调用该函数，给成员赋值
/****************************************************************/
/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
#ifndef CONFIG_DEBUG_LIST
static inline void __list_add(struct list_head *new,
                  struct list_head *prev,
                  struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}
#else
extern void __list_add(struct list_head *new,
                  struct list_head *prev,
                  struct list_head *next);
#endif

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
#ifndef CONFIG_DEBUG_LIST
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}
#else
extern void list_del(struct list_head *entry);
#endif




#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)


/**
 * container_of - cast a member of a structure out to the containing structure 根据结构体重的一个成员变量地址导出包含这个成员变量mem的struct地址
 * @ptr:    the pointer to the member. 结构体变量中某个成员的地址
 * @type:   the type of the container struct this is embedded in. 结构体类型
 * @member: the name of the member within the struct.  该结构体变量的具体名字
 *
 */

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)



#define list_for_each_entry(pos, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = list_entry(pos->member.next, typeof(*pos), member)) \

	
typedef struct
{
	char	*name;
	char	*usage;
    int 	(*exec)(int argc,char *argv[]);
    struct list_head list;
}Cmd_List_t;

#define cmd_list_create_node(name,usage,cmd)   static Cmd_List_t name = { #name, usage,cmd, &(name.list), &(name.list) }
#define cmd_list_linked_list_tail(Name,Head)   list_add_tail(&Name.list,&Head)


#endif // LIST_H
