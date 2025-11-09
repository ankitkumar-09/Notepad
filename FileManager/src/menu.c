#include "../include/menu.h"
#include "../include/file_ops.h"
#include "editor.h"
#include "../include/encryption.h"

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void get_user_input(char* buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void pause_screen(void) {
    printf("\nPress Enter to continue...");
    getchar();
}

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void show_main_menu(void) {
    clear_screen();
    printf("╔══════════════════════════════════════╗\n");
    printf("║        FILE MANAGEMENT SYSTEM        ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. File Operations                  ║\n");
    printf("║  2. Directory Operations             ║\n");
    printf("║  3. Show Current Directory           ║\n");
    printf("║  4. List Files in Current Directory  ║\n");
    printf("║  5. Search Files                     ║\n");
    printf("║  6. Edit File                        ║\n");
    printf("║  7. Open file in GUI Notepad         ║\n");
    printf("║  8. Encrypt / Decrypt File           ║\n"); 
    printf("║  0. Exit                             ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("Enter your choice (0-5): ");
}

void show_file_menu(void) {
    clear_screen();
    printf("╔══════════════════════════════════════╗\n");
    printf("║          FILE OPERATIONS             ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. Create File                      ║\n");
    printf("║  2. Write to File                    ║\n");
    printf("║  3. Append to File                   ║\n");
    printf("║  4. Read File                        ║\n");
    printf("║  5. Delete File                      ║\n");
    printf("║  6. Copy File                        ║\n");
    printf("║  7. Move/Rename File                 ║\n");
    printf("║  8. Show File Information            ║\n");
    printf("║  0. Back to Main Menu                ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("Enter your choice (0-8): ");
}

// operations menu
void show_directory_menu(void) {
    clear_screen();
    printf("╔══════════════════════════════════════╗\n");
    printf("║        DIRECTORY OPERATIONS          ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. Create Directory                 ║\n");
    printf("║  2. Delete Directory                 ║\n");
    printf("║  3. Change Directory                 ║\n");
    printf("║  4. List Directory Contents          ║\n");
    printf("║  5. Go to Parent Directory (..)      ║\n");
    printf("║  6. Go to Home Directory (~)         ║\n");
    printf("║  0. Back to Main Menu                ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("Enter your choice (0-6): ");
}

// operations menu
void handle_file_operations(void) {
    int choice;
    char filename[256], destination[256], content[2048];
    
    while (1) {
        show_file_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            pause_screen();
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1: // Create File
                printf("Enter filename to create: ");
                get_user_input(filename, sizeof(filename));
                create_file(filename);
                pause_screen();
                break;
                
            case 2: // Write to File
                printf("Enter filename: ");
                get_user_input(filename, sizeof(filename));
                printf("Enter content to write (end with empty line):\n");
                content[0] = '\0';
                char line[256];
                while (1) {
                    fgets(line, sizeof(line), stdin);
                    if (line[0] == '\n') break;
                    strcat(content, line);
                }
                write_to_file(filename, content, 0);
                pause_screen();
                break;
                
            case 3: // Append to File
                printf("Enter filename: ");
                get_user_input(filename, sizeof(filename));
                printf("Enter content to append:\n");
                get_user_input(content, sizeof(content));
                strcat(content, "\n");
                write_to_file(filename, content, 1);
                pause_screen();
                break;
                
            case 4: // Read File
                printf("Enter filename to read: ");
                get_user_input(filename, sizeof(filename));
                read_file(filename);
                pause_screen();
                break;
                
            case 5: // Delete File
                printf("Enter filename to delete: ");
                get_user_input(filename, sizeof(filename));
                delete_file(filename);
                pause_screen();
                break;
                
            case 6: // Copy File
                printf("Enter source filename: ");
                get_user_input(filename, sizeof(filename));
                printf("Enter destination filename: ");
                get_user_input(destination, sizeof(destination));
                copy_file(filename, destination);
                pause_screen();
                break;
                
            case 7: // Move/Rename File
                printf("Enter current filename: ");
                get_user_input(filename, sizeof(filename));
                printf("Enter new filename: ");
                get_user_input(destination, sizeof(destination));
                rename_file(filename, destination);
                pause_screen();
                break;
                
            case 8: // Show File Information
                printf("Enter filename: ");
                get_user_input(filename, sizeof(filename));
                show_file_info(filename);
                pause_screen();
                break;
                
            case 0: // Back to Main Menu
                return;
                
            default:
                printf("Invalid choice! Please try again.\n");
                pause_screen();
        }
    }
}

// Handle directory operations menu
void handle_directory_operations(void) {
    int choice;
    char dirname[256], pattern[256];
    
    while (1) {
        show_directory_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            pause_screen();
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1: // Create Directory
                printf("Enter directory name to create: ");
                get_user_input(dirname, sizeof(dirname));
                create_directory(dirname);
                pause_screen();
                break;
                
            case 2: // Delete Directory
                printf("Enter directory name to delete: ");
                get_user_input(dirname, sizeof(dirname));
                delete_directory(dirname);
                pause_screen();
                break;
                
            case 3: // Change Directory
                printf("Enter directory path: ");
                get_user_input(dirname, sizeof(dirname));
                change_directory(dirname);
                pause_screen();
                break;
                
            case 4: // List Directory Contents
                printf("Enter directory path (or '.' for current): ");
                get_user_input(dirname, sizeof(dirname));
                if (strlen(dirname) == 0) {
                    strcpy(dirname, ".");
                }
                list_files(dirname);
                pause_screen();
                break;
                
            case 5: // Go to Parent Directory
                change_directory("..");
                pause_screen();
                break;
                
            case 6: // Go to Home Directory
                change_directory(getenv("HOME") ? getenv("HOME") : "~");
                pause_screen();
                break;
                
            case 0: // Back to Main Menu
                return;
                
            default:
                printf("Invalid choice! Please try again.\n");
                pause_screen();
        }
    }
}

// Handle main menu
void handle_main_menu(void) {
    int choice;
    char dirname[256], pattern[256];
    char filename[256];
    
    while (1) {
        show_main_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            pause_screen();
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1: // File Operations
                handle_file_operations();
                break;
                
            case 2: // Directory Operations
                handle_directory_operations();
                break;
                
            case 3: // Show Current Directory
                show_current_directory();
                pause_screen();
                break;
                
            case 4: // List Files in Current Directory
                list_files(".");
                pause_screen();
                break;
                
            case 5: // Search Files
                printf("Enter directory to search (or '.' for current): ");
                get_user_input(dirname, sizeof(dirname));
                if (strlen(dirname) == 0) {
                    strcpy(dirname, ".");
                }
                printf("Enter search pattern: ");
                get_user_input(pattern, sizeof(pattern));
                search_files(dirname, pattern);
                pause_screen();
                break;
            case 6:
                printf("Enter filename to edit: ");
                get_user_input(filename, sizeof(filename));
                // Launch ncurses editor on this file
                edit_file_ui(filename);
                pause_screen();
                break;
                case 7: {
                  char filename[256];
                  printf("Enter the file name to open in GUI: ");
                  scanf("%s", filename);
                  open_in_gui_editor(filename);
                  break;
}
case 8: {
    char src[256], dest[256], password[256];
    int choice;

    printf("1. Encrypt File\n");
    printf("2. Decrypt File\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clear_input_buffer();

    printf("Enter source filename: ");
    get_user_input(src, sizeof(src));

    printf("Enter destination filename: ");
    get_user_input(dest, sizeof(dest));

    printf("Enter password: ");
    get_user_input(password, sizeof(password));

    if (choice == 1)
        aes_encrypt_file(src, dest, password);
    else if (choice == 2)
        aes_decrypt_file(src, dest, password);
    else
        printf("Invalid option.\n");

    pause_screen();
    break;
}


            case 0: // Exit
                printf("Thank you for using File Management System!\n");
                printf("Goodbye!\n");
                return;
            
            default:
                printf("Invalid choice! Please try again.\n");
                pause_screen();
        }
    }
}