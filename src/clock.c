#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "nucleo.h"
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "serial.h"
#include "syslog.h"

#ifdef ASP
static mrb_int
getTim()
{
	SYSTIM time;
	get_tim(&time);
	return time;
}
#else
// USE_64BIT_HRTCNT は#defineされていない前提
static mrb_value
mrb_mruby_get_utim()
{
	HRTCNT time;
	time = fch_hrt();
	return mrb_fixnum_value(time);
}
#endif

static mrb_value
mrb_mruby_clock_reset(mrb_state *mrb, mrb_value self)
{
	mrb_int start_clock;
#ifdef ASP
	start_clock = getTim();
#else
	start_clock = mrb_mruby_get_utim();
#endif
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@start_clock"), mrb_fixnum_value(start_clock));
	return mrb_true_value();
}

static mrb_value
mrb_mruby_clock_initialize(mrb_state *mrb, mrb_value self)
{
	return mrb_mruby_clock_reset(mrb, self);
}

static mrb_value
mrb_mruby_clock_now(mrb_state *mrb, mrb_value self)
{
	mrb_value start_clock_val = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@start_clock"));
	mrb_int start_clock = mrb_fixnum(start_clock_val);
	mrb_int now;
#ifdef ASP
	now = getTim() - start_clock;
#else
	now = mrb_mruby_get_utim() - start_clock;
#endif
	return mrb_fixnum_value(now);
}

void
mrb_mruby_clock_gem_init(mrb_state* mrb)
{
	struct RClass * nucleo_class = mrb_class_get(mrb, "Nucleo");
	struct RClass * clock_class = mrb_define_class_under(mrb, nucleo_class, "Clock", mrb->object_class);

	mrb_define_method(mrb, clock_class, "initialize", mrb_mruby_clock_initialize, MRB_ARGS_NONE());
	mrb_define_method(mrb, clock_class, "reset", mrb_mruby_clock_reset, MRB_ARGS_NONE());
	mrb_define_method(mrb, clock_class, "now", mrb_mruby_clock_now, MRB_ARGS_NONE());
	mrb_define_method(mrb, clock_class, "utim", mrb_mruby_get_utim, MRB_ARGS_NONE());
}

void
mrb_mruby_clock_gem_final(mrb_state* mrb)
{
}