#include "apc.h"

/* ============================================================================
 * DIVISION HELPER FUNCTIONS
 * ============================================================================ */

/**
 * Check if first number >= second number (without modifying the numbers)
 * @param head1: head of first number
 * @param head2: head of second number
 * @return: 1 if first >= second, 0 otherwise
 */
int is_greater_or_equal(Dlist *head1, Dlist *head2)
{
    // Get lengths (count digits for comparison)
    int len1 = get_list_length(head1); // Length of first number
    int len2 = get_list_length(head2); // Length of second number

    // Compare by length first (longer number is larger)
    if (len1 > len2)
        return 1; // head1 is larger
    if (len1 < len2)
        return 0; // head1 is smaller

    // Lengths equal: compare digit by digit from left to right
    Dlist *ptr1 = head1; // Pointer to first number's digits
    Dlist *ptr2 = head2; // Pointer to second number's digits

    while (ptr1 != NULL && ptr2 != NULL) // Compare all digits
    {
        if (ptr1->data > ptr2->data)
            return 1; // head1 is larger (found larger digit)
        if (ptr1->data < ptr2->data)
            return 0; // head1 is smaller (found smaller digit)
        ptr1 = ptr1->next; // Move to next digit
        ptr2 = ptr2->next; // Move to next digit
    }

    return 1;  // Numbers are equal (all digits matched)
}

/**
 * Copy a double linked list (create a duplicate)
 * @param src_head: head of source list to copy
 * @param dest_head: pointer to head pointer of destination (will be created)
 * @param dest_tail: pointer to tail pointer of destination (will be created)
 */
void copy_list(Dlist *src_head, Dlist **dest_head, Dlist **dest_tail)
{
    *dest_head = NULL; // Initialize destination
    *dest_tail = NULL;

    if (src_head == NULL)
        return; // Nothing to copy if source is NULL

    Dlist *current = src_head; // Start from source's head
    while (current != NULL) // Copy all digits
    {
        dl_insert_last(dest_head, dest_tail, current->data); // Copy each digit
        current = current->next; // Move to next digit
    }
}

/**
 * Subtract second list from first list (assumes first >= second)
 * Result is stored in result list (result is cleared first)
 * @param head1: pointer to head pointer of first number (must be >= second)
 * @param tail1: pointer to tail pointer of first number
 * @param head2: pointer to head pointer of second number
 * @param tail2: pointer to tail pointer of second number
 * @param result_head: pointer to head pointer of result (will be cleared and filled)
 * @param result_tail: pointer to tail pointer of result (will be cleared and filled)
 */
void subtract_lists(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
                    Dlist **result_head, Dlist **result_tail)
{
    // Clear result first (start with empty result)
    dl_delete_list(result_head, result_tail); // Clear existing digits

    // Start from tail (rightmost digits) and work backwards
    // Double linked list allows us to traverse backwards easily
    Dlist *temp1 = *tail1; // Start from rightmost digit of first number
    Dlist *temp2 = *tail2; // Start from rightmost digit of second number

    int borrow = 0; // Borrow flag (1 if we borrowed, 0 otherwise)
    int digit1, digit2, res_digit; // Current digits and result

    while (temp1 != NULL) // Process all digits of first number
    {
        digit1 = temp1->data; // Get digit from first number
        digit2 = (temp2 != NULL) ? temp2->data : 0; // Get digit from second number (0 if exhausted)

        if (borrow) // If we borrowed in previous step
        {
            digit1 -= 1; // Subtract 1 (we borrowed it)
            borrow = 0; // Reset borrow flag
        }

        if (digit1 < digit2) // If we need to borrow (like 2 - 7)
        {
            digit1 += 10; // Borrow 10 from next digit
            borrow = 1; // Set borrow flag for next iteration
        }

        res_digit = digit1 - digit2; // Calculate result digit
        dl_insert_first(result_head, result_tail, res_digit); // Store result (building from right to left)

        temp1 = temp1->prev; // Move to previous digit (leftward)
        if (temp2 != NULL)
            temp2 = temp2->prev; // Move to previous digit in second number
    }

    trim_leading_zeroes(result_head, result_tail); // Remove leading zeros from result
}

/* ============================================================================
 * DIVISION OPERATION
 * Divides first number by second number using long division algorithm
 * Works with arbitrarily large numbers
 * ============================================================================ */

/**
 * Divide first number by second number
 * Algorithm: Long division (like division done by hand)
 * 
 * @param head1: pointer to head pointer of dividend
 * @param tail1: pointer to tail pointer of dividend
 * @param head2: pointer to head pointer of divisor
 * @param tail2: pointer to tail pointer of divisor
 * @param head3: pointer to head pointer of quotient (will be created)
 * @param tail3: pointer to tail pointer of quotient (will be created)
 * @param remainder_head: pointer to head pointer of remainder (will be created)
 * @param remainder_tail: pointer to tail pointer of remainder (will be created)
 * @param ch: operation character ('/' for division, '%' for modulus)
 * @return: SUCCESS on success, FAILURE if division by zero
 */
int div_number(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
               Dlist **head3, Dlist **tail3, Dlist **remainder_head, Dlist **remainder_tail, char ch)
{
    // Step 1: Check for division by zero (cannot divide by zero)
    if (*head2 == NULL || ((*head2)->data == 0 && (*head2)->next == NULL))
    {
        printf("❌ Division by zero error!\n");
        printf("⚠️ Cannot divide by zero. Please use a non-zero divisor.\n");
        return FAILURE; // Return error if divisor is zero
    }

    // Clear result and remainder containers (start fresh)
    dl_delete_list(head3, tail3); // Clear quotient
    dl_delete_list(remainder_head, remainder_tail); // Clear remainder

    // Step 2: Check if dividend < divisor (result is 0, remainder is dividend)
    if (!is_greater_or_equal(*head1, *head2)) // If dividend is smaller than divisor
    {
        // Dividend is smaller: quotient is 0, remainder is dividend
        if (ch == '%') // Modulus operation requested
        {
            // For modulus, result is the dividend itself (remainder when dividing by larger number)
            copy_list(*head1, head3, tail3); // Copy dividend to result
        }
        else // Regular division
        {
            // For division, quotient is 0 (smaller number divided by larger number)
            dl_insert_last(head3, tail3, 0); // Quotient is zero
        }
        
        // Remainder is the dividend (when dividend < divisor, remainder = dividend)
        copy_list(*head1, remainder_head, remainder_tail); // Copy dividend to remainder
        return SUCCESS; // Return success (no actual division needed)
    }

    // Step 3: Perform long division (dividend >= divisor)
    // Create working copy of dividend for current partial (portion being divided)
    Dlist *current_partial_head = NULL; // Will hold current portion of dividend
    Dlist *current_partial_tail = NULL;

    // Process dividend digit by digit from left to right (like long division)
    Dlist *dividend_ptr = *head1; // Pointer to current digit in dividend

    while (dividend_ptr != NULL) // Process all digits of dividend
    {
        // Bring down next digit from dividend (like bringing down in long division)
        dl_insert_last(&current_partial_head, &current_partial_tail, dividend_ptr->data); // Add next digit
        trim_leading_zeroes(&current_partial_head, &current_partial_tail); // Remove any leading zeros

        // If current partial is still less than divisor, quotient digit is 0
        if (!is_greater_or_equal(current_partial_head, *head2)) // Partial < divisor
        {
            dl_insert_last(head3, tail3, 0); // Quotient digit is 0
            dividend_ptr = dividend_ptr->next; // Move to next digit
            continue; // Skip to next iteration
        }

        // Find quotient digit by repeatedly subtracting divisor (count how many times divisor fits)
        int quotient_digit = 0; // Will count how many times divisor fits
        Dlist *temp_result_head = NULL; // Temporary storage for subtraction result
        Dlist *temp_result_tail = NULL;

        // Keep subtracting divisor from current_partial until it's smaller
        while (is_greater_or_equal(current_partial_head, *head2)) // While partial >= divisor
        {
            // Subtract divisor from current_partial
            subtract_lists(&current_partial_head, &current_partial_tail, head2, tail2,
                          &temp_result_head, &temp_result_tail); // Perform subtraction

            // Update current_partial with the result
            dl_delete_list(&current_partial_head, &current_partial_tail); // Free old partial
            copy_list(temp_result_head, &current_partial_head, &current_partial_tail); // Copy result to partial

            quotient_digit++; // Increment quotient digit (one more subtraction)
            
            // Clean up temp result for next iteration
            dl_delete_list(&temp_result_head, &temp_result_tail);
        }

        // Store quotient digit (how many times divisor fit into partial)
        dl_insert_last(head3, tail3, quotient_digit);
        dividend_ptr = dividend_ptr->next; // Move to next digit in dividend
    }

    // Step 4: Remove leading zeros from quotient (e.g., 000123 becomes 123)
    trim_leading_zeroes(head3, tail3);

    // Step 5: Store remainder (current_partial is the final remainder after all divisions)
    copy_list(current_partial_head, remainder_head, remainder_tail); // Copy remainder
    trim_leading_zeroes(remainder_head, remainder_tail); // Remove leading zeros from remainder

    // Step 6: Handle modulus operation - replace quotient with remainder
    if (ch == '%') // If modulus operation was requested
    {
        // Clear quotient and copy remainder to result (modulus returns remainder, not quotient)
        dl_delete_list(head3, tail3); // Clear quotient
        copy_list(*remainder_head, head3, tail3); // Copy remainder to result
    }

    // Clean up temporary variables
    dl_delete_list(&current_partial_head, &current_partial_tail); // Free working partial

    return SUCCESS; // Division completed successfully
}
