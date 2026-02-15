//1. Insert first in array

void insertFirst (int arr[], int &size, int capacity, int value) {
    if (size >= capacity) {
        // Array is full, cannot insert
        return;
    }

    for (int i= size; i>0; i--)
    {
        arr[i] = arr[i-1]; // Shift elements to the right
    }
    arr[0] = value; // Insert new value at the beginning
    size++;


2. // delete first in array

void deleteFirst(int arr[], int &size) {
    if (size == 0) {
        // Array is empty, cannot delete
        return;
    }

    for (int i = 0; i < size - 1; i++) { // Shift elements to the left
        arr[i] = arr[i + 1]; // Shift elements to the left
    }
    size--; // Decrease the size of the array
}

3. //inser last in array
void insertLast(int arr[], int &size, int capacity, int value) {
    if (size >= capacity) {
        // Array is full, cannot insert
        return;
    }

    arr[size] = value; // Insert new value at the end
    size++;
}


4. //delete last in array
void deleteLast(int arr[], int &size) {
    if (size == 0) {
        // Array is empty, cannot delete
        return;
    }

    size--; // Decrease the size of the array
}



5. //insert at any position in array
void insertAtPosition(int arr[], int &size, int capacity, int value, int position) {
    if (size >= capacity || position < 1 || position > size + 1) {
        // Array is full or position is invalid
        return;
    }

    for (int i = size; i >= position; i--) {
        arr[i] = arr[i - 1]; // Shift elements to the right
    }
    arr[position - 1] = value; // Insert new value at the specified position
    size++;
}


6. //delete at any position in array
// Delete element at a specific position (1-based index)
void deleteAtPosition(int arr[], int &size, int position) {
    if (size == 0 || position < 1 || position > size) {
        // Array is empty or position is invalid
        return;
    }

    for (int i = position - 1; i < size - 1; i++) {
        arr[i] = arr[i + 1]; // Shift elements to the left
    }

    size--; // Reduce the size
}


//linear search in array
int linearSearch(int arr[], int size, int key) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == key) {
            return i; // Return the index of the found element
        }
    }
    return -1; // Return -1 if the element is not found
}

//binary search in array
int binarySearch(int arr[], int size, int key) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Avoid overflow

        if (arr[mid] == key) {
            return mid; // Element found
        } else if (arr[mid] < key) {
            low = mid + 1; // Search in the right half
        } else {
            high = mid - 1; // Search in the left half
        }
    }
    return -1; // Element not found
}


//bubble sort in array

void bubblesort (int arr[] , int size) {
    for (int j = 0; j < size ; j++) {
        for (int i = 0; i < size - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                // Swap arr[i] and arr[i + 1]
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
        }
    }
}


//merge sort in array

while (i < n1 && j < n2) { // Merge elements from both arrays
    if (arr1 [i] <= arr2[j]) {
        new_arr[k] = arr1[i];
        i++;
        k++;
    } else {
        new_arr[k] = arr2[j];
        j++;
        k++;
    }
}

while (i < n1)
{
    new_arr[k] = arr1[i]; // Copy remaining elements from arr1
    i++;
    k++;
}

while (j < n2)
{
    new_arr[k] = arr2[j]; // Copy remaining elements from arr2
    j++;
    k++;
}


//Traverse linked list

struct Node {
    int data; // Data of the node
    Node* next; // Pointer to the next node
};

void traverselinkedList (Node* head) {
    Node* temp;
    temp = head; // Start from the head of the linked list
    while(temp!=NULL) {
        cout << temp->data <<" "; // Print the data of the current node
        temp = temp->next; // Move to the next node
    }
    cout << endl; // Print a new line after traversing the list
}


//traverse doubly linked list
struct Node {
    int data; // Data of the node
    Node* next; // Pointer to the next node
    Node* prev; // Pointer to the previous node
};

void traverseDoublyLinkedList(Node* head) {
    Node* temp = head; // Start from the head of the doubly linked list
    while (temp != NULL) {
        cout << temp->data << " "; // Print the data of the current node
        temp = temp->next; // Move to the next node
    }
    cout << endl; // Print a new line after traversing the list
}


    void traverseDoublyLinkedListReverse(Node* tail) {
        Node* temp = tail; // Start from the tail of the doubly linked list
        while (temp != NULL) {
            cout << temp->data << " "; // Print the data of the current node
            temp = temp->prev; // Move to the previous node
        }
        cout << endl; // Print a new line after traversing the list
    }




//Singly linked list insertion at the beginning
void insertAtBeginning(Node*& head, int value) { //
    Node* newNode = new Node; // Create a new node
    newNode->data = value; // Set the data of the new node
    newNode->next = head; // Point the new node to the current head
    head = newNode; // Update the head to point to the new node
}


//Singly Linked list delete at the beginning
void deleteAtBeginning(Node*& head) {
    if (head == NULL) {
        // List is empty, nothing to delete
        return;
    }
    Node* temp = head; // Store the current head
    head = head->next; // Update the head to the next node
    delete temp; // Delete the old head node
}

//Singly linked list insertion at the end
void insertAtEnd(Node*& head, int value) {
    Node* newNode = new Node; // Create a new node
    newNode->data = value; // Set the data of the new node
    newNode->next = NULL; // The new node will be the last node

    if (head == NULL) {
        head = newNode; // If the list is empty, set head to the new node
        return;
    }

    Node* temp = head; // Start from the head
    while (temp->next != NULL) {
        temp = temp->next; // Traverse to the last node
    }
    temp->next = newNode; // Link the last node to the new node
}


//Singly linked list delete at the end
void deleteAtEnd(Node*& head) {
    if (head == NULL) {
        // List is empty, nothing to delete
        return;
    }
    if (head->next == NULL) {
        // List has only one node
        delete head;
        head = NULL;
        return;
    }
    Node* temp = head;
    while (temp->next->next != NULL) { // Traverse to the second last node
        temp = temp->next; // Move to the next node
    }
    delete temp->next; // Delete the last node
    temp->next = NULL; // Update the second last node's next pointer
}
    while (temp != NULL) {
        cout << temp->data << " "; // Print the data of the current node
        temp = temp->next; // Move to the next node
    }
    cout << endl; // Print a new line after traversing the list
}


//Singly linked list insertion at a specific position

void insertAtPosition(Node*& head, int value, int position) {
    if (position < 1) {
        // Invalid position
        return;
    }
    Node* newNode = new Node; // Create a new node
    newNode->data = value; // Set the data of the new node

    if (position == 1) {
        newNode->next = head; // Insert at the beginning
        head = newNode; // Update head
        return;
    }

    Node* temp = head;
    int currentPosition = 1; // Start from the head
    while (temp != NULL && currentPosition < position - 1) { // Traverse to the position before the desired position
        temp = temp->next; // Move to the next node
        currentPosition++; // Increment the current position
    }

    if (temp == NULL) {
        // Position is greater than the length of the list, insert at the end
        delete newNode; // Clean up if we can't insert
        return;
    }

    newNode->next = temp->next; // Link the new node to the next node
    temp->next = newNode; // Link the previous node to the new node
}


//Singly linked list delete at a specific position

void deleteAtPosition(Node*& head, int position) {
    if (head == NULL || position < 1) {
        // List is empty or invalid position
        return;
    }

    if (position == 1) {
        Node* temp = head; // Store the current head
        head = head->next; // Update the head to the next node
        delete temp; // Delete the old head node
        return;
    }

    Node* temp = head;
    int currentPosition = 1; // Start from the head
    while (temp != NULL && currentPosition < position - 1) { // Traverse to the node before the desired position
        temp = temp->next; // Move to the next node
        currentPosition++; // Increment the current position
    }

    if (temp == NULL || temp->next == NULL) {
        // Position is greater than the length of the list or no node to delete
        return;
    }

    Node* nodeToDelete = temp->next; // Node to be deleted
    temp->next = nodeToDelete->next; // Link the previous node to the next node of the node to be deleted
    delete nodeToDelete; // Delete the node
}


//Insert first in Doubly linked list

void insertAtBeginningDoubly(Node*& head, int value) {
    Node* newNode = new Node; // Create a new node
    newNode->data = value; // Set the data of the new node
    newNode->next = head; // Point the new node to the current head
    newNode->prev = NULL; // New node will be the first node
    if (head != NULL) {
        head->prev = newNode; // Update the previous pointer of the current head
    }
    head = newNode; // Update the head to point to the new node
}


//Delete first in Doubly linked list
void deleteAtBeginningDoubly(Node*& head) {
    if (head == NULL) {
        // List is empty, nothing to delete
        return;
    }
    Node* temp = head; // Store the current head
    head = head->next; // Update the head to the next node
    if (head != NULL) {
        head->prev = NULL; // Update the previous pointer of the new head
    }
    delete temp; // Delete the old head node
}


//Insert last in Doubly linked list
void insertAtEndDoubly(Node*& head, int value) {
    Node* newNode = new Node; // Create a new node
    newNode->data = value; // Set the data of the new node
    newNode->next = NULL; // The new node will be the last node
    newNode->prev = NULL; // Initialize previous pointer

    if (head == NULL) {
        head = newNode; // If the list is empty, set head to the new node
        return;
    }

    Node* temp = head; // Start from the head
    while (temp->next != NULL) {
        temp = temp->next; // Traverse to the last node
    }
    temp->next = newNode; // Link the last node to the new node
    newNode->prev = temp; // Link the new node back to the last node
}

//Delete last in Doubly linked list
void deleteAtEndDoubly(Node*& head) {
    if (head == NULL) {
        // List is empty, nothing to delete
        return;
    }
    if (head->next == NULL) {
        // List has only one node
        delete head;
        head = NULL;
        return;
    }
    Node* temp = head;
    while (temp->next != NULL) { // Traverse to the second last node
        temp = temp->next; // Move to the next node
    }
    temp->prev->next = NULL; // Update the second last node's next pointer
    delete temp; // Delete the last node
}


//Insert at any position in Doubly linked list
void insertAtPositionDoubly(Node*& head, int value, int position) {
    if (position < 1) {
        // Invalid position
        return;
    }
    Node* newNode = new Node; // Create a new node
    newNode->data = value; // Set the data of the new node

    if (position == 1) {
        insertAtBeginningDoubly(head, value); // Insert at the beginning
        return;
    }

    Node* temp = head;
    int currentPosition = 1; // Start from the head
    while (temp != NULL && currentPosition < position - 1) { // Traverse to the position before the desired position
        temp = temp->next; // Move to the next node
        currentPosition++; // Increment the current position
    }

    if (temp == NULL) {
        // Position is greater than the length of the list, insert at the end
        delete newNode; // Clean up if we can't insert
        return;
    }

    newNode->next = temp->next; // Link the new node to the next node
    newNode->prev = temp; // Link the new node back to the previous node
    if (temp->next != NULL) {
        temp->next->prev = newNode; // Update the previous pointer of the next node
    }
    temp->next = newNode; // Link the previous node to the new node
}


//Delete at any position in Doubly linked list
void deleteAtPositionDoubly(Node*& head, int position) {
    if (head == NULL || position < 1) {
        // List is empty or invalid position
        return;
    }

    if (position == 1) {
        deleteAtBeginningDoubly(head); // Delete at the beginning
        return;
    }

    Node* temp = head;
    int currentPosition = 1; // Start from the head
    while (temp != NULL && currentPosition < position - 1) { // Traverse to the node before the desired position
        temp = temp->next; // Move to the next node
        currentPosition++; // Increment the current position
    }

    if (temp == NULL || temp->next == NULL) {
        // Position is greater than the length of the list or no node to delete
        return;
    }

    Node* nodeToDelete = temp->next; // Node to be deleted
    temp->next = nodeToDelete->next; // Link the previous node to the next node of the node to be deleted
    if (nodeToDelete->next != NULL) {
        nodeToDelete->next->prev = temp; // Update the previous pointer of the next node
    }
    delete nodeToDelete; // Delete the node
}


//merge two sorted linked lists
//merge two sorted linked list 
// Merge two sorted linked lists
Node* mergeSortedLists(Node* link1, Node* link2) {
    Node* link3 = NULL;     // Merged list head
    Node* temp3 = NULL;     // Pointer to build the merged list

    while (link1 != NULL && link2 != NULL) {// Compare nodes from both lists
        Node* temp = NULL; // Pointer to hold the smaller node

        if (link1->data < link2->data) {// Choose the smaller node
            temp = link1;// Link from the first list
            link1 = link1->next; // Move to the next node in the first list
        } else {
            temp = link2; // Link from the second list
            link2 = link2->next; // Move to the next node in the second list
        }

        if (link3 == NULL) { // If merged list is empty, initialize it
            link3 = temp; // Set the head of the merged list
            temp3 = temp; // Set the head of the merged list
        } else {
            temp3->next = temp; // Link the new node to the merged list
            temp3 = temp; // Move the pointer to the last node in the merged list
        }
    }

    // Attach remaining nodes
    if (link1 != NULL) { // If there are remaining nodes in the first list
        if (link3 == NULL) link3 = link1; // If merged list is empty, set it to the first list
        else temp3->next = link1; // Link the remaining nodes to the merged list
    }
    if (link2 != NULL) { // If there are remaining nodes in the second list
        if (link3 == NULL) link3 = link2; // If merged list is empty, set it to the second list
        else temp3->next = link2; // Link the remaining nodes to the merged list
    }

    return link3; // Return the head of the merged list
}
