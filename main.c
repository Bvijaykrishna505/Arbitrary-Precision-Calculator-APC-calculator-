// NAME : B.VIJAY KRISHNA
// MODULE : DATA STRUCTURES
// DESCRIPTION : Arbitrary PRECISION CALCULATOR
// DATE : 7/11/2025

#include "apc.h"

/* ============================================================================
 * INPUT VALIDATION
 * ============================================================================ */

/**
 * Validate if a string represents a valid number
 * Valid formats: "123", "-456", "0", "-0"
 * Invalid: "abc", "12.34", "", "-"
 * 
 *  string to validate
 *  1 if valid, 0 if invalid
 */
int is_valid_number(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;
    
    size_t i = 0;
    
    // Allow optional minus sign at the beginning
    if (str[0] == '-')
        i = 1;
    
    // Check if string is just a minus sign (invalid)
    if (i >= strlen(str))
        return 0;
    
    // Check that all remaining characters are digits (0-9)
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

/* ============================================================================
 * MAIN FUNCTION
 * ============================================================================ */

int main(int argc, char *argv[])
{
    // Step 1: Validate command line arguments - must have exactly 4 arguments (program name + 3 inputs)
    if (argc != 4)
    {
        printf("❌ Invalid input\n");
        printf("📝 Usage: ./a.out operand1 operation(+,-,x,/) operand2\n");
        printf("💡 Example: ./a.out 123 + 456\n");
        return 1; // Exit with error code
    }

    // Extract operands and operator from command line arguments
    char *operand1_string = argv[1]; // First number as string
    char *operand2_string = argv[3]; // Second number as string
    
    // Validate operator is a single character (not multiple characters)
    if (strlen(argv[2]) != 1)
    {
        printf("⚠️ Invalid operation format\n");
        printf("💡 Use one of: + ➕, - ➖, x ✖️, / ➗\n");
        return 1; // Exit with error code
    }
    
    char operator = argv[2][0]; // Get the operator character
    // Convert uppercase X to lowercase x for consistency
    if (operator == 'X')
        operator = 'x';

    // Step 2: Validate that operands are valid numbers (only digits, optional minus sign)
    if (!is_valid_number(operand1_string))
    {
        printf("😟 Please check the first operand\n");
        printf("❌ Error: First operand '%s' is not a valid number\n", operand1_string);
        printf("💡 Tip: Use only digits (0-9) and optional minus sign (-)\n");
        return 1; // Exit if first operand is invalid
    }
    if (!is_valid_number(operand2_string))
    {
        printf("😟 Please check the second operand\n");
        printf("❌ Error: Second operand '%s' is not a valid number\n", operand2_string);
        printf("💡 Tip: Use only digits (0-9) and optional minus sign (-)\n");
        return 1; // Exit if second operand is invalid
    }

    // Step 3: Validate operator is one of the supported operations
    if (operator != '+' && operator != '-' && operator != 'x' && operator != '/')
    {
        printf("😟 Please check the operator operand\n");
        printf("❌ Invalid operation: '%c'\n", operator);
        printf("💡 Supported operations: + ➕ (Addition), - ➖ (Subtraction), x ✖️ (Multiplication), / ➗ (Division)\n");
        return 1; // Exit if operator is invalid
    }

    // Step 4: Create double linked lists to store numbers
    Dlist *head1 = NULL, *tail1 = NULL; // Will store first number
    Dlist *head2 = NULL, *tail2 = NULL; // Will store second number
    Dlist *head3 = NULL, *tail3 = NULL; // Will store calculation result

    // Parse input strings into double linked lists (convert string to linked list)
    int sign1 = stored_num(&head1, &tail1, operand1_string);  // Get sign: +1 for positive, -1 for negative
    int sign2 = stored_num(&head2, &tail2, operand2_string);   // Get sign: +1 for positive, -1 for negative
    int result_sign = 1;  // Result sign will be calculated based on operation and input signs

    // Calculate widths for dynamic borders - use actual string widths
    // Note: get_formatted_width includes minus sign, but we print minus separately, so subtract it
    int width1 = get_formatted_width(head1, 0) + ((sign1 == -1) ? 1 : 0); // Width without minus, then add if needed
    int width2 = get_formatted_width(head2, 0) + ((sign2 == -1) ? 1 : 0); // Width without minus, then add if needed
    int operator_width = get_string_display_width(" ➕ "); // Width of operator emoji with spaces
    int border_prefix = get_string_display_width("║  "); // Width of border prefix
    int operation_label_text = get_string_display_width("📊 OPERATION: "); // Width of label text only
    int operation_label = border_prefix + operation_label_text; // Total label width with border
    int operation_total = operation_label + width1 + operator_width + width2 + 2; // +2 for closing "  ║"
    
    // Calculate result width (will be updated after calculation)
    int result_label_text = get_string_display_width("🎯 RESULT: "); // Width of label text only
    int result_label = border_prefix + result_label_text; // Total label width with border
    int max_width = operation_total; // Start with operation width

    // Display the operation being performed (for better user experience)
    printf("\n");
    // Print header with dynamic width (calculator title)
    // Use max_width but ensure minimum width for title
    int header_title_width = get_string_display_width("🧮 APC CALCULATOR 🧮");
    int header_min_width = header_title_width + 4; // Minimum width for title with padding
    if (max_width < header_min_width) max_width = header_min_width; // Update max_width if needed
    
    print_border_top(max_width);
    printf("║");
    int header_padding = (max_width - header_title_width) / 2; // Center the title
    for (int i = 0; i < header_padding; i++) printf(" ");
    printf("🧮 APC CALCULATOR 🧮");
    int header_right_padding = max_width - header_title_width - header_padding;
    for (int i = 0; i < header_right_padding; i++) printf(" ");
    printf("║\n");
    print_border_middle(max_width);
    
    // Print operation line with dynamic width
    printf("║  📊 OPERATION: ");
    int current_width = operation_label; // Start with label width
    if (sign1 == -1)
    {
        printf("-"); // Print minus sign if first number is negative
        current_width += 1;
    }
    print_list_formatted(head1, tail1); // Print first number with formatting
    current_width += width1;
    
    // Add emoji for operator
    if (operator == '+')
    {
        printf(" ➕ ");
        current_width += operator_width;
    }
    else if (operator == '-')
    {
        printf(" ➖ ");
        current_width += operator_width;
    }
    else if (operator == 'x')
    {
        printf(" ✖️ ");
        current_width += operator_width;
    }
    else if (operator == '/')
    {
        printf(" ➗ ");
        current_width += operator_width;
    }
    else
    {
        printf(" %c ", operator);
        current_width += 3; // " %c " is 3 chars
    }
    
    if (sign2 == -1)
    {
        printf("-"); // Print minus sign if second number is negative
        current_width += 1;
    }
    print_list_formatted(head2, tail2); // Print second number with formatting
    current_width += width2;
    
    // Print padding and closing border
    print_padded_line(max_width, current_width);
    print_border_middle(max_width);
    printf("║  🎯 RESULT: ");
    int result_start_width = result_label; // Track width from start of result line

    // Step 5: Perform the requested operation based on operator
    switch (operator)
    {
        case '+':  // Addition operation
            if (sign1 == sign2) // Both numbers have same sign (both positive or both negative)
            {
                // Both positive or both negative: add magnitudes and keep the sign
                add_number(&head1, &tail1, &head2, &tail2, &head3, &tail3); // Perform addition
                result_sign = sign1;  // Result has same sign as inputs
            }
            else // Numbers have different signs
            {
                // Different signs: subtract smaller from larger (like 5 + (-3) = 5 - 3)
                int is_negative = sub_number(&head1, &tail1, &head2, &tail2, &head3, &tail3,
                                            operand1_string, operand2_string);
                // Determine result sign based on which number was larger
                result_sign = (sign1 == 1) ? (is_negative ? -1 : 1)
                                          : (is_negative ? 1 : -1);
            }
            break;

        case '-':  // Subtraction operation
            if (sign1 != sign2) // Numbers have different signs
            {
                // Different signs: add magnitudes (like 5 - (-3) = 5 + 3)
                add_number(&head1, &tail1, &head2, &tail2, &head3, &tail3); // Perform addition
                result_sign = sign1;  // Result has sign of first number
            }
            else // Numbers have same sign
            {
                // Same signs: subtract smaller from larger (like 5 - 3 or -5 - (-3))
                int is_negative = sub_number(&head1, &tail1, &head2, &tail2, &head3, &tail3,
                                            operand1_string, operand2_string);
                // Determine result sign based on which number is larger
                result_sign = (sign1 == 1) ? (is_negative ? -1 : 1)
                                          : (is_negative ? 1 : -1);
            }
            break;

        case 'x':  // Multiplication operation
        case 'X':  // Also handle uppercase X
            mul_number(&head1, &tail1, &head2, &tail2, &head3, &tail3); // Perform multiplication
            // Result sign: positive if signs same, negative if signs different
            result_sign = (sign1 == sign2) ? 1 : -1;
            break;

        case '/':  // Division operation
            {
                Dlist *remainder_head = NULL, *remainder_tail = NULL; // Create containers for remainder
                if (div_number(&head1, &tail1, &head2, &tail2, &head3, &tail3,
                              &remainder_head, &remainder_tail, operator) == FAILURE)
                {
                    // Division by zero or other error: clean up all memory and exit
                    dl_delete_list(&head1, &tail1);
                    dl_delete_list(&head2, &tail2);
                    dl_delete_list(&head3, &tail3);
                    dl_delete_list(&remainder_head, &remainder_tail);
                    return 1; // Exit with error
                }
                // Result sign: positive if signs same, negative if signs different
                result_sign = (sign1 == sign2) ? 1 : -1;
                
                // Calculate widths for division output
                // Note: get_formatted_width includes minus sign, but we print minus separately
                int quotient_has_minus = (result_sign == -1 && !is_zero(head3));
                int quotient_width = get_formatted_width(head3, 0) + (quotient_has_minus ? 1 : 0);
                int remainder_has_minus = (sign1 == -1 && !is_zero(remainder_head));
                int remainder_width = get_formatted_width(remainder_head, 0) + (remainder_has_minus ? 1 : 0);
                int remainder_label_text = get_string_display_width("📃 REMAINDER: "); // Width of label text only
                int remainder_label = border_prefix + remainder_label_text; // Total label width with border
                int quotient_line_width = result_start_width + quotient_width + 2; // +2 for closing "  ║"
                int remainder_line_width = remainder_label + remainder_width + 2; // +2 for closing "  ║"
                int division_max_width = (quotient_line_width > remainder_line_width) ? quotient_line_width : remainder_line_width;
                if (division_max_width > max_width) max_width = division_max_width;
                
                // Print quotient with better formatting
                int quotient_current_width = result_start_width;
                // Check if result is zero (don't print negative sign for zero)
                if (quotient_has_minus)
                {
                    printf("-"); // Print minus only if result is negative and not zero
                    quotient_current_width += 1;
                }
                print_list_formatted(head3, tail3); // Print quotient with formatting
                quotient_current_width += get_formatted_width(head3, 0); // Add number width without minus
                print_padded_line(max_width, quotient_current_width);
                
                // Print remainder
                printf("║  📃 REMAINDER: ");
                int remainder_current_width = remainder_label;
                // Remainder has same sign as dividend, but don't print negative for zero
                if (sign1 == -1 && !is_zero(remainder_head))
                {
                    printf("-"); // Print minus only if remainder is negative and not zero
                    remainder_current_width += 1;
                }
                print_list_formatted(remainder_head, remainder_tail); // Print remainder with formatting
                remainder_current_width += remainder_width;
                print_padded_line(max_width, remainder_current_width);
                print_border_bottom(max_width);
                
                dl_delete_list(&remainder_head, &remainder_tail); // Free remainder memory
                // Skip the normal print since we already printed above
                operator = ' ';  // Mark that we've already printed (to skip Step 6)
            }
            break;

        default:
            printf("❌ Invalid operation: '%c'\n", operator);
            printf("💡 Supported operations: + ➕, - ➖, x ✖️, / ➗\n");
            // Clean up memory
            dl_delete_list(&head1, &tail1);
            dl_delete_list(&head2, &tail2);
            dl_delete_list(&head3, &tail3);
            return 1;
    }

    // Step 6: Print the result (unless division already printed)
    if (operator != ' ') // Division already printed, skip this step
    {
        // Calculate result width and update max_width if needed
        int result_width = get_formatted_width(head3, (result_sign == -1 && !is_zero(head3)));
        int result_line_width = result_start_width + result_width + 2; // +2 for closing "  ║"
        if (result_line_width > max_width) max_width = result_line_width;
        
        // Track current width as we print
        int result_current_width = result_start_width;
        
        // Check if result is zero (don't print negative sign for zero)
        if (result_sign == -1 && !is_zero(head3))
        {
            printf("-");  // Print minus sign if result is negative and not zero
            result_current_width += 1;
        }
        
        print_list_formatted(head3, tail3);  // Print the digits with formatting (commas for large numbers)
        result_current_width += result_width;
        
        // Print padding and closing border
        print_padded_line(max_width, result_current_width);
        print_border_bottom(max_width);
        printf("✨ Calculation completed successfully! ✨\n");
    }
    // Step 7: Clean up all allocated memory to prevent memory leaks
    dl_delete_list(&head1, &tail1); // Free first number
    dl_delete_list(&head2, &tail2); // Free second number
    dl_delete_list(&head3, &tail3);  // Free result

    return 0; // Exit successfully
}
