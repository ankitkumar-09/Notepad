#include "../include/file_ops.h"

//if file exists or not
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Get file size
long get_file_size(const char* filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

// Create a new file
int create_file(const char* filename) {
    if (file_exists(filename)) {
        printf("Warning: File '%s' already exists!\n", filename);
        printf("Do you want to overwrite it? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            printf("File creation cancelled.\n");
            return 0;
        }
    }
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not create file '%s'\n", filename);
        return 0;
    }
    
    printf("File '%s' created successfully!\n", filename);
    fclose(file);
    return 1;
}

// Write content to file
int write_to_file(const char* filename, const char* content, int append) {
    FILE* file = fopen(filename, append ? "a" : "w");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for writing\n", filename);
        return 0;
    }
    
    fprintf(file, "%s", content);
    fclose(file);
    
    if (append) {
        printf("Content appended to '%s' successfully!\n", filename);
    } else {
        printf("Content written to '%s' successfully!\n", filename);
    }
    return 1;
}

// Read and display file content
int read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for reading\n", filename);
        return 0;
    }
    
    printf("\n=== Contents of '%s' ===\n", filename);
    char ch;
    int line_number = 1;
    printf("%3d: ", line_number);
    
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
        if (ch == '\n') {
            line_number++;
            if (!feof(file)) {
                printf("%3d: ", line_number);
            }
        }
    }
    printf("\n=== End of file ===\n\n");
    
    fclose(file);
    return 1;
}

// Delete a file
int delete_file(const char* filename) {
    if (!file_exists(filename)) {
        printf("Error: File '%s' does not exist\n", filename);
        return 0;
    }
    
    printf("Are you sure you want to delete '%s'? (y/n): ", filename);
    char choice;
    scanf(" %c", &choice);
    
    if (choice == 'y' || choice == 'Y') {
        if (remove(filename) == 0) {
            printf("File '%s' deleted successfully!\n", filename);
            return 1;
        } else {
            printf("Error: Could not delete file '%s'\n", filename);
            return 0;
        }
    } else {
        printf("File deletion cancelled.\n");
        return 0;
    }
}

// Copy a file
int copy_file(const char* source, const char* destination) {
    FILE *src, *dest;
    char buffer[4096];
    size_t bytes;
    
    if (!file_exists(source)) {
        printf("Error: Source file '%s' does not exist\n", source);
        return 0;
    }
    
    src = fopen(source, "rb");
    if (src == NULL) {
        printf("Error: Could not open source file '%s'\n", source);
        return 0;
    }
    
    dest = fopen(destination, "wb");
    if (dest == NULL) {
        printf("Error: Could not create destination file '%s'\n", destination);
        fclose(src);
        return 0;
    }
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }
    
    fclose(src);
    fclose(dest);
    printf("File copied from '%s' to '%s' successfully!\n", source, destination);
    return 1;
}

// Move a file
int move_file(const char* source, const char* destination) {
    if (rename(source, destination) == 0) {
        printf("File moved from '%s' to '%s' successfully!\n", source, destination);
        return 1;
    } else {
        printf("Error: Could not move file from '%s' to '%s'\n", source, destination);
        return 0;
    }
}

// Rename a file
int rename_file(const char* old_name, const char* new_name) {
    if (!file_exists(old_name)) {
        printf("Error: File '%s' does not exist\n", old_name);
        return 0;
    }
    
    if (file_exists(new_name)) {
        printf("Warning: File '%s' already exists!\n", new_name);
        printf("Do you want to overwrite it? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            printf("File rename cancelled.\n");
            return 0;
        }
    }
    
    if (rename(old_name, new_name) == 0) {
        printf("File renamed from '%s' to '%s' successfully!\n", old_name, new_name);
        return 1;
    } else {
        printf("Error: Could not rename file from '%s' to '%s'\n", old_name, new_name);
        return 0;
    }
}

// List files in directory
void list_files(const char* directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];
    
    dir = opendir(directory);
    if (dir == NULL) {
        printf("Error: Could not open directory '%s'\n", directory);
        return;
    }
    
    printf("\n=== Files in directory '%s' ===\n", directory);
    printf("%-30s %10s %20s\n", "Name", "Size", "Modified");
    printf("%-30s %10s %20s\n", "----", "----", "--------");
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);
        
        if (stat(full_path, &file_stat) == 0) {
            char time_str[20];
            struct tm *time_info = localtime(&file_stat.st_mtime);
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", time_info);
            
            if (S_ISREG(file_stat.st_mode)) {
                printf("📄 %-28s %8ld B %20s\n", entry->d_name, file_stat.st_size, time_str);
            } else if (S_ISDIR(file_stat.st_mode)) {
                printf("📁 %-28s %8s %20s\n", entry->d_name, "<DIR>", time_str);
            }
        }
    }
    printf("=== End of directory listing ===\n\n");
    
    closedir(dir);
}

// Show detailed file information
void show_file_info(const char* filename) {
    struct stat file_stat;
    
    if (stat(filename, &file_stat) != 0) {
        printf("Error: Could not get information for '%s'\n", filename);
        return;
    }
    
    printf("\n=== File Information: %s ===\n", filename);
    printf("Size: %ld bytes\n", file_stat.st_size);
    printf("Last modified: %s", ctime(&file_stat.st_mtime));
    printf("Last accessed: %s", ctime(&file_stat.st_atime));
    
    printf("Type: ");
    if (S_ISREG(file_stat.st_mode)) printf("Regular file\n");
    else if (S_ISDIR(file_stat.st_mode)) printf("Directory\n");
    else printf("Other\n");
    
    printf("Permissions: ");
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n=== End of file information ===\n\n");
}

// Search for files by pattern
int search_files(const char* directory, const char* pattern) {
    DIR *dir;
    struct dirent *entry;
    int found = 0;
    
    dir = opendir(directory);
    if (dir == NULL) {
        printf("Error: Could not open directory '%s'\n", directory);
        return 0;
    }
    
    printf("\n=== Search results for '%s' in '%s' ===\n", pattern, directory);
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        if (strstr(entry->d_name, pattern) != NULL) {
            printf("Found: %s\n", entry->d_name);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No files found matching '%s'\n", pattern);
    } else {
        printf("Found %d matching files\n", found);
    }
    printf("=== End of search ===\n\n");
    
    closedir(dir);
    return found;
}

// Create directory
int create_directory(const char* dirname) {
    if (mkdir(dirname, 0755) == 0) {
        printf("Directory '%s' created successfully!\n", dirname);
        return 1;
    } else {
        printf("Error: Could not create directory '%s'\n", dirname);
        return 0;
    }
}

// Delete directory
int delete_directory(const char* dirname) {
    printf("Are you sure you want to delete directory '%s'? (y/n): ", dirname);
    char choice;
    scanf(" %c", &choice);
    
    if (choice == 'y' || choice == 'Y') {
        if (rmdir(dirname) == 0) {
            printf("Directory '%s' deleted successfully!\n", dirname);
            return 1;
        } else {
            printf("Error: Could not delete directory '%s' (make sure it's empty)\n", dirname);
            return 0;
        }
    } else {
        printf("Directory deletion cancelled.\n");
        return 0;
    }
}

// Change current directory
void change_directory(const char* dirname) {
    if (chdir(dirname) == 0) {
        printf("Changed to directory: %s\n", dirname);
        show_current_directory();
    } else {
        printf("Error: Could not change to directory '%s'\n", dirname);
    }
}

void show_current_directory(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current directory: %s\n", cwd);
    } else {
        printf("Error: Could not get current directory\n");
    }
}