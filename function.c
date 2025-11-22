#include "apc.h"

/* ============================================================================
 * DOUBLE LINKED LIST OPERATIONS
 * These functions work with Dlist (double linked list nodes)
 * ============================================================================ */

/**
 * Insert a new node at the end of the double linked list
 * @param head: pointer to head pointer of the list
 * @param tail: pointer to tail pointer of the list
 * @param data: digit to insert (0-9)
 * @return: SUCCESS if insertion successful, FAILURE otherwise
 */
int dl_insert_last(Dlist **head, Dlist **tail, int data)
{
    // Allocate memory for new node
    Dlist *new_node = malloc(sizeof(Dlist)); // Create new node
    if (new_node == NULL)
        return FAILURE; // Return error if memory allocation fails

    // Initialize new node
    new_node->data = data; // Store the digit (0-9)
    new_node->prev = NULL; // Will be set below
    new_node->next = NULL; // Will be last node

    // If list is empty, new node becomes both head and tail
    if (*head == NULL)
    {
        *head = new_node; // First node is head
        *tail = new_node; // First node is also tail
        new_node->prev = NULL; // No previous node
    }
    else
    {
        // Add new node after current tail and update tail
        new_node->prev = *tail; // Link new node's prev to current tail
        (*tail)->next = new_node; // Link current tail's next to new node
        *tail = new_node; // Update tail to point to new node
    }
    return SUCCESS; // Insertion successful
}

/**
 * Insert a new node at the beginning of the double linked list
 * @param head: pointer to head pointer of the list
 * @param tail: pointer to tail pointer of the list
 * @param data: digit to insert (0-9)
 * @return: SUCCESS if insertion successful, FAILURE otherwise
 */
int dl_insert_first(Dlist **head, Dlist **tail, int data)
{
    // Allocate memory for new node
    Dlist *new_node = malloc(sizeof(Dlist)); // Create new node
    if (new_node == NULL)
        return FAILURE; // Return error if memory allocation fails

    // Initialize new node
    new_node->data = data; // Store the digit (0-9)
    new_node->prev = NULL; // New node will be first, so no previous
    new_node->next = *head; // Point to current head

    // If list was empty, new node is also the tail
    if (*head == NULL)
    {
        *tail = new_node; // Only node is both head and tail
    }
    else
    {
        (*head)->prev = new_node; // Update current head's prev pointer
    }
    
    // Update head to point to new node (new node becomes first)
    *head = new_node;
    return SUCCESS; // Insertion successful
}

/**
 * Delete the first node from the double linked list
 * @param head: pointer to head pointer of the list
 * @param tail: pointer to tail pointer of the list
 * @return: SUCCESS if deletion successful, FAILURE if list is empty
 */
int dl_delete_first(Dlist **head, Dlist **tail)
{
    if (*head == NULL)
        return FAILURE; // Return error if list is empty

    // Save current head to free later
    Dlist *node_to_delete = *head; // Store pointer to node being deleted
    
    // Move head to next node (second node becomes new head)
    *head = (*head)->next;

    // If list becomes empty, update tail to NULL
    if (*head == NULL)
    {
        *tail = NULL; // List is now empty, tail should be NULL
    }
    else
    {
        (*head)->prev = NULL; // New head has no previous node
    }

    // Free the deleted node (prevent memory leak)
    free(node_to_delete);
    return SUCCESS; // Deletion successful
}

/**
 * Delete all nodes in the double linked list and free memory
 * @param head: pointer to head pointer of the list
 * @param tail: pointer to tail pointer of the list
 * @return: SUCCESS
 */
int dl_delete_list(Dlist **head, Dlist **tail)
{
    if (*head == NULL)
    {
        *tail = NULL; // Ensure tail is NULL if list is empty
        return SUCCESS; // Nothing to delete
    }

    // Traverse list and free each node (prevent memory leaks)
    Dlist *current = *head; // Start from head
    Dlist *next_node; // Will store next node pointer

    while (current != NULL) // Process all nodes
    {
        next_node = current->next;  // Save next before freeing (can't access after free)
        free(current);               // Free current node
        current = next_node;          // Move to next node
    }

    // Reset head and tail pointers (list is now empty)
    *head = NULL; // Clear head pointer
    *tail = NULL; // Clear tail pointer
    return SUCCESS; // Deletion successful
}

/**
 * Print all digits in the double linked list from head to tail
 * @param head: pointer to head of the list
 */
void print_list(Dlist *head)
{
    while (head != NULL) // Traverse the entire list
    {
        printf("%d", head->data); // Print each digit
        head = head->next; // Move to next node
    }
}

/**
 * Print list with digit grouping for better readability
 * Groups digits in sets of 3 (e.g., 1234567 -> 1,234,567)
 * @param head: pointer to head of the list
 * @param tail: pointer to tail of the list
 */
void print_list_formatted(Dlist *head, Dlist *tail)
{
    if (head == NULL)
    {
        printf("0"); // Print zero if list is empty
        return;
    }

    // Count total digits first
    int total_digits = 0; // Initialize counter
    Dlist *temp = head; // Temporary pointer
    while (temp != NULL) // Count all digits
    {
        total_digits++;
        temp = temp->next; // Move to next node
    }

    // Print with grouping (single pass through list)
    int position = 0; // Current position in list
    temp = head; // Pointer to current node
    while (temp != NULL) // Process all nodes
    {
        printf("%d", temp->data); // Print current digit
        position++; // Increment position counter
        temp = temp->next; // Move to next node
        
        // Add comma after every 3 digits (except for the last group)
        if (temp != NULL && (total_digits - position) % 3 == 0) // Check if comma needed
        {
            printf(","); // Add comma separator for readability
        }
    }
}

/**
 * Count the number of nodes in the double linked list
 * @param head: pointer to head of the list
 * @return: number of nodes in the list
 */
int get_list_length(Dlist *head)
{
    int count = 0; // Initialize counter
    while (head != NULL) // Traverse entire list
    {
        count++; // Increment for each node
        head = head->next; // Move to next node
    }
    return count; // Return total number of nodes
}

/**
 * Calculate the display width of a formatted number (including commas)
 * @param head: pointer to head of the list
 * @param has_minus: 1 if number is negative (needs minus sign), 0 otherwise
 * @return: total display width in characters
 */
int get_formatted_width(Dlist *head, int has_minus)
{
    if (head == NULL)
        return 1; // "0" is 1 character
    
    int digits = get_list_length(head); // Count total digits
    int commas = (digits > 0) ? ((digits - 1) / 3) : 0; // Number of commas (every 3 digits)
    int minus_sign = has_minus ? 1 : 0; // Minus sign width
    
    return digits + commas + minus_sign; // Total width = digits + commas + minus
}

/**
 * Calculate the display width of a string (emojis count as 1 character width)
 * This is a simplified version - in reality emojis might take 2 display cells
 * but we'll count them as 1 for alignment purposes
 * @param str: string to measure
 * @return: display width in characters
 */
int get_string_display_width(const char *str)
{
    if (str == NULL)
        return 0;
    
    int width = 0;
    const unsigned char *p = (const unsigned char *)str;
    
    while (*p)
    {
        // Check if this is the start of a UTF-8 multi-byte character
        if ((*p & 0x80) == 0)
        {
            // ASCII character (1 byte)
            width++;
            p++;
        }
        else if ((*p & 0xE0) == 0xC0)
        {
            // 2-byte UTF-8 character (like some emojis)
            width++; // Count as 1 display width
            p += 2;
        }
        else if ((*p & 0xF0) == 0xE0)
        {
            // 3-byte UTF-8 character (like most emojis)
            width++; // Count as 1 display width
            p += 3;
        }
        else if ((*p & 0xF8) == 0xF0)
        {
            // 4-byte UTF-8 character
            width++; // Count as 1 display width
            p += 4;
        }
        else
        {
            // Invalid UTF-8, skip
            p++;
        }
    }
    
    return width;
}

/**
 * Print padding spaces and closing border
 * @param max_width: maximum content width
 * @param content_width: actual content width printed so far
 */
void print_padded_line(int max_width, int content_width)
{
    int padding = max_width - content_width;
    if (padding < 0) padding = 0; // Ensure non-negative
    for (int i = 0; i < padding; i++)
        printf(" ");
    printf("  ║\n"); // 2 spaces before border
}

/**
 * Print top border with specified width
 * @param width: total width of content (not including border characters)
 */
void print_border_top(int width)
{
    printf("╔"); // Top-left corner
    for (int i = 0; i < width + 2; i++) // +2 for padding on each side
        printf("═");
    printf("╗\n"); // Top-right corner
}

/**
 * Print middle border (horizontal divider) with specified width
 * @param width: total width of content (not including border characters)
 */
void print_border_middle(int width)
{
    printf("╠"); // Middle-left connector
    for (int i = 0; i < width + 2; i++)
        printf("═");
    printf("╣\n"); // Middle-right connector
}

/**
 * Print bottom border with specified width
 * @param width: total width of content (not including border characters)
 */
void print_border_bottom(int width)
{
    printf("╚"); // Bottom-left corner
    for (int i = 0; i < width + 2; i++)
        printf("═");
    printf("╝\n"); // Bottom-right corner
}

/**
 * Print a line with label and content, with proper border alignment
 * @param width: total width of content area
 * @param label: label text (e.g., "📊 OPERATION: ")
 * @param label_width: width of the label (for padding calculation)
 */
void print_border_line(int width, const char *label, int label_width)
{
    printf("║  %s", label); // Print left border and label
    // Calculate padding needed to align right border
    int content_width = width - label_width;
    // Note: Actual content will be printed separately, this just sets up the line
    // The caller should print content and then close with "  ║\n"
}

/* ============================================================================
 * INPUT PARSING
 * Convert string representation of number to double linked list
 * ============================================================================ */

/**
 * Parse a string number and store it in a double linked list
 * Removes leading zeros and handles negative sign
 * @param head: pointer to head pointer (will be created)
 * @param tail: pointer to tail pointer (will be created)
 * @param data: string representation of number (e.g., "123" or "-456")
 * @return: sign of the number (+1 for positive, -1 for negative)
 */
int stored_num(Dlist **head, Dlist **tail, char *data)
{
    int i = 0; // Index for traversing string
    int sign = 1;  // Default: positive number (+1)

    // Check for negative sign at the beginning
    if (data[0] == '-')
    {
        sign = -1; // Set sign to negative
        i++;  // Skip the minus sign (move past it)
    }

    // Skip leading zeros (but keep at least one zero if number is "0")
    while (data[i] == '0' && data[i + 1] != '\0') // Skip zeros but not if it's the last character
    {
        i++; // Move past leading zeros
    }

    // Initialize head and tail pointers
    *head = NULL;
    *tail = NULL;

    // Convert each character digit to integer and store in list
    while (data[i] != '\0') // Process all remaining characters
    {
        dl_insert_last(head, tail, data[i] - '0');  // Convert '0'-'9' to 0-9 and store
        i++; // Move to next character
    }

    // If number was empty (shouldn't happen), store 0
    if (*head == NULL)
    {
        dl_insert_last(head, tail, 0); // Store zero if no digits found
    }

    return sign;  // Return the sign (+1 or -1) for later use
}

/* ============================================================================
 * UTILITY FUNCTIONS
 * Helper functions for number manipulation
 * ============================================================================ */

/**
 * Remove leading zeros from a number stored in double linked list
 * Example: 000123 becomes 123, but 0 stays as 0
 * @param head: pointer to head pointer of the list
 * @param tail: pointer to tail pointer of the list
 */
void trim_leading_zeroes(Dlist **head, Dlist **tail)
{
    if (head == NULL || tail == NULL || *head == NULL)
        return; // Nothing to trim if pointers are NULL or list is empty

    // Remove leading zeros, but keep at least one digit (even if it's 0)
    while ((*head)->next != NULL && (*head)->data == 0) // While there are leading zeros
    {
        dl_delete_first(head, tail); // Delete first zero
    }
}

/**
 * Check if a double linked list represents zero
 * @param head: pointer to head of the list
 * @return: 1 if the number is zero, 0 otherwise
 */
int is_zero(Dlist *head)
{
    if (head == NULL)
        return 1;  // NULL is considered zero

    // Check if all digits are zero (traverse entire list)
    Dlist *current = head; // Start from head
    while (current != NULL) // Process all nodes
    {
        if (current->data != 0)
            return 0;  // Found a non-zero digit (number is not zero)
        current = current->next; // Move to next digit
    }

    return 1;  // All digits are zero (number is zero)
}
