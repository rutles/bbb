// G_Analyzer v1.0
// compile: ./gtkcc g_analyzer
// execute: ./g_analyzer
// NOTE: GTK3 required.
// GTK3 install: apt-get install libgtk-3-dev

#include <gtk/gtk.h>
#include <remocon.h>

typedef struct {
  // Flags
  int new_data;
  int not_saved;

  // Parameters
  int serial;
  option_t op;
  irform_t ir;
  int *array_1;
  int length_1;
  int *array_0;
  int length_0;

  //Widgets
  GtkWidget* win;
  GtkWidget* form;
  GtkWidget* hist_t;
  GtkWidget* hist_1;
  GtkWidget* hist_0;
  GtkWidget* btn_analyze;
  GtkWidget* btn_shot;
  GtkWidget* btn_get;
  GtkWidget* btn_save;
  GtkWidget* btn_exit;
  GtkWidget* textview;
} args_t;

// Wave form on Draw callback
gboolean cb_form(GtkWidget* widget, cairo_t* cr, args_t *args){
  char buf[40];

  // Background
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_rectangle(cr, 0, 0, 650, 100);
  cairo_fill(cr);

  // Title
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 2, 10);
  cairo_show_text(cr, "wave form");

  if(args->new_data == 0){
    cairo_show_text(cr, "(not yet analyzed)");
    return FALSE;
  }

  // Waveform
#define SIZE_W 650
#define SIZE_H 100
#define POS_TOP (SIZE_H - 75)
#define POS_BTM (SIZE_H - 20)

  int total = args->ir.lon + args->ir.loff + args->ir.t * 4 + args->ir.h + args->ir.l * 2;
  int len_lon = SIZE_W * args->ir.lon / total;
  int len_loff = SIZE_W * args->ir.loff / total;
  int len_t = SIZE_W * args->ir.t / total;
  int len_1 = SIZE_W * args->ir.h / total;
  int len_0 = SIZE_W * args->ir.l / total;

  int x = len_t;
  cairo_move_to(cr, x, POS_TOP - 2);
  sprintf(buf, "%dusec", args->ir.lon); cairo_show_text(cr, buf);
  x += len_lon;
  cairo_move_to(cr, x, POS_BTM + 10);
  sprintf(buf, "%dusec", args->ir.loff); cairo_show_text(cr, buf);
  x += len_loff;
  cairo_move_to(cr, x, POS_TOP - 2);
  sprintf(buf, "%dusec", args->ir.t); cairo_show_text(cr, buf);
  x += len_t;
  cairo_move_to(cr, x, POS_BTM + 10);
  sprintf(buf, "%dusec", args->ir.h); cairo_show_text(cr, buf);
  x += len_1;
  cairo_move_to(cr, x, POS_TOP - 2);
  sprintf(buf, "%dusec", args->ir.t); cairo_show_text(cr, buf);
  x += len_t;
  cairo_move_to(cr, x, POS_BTM + 10);
  sprintf(buf, "%dusec", args->ir.l); cairo_show_text(cr, buf);

  cairo_set_source_rgb(cr, 0, 1, 0);
  x = 0;
  cairo_move_to(cr, x, POS_BTM);
  x += len_t;
  cairo_line_to(cr, x, POS_BTM); cairo_line_to(cr, x, POS_TOP);
  x += len_lon;
  cairo_line_to(cr, x, POS_TOP); cairo_line_to(cr, x, POS_BTM);
  x += len_loff;
  cairo_line_to(cr, x, POS_BTM); cairo_line_to(cr, x, POS_TOP);
  x += len_t;
  cairo_line_to(cr, x, POS_TOP); cairo_line_to(cr, x, POS_BTM);
  x += len_1;
  cairo_line_to(cr, x, POS_BTM); cairo_line_to(cr, x, POS_TOP);
  x += len_t;
  cairo_line_to(cr, x, POS_TOP); cairo_line_to(cr, x, POS_BTM);
  x += len_0;
  cairo_line_to(cr, x, POS_BTM); cairo_line_to(cr, x, POS_TOP);
  x += len_t;
  cairo_line_to(cr, x, POS_TOP); cairo_line_to(cr, x, POS_BTM);
  x += len_0;
  cairo_line_to(cr, x, POS_BTM);
  cairo_stroke(cr);

  return FALSE;

#undef SIZE_W
#undef SIZE_H
#undef POS_TOP
#undef POS_BTM
}

// Histgram T on Draw callback
gboolean cb_hist_t(GtkWidget* widget, cairo_t* cr, args_t *args){
#define TITLE "histgram T"
#define SIZE_W 320
#define SIZE_H 100
#define COUNT args->ir.length
#define DATA args->ir.son
#define CENTER args->ir.t

  int i;
  char buf[40];
  int pitch, width, height;

  // Background
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_rectangle(cr, 0, 0, SIZE_W, SIZE_H);
  cairo_fill(cr);

  // Title
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 2, 10);
  cairo_show_text(cr, TITLE);

  if(args->new_data == 0){
    cairo_show_text(cr, "(not yet analyzed)");
    return FALSE;
  }
  sprintf(buf, "(%dsamples)", COUNT);
  cairo_show_text(cr, buf);

  // Histgram
  pitch = SIZE_W / COUNT;
  width = pitch > 1 ? pitch - 1 : pitch;
  cairo_set_source_rgb(cr, 0, 1, 0);
  for(i = 0; i < COUNT; i++){
    //height = DATA[i] - CENTER + SIZE_H / 2; // range +/- 50usec
    height = (DATA[i] - CENTER + SIZE_H) / 2; // range +/- 100usec
    cairo_rectangle(cr, i * pitch, SIZE_H - height, width, height);
  }
  cairo_fill(cr);

  // Center guide
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 0, SIZE_H / 2);
  cairo_line_to(cr, SIZE_W, SIZE_H / 2);
  cairo_stroke(cr);

  cairo_move_to(cr, 2, SIZE_H / 2 - 2);
  sprintf(buf, "%dusec", CENTER);
  cairo_show_text(cr, buf);

  return FALSE;

#undef TITLE
#undef SIZE_W
#undef SIZE_H
#undef COUNT
#undef DATA
#undef CENTER
}

// Histgram 1 on Draw callback
gboolean cb_hist_1(GtkWidget* widget, cairo_t* cr, args_t *args){
#define TITLE "histgram 1"
#define SIZE_W 320
#define SIZE_H 100
#define COUNT args->length_1
#define DATA args->array_1
#define CENTER args->ir.h

  int i;
  char buf[40];
  int pitch, width, height;

  // Background
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_rectangle(cr, 0, 0, SIZE_W, SIZE_H);
  cairo_fill(cr);

  // Title
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 2, 10);
  cairo_show_text(cr, TITLE);

  if(args->new_data == 0){
    cairo_show_text(cr, "(not yet analyzed)");
    return FALSE;
  }
  sprintf(buf, "(%dsamples)", COUNT);
  cairo_show_text(cr, buf);

  // Histgram
  pitch = SIZE_W / COUNT;
  width = pitch > 1 ? pitch - 1 : pitch;
  cairo_set_source_rgb(cr, 0, 1, 0);
  for(i = 0; i < COUNT; i++){
    //height = DATA[i] - CENTER + SIZE_H / 2; // range +/- 50usec
    height = (DATA[i] - CENTER + SIZE_H) / 2; // range +/- 100usec
    cairo_rectangle(cr, i * pitch, SIZE_H - height, width, height);
  }
  cairo_fill(cr);

  // Center guide
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 0, SIZE_H / 2);
  cairo_line_to(cr, SIZE_W, SIZE_H / 2);
  cairo_stroke(cr);

  cairo_move_to(cr, 2, SIZE_H / 2 - 2);
  sprintf(buf, "%dusec", CENTER);
  cairo_show_text(cr, buf);

  return FALSE;

#undef TITLE
#undef SIZE_W
#undef SIZE_H
#undef COUNT
#undef DATA
#undef CENTER
}

// Histgram 0 on Draw callback
gboolean cb_hist_0(GtkWidget* widget, cairo_t* cr, args_t *args){
#define TITLE "histgram 0"
#define SIZE_W 320
#define SIZE_H 100
#define COUNT args->length_0
#define DATA args->array_0
#define CENTER args->ir.l

  int i;
  char buf[40];
  int pitch, width, height;

  // Background
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_rectangle(cr, 0, 0, SIZE_W, SIZE_H);
  cairo_fill(cr);

  // Title
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 2, 10);
  cairo_show_text(cr, TITLE);

  if(args->new_data == 0){
    cairo_show_text(cr, "(not yet analyzed)");
    return FALSE;
  }
  sprintf(buf, "(%dsamples)", COUNT);
  cairo_show_text(cr, buf);

  // Histgram
  pitch = SIZE_W / COUNT;
  width = pitch > 1 ? pitch - 1 : pitch;
  cairo_set_source_rgb(cr, 0, 1, 0);
  for(i = 0; i < COUNT; i++){
    //height = DATA[i] - CENTER + SIZE_H / 2; // range +/- 50usec
    height = (DATA[i] - CENTER + SIZE_H) / 2; // range +/- 100usec
    cairo_rectangle(cr, i * pitch, SIZE_H - height, width, height);
  }
  cairo_fill(cr);

  // Center guide
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 0, SIZE_H / 2);
  cairo_line_to(cr, SIZE_W, SIZE_H / 2);
  cairo_stroke(cr);

  cairo_move_to(cr, 2, SIZE_H / 2 - 2);
  sprintf(buf, "%dusec", CENTER);
  cairo_show_text(cr, buf);

  return FALSE;

#undef TITLE
#undef SIZE_W
#undef SIZE_H
#undef COUNT
#undef DATA
#undef CENTER
}

// Get center value
int center(int *array, int count){
  int temp;
  int i, j;

  // sort
  for(i = 0; i < count - 1; i++)
    for(j = count - 1; j > i; j--)
      if(array[j - 1] > array[j]){
        temp = array[j];
        array[j] = array[j - 1];
        array[j - 1] = temp;
      }

  return array[count / 2];
}

// On click Analyze button callback
gboolean cb_analyze(GtkScale* scale, args_t *args){
  int i;
  int fd;
  int ret;
  char buf[80];
  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  int *work_1, *work_0;

  // clear textview
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args->textview));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_delete(buffer, &start, &end);

  // receiver open
  fd = rc_ropen(args->op.r);
  if(fd <= 0){
    perror("cb_analyze:rc_ropen");
    gtk_text_buffer_insert_at_cursor(buffer,
      "GPIO can not open.\n", -1);
    return TRUE;
  }

  // analyze
  ret = rc_analyze(fd, &args->ir);
  close(fd);
  if(ret < 0){
    perror("cb_analyze:rc_analyze");
    gtk_text_buffer_insert_at_cursor(buffer,
      "Signal can not read.\n", -1);
    return TRUE;
  }

  // Statistical analysis
  args->ir.t = center(args->ir.son, args->ir.length);

  work_1 = (int *)calloc(args->ir.length, sizeof(int));
  work_0 = (int *)calloc(args->ir.length, sizeof(int));
  args->length_1 = 0;
  args->length_0 = 0;

  for(i = 0; i < args->ir.length; i++){
    if(args->ir.t * 2 < args->ir.soff[i]){
      work_1[args->length_1++] = args->ir.soff[i];
    } else {
      work_0[args->length_0++] = args->ir.soff[i];
    }
  }

  free(args->array_1);
  args->array_1 = (int *)calloc(args->length_1, sizeof(int));
  for(i = 0; i < args->length_1; i++)
    args->array_1[i] = work_1[i];
  free(work_1);
  args->ir.h = rc_center(args->array_1, args->length_1);

  free(args->array_0);
  args->array_0 = (int *)calloc(args->length_0, sizeof(int));
  for(i = 0; i < args->length_0; i++)
    args->array_0[i] = work_0[i];
  free(work_0);
  args->ir.l = rc_center(args->array_0, args->length_0);

  // Report
  gtk_text_buffer_insert_at_cursor(buffer, "Analyze report.\n\n", -1);
  sprintf(buf, "Format: %s, %s.\n",
    args->ir.lon < RC_FORMAT ? "IEHA" : "NEC",
    args->ir.multi ? "multi shot" : "single shot");
  gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

  sprintf(buf, "Leader ON: %dusec, OFF: %dusec.\n",
    args->ir.lon, args->ir.loff);
  gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

  sprintf(buf, "Signal T: %dusec, 1: %dusec, 0: %dusec.\n\n",
    args->ir.t, args->ir.h, args->ir.l);
  gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

  if(args->ir.multi)
    gtk_text_buffer_insert_at_cursor(buffer, "1st shot ", -1);

  sprintf(buf, "Signal count: %dbytes.\n", args->ir.count1);
  gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

  for(i = 0; i < args->ir.count1 - 1; i++){
    sprintf(buf, "%02X%s", args->ir.code1[i], (i + 1) % 8 ? ", " : "\n");
    gtk_text_buffer_insert_at_cursor(buffer, buf, -1);
  }
  sprintf(buf, "%02X.\n\n", args->ir.code1[i]);
  gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

  if(args->ir.multi){

    sprintf(buf, "Gap: %dusec.\n", args->ir.gap);
    gtk_text_buffer_insert_at_cursor(buffer, buf, -1);
    sprintf(buf, "Interval: %dusec.\n\n", args->ir.interval);
    gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

    gtk_text_buffer_insert_at_cursor(buffer, "2nd shot ", -1);

    sprintf(buf, "Signal count: %dbytes.\n", args->ir.count2);
    gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

    for(i = 0; i < args->ir.count2 - 1; i++){
      sprintf(buf, "%02X%s", args->ir.code2[i], (i + 1) % 8 ? ", " : "\n");
      gtk_text_buffer_insert_at_cursor(buffer, buf, -1);
    }
    sprintf(buf, "%02X.\n\n", args->ir.code2[i]);
    gtk_text_buffer_insert_at_cursor(buffer, buf, -1);
  }

  // Update flags
  args->new_data = 1;
  gtk_widget_set_sensitive(args->btn_shot, TRUE);
  gtk_widget_set_sensitive(args->btn_get, TRUE);

  // Draw graph
  gtk_widget_queue_draw(args->form);
  gtk_widget_queue_draw(args->hist_t);
  gtk_widget_queue_draw(args->hist_1);
  gtk_widget_queue_draw(args->hist_0);

  return TRUE;
}

// On destroy callback
gboolean cb_exit(GtkWidget* widget, args_t *args){
  GtkWidget *dialog;
  gint ret;

  // Dialog
  if(args->not_saved){
    dialog = gtk_dialog_new_with_buttons(
      "Caution!",
      GTK_WINDOW(args->win),
      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
      "Exit", GTK_RESPONSE_ACCEPT,
      "Cancel", GTK_RESPONSE_REJECT,
      NULL);

    gtk_container_add(
      GTK_CONTAINER(gtk_dialog_get_content_area(
        GTK_DIALOG(dialog))),
      gtk_label_new(
        "Data are not saved.")
    );

    gtk_widget_set_size_request(dialog, 196, 64);
    gtk_widget_show_all(dialog);
    ret = gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    if(ret == GTK_RESPONSE_REJECT)
      return TRUE;
  }

  // Free receiver
  rc_rfree(args->op.r);
  system("rm -f /tmp/rcmemo??.tmp");

  // Exit
  gtk_main_quit();
}

// On click Get button callback
gboolean cb_get(GtkWidget* widget, args_t *args){
  int ret;
  char buf[40];
  GtkTextBuffer *buffer;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args->textview));

  // Check serial number
  if(args->serial >= 99){
    gtk_text_buffer_insert_at_cursor(buffer,
      "Too many entries.Do nothing.\n", -1);
    return TRUE;
  }

  // Get
  ret = rc_memo(args->serial, &args->ir);

  // Report
  if(ret < 0){
    perror("cb_get:rc_memo");
    gtk_text_buffer_insert_at_cursor(buffer,
      "Fail to Get.\n", -1);
    return TRUE;
  }
  sprintf(buf, "Get button %02d.\n", args->serial);
  gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

  // Update flags
  args->serial++;
  args->not_saved = 1;
  gtk_widget_set_sensitive(args->btn_get, FALSE);
  gtk_widget_set_sensitive(args->btn_save, TRUE);

  return TRUE;
}

// On click Save button callback
gboolean cb_save(GtkWidget* widget, args_t *args){
  char buf[80];
  int ret;
  GtkTextBuffer *buffer;
  GtkTextIter start, end;

  // clear textview
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args->textview));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_delete(buffer, &start, &end);

  // Save
  ret = rc_save(args->serial, args->op.d, &args->ir);

  // Report
  if(ret < 0){
    perror("cb_get:rc_save");
    gtk_text_buffer_insert_at_cursor(buffer,
      "Fail to Save.\n", -1);
    return TRUE;
  }
  sprintf(buf, "Create %s/analyzer.conf.\n", args->op.d);
  gtk_text_buffer_insert_at_cursor(buffer, buf, -1);

  // Update flags
  args->not_saved = 0;
  gtk_widget_set_sensitive(args->btn_get, FALSE);
  gtk_widget_set_sensitive(args->btn_shot, FALSE);
  gtk_widget_set_sensitive(args->btn_save, FALSE);

  return TRUE;
}

// On click Shot button callback
gboolean cb_shot(GtkWidget* widget, args_t *args){
  int fd;
  GtkTextBuffer *buffer;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(args->textview));

  // Open transmitter
  fd = rc_topen(args->op.t);
  if(fd < 0){
    perror("cb_shot:rc_topen");
    gtk_text_buffer_insert_at_cursor(buffer,
      "Fail to open transmitter.\n", -1);
    return TRUE;
  }

  // Shot
  rc_transmit(fd, &args->ir);
  close(fd);
  gtk_text_buffer_insert_at_cursor(buffer, "Ir Shot done.\n", -1);
  return TRUE;
}

// main routine
int main(int argc, char* argv[]){
  int ret;
  args_t args;
  int son[RC_SIGLEN], soff[RC_SIGLEN];
  uint8_t code1[RC_SIGCNT], code2[RC_SIGCNT];

// GdkPixbuf RGBA C-Source image dump 1-byte-run-length-encoded
// Generated by gdk-pixbuf-csource
static const guint8 icon_analyze[] __attribute__ ((__aligned__ (4))) = 
{ ""
  "GdkP"
  "\0\0\0\327"
  "\2\1\0\2"
  "\0\0\0@"
  "\0\0\0\20"
  "\0\0\0\20"
  "\220\377\377\377\0\244#\31\26\377\203\0\0\0\0\215#\31\26\377\3\0\0\0"
  "\0#\31\26\377\0\0\0\0\215#\31\26\377\3\0\0\0\0#\31\26\377\0\0\0\0\215"
  "#\31\26\377\3\0\0\0\0#\31\26\377\0\0\0\0\215#\31\26\377\3\0\0\0\0#\31"
  "\26\377\0\0\0\0\212#\31\26\377\204\0\0\0\0\1#\31\26\377\204\0\0\0\0\1"
  "#\31\26\377\204\0\0\0\0\212#\31\26\377\3\0\0\0\0#\31\26\377\0\0\0\0\215"
  "#\31\26\377\3\0\0\0\0#\31\26\377\0\0\0\0\215#\31\26\377\3\0\0\0\0#\31"
  "\26\377\0\0\0\0\215#\31\26\377\203\0\0\0\0\244#\31\26\377\220\377\377"
  "\377\0"};

static const guint8 icon_get[] __attribute__ ((__aligned__ (4))) = 
{ ""
  "GdkP"
  "\0\0\0\244"
  "\2\1\0\2"
  "\0\0\0@"
  "\0\0\0\20"
  "\0\0\0\20"
  "\240\377\377\377\0\203\0\0\0\0\203#\31\26\377\212\0\0\0\0\223#\31\26"
  "\377\203\0\0\0\0\223#\31\26\377\203\0\0\0\0\214#\31\26\377\205\0\0\0"
  "\0\212#\31\26\377\202\0\0\0\0\203#\31\26\377\202\0\0\0\0\211#\31\26\377"
  "\202\0\0\0\0\203#\31\26\377\202\0\0\0\0\211#\31\26\377\202\0\0\0\0\203"
  "#\31\26\377\202\0\0\0\0\212#\31\26\377\205\0\0\0\0\214#\31\26\377\203"
  "\0\0\0\0\244#\31\26\377\240\377\377\377\0"};

static const guint8 icon_save[] __attribute__ ((__aligned__ (4))) = 
{ ""
  "GdkP"
  "\0\0\1b"
  "\2\1\0\2"
  "\0\0\0@"
  "\0\0\0\20"
  "\0\0\0\20"
  "\221\377\377\377\0\215#\31\26\377\1\0\0\0\0\202\377\377\377\0\202#\31"
  "\26\377\207\0\0\0\0\202#\31\26\377\1\0\0\0\0\202#\31\26\377\202\377\377"
  "\377\0\202#\31\26\377\207\0\0\0\0\202#\31\26\377\1\0\0\0\0\202#\31\26"
  "\377\202\377\377\377\0\202#\31\26\377\207\0\0\0\0\202#\31\26\377\1\0"
  "\0\0\0\202#\31\26\377\202\377\377\377\0\202#\31\26\377\207\0\0\0\0\202"
  "#\31\26\377\1\0\0\0\0\202#\31\26\377\202\377\377\377\0\202#\31\26\377"
  "\212\0\0\0\0\202#\31\26\377\202\377\377\377\0\216#\31\26\377\202\377"
  "\377\377\0\202#\31\26\377\212\0\0\0\0\202#\31\26\377\202\377\377\377"
  "\0\202#\31\26\377\1\0\0\0\0\210#\31\26\377\1\0\0\0\0\202#\31\26\377\202"
  "\377\377\377\0\202#\31\26\377\212\0\0\0\0\202#\31\26\377\202\377\377"
  "\377\0\202#\31\26\377\1\0\0\0\0\210#\31\26\377\1\0\0\0\0\202#\31\26\377"
  "\202\377\377\377\0\202#\31\26\377\212\0\0\0\0\202#\31\26\377\202\377"
  "\377\377\0\202#\31\26\377\1\0\0\0\0\210#\31\26\377\1\0\0\0\0\202#\31"
  "\26\377\202\377\377\377\0\216#\31\26\377\221\377\377\377\0"};

static const guint8 icon_shot[] __attribute__ ((__aligned__ (4))) = 
{ ""
  "GdkP"
  "\0\0\1O"
  "\2\1\0\2"
  "\0\0\0@"
  "\0\0\0\20"
  "\0\0\0\20"
  "\1\0\0\0\0\202#\31\26\377\212\0\0\0\0\202#\31\26\377\203\0\0\0\0\202"
  "#\31\26\377\210\0\0\0\0\202#\31\26\377\205\0\0\0\0\1#\31\26\377\203\0"
  "\0\0\0\202#\31\26\377\203\0\0\0\0\1#\31\26\377\203\0\0\0\0\202#\31\26"
  "\377\203\0\0\0\0\206#\31\26\377\203\0\0\0\0\202#\31\26\377\1\0\0\0\0"
  "\202#\31\26\377\1\0\0\0\0\205#\31\26\377\1\0\0\0\0\202#\31\26\377\1\0"
  "\0\0\0\202#\31\26\377\203\0\0\0\0\2#\31\26\377\0\0\0\0\206#\31\26\377"
  "\4\0\0\0\0#\31\26\377\0\0\0\0#\31\26\377\206\0\0\0\0\210#\31\26\377\210"
  "\0\0\0\0\210#\31\26\377\210\0\0\0\0\210#\31\26\377\210\0\0\0\0\210#\31"
  "\26\377\210\0\0\0\0\210#\31\26\377\210\0\0\0\0\210#\31\26\377\207\0\0"
  "\0\0\212#\31\26\377\210\0\0\0\0\1#\31\26\377\204\0\0\0\0\1#\31\26\377"
  "\212\0\0\0\0\1#\31\26\377\204\0\0\0\0\1#\31\26\377\212\0\0\0\0\1#\31"
  "\26\377\204\0\0\0\0\1#\31\26\377\205\0\0\0\0"};

  // Parameter initialize
  args.new_data = 0;
  args.not_saved = 0;
  args.serial = 0;
  args.ir.son = son;
  args.ir.soff = soff;
  args.ir.code1 = code1;
  args.ir.code2 = code2;
  args.array_1 = NULL;
  args.array_0 = NULL;

  gtk_init(&argc, &argv);
  rc_getopt(argc, argv, &args.op);// Get option or set default

  // Transmitter export
  ret = rc_tassign(args.op.t);
  if(ret == -1){
    perror("rc_tassign");
    fprintf(stderr, "Transmitter: %s\n", args.op.t);
  }

  // Receiver export
  ret = rc_rassign(args.op.r);
  if(ret == -1){
    perror("rc_rassign");
  }

  // Generate UI
  args.win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(args.win), "G_Analyzer");

  GtkWidget *fix = gtk_fixed_new();
  gtk_widget_set_size_request(fix, 670, 450);
  gtk_container_add(GTK_CONTAINER(args.win), fix);

  GtkWidget *scrollwin = gtk_scrolled_window_new (NULL,NULL);
  gtk_widget_set_size_request(scrollwin, 320, 266);
  gtk_fixed_put((GtkFixed*)fix, scrollwin, 340, 120);

  args.form = gtk_drawing_area_new();
  gtk_widget_set_size_request(args.form, 650, 100);
  gtk_fixed_put((GtkFixed*)fix, args.form, 10, 10);

  args.hist_t = gtk_drawing_area_new();
  gtk_widget_set_size_request(args.hist_t, 320, 100);
  gtk_fixed_put((GtkFixed*)fix, args.hist_t, 10, 120);

  args.hist_1 = gtk_drawing_area_new();
  gtk_widget_set_size_request(args.hist_1, 320, 100);
  gtk_fixed_put((GtkFixed*)fix, args.hist_1, 10, 230);

  args.hist_0 = gtk_drawing_area_new();
  gtk_widget_set_size_request(args.hist_0, 320, 100);
  gtk_fixed_put((GtkFixed*)fix, args.hist_0, 10, 340);

  args.textview = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(args.textview), FALSE);
  gtk_container_add(GTK_CONTAINER(scrollwin), args.textview);

  args.btn_exit = gtk_button_new_with_label("Exit");
  gtk_widget_set_size_request(args.btn_exit, 100, 24);
  gtk_fixed_put((GtkFixed*)fix, args.btn_exit, 560, 406);

  GdkPixbuf *pixbuf;
  GtkWidget *image;

  args.btn_analyze = gtk_button_new();
  gtk_widget_set_size_request(args.btn_analyze, 24, 24);
  pixbuf = gdk_pixbuf_new_from_inline(-1, icon_analyze, TRUE, NULL);
  image = gtk_image_new_from_pixbuf(pixbuf);
  gtk_button_set_image((GtkButton *)args.btn_analyze, image);
  gtk_fixed_put((GtkFixed*)fix, args.btn_analyze, 340, 406);

  args.btn_shot = gtk_button_new();
  gtk_widget_set_size_request(args.btn_shot, 24, 24);
  pixbuf = gdk_pixbuf_new_from_inline(-1, icon_shot, TRUE, NULL);
  image = gtk_image_new_from_pixbuf(pixbuf);
  gtk_button_set_image((GtkButton *)args.btn_shot, image);
  gtk_fixed_put((GtkFixed*)fix, args.btn_shot, 379, 406);

  args.btn_get = gtk_button_new();
  gtk_widget_set_size_request(args.btn_get, 24, 24);
  pixbuf = gdk_pixbuf_new_from_inline(-1, icon_get, TRUE, NULL);
  image = gtk_image_new_from_pixbuf(pixbuf);
  gtk_button_set_image((GtkButton *)args.btn_get, image);
  gtk_fixed_put((GtkFixed*)fix, args.btn_get, 418, 406);

  args.btn_save = gtk_button_new();
  gtk_widget_set_size_request(args.btn_save, 24, 24);
  pixbuf = gdk_pixbuf_new_from_inline(-1, icon_save, TRUE, NULL);
  image = gtk_image_new_from_pixbuf(pixbuf);
  gtk_button_set_image((GtkButton *)args.btn_save, image);
  gtk_fixed_put((GtkFixed*)fix, args.btn_save, 457, 406);

  // Set callback
  g_signal_connect(args.win, "destroy", G_CALLBACK(cb_exit), NULL);
  g_signal_connect(args.btn_exit, "clicked", G_CALLBACK(cb_exit), &args);
  g_signal_connect(args.btn_analyze, "clicked", G_CALLBACK(cb_analyze), &args);
  g_signal_connect(args.btn_get, "clicked", G_CALLBACK(cb_get), &args);
  g_signal_connect(args.btn_save, "clicked", G_CALLBACK(cb_save), &args);
  g_signal_connect(args.btn_shot, "clicked", G_CALLBACK(cb_shot), &args);
  g_signal_connect(args.form, "draw", G_CALLBACK(cb_form), &args);
  g_signal_connect(args.hist_t, "draw", G_CALLBACK(cb_hist_t), &args);
  g_signal_connect(args.hist_1, "draw", G_CALLBACK(cb_hist_1), &args);
  g_signal_connect(args.hist_0, "draw", G_CALLBACK(cb_hist_0), &args);

  // Set sensitive
  gtk_widget_set_sensitive(args.btn_analyze, TRUE);
  gtk_widget_set_sensitive(args.btn_shot, FALSE);
  gtk_widget_set_sensitive(args.btn_get, FALSE);
  gtk_widget_set_sensitive(args.btn_save, FALSE);

  // Start message
  gtk_text_buffer_insert_at_cursor(
    gtk_text_view_get_buffer(GTK_TEXT_VIEW(args.textview)),
    "Click Analyze button to start analyze.\n", -1);

  // Show all
  gtk_widget_show_all(args.win);
  gtk_main();
  return 0;
}
