#include "apc.h"

/* ============================================================================
 * SUBTRACTION OPERATION
 * Subtracts second number from first number digit by digit from right to left
 * ============================================================================ */

/**
 * Subtract second number from first number
 * Algorithm: Compare numbers, swap if needed, then subtract digit by digit
 * 
 * @param head1: pointer to head pointer of first number
 * @param tail1: pointer to tail pointer of first number
 * @param head2: pointer to head pointer of second number
 * @param tail2: pointer to tail pointer of second number
 * @param head3: pointer to head pointer of result (will be created)
 * @param tail3: pointer to tail pointer of result (will be created)
 * @param op1: original string of first operand (for comparison)
 * @param op2: original string of second operand (for comparison)
 * @return: 0 if result is positive, 1 if result is negative
 */
int sub_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3, char *op1, char *op2)
{
    // Step 1: Compare numbers and swap if head1 < head2
    // This ensures we always subtract smaller from larger (to avoid negative intermediate results)
    int is_negative = compare_numbers(head1, tail1, head2, tail2, op1, op2); // Compare and swap if needed
    
    // Step 2: Start from tail (rightmost digits) and work backwards
    // Double linked list allows us to traverse backwards easily
    Dlist *temp1 = *tail1; // Start from rightmost digit of first number
    Dlist *temp2 = *tail2; // Start from rightmost digit of second number
    
    // Step 3: Subtract digits from right to left (ones, tens, hundreds, etc.)
    int borrow = 0;      // Borrow flag (1 if we borrowed from next digit, 0 otherwise)
    int digit1, digit2;  // Current digits from both numbers
    int result_digit;     // Result of subtraction for current position

    while (temp1 != NULL) // Process all digits of first number
    {
        // Get current digits
        digit1 = temp1->data; // Digit from first number
        digit2 = (temp2 != NULL) ? temp2->data : 0; // Digit from second number (0 if exhausted)

        // Apply borrow from previous subtraction (if we borrowed, subtract 1)
        if (borrow == 1)
        {
            digit1 -= 1;  // Subtract 1 because we borrowed in previous step
            borrow = 0;    // Reset borrow flag
        }

        // If digit1 < digit2, we need to borrow from next digit (like 12 - 7)
        if (digit1 < digit2)
        {
            digit1 += 10;  // Borrow 10 from next digit (12 becomes 12)
            borrow = 1;     // Set borrow flag for next iteration
        }

        // Calculate result digit
        result_digit = digit1 - digit2;
        
        // Store result (insert at beginning since we're building result from right to left)
        dl_insert_first(head3, tail3, result_digit);

        // Move to previous digits (leftward)
        temp1 = temp1->prev; // Move to previous digit in first number
        if (temp2 != NULL)
            temp2 = temp2->prev; // Move to previous digit in second number
    }

    // Step 4: Remove leading zeros from result (e.g., 000123 becomes 123)
    trim_leading_zeroes(head3, tail3);

    // Return whether result is negative (1 if negative, 0 if positive)
    return is_negative;
}

/**
 * Compare two numbers to determine which is larger
 * Swaps head1/tail1 and head2/tail2 if head1 < head2
 * 
 * @param head1: pointer to head pointer of first number (may be swapped)
 * @param tail1: pointer to tail pointer of first number (may be swapped)
 * @param head2: pointer to head pointer of second number (may be swapped)
 * @param tail2: pointer to tail pointer of second number (may be swapped)
 * @param op1: original string of first operand (unused, kept for compatibility)
 * @param op2: original string of second operand (unused, kept for compatibility)
 * @return: 0 if head1 >= head2 (no swap), 1 if head1 < head2 (swapped)
 */
int compare_numbers(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
                    char *op1, char *op2)
{
    (void)op1;  // Unused parameter (kept for compatibility)
    (void)op2;  // Unused parameter (kept for compatibility)

    // Get lengths (count digits for comparison)
    int len1 = get_list_length(*head1); // Length of first number
    int len2 = get_list_length(*head2); // Length of second number

    // Compare by length first (longer number is larger)
    if (len1 < len2) // First number is shorter
    {
        swap_lists(head1, tail1, head2, tail2);  // Swap so we subtract smaller from larger
        return 1;  // Result will be negative (we swapped, so original head1 < head2)
    }
    else if (len1 > len2) // First number is longer
    {
        return 0;  // head1 is larger, result will be positive (no swap needed)
    }

    // Lengths are equal: compare digit by digit from left to right
    Dlist *ptr1 = *head1; // Pointer to first number's digits
    Dlist *ptr2 = *head2; // Pointer to second number's digits

    while (ptr1 != NULL && ptr2 != NULL) // Compare all digits
    {
        if (ptr1->data < ptr2->data) // Found a smaller digit in head1
        {
            // head1 is smaller: swap them (ensure we subtract smaller from larger)
            swap_lists(head1, tail1, head2, tail2);
            return 1;  // Result will be negative (original head1 < head2)
        }
        else if (ptr1->data > ptr2->data) // Found a larger digit in head1
        {
            // head1 is larger: no swap needed
            return 0;  // Result will be positive (head1 > head2)
        }

        // Digits are equal: continue to next digit (keep comparing)
        ptr1 = ptr1->next; // Move to next digit
        ptr2 = ptr2->next; // Move to next digit
    }

    // Numbers are equal: result is zero (positive, no swap needed)
    return 0;
}

/**
 * Swap two double linked lists
 * Used when we need to ensure we subtract smaller from larger
 * 
 * @param head1: pointer to head pointer of first list
 * @param tail1: pointer to tail pointer of first list
 * @param head2: pointer to head pointer of second list
 * @param tail2: pointer to tail pointer of second list
 */
void swap_lists(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2)
{
    Dlist *temp_head = *head1; // Save first head
    Dlist *temp_tail = *tail1; // Save first tail
    *head1 = *head2; // Assign second head to first
    *tail1 = *tail2; // Assign second tail to first
    *head2 = temp_head; // Assign saved first head to second
    *tail2 = temp_tail; // Assign saved first tail to second (swap complete)
}
