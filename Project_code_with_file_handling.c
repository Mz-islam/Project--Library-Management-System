#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structure to represent each reservation (waiting member)
typedef struct reservation
{
    int memberId;
    struct reservation *next;
} Reservation;

// Structure to represent the queue of reservations
typedef struct queue
{
    Reservation *front;
    Reservation *rear;
} Queue;

// Structure to store book information
typedef struct book
{
    int id;     
    char title[20];
    char author[20];
    int isIssued;           
    Queue reservationQueue; 
    struct book *next;      
} Book;

// Structure to store user information
typedef struct userInfo
{
    char username[50];     
    char password[20];     
    struct userInfo *next; 
} User;

// Structure to store member information
typedef struct memberInfo
{
    int id;                 
    char name[30];           
    int penalty;             
    struct memberInfo *next; 
} Member;

// Global variables for linked lists
User *userhead = NULL;     
Book *bookHead = NULL;     
Member *memberHead = NULL; 

// Function declarations
void adminLogin();
void registerUser();
void userLogin();
void adminMenu();
void userMenu();
void addBook();
void displayBook();
void editBook();
void deleteBook();
void searchBook();
void addMember();
void displayAllMember();
void issueBook();
void returnBook();
void calculatePenalty();
void reserveBook();

// Initialize an empty reservation queue
void initQueue(Queue *q)
{
    q->front = q->rear = NULL;
}

// Add a member to the reservation queue
void enqueue(Queue *q, int memberId)
{
    Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
    newReservation->memberId = memberId;
    newReservation->next = NULL;

    // if the queue is empty
    if (q->front == NULL)
    {
        q->front = q->rear = newReservation;
    }
    else
    {
        q->rear->next = newReservation;
        q->rear = newReservation;
    }
}

// Remove the first reservation from the queue (FIFO)
int dequeue(Queue *q)
{
    if (q->front == NULL) // queue is empty, no reservation
    {
        return -1;
    }
    int memberId = q->front->memberId;
    Reservation *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    free(temp);

    return memberId;
}

// Check if the queue is empty
int isQueueEmpty(Queue *q)
{
    return q->front == NULL;
}
//Save Functions for Books
void saveBooksToFile() {
    FILE *file = fopen("books.dat", "wb");
    if (!file) {
        printf("Error opening books file for writing.\n");
        return;
    }
    Book *temp = bookHead;
    while (temp) {
        fwrite(temp, sizeof(Book), 1, file);
        temp = temp->next;
    }
    fclose(file);
}
//Load Functions for Books
void loadBooksFromFile() {
    FILE *file = fopen("books.dat", "rb");
    if (!file) {
        printf("No books file found. Starting fresh.\n");
        return;
    }
    Book temp;
    while (fread(&temp, sizeof(Book), 1, file)) {
        Book *newBook = (Book *)malloc(sizeof(Book));
        *newBook = temp;
        newBook->next = bookHead;
        bookHead = newBook;
    }
    fclose(file);
}
//Save Functions for Members
void saveMembersToFile() {
    FILE *file = fopen("members.dat", "wb");
    if (!file) {
        printf("Error opening members file for writing.\n");
        return;
    }
    Member *temp = memberHead;
    while (temp) {
        fwrite(temp, sizeof(Member), 1, file);
        temp = temp->next;
    }
    fclose(file);
}
//Load Functions for Members
void loadMembersFromFile() {
    FILE *file = fopen("members.dat", "rb");
    if (!file) {
        printf("No members file found. Starting fresh.\n");
        return;
    }
    Member temp;
    while (fread(&temp, sizeof(Member), 1, file)) {
        Member *newMember = (Member *)malloc(sizeof(Member));
        *newMember = temp;
        newMember->next = memberHead;
        memberHead = newMember;
    }
    fclose(file);
}
//Save Functions for Users
void saveUsersToFile() {
    FILE *file = fopen("users.dat", "wb");
    if (!file) {
        printf("Error opening users file for writing.\n");
        return;
    }
    User *temp = userhead;
    while (temp) {
        fwrite(temp, sizeof(User), 1, file);
        temp = temp->next;
    }
    fclose(file);
}
//Load Functions for Users
void loadUsersFromFile() {
    FILE *file = fopen("users.dat", "rb");
    if (!file) {
        printf("No users file found. Starting fresh.\n");
        return;
    }
    User temp;
    while (fread(&temp, sizeof(User), 1, file)) {
        User *newUser = (User *)malloc(sizeof(User));
        *newUser = temp;
        newUser->next = userhead;
        userhead = newUser;
    }
    fclose(file);
}


int main()
{
    loadBooksFromFile();
    loadMembersFromFile();
    loadUsersFromFile();
    int choice;

    printf("Welcome to Library Management System\n");
    do
    {
        // Display menu options
        printf("1. Admin login\n");
        printf("2. User login\n");
        printf("3. Registration\n");
        printf("0. Exit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        // Handle user choice
        switch (choice)
        {
        case 1:
            adminLogin();
            break;
        case 2:
            userLogin();
            break;
        case 3:
            registerUser();
            break;
        case 0:
            printf("Exiting.....\n");
            break;
        default:
            printf("Invalid option\n");
        }
    } while (choice != 0); // Loop until user chooses to exit
}

// Function for admin login
void adminLogin()
{
    char password[20];
    printf("Enter your admin password :");
    scanf("%s", password);

    // Check if the entered password matches the predefined admin password
    if (strcmp(password, "admin") != 0)
    {
        printf("Invalid password\n");
    }
    else
    {
        printf("Login successfull!\n");
        adminMenu(); // Redirect to admin menu
    }
}

// Function for user login
void userLogin()
{
    char username[50], password[20];
    int loggedIn = 0;

    printf("Enter your username :");
    scanf("%s", username);
    printf("Enter your password :");
    scanf("%s", password);

    // Traverse the user list to check login credentials
    User *temp = userhead;
    while (temp != NULL)
    {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
        {
            loggedIn = 1; // Set login status to true
            break;
        }
        temp = temp->next;
    }

    if (loggedIn)
    {
        printf("Login successful!\n");
        userMenu(); // Redirect to user menu
    }
    else
    {
        printf("Invalid username or password\n");
    }
}

// Function to register a new user
void registerUser()
{
    User *newUser = (User *)malloc(sizeof(User));

    // Collect username and password
    printf("Enter your username :");
    scanf("%s", newUser->username);
    printf("Enter your password :");
    scanf("%s", newUser->password);

    // Add the new user to the linked list
    newUser->next = userhead;
    userhead = newUser;

    printf("Registration successful\n");
    saveUsersToFile();
}

// Function to add a new book
void addBook()
{
    Book *newBook = (Book *)malloc(sizeof(Book));
    if (newBook == NULL)
    {
        printf("Book limit reached\n");
        return;
    }

    // Collect book details
    printf("Enter book ID :");
    scanf("%d", &newBook->id);
    getchar(); // Clear buffer
    printf("Enter book title :");
    fgets(newBook->title, sizeof(newBook->title), stdin);
    newBook->title[strcspn(newBook->title, "\n")] = '\0'; // Remove trailing newline
    printf("Enter book author :");
    fgets(newBook->author, sizeof(newBook->author), stdin);
    newBook->author[strcspn(newBook->author, "\n")] = '\0';

    newBook->isIssued = 0;                 // Mark book as not issued
    initQueue(&newBook->reservationQueue); // Initialize reservation queue
    newBook->next = bookHead;
    bookHead = newBook;

    printf("Book added successfully!\n");
    saveBooksToFile();
}

// Function to display all books
void displayBook()
{
    Book *temp = bookHead;
    printf("\nList of Books:\n");

    // Traverse and print the book list
    while (temp != NULL)
    {
        printf("ID: %d, Title: %s, Author: %s, Issued: %s\n",
               temp->id, temp->title, temp->author, temp->isIssued ? "Yes" : "No");
        temp = temp->next;
    }

}

// Function to edit book details
void editBook()
{
    int id;
    printf("Enter book ID to edit :");
    scanf("%d", &id);

    // Find the book by ID
    Book *temp = bookHead;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            printf("Editing Book: %s\n", temp->title);

            // Update book details
            printf("Enter new title:");
            getchar();
            fgets(temp->title, sizeof(temp->title), stdin);
            temp->title[strcspn(temp->title, "\n")] = '\0';
            printf("Enter new author:");
            fgets(temp->author, sizeof(temp->author), stdin);
            temp->author[strcspn(temp->author, "\n")] = '\0';

            printf("Book updated successfully!\n");
            saveBooksToFile();
            return;
        }
        temp = temp->next;
    }
    printf("Book not found!\n");
}

// Function to delete a book
void deleteBook()
{
    int id;
    printf("Enter book ID to delete :");
    scanf("%d", &id);

    // Check if the book to delete is the head of the list
    if (bookHead != NULL && bookHead->id == id)
    {
        Book *temp = bookHead;
        bookHead = bookHead->next; // Update head pointer
        free(temp);
        printf("Book deleted successfully!\n");
        saveBooksToFile();
        return;
    }

    // Traverse to find and delete the book
    Book *temp = bookHead;
    while (temp->next != NULL)
    {
        if (temp->next->id == id)
        {
            Book *toDelete = temp->next;
            temp->next = temp->next->next;
            free(toDelete);
            printf("Book deleted successfully!\n");
            saveBooksToFile();
            return;
        }
        temp = temp->next;
    }
    printf("Book not found!\n");
}

// The rest of the functions continue with similar detailed comments...
// Function to search for a book by title
void searchBook()
{
    char title[20];
    printf("Enter book title to search: ");
    getchar(); // Clear buffer
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // Remove trailing newline

    // Traverse the book list to find the book
    Book *temp = bookHead;
    while (temp != NULL)
    {
        if (strcmp(temp->title, title) == 0)
        {
            // Print book details if found
            printf("Found book: ID: %d, Title: %s, Author: %s, Issued: %s\n",
                   temp->id, temp->title, temp->author, temp->isIssued ? "Yes" : "No");
            return;
        }
        temp = temp->next;
    }
    printf("No book found with the title '%s'\n", title);
}

// Function to add a new member to the library
void addMember()
{
    Member *newMember = (Member *)malloc(sizeof(Member));
    if (newMember == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    // Collect member details
    printf("Enter member ID: ");
    scanf("%d", &newMember->id);
    printf("Enter member name: ");
    getchar(); // Clear buffer
    fgets(newMember->name, sizeof(newMember->name), stdin);
    newMember->name[strcspn(newMember->name, "\n")] = '\0'; // Remove trailing newline

    newMember->penalty = 0;       // Initialize penalty to 0
    newMember->next = memberHead; // Add to the front of the list
    memberHead = newMember;

    printf("Member added successfully!\n");
    saveMembersToFile();
}

// Function to display all members
void displayAllMember()
{
    Member *temp = memberHead;
    printf("\nList of Members:\n");

    // Traverse and print the member list
    while (temp != NULL)
    {
        printf("ID: %d, Name: %s, Penalty: %d\n", temp->id, temp->name, temp->penalty);
        temp = temp->next;
    }
}

// Function to issue a book to a member
void issueBook()
{
    int bookId, memberId;
    printf("Enter member ID: ");
    scanf("%d", &memberId);
    printf("Enter book ID to issue: ");
    scanf("%d", &bookId);

    // Find the book in the library
    Book *bookTemp = bookHead;
    while (bookTemp != NULL)
    {
        if (bookTemp->id == bookId && bookTemp->isIssued == 0)
        {
            // Find the member
            Member *memberTemp = memberHead;
            while (memberTemp != NULL)
            {
                if (memberTemp->id == memberId)
                {
                    // Issue the book
                    bookTemp->isIssued = 1;
                    printf("Book issued successfully to %s\n", memberTemp->name);
                    saveBooksToFile();
                    return;
                }
                memberTemp = memberTemp->next;
            }
            printf("Member not found!\n");
            return;
        }
        bookTemp = bookTemp->next;
    }
    printf("Either the book is already issued or the book/member was not found!\n");
}

// Function to return a book
void returnBook()
{
    int bookId, memberId;
    printf("Enter member Id: ");
    scanf("%d", &memberId);
    printf("Enter book ID to return: ");
    scanf("%d", &bookId);

    // Traverse the book list to find the book
    Book *temp = bookHead;
    while (temp != NULL)
    {
        if (temp->id == bookId)
        {
            Member *tempMember = memberHead;
            while (tempMember != NULL)
            {
                if (tempMember->id == memberId)
                {

                    if (temp->isIssued == 1)
                    {
                        // Check if there are reservations in the queue
                        if (!isQueueEmpty(&temp->reservationQueue))
                        {
                            int nextMemberId = dequeue(&temp->reservationQueue);
                            printf("Book returned successfully!\n");
                            printf("Member %d has been notified to collect the book.\n", nextMemberId);
                        }
                        else
                        {
                            temp->isIssued = 0; // Mark book as available
                            printf("Book returned successfully! The book is now available.\n");
                            saveBooksToFile();
                        }
                        return;
                    }
                    else
                    {
                        printf("The book is not currently issued!\n");
                        return;
                    }
                }
                tempMember = tempMember->next;
            }
            printf("This Member is not the current borrower of this book!\n");
            return;
        }
        temp = temp->next;
    }
    printf("Book not found!\n");
}
// Function to calculate penalty for overdue books
void calculatePenalty()
{
    int memberId, daysLate;
    printf("Enter member ID for penalty calculation: ");
    scanf("%d", &memberId);

    // Find the member in the list
    Member *temp = memberHead;
    while (temp != NULL)
    {
        if (temp->id == memberId)
        {
            printf("Enter days late: ");
            scanf("%d", &daysLate);

            // Calculate penalty based on days late
            temp->penalty += daysLate * 10;
            printf("Penalty for %s: %d TK\n", temp->name, temp->penalty);
            return;
        }
        temp = temp->next;
    }
    printf("Member not found!\n");
}

// Add Member to Queue When Book is Issued
void reserveBook()
{
    int memberId, bookId;
    printf("Enter your member ID: ");
    scanf("%d", &memberId);
    printf("Enter book ID to reserve: ");
    scanf("%d", &bookId);

    Book *temp = bookHead;
    while (temp != NULL)
    {
        if (temp->id == bookId)
        {
            Member *tempMember = memberHead;
            while (tempMember != NULL)
            {
                if (tempMember->id == memberId)
                {

                    if (temp->isIssued)
                    { // Book is issued, so we add the member to the queue
                        enqueue(&temp->reservationQueue, memberId);
                        printf("Book is issued. You are added to the reservation.\n");
                        return;
                    }
                    else
                    {
                        printf("Book is available, no need to reserve.\n");
                        return;
                    }
                }
                tempMember = tempMember->next;
            }
            printf("Member not found!\n");
            return ;
        }
        temp = temp->next;
    }
    printf("Book not found!\n");
}
// Admin menu function
void adminMenu()
{
    int choice;
    do
    {
        // Display admin menu options
        printf("\nAdmin Menu:\n");
        printf("1. Add Book\n");
        printf("2. Delete Book\n");
        printf("3. Edit Book\n");
        printf("4. Search Book\n");
        printf("5. Display All Books\n");
        printf("6. Add Member\n");
        printf("7. Display All Members\n");
        printf("8. Issue Book\n");
        printf("9. Return Book\n");
        printf("10. Calculate Penalty\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Execute chosen operation
        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            deleteBook();
            break;
        case 3:
            editBook();
            break;
        case 4:
            searchBook();
            break;
        case 5:
            displayBook();
            break;
        case 6:
            addMember();
            break;
        case 7:
            displayAllMember();
            break;
        case 8:
            issueBook();
            break;
        case 9:
            returnBook();
            break;
        case 10:
            calculatePenalty();
            break;
        case 0:
            printf("Logging out...\n");
            break;
        default:
            printf("Invalid option!\n");
        }
    } while (choice != 0); // Loop until admin chooses to logout
}

// User menu function
void userMenu()
{
    int choice;
    do
    {
        // Display user menu options
        printf("\nUser Menu:\n");
        printf("1. Display All Books\n");
        printf("2. Search Book\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Reserve Book\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Execute chosen operation
        switch (choice)
        {
        case 1:
            displayBook();
            break;
        case 2:
            searchBook();
            break;
        case 3:
            issueBook();
            break;
        case 4:
            returnBook();
            break;
        case 5:
            reserveBook();
            break;
        case 0:
            printf("Logging out...\n");
            break;
        default:
            printf("Invalid option!\n");
        }
    } while (choice != 0); // Loop until user chooses to logout
}
