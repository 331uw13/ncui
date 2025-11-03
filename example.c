

#include <ncurses.h>

#include "ncui/ncui.h"


// Extra ncurses color pairs.
#define COLOR_DARK 10


void init_curses() {
    initscr();
    raw();    // Do not generate any interupt signal.
    noecho(); // Do not print characters that user types.
    keypad(stdscr, 1); // Enable arrow, backspace keys etc...
    curs_set(0); // Hide cursor.
    start_color();
    use_default_colors();

    init_pair(COLOR_BLACK,   COLOR_BLACK, -1);
    init_pair(COLOR_RED,     COLOR_RED, -1);
    init_pair(COLOR_GREEN,   COLOR_GREEN, -1);
    init_pair(COLOR_YELLOW,  COLOR_YELLOW, -1);
    init_pair(COLOR_BLUE,    COLOR_BLUE, -1);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, -1);
    init_pair(COLOR_CYAN,    COLOR_CYAN, -1);
    init_pair(COLOR_WHITE,   COLOR_WHITE, -1);
   
    init_color(COLOR_DARK, 400, 400, 400);
    init_pair(COLOR_DARK, COLOR_DARK, -1);
}


void handle_input(struct ncui_t* ncui, int input) {
    
    switch(input) {
        case KEY_LEFT:
            ncui_event_move(ncui, -1, 0);
            break;

        case KEY_RIGHT:
            ncui_event_move(ncui, 1, 0);
            break;

        case KEY_UP:
            ncui_event_move(ncui, 0, -1);
            break;

        case KEY_DOWN:
            ncui_event_move(ncui, 0, 1);
            break;

        case 0x0A: // Enter
            ncui_event_key_press(ncui, input);
            break;

        case KEY_BACKSPACE:
            ncui_event_key_press(ncui, input);
            break;

        default:
            // NOTE: ncui_event_char_input only accepts ASCII characters.
            ncui_event_char_input(ncui, input);
            break;
    }

}



static bool TEST = false;
static char TEST_BUFFER[32] = { 0 };

void update_ui(struct ncui_t* ncui) {
    ncui_new_update_begin(ncui);

    if(ncui_button(ncui, 
                (struct ncui_elem_mappos_t){ 0, 0 },
                (struct ncui_elem_drawpos_t){ 5, 5 },
                "[test button]",
                NULL, 0)) {
        TEST = !TEST;
    }

    if(ncui_button(ncui, 
                (struct ncui_elem_mappos_t){ 0, 1 },
                (struct ncui_elem_drawpos_t){ 5, 6 },
                "[another button]", 
                " Additional info when selected", COLOR_PAIR(COLOR_GREEN) | A_BLINK)) {
    }

    ncui_inputbox(ncui,
            (struct ncui_elem_mappos_t){ 1, 0 },
            (struct ncui_elem_drawpos_t){ 30, 5 },
            "Text input: ",
            TEST_BUFFER, sizeof(TEST_BUFFER));

        
}

int main() {
    init_curses();

    const int max_elements_rows = 16;
    const int max_elements_cols = 16;
    struct ncui_t ncui = ncui_init(max_elements_rows, max_elements_cols);

    ncui.style = (struct ncui_style_t) {
        .selected_attr   = COLOR_PAIR(COLOR_YELLOW) | A_BOLD,
        .unselected_attr = COLOR_PAIR(COLOR_DARK),

        .selected_inputbox_buffer_attr   = 0,
        .unselected_inputbox_buffer_attr = COLOR_PAIR(COLOR_DARK),

        .inputbox_cursor_char = '|',
        .inputbox_cursor_attr = COLOR_PAIR(COLOR_GREEN) | A_BLINK
    };

    while(true) {
        move(0, 0);
        clrtobot();

    
        update_ui(&ncui);

        mvprintw(0, 0, "NCUI Cursor: %i, %i | TEST=%i", ncui.cursor_x, ncui.cursor_y, TEST);
        
        const int input = getch();
        if(input == 'q') {
            break; // Exit.
        }
        

        ncui_clear_events(&ncui);
        handle_input(&ncui, input);
    }

    free_ncui(&ncui);
    endwin();
    return 0;
}

