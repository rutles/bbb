// Fl_Servo v1.0
// compile: ./fltkcc fl_servo -lbbb
// execute: ./fl_servo
// NOTE: FLTK1.3 required
// install: apt-get install libfltk1.3-dev

extern "C" {
#include <bbb.h>
}

#include <unistd.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Counter.H>

#define PIN "P9_16"
#define LABEL_SIZE 10

typedef struct {
  int fd;
  Fl_Button *btn_exit;
  Fl_Dial *dial;
  Fl_Box *center;
  Fl_Counter *max;
  Fl_Counter *min;
} params_t;

void cb_exit(Fl_Widget*, void* pall){
  params_t *all = (params_t *)pall;

  close(all->fd);

  all->fd = pwm_open(PIN, "run");
  write(all->fd, "0", 1);
  close(all->fd);

  exit(0);
}

void duty(params_t *all){
  double vmin;
  double vmax;
  double vknb;
  double v;
  int d;
  static char lbuf[80];
  char dbuf[80];

  vmax = all->max->value();
  vmin = all->min->value();
  vknb = all->dial->value();
  v = (vmax - vmin) * vknb + vmin;
  d = v * 1000000;
  sprintf(lbuf, "%2.2f", (float)d / 1000000);
  all->center->label(lbuf);

  sprintf(dbuf, "%d", d);
  write(all->fd, dbuf, strlen(dbuf));
}

void cb_min(Fl_Widget* widget, void* pall){
  params_t *all = (params_t *)pall;
  double vmin;
  double vmax;

  vmin = all->min->value();
  vmax = all->max->value();

  if(vmin < 0.5){
    all->min->value(0.5);
  } else
  if(vmin >= vmax){
    all->min->value(vmax - 0.1);
  }
  duty(all);
}

void cb_max(Fl_Widget*, void* pall){
  params_t *all = (params_t *)pall;
  double vmin;
  double vmax;

  vmax = all->max->value();
  vmin = all->min->value();
  if(vmax > 2.5){
    all->max->value(2.5);
  } else
  if(vmin >= vmax){
    all->max->value(vmin + 0.1);
  }

  duty(all);
}

void cb_dial(Fl_Widget*, void* pall){
  duty((params_t *)pall);
}

int main(int argc, char *argv[]){
  params_t all;
  int ret;

  ret = pwm_setup(PIN, 20000000, 1000000);
  if(ret < 0){
    perror("pwm_setup");
    return -1;
  }

  all.fd = pwm_open(PIN, "run");
  write(all.fd, "1", 1);
  close(all.fd);

  all.fd = pwm_open(PIN, "duty");

  Fl_Window win(190, 290, "Fl_analyzer");
  all.center = new Fl_Box(55, 10, 80, 20, "1.00");
  all.dial = new Fl_Dial(10, 30, 170, 170, "Angle");
  all.dial->labelsize(LABEL_SIZE);
  all.dial->callback(cb_dial, &all);
  all.max = new Fl_Counter(100, 220, 80, 20, "max");
  all.max->type(FL_SIMPLE_COUNTER);
  all.max->labelsize(LABEL_SIZE);
  all.max->value(2);
  all.max->callback(cb_max, &all);
  all.min = new Fl_Counter(10, 220, 80, 20, "min");
  all.min->type(FL_SIMPLE_COUNTER);
  all.min->labelsize(LABEL_SIZE);
  all.min->value(1);
  all.min->callback(cb_min, &all);
  all.btn_exit = new Fl_Button(55, 256, 80, 20, "Exit");
  all.btn_exit->labelsize(LABEL_SIZE);
  all.btn_exit->callback(cb_exit, &all);

  win.show();

  return(Fl::run());
}
