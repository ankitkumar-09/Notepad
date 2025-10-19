// include/menu.h
#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Menu function declarations
void show_main_menu(void);
void show_file_menu(void);
void show_directory_menu(void);
void handle_main_menu(void);
void handle_file_operations(void);
void handle_directory_operations(void);
void clear_input_buffer(void);
void get_user_input(char* buffer, int size);
void pause_screen(void);
void clear_screen(void);

#endif