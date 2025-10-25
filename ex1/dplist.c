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

void dpl_free(dplist_t **list) {
    if (list == NULL) return;
    if (*list == NULL) return;
    dplist_node_t *dummy = (*list)->head;
    dplist_node_t *nextNode = NULL;
    while (dummy != NULL) { // not (dummy->next != NULL) bc this would exit before setting the last node free
        nextNode = dummy->next;
        //free(dummy->element); //necessary if element is a pointer
        free(dummy);
        dummy = nextNode;
    }
    free(*list);
    *list = NULL;
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

    if (list == NULL) return NULL;
    if (list->sizeOfList ==0) return list;
    dplist_node_t *dummy = list->head;
    //Case 1
    if (index <= 0) {
        list->head = dummy->next; //second node becomes first node
        if(list->head != NULL){
        list->head->prev = NULL;//new first node should not have a pointer to the previous first node
        }
        //free(dummy->element); //necessary if element is a pointer
        free(dummy);
        list->sizeOfList--;
        return list;
    }
    int counter = 0;
    //Cases 2 and 3 together
    while (dummy->next != NULL && counter < index) {
        dummy = dummy->next;
        counter++;
    }

    if (dummy->prev != NULL) {
        dummy->prev->next = dummy->next;
    }
    if (dummy->next != NULL) {
        dummy->next->prev = dummy->prev;
    }
    /*
    //Case 2
    int counter = 0;
    while (dummy->next != NULL) {
        if(counter == index) {
            if (dummy->prev != NULL) {
                dummy->prev->next = dummy->next;
            }
            if (dummy->next != NULL) {
                dummy->next->prev = dummy->prev;
            }
            //free(dummy->element); //necessary if element is a pointer
            free(dummy);
            list->sizeOfList--;
            return list;
        }
        dummy = dummy->next;
        counter++;
    }
    //Case 3
    if (dummy->prev != NULL) {
        dummy->prev->next = dummy->next;
    }
    if (dummy->next != NULL) {
        dummy->next->prev = dummy->prev;
    }
    */

    //free(dummy->element); //necessary if element is a pointer
    free(dummy);
    list->sizeOfList--;
    return list;
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
    if (list == NULL) return (element_t) 0; //should be NULL for pointers
    if (list->sizeOfList ==0) return (element_t) 0;

    dplist_node_t *dummy = list->head;
    if (index <= 0) {
        return list->head->element;
    }
    int counter = 0;
    while (dummy->next != NULL && counter < index) {
        dummy = dummy->next;
        counter++;
    }
    return dummy->element;
}

int dpl_get_index_of_element(dplist_t *list, element_t element) {
    if (list == NULL) return -1; //should be NULL for pointers
    if (list->sizeOfList ==0) return -1;
    if (list->head == NULL) return -1;

    dplist_node_t *dummy = list->head;
    int index = 0;

    while (dummy != NULL) {
        if (dummy->element == element){
            return index;
        }
        dummy = dummy->next;
        index++;
    }
    return -1;
}



