#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "nucleo.h"
#include "kernel.h"
#include "t_stddef.h"
#include <t_syslog.h>
#include <t_stdlib.h>
#include "serial.h"
#include "syslog.h"

static mrb_value
mrb_mruby_data_que_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int n;
	mrb_get_args(mrb, "i", &n);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@id"), mrb_fixnum_value(n));

	return self;
}

// 受信　引数があった場合はタイムアウトつき
static mrb_value
mrb_mruby_data_que_rcv_dtq(mrb_state *mrb, mrb_value self)
{
	intptr_t	data;
	mrb_int timeout;
	mrb_int result = 0;

	ID dtqid = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
	mrb_get_args(mrb, "|i", &timeout);
	if(timeout == 0){
		result = (mrb_int)rcv_dtq(dtqid, &data);
	}else{
		result = (mrb_int)trcv_dtq(dtqid, &data, ((uint32_t)timeout * 1000));
	}
	syslog(LOG_INFO, "data receive:%d", result);
	if(result == E_OK){
		return mrb_fixnum_value((mrb_int)data);
	}else{
		return mrb_nil_value();
	}
}

static mrb_value
mrb_mruby_data_que_prcv_dtq(mrb_state *mrb, mrb_value self)
{
	intptr_t data;
	mrb_int result = 0;
	ID dtqid = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
	result = (mrb_int)prcv_dtq(dtqid, &data);
	if(result == E_OK){
		return mrb_fixnum_value((mrb_int)data);
	}else{
		return mrb_nil_value();
	}
}

//送信 引数２つの場合はタイムアウト付き
static mrb_value
mrb_mruby_data_que_snd_dtq(mrb_state *mrb, mrb_value self)
{
	ID dtqid = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
	mrb_int data, timeout;
	mrb_int result = 0;
	mrb_get_args(mrb, "i|i", &data, &timeout);
	if(timeout == 0){
		result = (mrb_int)snd_dtq(dtqid, (intptr_t)data);
	}else{
		result = (mrb_int)tsnd_dtq(dtqid, (intptr_t)data, ((uint32_t)timeout * 1000));
	}
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_data_que_psnd_dtq(mrb_state *mrb, mrb_value self)
{
	ID dtqid = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
	mrb_int data;
	mrb_int result = 0;
	mrb_get_args(mrb, "i", &data);
	result = (mrb_int)psnd_dtq(dtqid, (intptr_t)data);

	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_data_que_fsnd_dtq(mrb_state *mrb, mrb_value self)
{
	ID dtqid = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
	mrb_int data;
	mrb_int result = 0;
	mrb_get_args(mrb, "i", &data);
	result = (mrb_int)fsnd_dtq(dtqid, (intptr_t)data);

	return mrb_fixnum_value(result);
}

void
mrb_mruby_data_que_gem_init(mrb_state* mrb)
{
	struct RClass * nucleo_class = mrb_class_get(mrb, "Nucleo");
	struct RClass * class = mrb_define_class_under(mrb, nucleo_class, "DataQue", mrb->object_class);

	mrb_define_method(mrb, class, "initialize", mrb_mruby_data_que_initialize, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, class, "send", mrb_mruby_data_que_snd_dtq, MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, class, "send_polling", mrb_mruby_data_que_psnd_dtq, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, class, "force_send", mrb_mruby_data_que_fsnd_dtq, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, class, "receive", mrb_mruby_data_que_rcv_dtq, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, class, "receive_polling", mrb_mruby_data_que_prcv_dtq, MRB_ARGS_NONE());
}

void
mrb_mruby_data_que_gem_final(mrb_state* mrb)
{
}