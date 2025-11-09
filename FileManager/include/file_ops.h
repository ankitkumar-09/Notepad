// include/file_ops.h
#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

// Function declarations for file operations
int create_file(const char* filename);
int write_to_file(const char* filename, const char* content, int append);
int read_file(const char* filename);
int delete_file(const char* filename);
int copy_file(const char* source, const char* destination);
int move_file(const char* source, const char* destination);
int rename_file(const char* old_name, const char* new_name);
void list_files(const char* directory);
void show_file_info(const char* filename);
int search_files(const char* directory, const char* pattern);
int create_directory(const char* dirname);
int delete_directory(const char* dirname);
void change_directory(const char* dirname);
void show_current_directory(void);
long get_file_size(const char* filename);
int file_exists(const char* filename);
void open_in_gui_editor(const char* filename);

#endif