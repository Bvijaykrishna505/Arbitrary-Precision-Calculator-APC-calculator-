#ifndef APC_H
#define APC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * CONSTANTS
 * ============================================================================ */
#define SUCCESS 0
#define FAILURE -1

/* ============================================================================
 * TYPE DEFINITIONS
 * ============================================================================ */

typedef int data_t;  // Type for storing a single digit (0-9)

/* ----------------------------------------------------------------------------
 * Dlist: Double linked list node
 * Each node stores one digit of a large number
 * Example: Number 123 is stored as: NULL<-[1]<->[2]<->[3]->NULL
 * ---------------------------------------------------------------------------- */
typedef struct node
{
    struct node *prev;  // Pointer to previous digit
    data_t data;        // Single digit (0-9)
    struct node *next;  // Pointer to next digit
} Dlist;

/* ============================================================================
 * DOUBLE LINKED LIST OPERATIONS
 * Functions to manipulate Dlist (double linked list)
 * ============================================================================ */
int dl_insert_first(Dlist **head, Dlist **tail, int data);  // Insert at beginning
int dl_insert_last(Dlist **head, Dlist **tail, int data);   // Insert at end
int dl_delete_first(Dlist **head, Dlist **tail);           // Delete first node
int dl_delete_list(Dlist **head, Dlist **tail);             // Delete entire list
void print_list(Dlist *head);                               // Print list
void print_list_formatted(Dlist *head, Dlist *tail);        // Print with formatting
int get_list_length(Dlist *head);                          // Count nodes
int get_formatted_width(Dlist *head, int has_minus);        // Calculate display width of formatted number
int get_string_display_width(const char *str);               // Calculate display width of string (emojis count as 1)
void print_border_top(int width);                           // Print top border
void print_border_middle(int width);                        // Print middle border
void print_border_bottom(int width);                        // Print bottom border
void print_padded_line(int max_width, int content_width);    // Print padding and closing border

/* ============================================================================
 * INPUT PARSING
 * Convert string representation to double linked list
 * ============================================================================ */
int stored_num(Dlist **head, Dlist **tail, char *data);  // Returns sign (+1 or -1)

/* ============================================================================
 * ARITHMETIC OPERATIONS
 * Perform addition, subtraction, multiplication, and division on large numbers
 * ============================================================================ */
int add_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3);
int sub_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3, char *op1, char *op2);
int mul_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3);
int div_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3, Dlist **remainder_head, Dlist **remainder_tail, char ch);

/* ============================================================================
 * HELPER FUNCTIONS
 * ============================================================================ */

// Addition helper
int update_result_add(Dlist **head, Dlist **tail, int result, int *carry);

// Utility functions
void trim_leading_zeroes(Dlist **head, Dlist **tail);  // Remove leading zeros
int is_zero(Dlist *head);  // Check if list represents zero

// Subtraction helpers
int compare_numbers(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
                    char *op1, char *op2);
void swap_lists(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2);

// Division helpers
int is_greater_or_equal(Dlist *head1, Dlist *head2);  // Compare two numbers
void subtract_lists(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
                    Dlist **result_head, Dlist **result_tail);  // Subtract two lists
void copy_list(Dlist *src_head, Dlist **dest_head, Dlist **dest_tail);  // Copy a list

#endif // APC_H
