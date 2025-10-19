#include "editor.h"
#include "file_ops.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "ui.h"
#include <ncurses.h>

void init_editor(EditorState *E, const char *fn){
    E->filename=strdup(fn);
    FILE* f=fopen(fn,"r");
    if(f){
        fseek(f,0,SEEK_END);
        E->bufsize=ftell(f);
        rewind(f);
        E->buffer=malloc(E->bufsize+1);
        fread(E->buffer,1,E->bufsize,f);
        fclose(f);
    } else {
        E->bufsize=0;
        E->buffer=malloc(1);
    }
    E->buffer[E->bufsize]='\0';
}

void close_editor(EditorState *E){
    free(E->buffer); free(E->filename);
}

void editor_save(EditorState *E){
    write_to_file(E->filename,E->buffer,0);
}

void process_keypress(EditorState *E){
    int c=getch();
    if(c==17){ close_ui(); close_editor(E); exit(0); }
    if(c==19){ editor_save(E); return; }
    if(isprint(c)){
        E->bufsize++;
        E->buffer=realloc(E->buffer,E->bufsize+1);
        E->buffer[E->bufsize-1]=c;
        E->buffer[E->bufsize]='\0';
    }
}
