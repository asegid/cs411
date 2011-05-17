/*
 * CS 411 Team 08
 * C-Look I/O Scheduler
 * Team members:
 * Jake Cray
 * Torben Rasmussen 
 * Helen Shin
 * Rodney Keeling
 */
/*
 * Changed noop_data to include the read/write head location
 * Changed noop_init to initialize head location to 0
 * Changed noop_... functions to be named clook_...
 * Changes that need to be made: 
 * Modify dispatch function
 * Modify how add requests are handled
 *      Search through list, and insert new link in proper spot. Should be O(n) for search and add.
 * Modify dispatch
 *      Should be O(1), Rob suggested keeping track of current position and setting prev of curr to be dispatched I believe
 *      Use elevator_dispatch_add_tail or something like that instead of elevator_dispatch_sort -- dont care about sector order
 *      Needs to run as quickly as possible
 *      former/latter
 *          former - should return request to be dispatched when acessed from the current arg/node
 *          latter - return next request to handle
 */
/*
 * elevator clook
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

struct clook_data {
    struct list_head queue;
    long head_loc; // Location of the head
};

static void clook_merged_requests(struct request_queue *q, struct request *rq,
        struct request *next)
{
    list_del_init(&next->queuelist);
}

static int clook_dispatch(struct request_queue *q, int force)
{
    struct clook_data *cd = q->elevator->elevator_data;

    if (!list_empty(&cd->queue)) {
        struct request *rq;
        rq = list_entry(cd->queue.next, struct request, queuelist);
        list_del_init(&rq->queuelist);
        elv_dispatch_sort(q, rq);
        return 1;
    }
    return 0;
}

static void clook_add_request(struct request_queue *q, struct request *rq)
{
    struct clook_data *cd = q->elevator->elevator_data;

        // Instead of adding to end, iterate through queue to find correct position
        list_for_each_entry(rq, cd->queue, queuelist) {
        
        
        }
}

static int clook_queue_empty(struct request_queue *q)
{
    struct clook_data *cd = q->elevator->elevator_data;

    return list_empty(&cd->queue);
}

    static struct request *
clook_former_request(struct request_queue *q, struct request *rq)
{
    struct clook_data *cd = q->elevator->elevator_data;

    if (rq->queuelist.prev == &cd->queue)
        return NULL;
    return list_entry(rq->queuelist.prev, struct request, queuelist);
}

    static struct request *
clook_latter_request(struct request_queue *q, struct request *rq)
{
    struct clook_data *cd = q->elevator->elevator_data;

    if (rq->queuelist.next == &cd->queue)
        return NULL;
    return list_entry(rq->queuelist.next, struct request, queuelist);
}

static void *clook_init_queue(struct request_queue *q)
{
    struct clook_data *cd;
    cd = kmalloc_node(sizeof(*cd), GFP_KERNEL, q->node);
    if (!cd)
        return NULL;
    INIT_LIST_HEAD(&cd->queue);
    cd->head_loc = 0; //init head location as 0
    return cd;
}

static void clook_exit_queue(struct elevator_queue *e)
{
    struct clook_data *cd = e->elevator_data;

    BUG_ON(!list_empty(&cd->queue));
    kfree(*cd);
}

static struct elevator_type elevator_clook = {
    .ops = {
        .elevator_merge_req_fn		= clook_merged_requests,
        .elevator_dispatch_fn		= clook_dispatch,
        .elevator_add_req_fn		= clook_add_request,
        .elevator_queue_empty_fn	= clook_queue_empty,
        .elevator_former_req_fn		= clook_former_request,
        .elevator_latter_req_fn		= clook_latter_request,
        .elevator_init_fn		= clook_init_queue,
        .elevator_exit_fn		= clook_exit_queue,
    },
    .elevator_name = "clook",
    .elevator_owner = THIS_MODULE,
};

/* Allocates and initializes any data structures or other memory you will need
 * to make your scheduler work. For example, a list_head structure to represent
 * the head of your sorted request list; called when your scheduler is selected to
 * handle scheduling for a disk.
 */
static void elevator_init_fn()
{

}


/* Allocates and initializes any memory you need to associate with an 
 * individual request and stores it in one or both of the request's 
 * elevator_private and elevator_private2 fields.
 */
static void elevator_set_req_fn()
{

}


/* takes an I/O request from the kernel and inserts it into your scheduler in 
 * whatever sorted order you choose.
 */
static void elevator_add_req_fn()
{

}


/* takes the next request to be serviced from your scheduler's list and 
 * submits it to the dispatch queue.
 */
static void elevator_dispatch_fn()
{

}


/* deallocates memory allocated by elevator_set_req_fn; called after 
 * elevator_dispatch_fn
 */
static void elevator_put_req_fn()
{

}


/* deallocates memory allocated in elevator_init_fn; called when your 
 * scheduler is relieved of its scheduling duties for a disk.
 */
static void elevator_exit_req_fn()
{

}


/* tells the kernel whether or not your scheduler is holding any pending requests
*/
static void elevator_queue_empty_fn()
{

}


/* dunno */
static void elevator_former_req_fn()
{

}


/* dunno */
static void elevator_latter_req_fn()
{

}


static int __init clook_init(void)
{
    elv_register(&elevator_clook);

    return 0;
}

static void __exit clook_exit(void)
{
    elv_unregister(&elevator_clook);
}

module_init(clook_init);
module_exit(clook_exit);


MODULE_AUTHOR("Jake Cray, Rodney Keeling, Torben Rasmussen, Helen Shin");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Clook IO scheduler");