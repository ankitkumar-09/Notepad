#ifndef UI_H
#define UI_H

#include "editor.h"

void init_ui(void);
void close_ui(void);
void draw_rows(EditorState *E);
void draw_status_bar(void);
void move_cursor(EditorState *E);
void refresh_screen(EditorState *E);
void edit_file_ui(const char *filename); // <- ADD THIS


#endif
