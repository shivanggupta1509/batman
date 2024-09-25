#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants for maximum limits
#define MAX_STUDENTS 100   // Maximum number of students per subject
#define MAX_SUBJECTS 5     // Maximum number of subjects per branch
#define MAX_BRANCHES 3     // Maximum number of branches

// Structure to hold student information
typedef struct {
    int roll_number;         // Student's unique roll number
    char name[50];           // Student's name
    int attended;            // Number of lectures attended by the student
    int total_classes;       // Total number of lectures conducted
} Student;

// Structure to hold subject information
typedef struct {
    char name[50];           // Name of the subject
    Student students[MAX_STUDENTS];  // Array to store students enrolled in this subject
    int student_count;       // Current count of students in this subject
} Subject;

// Structure to hold branch information
typedef struct {
    char name[50];           // Name of the branch
    Subject subjects[MAX_SUBJECTS]; // Array of subjects offered in this branch
    int subject_count;       // Current count of subjects in this branch
} Branch;

// Global array to hold branches and a variable to track the number of branches
Branch branches[MAX_BRANCHES]; // Array to store branches
int branch_count = 0;           // Current number of branches added

// Function prototypes for managing branches, subjects, and students
void add_branch();
void add_subject();
void add_student();
void add_student_to_subject(int branch_index, int subject_index, int roll_number, char *name);
void remove_student();
void feed_attendance();
void check_eligibility();
void save_data();

// Function to add a new branch
void add_branch() {
    // Check if the maximum number of branches has been reached
    if (branch_count >= MAX_BRANCHES) { 
        printf("Maximum branches reached!\n");
        return; // Exit if limit is reached
    }
    printf("Enter branch name: ");
    scanf("%s", branches[branch_count].name); // Get branch name from user
    branches[branch_count].subject_count = 0; // Initialize subject count for the new branch
    branch_count++; // Increment the total branch count
    save_data(); // Save the current state to a file after adding a branch
}

// Function to save all data to a text file
void save_data() {
    FILE *file = fopen("attendance_data.txt", "w"); // Open file for writing
    if (!file) { // Check if the file opened successfully
        printf("Error opening file!\n");
        return; // Exit if there is an error
    }
    // Write each branch's details to the file
    for (int i = 0; i < branch_count; i++) {
        fprintf(file, "Branch: %s\n", branches[i].name);
        // Write each subject's details within the branch
        for (int j = 0; j < branches[i].subject_count; j++) {
            fprintf(file, "  Subject: %s\n", branches[i].subjects[j].name);
            // Write each student's details within the subject
            for (int k = 0; k < branches[i].subjects[j].student_count; k++) {
                Student s = branches[i].subjects[j].students[k];
                fprintf(file, "    Roll No: %d, Name: %s, Attended: %d, Total Classes: %d\n",
                        s.roll_number, s.name, s.attended, s.total_classes);
            }
        }
    }
    fclose(file); // Close the file after writing
}

// Function to add a new subject to a specific branch
void add_subject() {
    // Check if there are any branches to add a subject to
    if (branch_count == 0) {
        printf("No branches available. Please add a branch first.\n");
        return; // Exit if no branches exist
    }

    int branch_index; // Variable to hold the selected branch index
    printf("Select branch index (0 to %d): ", branch_count - 1);
    scanf("%d", &branch_index); // Get branch index from the user
    
    // Validate the selected branch index
    if (branch_index < 0 || branch_index >= branch_count) {
        printf("Invalid branch index!\n");
        return; // Exit if the index is invalid
    }
    
    // Check if the maximum number of subjects for this branch has been reached
    if (branches[branch_index].subject_count >= MAX_SUBJECTS) {
        printf("Maximum subjects reached for this branch!\n");
        return; // Exit if limit is reached
    }

    // Prompt user to enter the subject name
    printf("Enter subject name: ");
    scanf("%s", branches[branch_index].subjects[branches[branch_index].subject_count].name); // Get subject name
    branches[branch_index].subjects[branches[branch_index].subject_count].student_count = 0; // Initialize student count for new subject
    branches[branch_index].subject_count++; // Increment the subject count for the branch
    save_data(); // Save the current state to a file after adding a subject
}

// Function to add a student to a specific subject
void add_student() {
    // Check if there are branches available to add students
    if (branch_count == 0) {
        printf("No branches available. Please add a branch first.\n");
        return; // Exit if no branches exist
    }

    int branch_index, subject_index, roll_number; // Variables for branch and subject selection, and roll number
    char name[50]; // Variable to hold the student's name

    // Prompt user to select a branch
    printf("Select branch index (0 to %d): ", branch_count - 1);
    scanf("%d", &branch_index); // Get branch index from the user
    
    // Validate the selected branch index
    if (branch_index < 0 || branch_index >= branch_count) {
        printf("Invalid branch index!\n");
        return; // Exit if the index is invalid
    }

    // Check if there are subjects available in the selected branch
    if (branches[branch_index].subject_count == 0) {
        printf("No subjects available in this branch. Please add a subject first.\n");
        return; // Exit if no subjects exist
    }

    // Prompt user to select a subject
    printf("Select subject index (0 to %d): ", branches[branch_index].subject_count - 1);
    scanf("%d", &subject_index); // Get subject index from the user
    
    // Validate the selected subject index
    if (subject_index < 0 || subject_index >= branches[branch_index].subject_count) {
        printf("Invalid subject index!\n");
        return; // Exit if the index is invalid
    }

    // Prompt user to enter roll number and name for the student
    printf("Enter roll number: ");
    scanf("%d", &roll_number); // Get roll number from the user
    printf("Enter student name: ");
    scanf("%s", name); // Get student name from the user

    // Call the function to add the student to the selected subject
    add_student_to_subject(branch_index, subject_index, roll_number, name);
    save_data(); // Save the current state to a file after adding a student
}

// Function to add a student to the specified subject's student list
void add_student_to_subject(int branch_index, int subject_index, int roll_number, char *name) {
    // Get reference to the selected subject
    Subject *subject = &branches[branch_index].subjects[subject_index]; 
    
    // Check if the maximum number of students for this subject has been reached
    if (subject->student_count >= MAX_STUDENTS) {
        printf("Maximum students reached for this subject!\n");
        return; // Exit if limit is reached
    }
    
    // Reference the next available slot for the new student
    Student *s = &subject->students[subject->student_count++]; 
    s->roll_number = roll_number; // Assign roll number to the student
    strcpy(s->name, name); // Copy the student's name into the structure
    s->attended = 0; // Initialize attended lectures count
    s->total_classes = 0; // Initialize total classes count
    printf("Student added successfully!\n"); // Confirmation message
}

// Function to remove a student from a subject
void remove_student() {
    // Check if branches exist before trying to remove a student
    if (branch_count == 0) {
        printf("No branches available. Please add a branch first.\n");
        return; // Exit if no branches exist
    }

    int branch_index, subject_index, roll_number; // Variables for branch, subject, and roll number

    // Prompt user to select a branch
    printf("Select branch index (0 to %d): ", branch_count - 1);
    scanf("%d", &branch_index); // Get branch index from the user
    
    // Validate the selected branch index
    if (branch_index < 0 || branch_index >= branch_count) {
        printf("Invalid branch index!\n");
        return; // Exit if the index is invalid
    }

    // Check if subjects exist in the selected branch
    if (branches[branch_index].subject_count == 0) {
        printf("No subjects available in this branch.\n");
        return; // Exit if no subjects exist
    }

    // Prompt user to select a subject
    printf("Select subject index (0 to %d): ", branches[branch_index].subject_count - 1);
    scanf("%d", &subject_index); // Get subject index from the user
    
    // Validate the selected subject index
    if (subject_index < 0 || subject_index >= branches[branch_index].subject_count) {
        printf("Invalid subject index!\n");
        return; // Exit if the index is invalid
    }

    // Prompt user to enter the roll number of the student to be removed
    printf("Enter roll number of student to remove: ");
    scanf("%d", &roll_number); // Get roll number from the user

    // Get reference to the selected subject
    Subject *subject = &branches[branch_index].subjects[subject_index]; 
    // Loop through the students in the subject to find the student to remove
    for (int i = 0; i < subject->student_count; i++) {
        if (subject->students[i].roll_number == roll_number) { // Check if roll number matches
            // Shift students to fill the gap left by the removed student
            for (int j = i; j < subject->student_count - 1; j++) {
                subject->students[j] = subject->students[j + 1];
            }
            subject->student_count--; // Decrement the student count
            printf("Student removed successfully!\n"); // Confirmation message
            save_data(); // Save the current state to a file after removing a student
            return; // Exit the function
        }
    }
    printf("Student not found!\n"); // If student was not found
}

// Function to feed attendance for a student
void feed_attendance() {
    // Check if branches exist before trying to feed attendance
    if (branch_count == 0) {
        printf("No branches available. Please add a branch first.\n");
        return; // Exit if no branches exist
    }

    int branch_index, subject_index, roll_number, attended, total_classes; // Variables for branch, subject, roll number, and attendance

    // Prompt user to select a branch
    printf("Select branch index (0 to %d): ", branch_count - 1);
    scanf("%d", &branch_index); // Get branch index from the user
    
    // Validate the selected branch index
    if (branch_index < 0 || branch_index >= branch_count) {
        printf("Invalid branch index!\n");
        return; // Exit if the index is invalid
    }

    // Check if subjects exist in the selected branch
    if (branches[branch_index].subject_count == 0) {
        printf("No subjects available in this branch.\n");
        return; // Exit if no subjects exist
    }

    // Prompt user to select a subject
    printf("Select subject index (0 to %d): ", branches[branch_index].subject_count - 1);
    scanf("%d", &subject_index); // Get subject index from the user
    
    // Validate the selected subject index
    if (subject_index < 0 || subject_index >= branches[branch_index].subject_count) {
        printf("Invalid subject index!\n");
        return; // Exit if the index is invalid
    }

    // Prompt user to enter the roll number of the student
    printf("Enter roll number: ");
    scanf("%d", &roll_number); // Get roll number from the user

    // Get reference to the selected subject
    Subject *subject = &branches[branch_index].subjects[subject_index]; 
    // Loop through the students to find the student whose attendance is being fed
    for (int i = 0; i < subject->student_count; i++) {
        if (subject->students[i].roll_number == roll_number) { // Check if roll number matches
            // Get attended and total classes from the user
            printf("Enter attended lectures and total lectures (e.g., 10 20): ");
            scanf("%d %d", &attended, &total_classes); // Get attendance details
            
            // Update student's attended and total classes
            subject->students[i].attended += attended; // Increment attended lectures
            subject->students[i].total_classes += total_classes; // Increment total classes
            printf("Attendance updated successfully!\n"); // Confirmation message
            save_data(); // Save the current state to a file after feeding attendance
            return; // Exit the function
        }
    }
    printf("Student not found!\n"); // If student was not found
}

// Function to check student eligibility based on attendance
void check_eligibility() {
    // Check if branches exist before checking eligibility
    if (branch_count == 0) {
        printf("No branches available. Please add a branch first.\n");
        return; // Exit if no branches exist
    }

    int branch_index, subject_index; // Variables for branch and subject selection

    // Prompt user to select a branch
    printf("Select branch index (0 to %d): ", branch_count - 1);
    scanf("%d", &branch_index); // Get branch index from the user
    
    // Validate the selected branch index
    if (branch_index < 0 || branch_index >= branch_count) {
        printf("Invalid branch index!\n");
        return; // Exit if the index is invalid
    }

    // Check if subjects exist in the selected branch
    if (branches[branch_index].subject_count == 0) {
        printf("No subjects available in this branch.\n");
        return; // Exit if no subjects exist
    }

    // Prompt user to select a subject
    printf("Select subject index (0 to %d): ", branches[branch_index].subject_count - 1);
    scanf("%d", &subject_index); // Get subject index from the user
    
    // Validate the selected subject index
    if (subject_index < 0 || subject_index >= branches[branch_index].subject_count) {
        printf("Invalid subject index!\n");
        return; // Exit if the index is invalid
    }

    // Get reference to the selected subject
    Subject *subject = &branches[branch_index].subjects[subject_index]; 
    // Print a header for the eligibility report
    printf("Students not eligible (below 75%% attendance):\n");
    for (int i = 0; i < subject->student_count; i++) { // Loop through each student
        Student s = subject->students[i]; // Get student reference
        // Calculate attendance percentage
        if (s.total_classes > 0) { // Avoid division by zero
            float percentage = (float)s.attended / s.total_classes * 100; // Calculate percentage
            if (percentage < 75.0) { // Check if below 75%
                // Print student details if not eligible
                printf("Roll No: %d, Name: %s, Attendance: %.2f%%\n", s.roll_number, s.name, percentage);
            }
        }
    }
}

// Main function to run the program
int main() {
    int choice; // Variable for user menu choice

    // Loop to display the menu until the user chooses to exit
    do {
        printf("\n--- Attendance Management System ---\n");
        printf("1. Add Branch\n");
        printf("2. Add Subject\n");
        printf("3. Add Student\n");
        printf("4. Remove Student\n");
        printf("5. Feed Attendance\n");
        printf("6. Check Eligibility\n");
        printf("7. Save Data\n");
        printf("8. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice); // Get user's choice

        // Switch statement to handle user's menu selection
        switch (choice) {
            case 1:
                add_branch(); // Call function to add a branch
                break;
            case 2:
                add_subject(); // Call function to add a subject
                break;
            case 3:
                add_student(); // Call function to add a student
                break;
            case 4:
                remove_student(); // Call function to remove a student
                break;
            case 5:
                feed_attendance(); // Call function to feed attendance
                break;
            case 6:
                check_eligibility(); // Call function to check student eligibility
                break;
            case 7:
                save_data(); // Call function to save all data
                printf("Data saved successfully!\n"); // Confirmation message
                break;
            case 8:
                printf("Exiting...\n"); // Exit message
                break;
            default:
                printf("Invalid choice! Please try again.\n"); // Error message for invalid choice
        }
    } while (choice != 8); // Continue until user chooses to exit

    return 0; // End of the program
}
