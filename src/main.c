#include <stdio.h>
#include <stdlib.h>
#include "../include/menu.h"
#include "../include/file_ops.h"

int main() {
    // welcome message
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║                                                    ║\n");
    printf("║            WELCOME TO FILE MANAGER                 ║\n");
    printf("║                                                    ║\n");
    printf("║  A Simple File Management System in C             ║\n");
    printf("║  Uses your system's file storage                  ║\n");
    printf("║                                                    ║\n");
    printf("║  Features:                                         ║\n");
    printf("║  • Create, Read, Write, Delete files              ║\n");
    printf("║  • Copy, Move, Rename files                       ║\n");
    printf("║  • Directory operations                           ║\n");
    printf("║  • File search and information                    ║\n");
    printf("║                                                    ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    
    printf("\nPress Enter to continue...");
    getchar();
    
    // current directory
    printf("\n");
    show_current_directory();
    printf("\nPress Enter to start...");
    getchar();
    
    // Start the main menu system
    handle_main_menu();
    
    return 0;
}