#include <lcthw/list_algos.h>
#include <lcthw/list.h>
#include <lcthw/dbg.h>

/* List_bubble_sort will sort the List object with a bubble sort. */
int List_bubble_sort( List *list, List_compare compare_fn )
{
	int flipped = 1;

	while (flipped != 0) {
		flipped = 0;
		LIST_FOREACH(list, first, next, cur) {
			if( cur->next ) {
				if(compare_fn(cur->value, cur->next->value) > 0){
					flipped++;
					List_swap_nodes(cur, cur->next);
				}
			}
		}
	}

	return 0;
}

List* List_merge( List *left, List *right, List_compare compare_fn)
{
	List *result = List_create();

	while( ( List_count(left) > 0 ) || ( List_count(right) > 0 ) ){
		if( ( List_count(left) > 0 ) && ( List_count(right) > 0 ) ) {
			if( compare_fn( List_first(left), List_first(right) ) < 0 ) {
				List_push(result, List_unshift(left) );
			} else {
				List_push(result, List_unshift(right) );
			}
		} else if ( List_count(left) > 0 ) {
			List_push(result, List_unshift(left) );
		} else if ( List_count(right) > 0 ) {
			List_push(result, List_unshift(right) );
		}
				
	}

	log_info(">>>> List >>>>>:");
	LIST_FOREACH(result, first, next, cur) {
		log_info("  %s", (char*)cur->value);
	}
	return result;
}


List* List_merge_sort(List *list, List_compare compare_fn)
{
	int middle, i;
	//List *sorted = List_create();

	// if list size is 1, consider it sorted and return it
	if( List_count(list) <= 1 ){
		return list;
	}

	// else list size is > 1, so split the list into two sublists
	List *left = List_create();
	List *right = List_create();
	log_info("Initialised lists");

	middle = List_count(list) / 2;
	log_info("Middle %d", middle);
	ListNode *cur = list->first;
	for( i = 0; i < middle; i++ ) {
		List_push(left, cur->value);
		cur = cur->next;
	}
	log_info("Built 'left' list.");
	for( i = middle; i < List_count(list); i++ ) {
		List_push(right, cur->value);
		cur = cur->next;
	}
	log_info("Built 'right' list.");

#ifndef TESTME
	ListNode *tmp;
	log_info("LISTS---------------------------------------");
	log_info("LEFT:");
	for( tmp = left->first; tmp != NULL; tmp = tmp->next ){
		log_info(" %s", (char*)tmp->value );
	}

	log_info("RIGHT:");
	for( tmp = right->first; tmp != NULL; tmp = tmp->next ){
		log_info(" %s", (char*)tmp->value );
	}
	log_info("--------------------------------------------");

	// recursively call merge_sort() to further split each sublist
	// until sublist size is 1
	left = List_merge_sort(left, compare_fn);
	right = List_merge_sort(right, compare_fn);
#endif

	// merge the sublists returned from prior calls to merge_sort()
	// and return the resulting merged sublist
	return List_merge(left, right, compare_fn);
}
