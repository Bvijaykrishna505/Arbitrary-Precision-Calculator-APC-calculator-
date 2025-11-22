#include "apc.h"

/* ============================================================================
 * ADDITION OPERATION
 * Adds two large numbers digit by digit from right to left (like manual addition)
 * ============================================================================ */

/**
 * Add two numbers stored in double linked lists
 * Algorithm: Add digits from right to left with carry, using tail pointers
 * 
 * @param head1: pointer to head pointer of first number
 * @param tail1: pointer to tail pointer of first number
 * @param head2: pointer to head pointer of second number
 * @param tail2: pointer to tail pointer of second number
 * @param head3: pointer to head pointer of result (will be created)
 * @param tail3: pointer to tail pointer of result (will be created)
 * @return: SUCCESS on success
 */
int add_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3)
{
    int carry = 0;      // Carry from previous digit addition (0 or 1)
    int digit_sum;      // Sum of current digits plus carry

    // Step 1: Start from tail (rightmost digits) and work backwards
    // Double linked list allows us to traverse backwards easily
    Dlist *temp1 = *tail1; // Start from rightmost digit of first number
    Dlist *temp2 = *tail2; // Start from rightmost digit of second number

    // Step 2: Add digits from right to left (ones, tens, hundreds, etc.)
    // Process until both lists are exhausted
    while (temp1 != NULL || temp2 != NULL)
    {
        digit_sum = carry;  // Start with carry from previous addition

        // Add digit from first number (if available)
        if (temp1 != NULL)
        {
            digit_sum += temp1->data; // Add current digit
            temp1 = temp1->prev; // Move to previous digit (leftward)
        }

        // Add digit from second number (if available)
        if (temp2 != NULL)
        {
            digit_sum += temp2->data; // Add current digit
            temp2 = temp2->prev; // Move to previous digit (leftward)
        }

        // Store result digit and calculate new carry (handles sum > 9)
        update_result_add(head3, tail3, digit_sum, &carry);
    }

    // Step 3: If there's a final carry, add it as a new digit (e.g., 99 + 1 = 100)
    if (carry != 0)
    {
        dl_insert_first(head3, tail3, carry); // Insert carry as most significant digit
    }

    return SUCCESS; // Addition completed successfully
}

/**
 * Helper function: Process a digit sum and update result
 * Handles carry propagation (if sum > 9)
 * 
 * @param head: pointer to head pointer of result list
 * @param tail: pointer to tail pointer of result list
 * @param result: sum of digits plus carry (may be > 9)
 * @param carry: pointer to carry variable (will be updated)
 * @return: SUCCESS
 */
int update_result_add(Dlist **head, Dlist **tail, int result, int *carry)
{
    int result_digit; // Will store the digit to be inserted (0-9)

    if (result > 9) // If sum is two digits (e.g., 15)
    {
        // Result is two digits: extract ones place and tens place
        result_digit = result % 10;  // Ones place (0-9) - what we write down
        *carry = result / 10;        // Tens place becomes carry (0-9) - what we carry over
    }
    else // If sum is single digit (0-9)
    {
        // Result is single digit: no carry
        result_digit = result; // Use the result directly
        *carry = 0; // No carry needed
    }

    // Insert result digit at the beginning (we're building result from right to left)
    return dl_insert_first(head, tail, result_digit); // Insert and return success/failure
}
