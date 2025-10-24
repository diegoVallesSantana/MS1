/**
 * \author Jeroen Van Aken, Bert Lagaisse, Ludo Bruynseels
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "dplist.h"


/*
 * The real definition of struct list / struct node
 */
struct dplist_node {
    dplist_node_t *prev, *next;
    element_t element;
};

struct dplist {
    dplist_node_t *head;
    int sizeOfList;
    // more fields will be added later
};

dplist_t *dpl_create() {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
    list->sizeOfList = 0;
  return list; //Returns a pointer to the newly created empty list
}

/** Deletes all elements in the list
 * - Every list node of the list must be deleted. (free memory)
 * - The list itself also needs to be deleted. (free all memory)
 * - '*list' must be set to NULL.
 * \param list a double pointer to the list
*/
void dpl_free(dplist_t **list) {
    //dplist_node_t *dummy = NULL;
    //dummy = (*list)->head;
    //while (dummy->next != NULL) {
    //    free((*dummy)->element);
    //    dummy->prev = NULL;
    //    dummy = dummy->next;
    //    count++;
    //}
    //(*list)->sizeOfList = 0
    //*list = NULL;
    //TODO: add your code here
    //Do extensive testing with valgrind. 

}

/* Important note: to implement any list manipulation operator (insert, append, delete, sort, ...), always be aware of the following cases:
 * 1. empty list ==> avoid errors
 * 2. do operation at the start of the list ==> typically requires some special pointer manipulation
 * 3. do operation at the end of the list ==> typically requires some special pointer manipulation
 * 4. do operation in the middle of the list ==> default case with default pointer manipulation
 * ALWAYS check that you implementation works correctly in all these cases (check this on paper with list representation drawings!)
 **/


dplist_t *dpl_insert_at_index(dplist_t *list, element_t element, int index) {
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) return NULL;

    list_node = malloc(sizeof(dplist_node_t)); //create new space in heap for new node

    list_node->element = element; // allocate element to freshly created node
    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1, if node is the first one in the list, header points to our node
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        list->sizeOfList += 1;
        // pointer drawing breakpoint
    } else if (index <= 0) { // covers case 2, to place the new node before the first node of the list, essentially becoming the new head
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        list->sizeOfList += 1;
        // pointer drawing breakpoint
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            list->sizeOfList += 1;
            // pointer drawing breakpoint
        } else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            list->sizeOfList += 1;
            // pointer drawing breakpoint
        }
    }
    return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index) {

    //TODO: add your code here
    return NULL;
}


int dpl_size(dplist_t *list) {
    if (list == NULL){
        return -1;
    }
    else {
        return list->sizeOfList;
    }
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {
    int count = 0 ;
    dplist_node_t *dummy = NULL;
    if (list == NULL) {return NULL;}
    if (list->sizeOfList==0) {return NULL;}
    if (index <=0) {
        dummy = list->head;
        return dummy;
    }
    dummy = list->head;
    while (dummy->next != NULL && count < index) {
        dummy = dummy->next;
        count++;
        }
    return dummy; // If we reached the end before reaching 'index', dummy points to the last node.
}


element_t dpl_get_element_at_index(dplist_t *list, int index) {

    //TODO: add your code here
    return '\e';
}

int dpl_get_index_of_element(dplist_t *list, element_t element) {

    //TODO: add your code here
    return -1;
}



