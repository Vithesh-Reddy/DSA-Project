#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include "priority_queue.h"

#define infinite 100005

// creates a priority queue with a particular size
pqueue create_pqueue(length size)
{
    pqueue pq = (pqueue)malloc(sizeof(min_heap));
    pq->size = size;
    pq->heap = (element_type *)malloc(size * sizeof(element_type));
}

// checks if the heap is empty or not
int is_pqueue_empty(pqueue pq)
{
    if (pq->heap_size <= 0)
        return 1;
    else
        return 0;
}

// swaps two elements of the heap
void swap(element_type *a, element_type *b, length i[])
{
    length temp_id = i[(*a).vertex_id];
    i[(*a).vertex_id] = i[(*b).vertex_id];
    i[(*b).vertex_id] = temp_id;
    element_type t;
    t.key = (*a).key;
    t.vertex_id = (*a).vertex_id;
    (*a).key = (*b).key;
    (*a).vertex_id = (*b).vertex_id;
    (*b).key = t.key;
    (*b).vertex_id = t.vertex_id;
}

//function to get right child of an element in the heap
length get_right_child(pqueue pq, length index)
{
    if ((((2 * index) + 2) < pq->size) && (index >= 0))
        return (2 * index) + 2;
    return -1;
}

//function to get left child of an element in the heap
length get_left_child(pqueue pq, length index)
{
    if (((2 * index) + 1< pq->size) && (index >= 0))
        return (2 * index)+1;
    return -1;
}

//function to get the parent of an element in the heap
length get_parent(pqueue pq, length index)
{
    if ((index > 0) && (index < pq->size))
    {
        return (index-1) / 2;
    }
    return -1;
}

void min_heapify(pqueue pq, length index, length i[])
{
    length left_child_index = get_left_child(pq, index);
    length right_child_index = get_right_child(pq, index);

    // finding smallest among index, left child and right child
    length smallest = index;

    if ((left_child_index < pq->heap_size) && (left_child_index >= 0))
    {
        if (pq->heap[left_child_index].key < pq->heap[smallest].key)
        {
            smallest = left_child_index;
        }
    }

    if ((right_child_index < pq->heap_size && (right_child_index >= 0)))
    {
        if (pq->heap[right_child_index].key < pq->heap[smallest].key)
        {
            smallest = right_child_index;
        }
    }

    // smallest is not the element, element is not a heap
    if (smallest != index)
    {
        swap(&pq->heap[index], &pq->heap[smallest], i);
        min_heapify(pq, smallest, i);
    }
}

// builds the heap
void build_min_heap(pqueue pq, length i[])
{
    length j;
    for (j = (pq->heap_size / 2)-1; j >= 0; j--)
    {
        min_heapify(pq, j, i);
    }
}

// returns the minimum element (top element) of the heap
element_type minimum(pqueue pq)
{
    return pq->heap[1];
}

// extracts the minimum element of the heap and deletes it
element_type extract_min(pqueue pq, length i[])
{
    element_type ex_min = pq->heap[0];
    pq->heap[0].key = pq->heap[pq->heap_size-1].key;
    pq->heap[0].vertex_id = pq->heap[pq->heap_size-1].vertex_id;
    i[pq->heap[0].vertex_id] = 0;
    pq->heap_size--;
    min_heapify(pq, 0, i);
    return ex_min;
}

// adjusts the position of the element in the heap according to its key value
// Driver function 
void decrease_key(pqueue pq, length vertex_id, length key, length i[])
{
    element_type e;
    e.vertex_id = vertex_id;
    e.key = key;
    decrease_key_helper(pq, i[e.vertex_id], e, i);
}

// Helper function
void decrease_key_helper(pqueue pq, length index, element_type e, length i[])
{
    pq->heap[index].key = e.key;
    pq->heap[index].vertex_id = e.vertex_id;
    i[e.vertex_id] = index;
    while ((index > 0)  && (pq->heap[get_parent(pq, index)].key > pq->heap[index].key))
    {
        swap(&pq->heap[index], &pq->heap[get_parent(pq, index)], i);
        index = get_parent(pq, index);
    }
}

// inserts an element in the heap
void insert_pqueue(pqueue pq, length vertex_id, length key, length i[])
{
    element_type e;
    e.vertex_id = vertex_id;
    e.key = key;
    pq->heap[pq->heap_size].key = key;
    pq->heap[pq->heap_size].vertex_id = vertex_id;
    i[vertex_id] = pq->heap_size;
    pq->heap_size++;
    decrease_key_helper(pq, pq->heap_size-1, e, i);
}

// prints the elements of the heap
void print_heap(pqueue pq)
{
    length i;
    for (i = 0; i < pq->heap_size; i++)
    {
        printf("(%ld) %ld ", pq->heap[i].vertex_id, pq->heap[i].key);
    }
    printf("\n");
}