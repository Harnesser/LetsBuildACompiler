#include <lcthw/list.h>
#include <lcthw/dbg.h>

List *List_create()
{
	return calloc(1, sizeof(List));
}

void List_destroy(List *list)
{
	LIST_FOREACH(list, first, next, cur) {
		if(cur->prev) {
			free(cur->prev);
		}
	}
	free(list->last);
	free(list);
}

/* List_clear assumes the pieces of data are stored on the heap. */
void List_clear(List *list) 
{
	LIST_FOREACH(list, first, next, cur) {
		free(cur->value);
	}
}

void List_clear_destroy(List *list)
{
	List_clear(list);
	List_destroy(list);
}

void List_push(List *list, void *value)
{
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;
	if(list->last == NULL) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}
	list->count++;

error:
	return;
}

void *List_pop(List *list)
{
	ListNode *node = list->last;
	return node != NULL ? List_remove(list, node) : NULL;
}

void List_shift(List *list, void *value)
{
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;

	if(list->first == NULL) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}

	list->count++;

error:
	return;
}

void *List_unshift(List *list)
{
	ListNode *node = list->first;
	return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node)
{
	void *result = NULL;
	
	check(list->first && list->last, "List is empty.");
	check(node, "Node can't be null.");

	if(node == list->first && node == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else if( node == list->first) {
		list->first = node->next;
		check(list->first != NULL, "Invalid list, somehow first is NULL");
		list->first->prev = NULL;
	} else if( node == list->last ) {
		list->last = node->prev;
		check(list->last != NULL, "Invalid list, somehow a next is NULL");
		list->last->next = NULL;
	} else {
		ListNode *after = node->next;
		ListNode *before = node->prev;
		after->prev = before;
		before->next = after;
	}

	list->count--;
	result = node->value;
	free(node);

error:
	return result;
}

/* List_swap_nodes swaps the values of two given ListNodes 
This is easier than juggling the prev and next pointers.
I can't see a downside for this implementation. */ 
void List_swap_nodes(ListNode *node1, ListNode *node2){
	check(node1, "node1 can't be NULL");
	check(node2, "node2 can't be NULL");

	void *tmp = node2->value;
	node2->value = node1->value;
	node1->value = tmp;

error:
	return;
}
