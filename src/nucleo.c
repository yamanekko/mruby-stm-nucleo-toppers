#include "mruby.h"
#include "nucleo.h"

void mrb_mruby_testled_gem_init(mrb_state *mrb);
void mrb_mruby_clock_gem_init(mrb_state *mrb);
void mrb_mruby_task_gem_init(mrb_state *mrb);
void mrb_mruby_data_que_gem_init(mrb_state *mrb);
void mrb_mruby_serial_gem_init(mrb_state *mrb);

#ifndef USE_MODULE
static mrb_value
mrb_mruby_stm_nucleo_initialize(mrb_state *mrb, mrb_value self)
{
	return self;
}
#endif

void
mrb_mruby_stm_nucleo_toppers_gem_init(mrb_state* mrb)
{
#ifdef USE_MODULE
	struct RClass * nucleo_class = mrb_define_module(mrb, "Nucleo");
#else
	struct RClass * nucleo_class = mrb_define_class(mrb, "Nucleo", mrb->object_class);
#endif
//		mrb_define_const(mrb, nucleo_class, "SAMPLE", mrb_fixnum_value(1));	//example

	//methods
#ifndef USE_MODULE
	mrb_define_method(mrb, nucleo_class, "initialize", mrb_mruby_stm_nucleo_initialize, MRB_ARGS_REQ(1));
#endif
	// initialize under classes
	mrb_mruby_testled_gem_init(mrb);
	mrb_mruby_clock_gem_init(mrb);
	mrb_mruby_task_gem_init(mrb);
	mrb_mruby_data_que_gem_init(mrb);
	mrb_mruby_serial_gem_init(mrb);
}

void
mrb_mruby_stm_nucleo_toppers_gem_final(mrb_state* mrb)
{
}
