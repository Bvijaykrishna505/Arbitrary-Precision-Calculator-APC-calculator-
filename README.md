# Arbitrary-Precision-Calculator-APC-calculator-
A DS(data structures)-based Arbitrary Precision Calculator that performs arithmetic operations on very large integers using a Doubly Linked List (DLL). This allows the calculator to handle numbers of any size, far beyond built-in data types like int, long, or long long.

📌 Project Overview
The APC stores each digit of a large number inside a node of a doubly linked list.
This structure allows:
Efficient traversal from both ends
Easy addition & subtraction (traversing from tail)
Easy normalization of digits (carry/borrow)
Better performance than singly linked lists for big-number operations
This project demonstrates strong understanding of Data Structures, Dynamic Memory Management, and Big Integer Arithmetic.
This project demonstrates core concepts of DSA, memory allocation, and big number arithmetic.

🎯 Features
✔️ Supports integers of unlimited length
✔️ Uses doubly linked lists for storing digits
✔️ Performs:
Addition
Subtraction
Multiplication
Division 
✔️ Handles negative numbers
✔️ Input validation (rejects invalid characters)
✔️ Removes leading zeros
✔️ Menu-driven CLI interface

**🛠️ Technologies Used**
C programming
Doubly Linked Lists
Dynamic memory allocation (malloc, free)
Pointer manipulation
Big integer arithmetic (manual carry/borrow logic)
VS Code

RUN 
gcc *.c
./a.out (number1) (operator) (number2)



