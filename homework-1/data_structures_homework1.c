/*
 *      ##################################################
 *            EHB208E - DATA STRUCTURES & PROGRAMMING
 *      --------------------------------------------------
 *                          HOMEWORK - 1
 *      --------------------------------------------------
 *                   STUDENT AUTOMATION SYSTEM
 *      --------------------------------------------------
 *                     BIG O of EACH SECTION
 *
 *      1 - Add student -> O(1)
 *      2 - Delete the student -> O(n)
 *      3 - Display students from memory -> O(n)
 *      4 - Display the student names in ascending order..
 *          .. by the ID. -> O(n^2)
 *      5 - Display the student names in ascending order..
 *          .. by the first letter of their names. -> O(n^2)
 *      6 - Display the school numbers by the faculty ...
 *          ... codes in ascending order. -> O(n^2)
 *      --------------------------------------------------
 *      Class Lecturer: Academic Dr. Bora Döken
 *      Class Asistant: Res. Asst. Muhammed Yasin Adıyaman
 *
 *      Student and Writer: İsmet Erdoğan
 *      School Number: 040200522
 *      Department: Control and Automation Eng.
 *      ###################################################
 */

//  required libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//  required preprocessor defines
#define NAME_MAX_LIMIT 30

//  Linked list structure to keep characters in student name
struct ListNode {
    char character;
    struct ListNode* nextNode;
};

//  Linked list structure to keep the student number.
struct HeaderNode {
    int schoolNumber;
    struct ListNode* nextNode;
};

//  Type definitions to make writing and reading easier
typedef struct ListNode listNode;
typedef listNode* listNodePtr;

//  Type definitions to make writing and reading easier
typedef struct HeaderNode headerNode;
typedef headerNode* headerNodePtr;

//  stack data structure to create where needed in the program
struct StackNode {
    headerNodePtr student;
    struct StackNode* nextPtr;
};

//  Type definitions to make writing and reading easier
typedef struct StackNode stackNode;
typedef stackNode* stackNodePtr;

//  Structure to keep student name and id in the same structure
struct NameAndID
{
    char* name;
    int id;
};

//  Structure to keep school number and faculty code in the same structure
struct SchoolNumberAndFacultyCode
{
    int schoolNumber;
    int facultyCode;
};

//  Type definitions to make writing and reading easier
typedef struct NameAndID nameAndID;
typedef struct SchoolNumberAndFacultyCode schoolNumberAndFacultyCode;

//  function prototypes
void insert(listNodePtr *sPtr, char value);
void instructions(void);
void displayStudent(headerNodePtr sPtr);
void push(stackNodePtr *topPtr, headerNodePtr studentPtr);
headerNodePtr pop(stackNodePtr *topPtr);
void printStack(stackNodePtr currentPtr);
char* linkedListToString(headerNodePtr studentPtr);
size_t sizeOfStack(stackNodePtr currentPtr);

// main program
int main(void)
{
//    A pointer to the stack structure that will hold all student records
    stackNodePtr memoryStackPtr = NULL;

//    Instructions about program usage
    instructions();
//    programMode Variable that determines which part of the program will run
    int programMode = 0;

//    Loop structure that asks for program mode each time an operation is complete
    do
    {
//        Mode selection
        printf("\n[MODE SELECTION]: Enter number for program mode ->");
        scanf_s("%d", &programMode);

//        Insert Mode
        if (programMode == 1)
        {
            puts("\n>Insert Mode\n");

//            variable definitions required to save names and numbers
            char studentName[NAME_MAX_LIMIT];
            int studentNumber;
            int lenOfName;
//            Variables to show faculty code and ID related to school number
            int facultyCode;
            int ID;
//            variable required to validate school number
            bool validNumber = false;

//            get name
            printf("Enter the student name:\n");
            fflush(stdin);
            gets(studentName);

//            verify school number is 8 digits number
            do
            {
//                get school number
                printf("Enter the student Number:\n");
                scanf_s("%d", &studentNumber);
//                if number has 8 digits
                if (studentNumber / (int)pow(10,7) > 0 && studentNumber / (int)pow(10,7) < 10)
                {
                    validNumber = true;
                }
                else
                {
                    puts("[WARNING]: Student number has to be 8 digit number. Try again.");
                }
            }
//            prompt school number again
            while (validNumber == false);

//            How these numbers are found is explained in the following lines.
            facultyCode = studentNumber / 100000;
            ID = studentNumber % 10000;

//           get length of name required to save in linked list
            lenOfName = (int)strlen(studentName);

//            print student info
            puts("\nSTUDENT INFO");
            puts("-------------");
            printf("* student name: %s\n* school number: %d\n", studentName, studentNumber);
            printf("* faculty code: %d\n* ID: %d\n", facultyCode, ID);

//            Variable definitions required to print the number and name into the linked list
            headerNodePtr studentPtr = malloc(sizeof(headerNode));
            listNodePtr studentNamePtr = NULL;

//            adding the characters in the name to the linked list, respectively
            for (int i = 0; i < lenOfName; ++i) {
//                this function adds the given value into linked list
                insert(&studentNamePtr, studentName[i]);
            }
//            a different node holds the student's number
            studentPtr->schoolNumber = studentNumber;
//            this node connects to the first node of the linked list of names as the starting node
            studentPtr->nextNode = studentNamePtr;
//            bottom function is called to print the inserted student in linked list notation
            displayStudent(studentPtr);
//            It is added to the memory stack to save the student kept in the linked list format.
//            This stack keeps all added students in the order they were saved.
            push(&memoryStackPtr, studentPtr);
        }

//        Delete Mode - by school number
        else if (programMode == 2)
        {
            puts("\n>Delete Mode\n");

//            variable definitions required to delete a recorded student
            int targetSchoolNumber = 0;
            int exists = 0;

//            scan school number to be deleted
            printf("Enter the school number of student who you want to DELETE:\n");
            scanf_s("%d", &targetSchoolNumber);

//            2 stack pointers are defined for navigating the memory
//            and for deleting the stack (if there is a record).
            stackNodePtr currentMemoryStackPtr = memoryStackPtr;
            stackNodePtr previousMemoryStackPtr = NULL;

//            It is navigated through registered students, starting with the most recently added student.
            while (currentMemoryStackPtr != NULL)
            {
//                If the school number of the student indicated by the pointer showing...
//                ...the instantaneous place in the memory matches the school number of the student to be deleted.
                if (currentMemoryStackPtr->student->schoolNumber == targetSchoolNumber)
                {
//                    keep the address of that student by allocating a temporary pointer to memory.
                    stackNodePtr temp = malloc(sizeof(stackNode));
                    temp = currentMemoryStackPtr;

//                    In order to delete that student from the memory,
//                    the address relations in the memory are broken with the student to be deleted.
                    previousMemoryStackPtr->nextPtr = currentMemoryStackPtr->nextPtr;
                    printf("[INFO]: Student registration number %d named %s has been deleted.",
                           targetSchoolNumber, linkedListToString(temp->student));
//                    Then the student removed from the stack is also deleted from the memory.
                    free(temp);
//                    The variable "exist" was set to 1 to indicate that the student was found.
                    exists = 1;
//                    quit loop
                    break;
                }
//                lines written to advance one step in the stack
                previousMemoryStackPtr = currentMemoryStackPtr;
                currentMemoryStackPtr = currentMemoryStackPtr->nextPtr;
            }
//            if the variable "exist" is 0, there is no registered student with the student number sought.
            if (exists == 0)
            {
                printf("[WARNING]: There are no student with number %d.\n", targetSchoolNumber);
            }
        }

//        Display Mode - All recorded students according to order of insertion
        else if (programMode == 3)
        {
            puts("\n>Display Mode - *All Recorded Students from Memory Stack*\n");
//            The function is called to display the number of registered students...
//            ...in the stack indicated by the given stack pointer.
            printStack(memoryStackPtr);
        }

//        Display Mode - Displays names according to increasing id number
        else if (programMode == 4)
        {
            puts("\n>Display Mode - *According to the Increasing Number of IDs*\n");

//            stack pointer (currentMemoryStackPtr) are defined for navigating the memory
            stackNodePtr currentMemoryStackPtr = memoryStackPtr;
//            counter variable required to save the student's school number and
//            ID in the array while looping through the memory stack
            int i = 0;
//            required variables
            int schoolNumber;
            int ID;
//            a variable named "student" was created from the structure holding the student number and ID.
            nameAndID student;
//            an array containing elements in the "nameAndID" structure has been defined.
//            The array size was determined by the "sizeOfStack" function...
//            ...returning the number of registered students in the stack.
            nameAndID students[sizeOfStack(memoryStackPtr)];

//            It is navigated through registered students, starting with the most recently added student.
            while (currentMemoryStackPtr != NULL)
            {
//                The school number has been reached with the memory pointer.
                schoolNumber = currentMemoryStackPtr->student->schoolNumber;
//                school number consists of 9 digits
//                the last 4 digits of the number gives the ID
//                example:
//                          school number: 705102005
//                          id: 2005
//                          --------
//                          method: 705102005 % 10000 = 2005
                ID = schoolNumber % 10000;
//                the resulting ID is equal to the ID inside the student structure
                student.id = ID;
//                The "Linked List ToString" function gives the output in string format...
//                ...by adding the characters kept in the linked list given to it into an array.
//                then this resulting string is assigned to the name in the student structure
                student.name = linkedListToString(currentMemoryStackPtr->student);
//                The student structure created is added to the students array with i. added as an element.
                students[i] = student;
//                memory pointer goes to next place
                currentMemoryStackPtr = currentMemoryStackPtr->nextPtr;
//                The variable i is incremented by 1 to add a new element to the array.
                i++;
            }

//            The temp structure has been defined to be used as a temporary variable in the sorting algorithm.
            nameAndID temp;

//            Bubble Sort Algorithm for sorting names according to id.
            for(i=0; i<sizeOfStack(memoryStackPtr); i++){
                for(int j=0; j<sizeOfStack(memoryStackPtr)-1-i; j++){
                    if(students[j].id > students[j+1].id)
                    {
                        temp = students[j];
                        students[j] = students[j + 1];
                        students[j + 1] = temp;
                    }
                }
            }
//            prints the names in the sorted array in order.
            for(i = 0; i < sizeOfStack(memoryStackPtr); i++)
            {
                printf("%s\n", students[i].name);
            }
        }

//        Display Mode - Displays names according to order of alphabetic
        else if (programMode == 5)
        {
            puts("\n>Display Mode - *Names in Alphabetical Order*\n");

//            stack pointer (currentMemoryStackPtr) are defined for navigating the memory
            stackNodePtr currentMemoryStackPtr = memoryStackPtr;
//            Created array to hold student names.
//            The array size was determined by the "sizeOfStack" function...
//            ...returning the number of registered students in the stack.
            char* names[sizeOfStack(memoryStackPtr)];
//            counter variable required to save the name of the student into...
//            ...array named "names".
            int i = 0;

//            It is navigated through registered students, starting with the most recently added student.
            while (currentMemoryStackPtr != NULL)
            {
//                The name reached in memory is assigned to the variable "name".
//                The "Linked List ToString" function gives the output in string format...
//                ...by adding the characters kept in the linked list given to it into an array.
                char* name = linkedListToString(currentMemoryStackPtr->student);
//                "name" is added to the "names" array.
                names[i] = name;
//                memory pointer goes to next place
                currentMemoryStackPtr = currentMemoryStackPtr->nextPtr;
//                The variable i is incremented by 1 to add a new element to the array.
                i++;
            }
//            The temp string has been defined to be used as a temporary variable in the sorting algorithm.
            char temp[30];

//            Bubble Sort Algorithm for sorting names according to alpahabetical order
            for(i=0; i<sizeOfStack(memoryStackPtr); i++){
                for(int j=0; j<sizeOfStack(memoryStackPtr)-1-i; j++){
                    if(strcmp(names[j], names[j+1]) > 0){
                        //swap array[j] and array[j+1]
                        strcpy(temp, names[j]);
                        strcpy(names[j], names[j+1]);
                        strcpy(names[j+1], temp);
                    }
                }
            }
//            prints the names in the sorted array in order.
            for(i = 0; i < sizeOfStack(memoryStackPtr); i++)
            {
                printf("%s\n", names[i]);
            }
        }

//        Display Mode - Displays school numbers according to faculty codes
        else if (programMode == 6)
        {
            puts("\n>Display Mode - *According to the Increasing Number of Faculty Codes*\n");

//            stack pointer (currentMemoryStackPtr) are defined for navigating the memory
            stackNodePtr currentMemoryStackPtr = memoryStackPtr;
//            counter variable required to save the student's school number and
//            faculty code in the array while looping through the memory stack
            int i = 0;
//            required variables
            int schoolNumber;
            int facultyCode;
//            A variable named student was created from...
//            ...the structure that contains the school number and faculty code.
            schoolNumberAndFacultyCode student;
//            array containing the school number and faculty code was created.
//            The array size was determined by the "sizeOfStack" function...
//            ...returning the number of registered students in the stack.
            schoolNumberAndFacultyCode students[sizeOfStack(memoryStackPtr)];

//            It is navigated through registered students, starting with the most recently added student.
            while (currentMemoryStackPtr != NULL)
            {
//                The school number has been reached with the memory pointer.
                schoolNumber = currentMemoryStackPtr->student->schoolNumber;
//                school number consists of 9 digits
//                the first 3 digits of the number gives the ID
//                example:
//                          school number: 705102005
//                          faculty number: 2005
//                          --------
//                          method: 705102005 / 100000 = 705
                facultyCode = schoolNumber / 100000;
//                Necessary assigns have been made.
                student.schoolNumber = schoolNumber;
                student.facultyCode = facultyCode;
//                "student" is added to "students".
                students[i] = student;
//                memory pointer goes to next place
                currentMemoryStackPtr = currentMemoryStackPtr->nextPtr;
//                The variable i is incremented by 1 to add a new element to the array.
                i++;
            }

//            The temp struct has been defined to be used as a temporary variable in the sorting algorithm.
            schoolNumberAndFacultyCode temp;

//            Bubble Sort Algorithm for sorting school number according to faculty code
            for(i=0; i<sizeOfStack(memoryStackPtr); i++){
                for(int j=0; j<sizeOfStack(memoryStackPtr)-1-i; j++){
                    if(students[j].facultyCode > students[j+1].facultyCode)
                    {
                        temp = students[j];
                        students[j] = students[j + 1];
                        students[j + 1] = temp;
                    }
                }
            }

//            prints the school numbers in the sorted array in order.
            for(i = 0; i < sizeOfStack(memoryStackPtr); i++)
            {
                printf("%d\n", students[i].schoolNumber);
            }
        }

//        End of Program
        else if (programMode == 7)
        {
            puts("\n[INFO]: End of program!\n");
        }

//        Warning section for invalid inputs
        else
        {
            puts("\n[WARNING]: Invalid choice!\n");
            instructions();
        }
    }
//    If end of file mode is not selected, program loop run once again
    while (programMode != 7);
//    Else, terminate the program
    return 0;
}

//  Function Definitions
void instructions(void) {
    /*
     *          - instructions Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> Prints instructions of student automation system.
     *          -> Item numbers represent program modes.
     */

    puts("\n\t\t\tSTUDENT AUTOMATION SYSTEM INSTRUCTIONS");
    puts("---------------------------------------------------------------------------------------");
    puts("1 - Add the student.\n"
             "2 - Delete the student.\n"
             "3 - Display students from Memory.\n"
             "4 - Display the student names in ascending order by the ID.\n"
             "5 - Display the student names in ascending order by the first letter of their names.\n"
             "6 - Display the school numbers by the faculty codes in ascending order.\n"
             "7 - End the program.");
    puts("---------------------------------------------------------------------------------------");
}
void insert(listNodePtr *sPtr, char value) {
    /*
     *          - insert(listNodePtr, value) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> It is the function that adds the given character to the linked list.
     *          -> It is used for the linked list that holds the student name.
     */

//    The node to be added has been created.
    listNodePtr newPtr = malloc(sizeof(listNode));

    if (newPtr != NULL)
    { // is space available?
        newPtr->character = value;
        newPtr->nextNode = NULL;

        listNodePtr previousPtr = NULL;
        listNodePtr currentPtr = *sPtr;

//    loop to go to end of list
        while (currentPtr != NULL) {
            previousPtr = currentPtr;
            currentPtr = currentPtr->nextNode;
        }

//    insert new node at beginning of list
        if (previousPtr == NULL) {
            newPtr->nextNode = *sPtr;
            *sPtr = newPtr;
        }

//    insert new node between previousPtr and currentPtr, (last element)
        else {
            previousPtr->nextNode = newPtr;
//            currentPtr equals NULL.
            newPtr->nextNode = currentPtr;
        }
    }
    else
    {
        printf("%c not inserted. No memory available.\n", value);
    }
}
void displayStudent(headerNodePtr sPtr)
{
    /*
     *          - displayStudent(headerNodePtr) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> Prints student information as linked list notation.
     *          -> Gets student argument as sPtr.
     *          -> sPtr is defined as haderNodePtr
     */

    printf("%s","[LINKED LIST NOTATION]: ");
//    print school number
    printf("%d ", sPtr->schoolNumber);
//    create namePtr and assign as namePtr (it points first character of name)
    listNodePtr namePtr = sPtr->nextNode;
//    loop until end of string or end of linked list (NULL)
    while (namePtr != NULL)
    {
//        print character
        printf("-> %c ", namePtr->character);
//        memory pointer goes to next place
        namePtr = namePtr->nextNode;
    }
    printf("-> %s\n", "NULL");
    puts("");
}
void push(stackNodePtr *topPtr, headerNodePtr studentPtr)
{
    /*
     *          - push(stackNodePtr, headerNodePtr) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> The "push" function, which is the basic addition function of stacks,
     *          is used to add elements to the stack.
     *          ->This function adds the pointer to the student (headerNodePtr)...
     *          ...to the stack structure of the stack pointer given as the first argument.
     */

//    Then, a stack pointer (StackNodePtr) of type "newPtr" was created to be added to the stack,...
//    ...and space was allocated for this pointer in memory (malloc).
    stackNodePtr newPtr = malloc(sizeof(stackNode));

//    If memory is available
    if (newPtr != NULL)
    {
//        Points to the student pointer to be added in the new stack pointer.
        newPtr->student = studentPtr;
//        The new stack pointer is connected to the current stack's pointer, ie the topmost element.
        newPtr->nextPtr = *topPtr;
//        Then, the value of the main stack pointer is linked to...
//        ...the value of the new stack display (added pointer) (newPtr).
//        This is done so because the argument is given as the pointer address. (Double Indirection)
        *topPtr = newPtr;
    }
    else
    {
        printf("not inserted. No memory available.\n");
    }
}
headerNodePtr pop(stackNodePtr *topPtr)
{
    /*
     *          - headerNodePtr(stackNodePtr) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> It deletes the last value added in the stack structure
     *          indicated by the stack pointer given as an argument.
     *          -> Classis stack pop() function.
     *          -> Not used in this program.
     */
    stackNodePtr tempPtr = *topPtr;
    headerNodePtr popStudent = (*topPtr)->student;
    *topPtr = (*topPtr)->nextPtr;
    free(tempPtr);
    return popStudent;
}
void printStack(stackNodePtr currentPtr)
{
    /*
     *          - printStack(stackNodePtr) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> It prints the elements in the stack structure...
     *          ...that the argument given as an argument points to. (top to bottom)
     */
    if (currentPtr == NULL)
    {
        puts("The stack is empty.\n");
    }
    else
    {
        puts("Recorded Students - (School Number View):");
        puts("-----------------------------------------");
//        Loop that prints the numbers in the stack in order (top to bottom OR new to old)
        while (currentPtr != NULL)
        {
            printf("%d\n---------\n", currentPtr->student->schoolNumber);
            currentPtr = currentPtr->nextPtr;
        }
        puts("NULL\n");
    }
}
char* linkedListToString(headerNodePtr studentPtr)
{
    /*
     *          - linkedListToString(headerNodePtr) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> It takes the pointer pointing to the student as an argument...
     *          ...and creates a string from the retained characters, one ...
     *          ...character per node, and returns it.
     *          -> It is written because it gives practicality where necessary.
     */

//    NamePtr is created and assigned to hold the listNodePtr type (studentPtr->nextNode)...
//    ...where the characters in the student's name are started to be retained from the...
//    ...pointer pointing to the student.
    listNodePtr namePtr = studentPtr->nextNode;
//    The pointer to hold the name is defined and the spaces it occupies are separated...
//    ...with the calloc function and an empty value is entered.
    char* name = calloc(30, sizeof(char));
    int i = 0;
    //  Loop that scans the characters contained within the linked list nodes,...
    //  ...respectively, and adds them to the name representation.
    while (namePtr != NULL && i < NAME_MAX_LIMIT)
    {
//        name[i] -> pointer-index notation
        name[i] = namePtr->character;
//        namePtr points next place
        namePtr = namePtr->nextNode;
//        index is increased
        i = i + 1;
    }
    return name;
}
size_t sizeOfStack(stackNodePtr currentPtr)
{
    /*
     *          - sizeOfStack(StackNodePtr) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> This function returns the number of elements in the stack pointed to...
     *          ...by the stack pointer given as an argument.
     *          -> It is written because it provides usefulness where the number of...
     *          ...registered students is required in the program flow.
     */

//    size variable keeps the number of elements in the stack.
    size_t size = 0;
    if (currentPtr == NULL)
    {
        size = 0;
    }
    else
    {
//        This loop cycles through the stack sequentially.
        while (currentPtr != NULL)
        {
//           size is incremented by 1 each time.
            size += 1;
            currentPtr = currentPtr->nextPtr;
        }
    }
    return size;
}
