// src/editor_main.c
#include "editor.h"
#include "ui.h"

// Called from the file manager menu
void edit_file_ui(const char *filename) {
    EditorState E;
    init_editor(&E, filename);
    init_ui();
    while (1) {
        refresh_screen(&E);
        process_keypress(&E);
    }
    // (Unreachable; cleanup happens in Ctrl-Q)
    close_ui();
    close_editor(&E);
}
