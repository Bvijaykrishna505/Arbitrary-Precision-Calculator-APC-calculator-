#include "apc.h"

/* ============================================================================
 * MULTIPLICATION OPERATION
 * Multiplies two large numbers using the standard multiplication algorithm
 * (like long multiplication done by hand)
 * ============================================================================ */

/**
 * Multiply two numbers stored in double linked lists
 * Algorithm: Use intermediate array to store partial products,
 * then handle carries and convert to result list
 * 
 * @param head1: pointer to head pointer of first number
 * @param tail1: pointer to tail pointer of first number
 * @param head2: pointer to head pointer of second number
 * @param tail2: pointer to tail pointer of second number
 * @param head3: pointer to head pointer of result (will be created)
 * @param tail3: pointer to tail pointer of result (will be created)
 * @return: SUCCESS on success, FAILURE on memory allocation error
 */
int mul_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3)
{
    // Step 1: Get lengths of both numbers (for array allocation)
    int len1 = get_list_length(*head1); // Length of first number
    int len2 = get_list_length(*head2); // Length of second number

    // Step 2: Allocate array to store intermediate multiplication results
    // Result can have at most (len1 + len2) digits (e.g., 99 * 99 = 9801, 2+2=4 digits)
    int *result_array = calloc(len1 + len2, sizeof(int)); // Allocate and initialize to zero
    if (result_array == NULL)
    {
        return FAILURE; // Return error if memory allocation fails
    }

    // Step 3: Multiply each digit of first number with each digit of second number
    // Store partial products in result_array (like long multiplication)
    // Start from tail (rightmost digits) and work backwards
    // Example: 123 * 456
    //   result[0] = 3*6, result[1] = 3*5 + 2*6, result[2] = 3*4 + 2*5 + 1*6, etc.
    int i = 0, j = 0; // Loop counters for digit positions
    Dlist *digit1_ptr = *tail1; // Start from rightmost digit of first number
    
    for (i = 0; digit1_ptr != NULL; digit1_ptr = digit1_ptr->prev, i++) // Loop through first number (backwards)
    {
        j = 0;
        Dlist *digit2_ptr = *tail2; // Start from rightmost digit of second number
        for (j = 0; digit2_ptr != NULL; digit2_ptr = digit2_ptr->prev, j++) // Loop through second number (backwards)
        {
            // Multiply digits and add to appropriate position
            // Position (i+j) corresponds to the place value (ones, tens, hundreds, etc.)
            result_array[i + j] += digit1_ptr->data * digit2_ptr->data; // Accumulate partial products
        }
    }

    // Step 4: Handle carries (propagate carries from right to left)
    int carry = 0; // Carry from previous position
    for (i = 0; i < len1 + len2; i++) // Process all positions in result array
    {
        result_array[i] += carry;        // Add carry from previous position
        carry = result_array[i] / 10;     // Calculate new carry (tens place)
        result_array[i] = result_array[i] % 10;  // Keep only ones digit (0-9)
    }

    // Step 5: Find first non-zero digit (skip leading zeros in result)
    int first_digit_index = len1 + len2 - 1; // Start from most significant position
    while (first_digit_index > 0 && result_array[first_digit_index] == 0)
    {
        first_digit_index--; // Skip leading zeros (but keep at least one digit)
    }

    // Step 6: Convert result array to double linked list
    // Insert from most significant to least significant (left to right)
    for (; first_digit_index >= 0; first_digit_index--)
    {
        dl_insert_last(head3, tail3, result_array[first_digit_index]); // Add each digit to result
    }

    // Step 7: Free the intermediate array (prevent memory leak)
    free(result_array);
    return SUCCESS; // Multiplication completed successfully
}
