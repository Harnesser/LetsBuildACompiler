#ifndef _list_algos_h
#define _list_algos_h

#include <lcthw/list.h>

typedef int (*List_compare)( ListNode *node1, ListNode *node2);
int List_bubble_sort( List *list, List_compare compare_fn );
List* List_merge( List *left, List *right, List_compare compare_fn);
List* List_merge_sort( List *list, List_compare compare_fn );

#endif

