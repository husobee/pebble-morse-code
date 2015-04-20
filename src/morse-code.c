/*
 * main.c
 * Constructs a Window housing an output TextLayer to show data from 
 * either modes of operation of the accelerometer.
 */

#include <pebble.h>

#define TAP_NOT_DATA false

static Window *s_main_window;
static TextLayer *s_output_layer;

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, "hello ben sos");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

typedef struct moris_letter {
    int len;
    char letter;
    uint32_t *sequence;
} Letter;

#define DIT 100
#define DAH 300

static Letter *letters[26];

static void init_letter(char letter, int len, uint32_t a[]){
    Letter *l = (Letter *)malloc(len * sizeof(Letter *));
    l->letter = letter;
    l->len = len;
    l->sequence = (uint32_t*)malloc(len*sizeof(uint32_t));

    int i;
    for (i=0; i<len; i++ ) {
        l->sequence[i] = a[i];
    }
    int pos = (int)letter - 97;
    letters[pos] = l;
}


static void init_letters(){
    static const uint32_t dit = (uint32_t)DIT;
    static const uint32_t dah = (uint32_t)DAH;
    static const uint32_t base_gap = dit;

    init_letter('a', 3, (uint32_t[]){dit, base_gap, dah});
    init_letter('b', 7, (uint32_t[]){dah, base_gap,  dit,base_gap,  dit,base_gap,  dit});
    init_letter('c', 7, (uint32_t[]){dah, base_gap,  dit,base_gap,  dah,base_gap,  dit});
    init_letter('d', 5, (uint32_t[]){dah, base_gap,  dit,base_gap,  dit});
    init_letter('e', 1, (uint32_t[]){dit});
    init_letter('f', 7, (uint32_t[]){dit,base_gap,   dit,base_gap,  dah,base_gap,  dit});
    init_letter('g', 5, (uint32_t[]){dah,base_gap,   dah,base_gap,  dit});
    init_letter('h', 7, (uint32_t[]){dit,base_gap,  dit,base_gap,  dit,base_gap,  dit});
    init_letter('i', 3, (uint32_t[]){dit,base_gap,  dit});
    init_letter('j', 7, (uint32_t[]){dit,base_gap,  dah,base_gap,  dah,base_gap,  dah});
    init_letter('k', 5, (uint32_t[]){dah,base_gap,  dit,base_gap,  dah});
    init_letter('l', 7, (uint32_t[]){dit,base_gap,  dah,base_gap,  dit,base_gap,  dit});
    init_letter('m', 3, (uint32_t[]){dah,base_gap,  dah});
    init_letter('n', 3, (uint32_t[]){dah,base_gap,  dit});
    init_letter('o', 5, (uint32_t[]){dah,base_gap,  dah,base_gap,  dah});
    init_letter('p', 7, (uint32_t[]){dit,base_gap,  dah,base_gap,  dah,base_gap,  dit});
    init_letter('q', 7, (uint32_t[]){dah,base_gap,  dah,base_gap,  dit,base_gap,  dah});
    init_letter('r', 5, (uint32_t[]){dit,base_gap,  dah,base_gap,  dit});
    init_letter('s', 5, (uint32_t[]){dit,base_gap,  dit,base_gap,  dit});
    init_letter('t', 1, (uint32_t[]){dah});
    init_letter('u', 5, (uint32_t[]){dit,base_gap,  dit,base_gap,  dah});
    init_letter('v', 7, (uint32_t[]){dit,base_gap,  dit,base_gap,  dit,base_gap,  dah});
    init_letter('w', 5, (uint32_t[]){dit,base_gap,  dah,base_gap,  dah});
    init_letter('x', 7, (uint32_t[]){dah,base_gap,  dit,base_gap,  dit,base_gap,  dah});
    init_letter('y', 7, (uint32_t[]){dah,base_gap,  dit,base_gap,  dah,base_gap,  dah});
    init_letter('z', 5, (uint32_t[]){dit,base_gap,  dit,base_gap,  dit});
}



static void init() {
    // Create main Window
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload
    });
    window_stack_push(s_main_window, true);

    init_letters();

    char word[] = {'h','l','l','o','b','n','s','o','s','\0'};
    uint32_t *buffer = NULL;

    int i,j;
    int buffer_size_needed = 0;
    // figure out the length of buffer needed
    for (i=0; i<(int)ARRAY_LENGTH(word); i++) {
        if(word[i] == '\0'){
            break;
        }
        if (word[i] == ' ') {
            buffer_size_needed++;
            continue;
        }
        Letter *l = letters[(int)word[i]-97];
        buffer_size_needed += l->len;
    }
    buffer_size_needed += (int)ARRAY_LENGTH(word);

    // malloc the buffer
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Buffer will be %d long", buffer_size_needed);
    buffer = (uint32_t*)malloc(buffer_size_needed*sizeof(uint32_t));
    int counter = 0;
    // build out the buffer
    for (i=0; i<(int)ARRAY_LENGTH(word); i++) {
        if(word[i] == '\0'){
            break;
        }
        if (word[i] == ' ') {
            buffer[counter] = (int)DIT*7;
            counter++;
            continue;
        }
        Letter *l = letters[(int)word[i]-97];
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Letter: %c, length: %d", l->letter, l->len);
        for (j=0; j<l->len; j++) {
            buffer[counter] = l->sequence[j];
            counter++;
        }
        buffer[counter] = 3 * (int)DIT;
        counter++;
    }

  VibePattern pat = {
    .durations = buffer,
    .num_segments = buffer_size_needed,
  };
  vibes_enqueue_custom_pattern(pat);

  free(buffer);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
  int i;
  for (i=0; i<(int)ARRAY_LENGTH(letters); i++) {
    free(letters[i]);
  }
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
