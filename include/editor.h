#ifndef EDITOR_H
#define EDITOR_H

#include <stddef.h>

typedef struct {
    char *filename;
    char *buffer;
    size_t bufsize;
} EditorState;

void init_editor(EditorState *E, const char *filename);
void close_editor(EditorState *E);
void process_keypress(EditorState *E);
void editor_refresh_screen(EditorState *E);
void editor_save(EditorState *E);
void edit_file_ui(const char *filename);


#endif
