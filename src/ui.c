#include "ui.h"
#include <curses.h>

void init_ui(void){
    initscr(); raw(); noecho(); keypad(stdscr,TRUE);
}

void close_ui(void){
    endwin();
}

void draw_rows(EditorState *E){
    int rows=LINES-1;
    for(int y=0;y<rows&&y<E->bufsize;y++){
        mvaddch(y,0,E->buffer[y]);
    }
}

void draw_status_bar(void){
    attron(A_REVERSE);
    mvprintw(LINES-1,0,"CTRL-S=Save | CTRL-Q=Quit");
    clrtoeol();
    attroff(A_REVERSE);
}

void move_cursor(EditorState *E){
    move(0,0);
}

void refresh_screen(EditorState *E){
    clear(); draw_rows(E); draw_status_bar(); move_cursor(E); refresh();
}
