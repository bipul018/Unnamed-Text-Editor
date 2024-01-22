#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif
#define _CRT_SECURE_NO_WARNINGS
#include "raylib-out.h"

#define nullptr ((void*)0)
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define ifelse1(func, p1, e1)\
  if((func)(p1)) e1

#define ifelse2(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse1(func, __VA_ARGS__)

#define ifelse3(func, p1, e1, ...)			\
  ifelse1(func,p1,e1) else ifelse2(func, __VA_ARGS__)

#define ifelse4(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse3(func, __VA_ARGS__)

#define ifelse5(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse4(func, __VA_ARGS__)

#define ifelse6(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse5(func, __VA_ARGS__)

#define ifelse7(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse6(func, __VA_ARGS__)

#define ifelse8(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse7(func, __VA_ARGS__)

#define ifelse9(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse8(func, __VA_ARGS__)

#define ifelse10(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse9(func, __VA_ARGS__)

#define ifelse11(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse10(func, __VA_ARGS__)

#define ifelse12(func, p1, e1, ...)	\
  ifelse1(func,p1,e1) else ifelse11(func, __VA_ARGS__)


#ifndef _countof
#define _countof(arr) (sizeof(arr)/(sizeof((arr)[0])))
#endif


//Key value push only vector based on realloc
bool push_multiple(void* *baseptr, size_t *countptr,
		   size_t elem_size, size_t push_count, void* new_things);

bool push_unit(void* *base, size_t *count,
	       size_t elem_size, void* new_thing);

#define push_obj(baseptr, countptr, new_obj)		\
  push_unit((void**)(baseptr), countptr, sizeof((baseptr)[0][0]), (new_obj))

#define push_objs(baseptr, countptr, objs_count, new_objs)	\
  push_multiple((void**)baseptr, countptr, sizeof(baseptr[0][0]),objs_count, (new_objs))

typedef bool (*eq_pred_fn)(const void* key,const void* data);

void* get_unit(void* base, size_t count, size_t elem_size,
	       void* key, eq_pred_fn eqfn);

#define get_obj(base, count, keyptr, equality_fn)	\
  get_unit(base, count, sizeof(base[0]), keyptr, equality_fn);


typedef struct StringView StringView;
struct StringView {
  size_t len;
  char* base;
};

typedef struct StringViewNative StringViewNative;
struct StringViewNative {
  size_t len;
  char base[];
};

#define SVPARGS(sv) (int)((sv).len), (sv).base

bool strvieweq(StringView a,StringView b);
StringView view_cstr(char* str);

StringView view_from_native(StringViewNative *native_str){
  return (StringView){.base = native_str->base, .len = native_str->len};
}

bool push_multiple(void* *baseptr, size_t *countptr,
		size_t elem_size, size_t push_count, void* new_things){
  void* newptr = realloc(*baseptr, (*countptr + push_count) * elem_size);
  if(nullptr == newptr)
    return false;

  *baseptr = newptr;

  if(new_things){
    memcpy((char*)(*baseptr) + *countptr * elem_size, new_things,push_count* elem_size);
  }
  *countptr = *countptr + push_count;
  return true;
}


bool push_unit(void* *base, size_t *count,
	       size_t elem_size, void* new_thing){
  return push_multiple(base, count, elem_size, 1, new_thing);
}


void* get_unit(void* base, size_t count, size_t elem_size,
	       void* key, eq_pred_fn eqfn){
  if(nullptr == base)
    return nullptr;
  int inx = -1;
  for(int i = 0; i < (int)count; ++i){
    void* data = (void*)((char*)base + i * elem_size);
    if(eqfn(key, data)){
      inx = i;
      break;
    }
  }
  if(inx < 0)
    return nullptr;

  return inx * elem_size + (char*)base;
}


bool strvieweq(StringView a,StringView b){
  if(a.len > b.len){
    const StringView tmp = a;
    a = b;
    b = tmp;
  }
  
  for(size_t i = 0; i < a.len; ++i){
    if(a.base[i] != b.base[i])
      return false;
    if(!a.base[i] && !b.base[i])
      return true;
  }
  return (a.len == b.len) || (b.base[a.len] == 0);
}

StringView view_cstr(char* str){
  return (StringView){.base = str, .len = strlen(str)};
}


typedef struct LinkedNativeString LinkedNativeString;
struct LinkedNativeString {
  LinkedNativeString *prev;
  LinkedNativeString *next;
  size_t capacity;
  StringViewNative str;
};

typedef struct TextLocation TextLocation;
struct TextLocation {
  LinkedNativeString* node;
  int offset;
};


//Custom default font and stuff
RlFont default_font;
float default_spacing_factor = 0.1f;

void draw_text(const char* text, int posx, int posy, int fontsize, RlColor color){

  rl_draw_text_ex(default_font, text, (RlVector2){.x = posx, .y = posy},
		  fontsize, default_spacing_factor * fontsize, color);
}

int measure_text(const char* text, int fontsize){
  return (int)(0.5f + rl_measure_text_ex(default_font,
					 text,
					 fontsize,
					 fontsize * default_spacing_factor).x);
}

//-1 means newline
int get_char_width(char ch, int fontsize){
  
  int twospacelen = measure_text("  ", fontsize);
  int onespacelen = measure_text(" ", fontsize);

  if(('\n' == ch ) ||
     ('\r' == ch )){
    return -1;
  }
      
  char letter[2] = {ch};

  int len = measure_text(letter, fontsize) +
    twospacelen - onespacelen*2;
  return len;
}


typedef struct KeyRecorder KeyRecorder;
struct KeyRecorder{
  struct KeyTime{
    int key;
    double last_time;
  } *times ;
  size_t count ;
};
bool key_time_eq_fn(const void* key, const void* data){
  const int* ikey = key;
  const struct KeyTime* kdata = data;
  return *ikey == kdata->key;
}
int get_key_count(KeyRecorder* recorder, int keycode){   
    int count_press = 0;
    const double first_time = 0.5;
    const double not_first_time = 0.05;
    struct KeyTime* item = get_obj((recorder->times),
				   (recorder->count),
				   &keycode,
				   key_time_eq_fn);
    
    if(rl_is_key_pressed(keycode)){
      if(item == nullptr){
	struct KeyTime dat = {.key = keycode};
	if(!push_obj(&(recorder->times), &(recorder->count), &dat))
	  return 0;
	item = get_obj((recorder->times),
				   (recorder->count),
				   &keycode,
				   key_time_eq_fn);
      }
      
      item->last_time = rl_get_time() + first_time;
      count_press = 1;
    }
    if(!count_press &&
       rl_is_key_down(keycode) &&
       ((rl_get_time() - item->last_time ) > not_first_time)){
      if(!item)
	return 0;
      double diff = rl_get_time() - item->last_time;
      while(diff > 0.0){
	diff -= not_first_time;
	count_press++;
      }
      item->last_time = rl_get_time();
    }
    return count_press;
}


void snap_cursor_left(TextLocation *loc){

  while((loc->node->prev) && 
	((0 == loc->node->str.len) ||
	 (0 == loc->offset))){
    loc->node = loc->node->prev;
    loc->offset = loc->node->str.len;
  }
}

void snap_cursor_right(TextLocation *loc){
  while((loc->node->next) &&
	((0 == loc->node->str.len) ||
	 (loc->offset == loc->node->str.len))){
    loc->node = loc->node->next;
    loc->offset = 0;
  }
}

void move_cursor_left(TextLocation *loc){
  snap_cursor_left(loc);
  if(loc->offset == 0)
    return;
  loc->offset--;
}

void move_cursor_right(TextLocation *loc){
  snap_cursor_right(loc);
  if(loc->offset == loc->node->str.len)
    return;
  loc->offset++;
}  

void ins_char_left(TextLocation *loc, char ch){
  snap_cursor_left(loc);
  //Case filled
  if(loc->node->capacity == loc->node->str.len){
    //Create new node
    LinkedNativeString* new = malloc(loc->node->capacity + sizeof(*new));
    new->capacity = loc->node->capacity;
    new->prev = loc->node;
    new->next = loc->node->next;
    if(new->next)
      new->next->prev = new;
    loc->node->next = new;
    
    if(loc->offset <loc->node->capacity){
      new->str.len = loc->node->str.len - loc->offset;
      memcpy(new->str.base,
	     loc->node->str.base + loc->offset,
	     new->str.len);
      loc->node->str.len = loc->offset;
    }
    else{
      new->str.len = 0;
      loc->node = loc->node->next;
      loc->offset = 0;
    }
  }
  else{
    memmove(loc->node->str.base + loc->offset + 1,
	    loc->node->str.base + loc->offset,
	    loc->node->str.len - loc->offset);
  }
  
  loc->node->str.base[loc->offset++] = ch;
  loc->node->str.len++;
}

void del_char_left(TextLocation *loc){
  snap_cursor_left(loc);
  if(0 == loc->offset)
    return;

    memmove(loc->node->str.base + loc->offset - 1,
	    loc->node->str.base + loc->offset,
	    loc->node->str.len - loc->offset);
    loc->node->str.len--;
    loc->offset--;
}

void del_char_right(TextLocation *loc){
  snap_cursor_right(loc);
  if(loc->offset == loc->node->str.len)
    return;
  
  memmove(loc->node->str.base + loc->offset,
	  loc->node->str.base + loc->offset+1,
	  loc->node->str.len - loc->offset-1);
  loc->node->str.len--;
}
  

int main(int argc, char* argv[]){

  rl_init_lib("raylib");

  rl_set_trace_log_level(LOG_WARNING);
  rl_set_config_flags(FLAG_WINDOW_RESIZABLE);
  rl_init_window(1000,800, "Trying out stuff");
  rl_set_target_f_p_s(60);

  //Load font
  //default_font = rl_load_font("CascadiaMono.ttf");
  //  default_font = rl_load_font("l_10646.ttf");
  //default_font = rl_load_font("Sanskr.ttf");
  int font_size = 50;
  default_font = rl_load_font_ex("JetBrainsMonoNL-Regular.ttf",font_size,NULL,0);
  if(!rl_is_font_ready(default_font)){
    default_font = rl_get_font_default();
  }

  
  LinkedNativeString *head_node = malloc(sizeof(*head_node) + 10);
  head_node->prev = head_node->next = NULL;
  head_node->capacity =  10;
  head_node->str.len = 0;

  TextLocation curr_pos = {.node = head_node};
  
  int x0 = 10;
  int y0 = 10;


  //Load text.txt file
  const char* file_name = "test.txt";
  FILE* file = fopen(file_name, "r");
  if(nullptr == file){
    printf("Error in opening file %s for reading\n", file_name);
  }
  else{
    int c;
    while((c = fgetc(file)) != EOF){
      ins_char_left(&curr_pos, (char)c);
    }
    curr_pos.node = head_node;
    curr_pos.offset = 0;
    fclose(file);
  }
    
  

  double prev_blink_time = rl_get_time();
  bool blink_now = true;
  KeyRecorder recorder = {0};
  
  int max_count = 0;
  while(!rl_window_should_close()){
    
    rl_begin_drawing();
    rl_clear_background(WHITE);

    if((rl_get_time() - prev_blink_time) > 0.5){
      prev_blink_time = rl_get_time();
      blink_now = !blink_now;
    }

    //Scrolling section
    float m_zoom = rl_get_mouse_wheel_move();
    if(rl_is_key_down(KEY_LEFT_CONTROL) ||
       rl_is_key_down(KEY_RIGHT_CONTROL)){
      font_size += m_zoom;
      if(font_size < 10)
	font_size = 10;
    }
    else{
      y0 += m_zoom * font_size /2.0;
    }
    

    int press_count = 0;
    /* press_count = get_key_count(&recorder, KEY_UP); */
    /* y0 -= font_size * press_count / 3.0; */

    /* press_count = get_key_count(&recorder, KEY_DOWN); */
    /* y0 += font_size * press_count / 3.0; */

    //Previous/next text block
    press_count = get_key_count(&recorder, KEY_RIGHT);
    for(int i = 0 ; i<press_count; ++i){
      move_cursor_right(&curr_pos);
      blink_now = true;
    }

    press_count = get_key_count(&recorder, KEY_LEFT);
    for(int i = 0; i < press_count; ++i){
      move_cursor_left(&curr_pos);
      blink_now = true;
    }
    //Text input section
    int char_code ;

    while((char_code = rl_get_char_pressed())){
      ins_char_left(&curr_pos, char_code);
      blink_now = true;
    }

    //key pressed or (key down and time from last keypress > smthing)

    press_count = get_key_count(&recorder, KEY_ENTER);
    for(int i  = 0 ;i < press_count; ++i){
      ins_char_left(&curr_pos,  '\n');
      blink_now = true;
    }


    press_count = get_key_count(&recorder, KEY_TAB);
    for(int i = 0; i < 4 * press_count; ++i){
      ins_char_left(&curr_pos, ' ');
      blink_now = true;
    }
    
    press_count = get_key_count(&recorder, KEY_BACKSPACE);
    for(int i = 0; i < press_count; ++i){
      del_char_left(&curr_pos);
      blink_now = true;
    }

    press_count = get_key_count(&recorder, KEY_DELETE);
    for(int i = 0; i < press_count; ++i){
      del_char_right(&curr_pos);
      blink_now = true;
    }

    //Clean up empty nodes and move curr_pos if one of them
    {
      LinkedNativeString* node = head_node;
      while(node){
	bool empty = (0 == node->str.len);
	bool single = (nullptr == node->prev) && (nullptr == node->next);
	if(empty && single)
	  break;
	if(empty && (nullptr == node->prev)){
	  //This is head
	  head_node = head_node->next;
	  head_node->prev = nullptr;
	  if(curr_pos.node == node){
	    curr_pos.node = head_node;
	    curr_pos.offset = 0;
	  }
	  free(node);
	  node = head_node;
	}
	else if(empty){
	  //This is not head
	  LinkedNativeString* tmp = node;
	  node = node->next;
	  tmp->prev->next = tmp->next;
	  if(tmp->next)
	    tmp->next->prev = tmp->prev;
	  if(curr_pos.node == tmp){
	    curr_pos.node = tmp->prev;
	    curr_pos.offset = tmp->prev->str.len;
	  }
	  free(tmp);
	}
	else
	  node = node->next;
      }
    }
    
    int width = rl_get_screen_width();
    int height = rl_get_screen_height();    
    int cx = x0;
    int cy = y0;
      //    int height = font_size;

    LinkedNativeString* node_ptr = head_node;
    while(node_ptr){
      size_t i = 0;
      for(; i < node_ptr->str.len; ++i){

	if((node_ptr == curr_pos.node) &&
	   (i == curr_pos.offset)){
	  int wid = 3;
	  if((10 + cx + wid) >= (width + x0)){
	    cy += 10 + font_size;
	    cx = x0;
	  }
	  if(blink_now)
	    rl_draw_rectangle(cx,cy-2,wid,font_size+10,RED);
	  cx += wid;
	}
	
	int wid = get_char_width(node_ptr->str.base[i], font_size);

	if((0 > wid) || ((10 + cx + wid) >= (width + x0))){
	  cy += 10 + font_size;
	  cx = x0;
	}
				       

	{
	  char letter[2] = {node_ptr->str.base[i]};
	  draw_text(letter, cx, cy, font_size, BLACK);
	}
	cx += wid;
	      
      }
      
      if((node_ptr == curr_pos.node) &&
	 (i == curr_pos.offset)){
	int wid = 3;
	if((10 + cx + wid) >= (width + x0)){
	  cy += 10 + font_size;
	  cx = x0;
	}
	if(blink_now)
	  rl_draw_rectangle(cx,cy-2,wid,font_size+10,RED);
	cx += wid;
      }

      node_ptr = node_ptr->next;
    }
    
    rl_end_drawing();
  }
  
  file = fopen(file_name, "w");
  if(nullptr == file){
    printf("Error in opening file %s for writing\n", file_name);
  }
  else{
    LinkedNativeString* node_ptr = head_node;
    while(node_ptr){
      size_t i = 0;
      for(; i < node_ptr->str.len; ++i){
	fputc((int)node_ptr->str.base[i], file);
      }
      node_ptr = node_ptr->next;
    }
  }
    

  rl_unload_font(default_font);
  rl_close_window();
  rl_free_lib();
  return 0;
}
