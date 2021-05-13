#ifndef PQ_H
#define PQ_H

typedef long int length;

typedef struct min_heap min_heap;
typedef min_heap *pqueue;

typedef struct element_type element_type;

// NOTE - Priority queue used here is a min heap


// This struct contains the min heap parameters
struct min_heap
{
    length size;        // maximum size of the min heap
    element_type *heap; // pointer to the array containing elements
    length heap_size;   // stores the heap size
};

// This struct contains the parameters of each element in the heap
struct element_type
{
    length vertex_id; // to store the vertex id of the currency
    length key; // to store the dv value of currency
};

// These are the functions whose detailed procedure and implementation is provided in <priority_queue.c>
pqueue create_pqueue(length size);
void swap(element_type *a, element_type *b, length i[]);
length get_right_child(pqueue pq, length index);
length get_left_child(pqueue pq, length index);
length get_parent(pqueue pq, length index);
void min_heapify(pqueue pq, length index, length i[]);
void build_min_heap(pqueue pq, length i[]);
element_type minimum(pqueue pq);
element_type extract_min(pqueue pq, length i[]);
void decrease_key(pqueue pq, length vertex_id, length key, length i[]);
void decrease_key_helper(pqueue pq, length index, element_type e, length i[]);
void insert_pqueue(pqueue pq, length vertex_id, length key, length i[]);
void print_heap(pqueue pq);
int is_pqueue_empty(pqueue pq);

#endif