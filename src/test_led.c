#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include <mruby/class.h>
#include <mruby/data.h>
#include "nucleo.h"
#if defined(STM32F411xE) || defined(STM32F401xE)
#include "stm32f4xx_nucleo.h"
#elif defined(STM32F767xx)
#include "stm32f7xx_nucleo_144.h"
#endif

static mrb_value
mrb_mruby_testled_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int led_no = -1;
	mrb_get_args(mrb, "|i", &led_no);
	if(led_no < 0){
		//省略された場合は0に該当するF4xxはLED2, F7xxはLED1とする
		led_no = 0;
	}
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@led_no"), mrb_fixnum_value(led_no));
	return self;
}

static mrb_value
mrb_mruby_testled_toggle(mrb_state *mrb, mrb_value self)
{
	mrb_int led_no = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@led_no")));
	BSP_LED_Toggle(led_no);
	return self;
}

static mrb_value
mrb_mruby_testled_off(mrb_state *mrb, mrb_value self)
{
	mrb_int led_no = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@led_no")));
	BSP_LED_Off(led_no);
	return self;
}

static mrb_value
mrb_mruby_testled_on(mrb_state *mrb, mrb_value self)
{
	mrb_int led_no = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@led_no")));
	BSP_LED_On(led_no);
	return self;
}

void
mrb_mruby_testled_gem_init(mrb_state* mrb)
{
#ifdef USE_MODULE
	struct RClass * nucleo_class = mrb_module_get(mrb, "Nucleo");
#else
	struct RClass * nucleo_class = mrb_class_get(mrb, "Nucleo");
#endif
	struct RClass * led_class = mrb_define_class_under(mrb, nucleo_class, "TestLed", mrb->object_class);

	mrb_define_method(mrb, led_class, "initialize", mrb_mruby_testled_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, led_class, "on", mrb_mruby_testled_on, MRB_ARGS_NONE());
	mrb_define_method(mrb, led_class, "off", mrb_mruby_testled_off, MRB_ARGS_NONE());
	mrb_define_method(mrb, led_class, "toggle", mrb_mruby_testled_toggle, MRB_ARGS_NONE());
}

void
mrb_mruby_testled_gem_final(mrb_state* mrb)
{
}


