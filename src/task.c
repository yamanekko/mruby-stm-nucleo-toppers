
#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "nucleo.h"
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "serial.h"
#include "syslog.h"

/*
 *  サービスコールのエラーのログ出力
 */
Inline ER
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
	return ercd;
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

static mrb_value
mrb_mruby_task_initialize(mrb_state *mrb, mrb_value self)
{
	return self;
}

static mrb_value
mrb_mruby_task_wake_up(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(wup_tsk(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_active(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(act_tsk(task_id));	
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_wakeup(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(wup_tsk(task_id));	
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_delay(mrb_state *mrb, mrb_value self)
{
	mrb_int timeout;
	mrb_int result = 0;
	mrb_int ret = mrb_get_args(mrb, "i", &timeout);
	result = (mrb_int)SVC_PERROR(dly_tsk((timeout * 1000)));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_sleep(mrb_state *mrb, mrb_value self)
{
	mrb_int timeout;
	mrb_int result = 0;
	mrb_int ret = mrb_get_args(mrb, "|i", &timeout);
	if(ret == 0){
		result = (mrb_int)SVC_PERROR(slp_tsk());
	}else{
		result = (mrb_int)SVC_PERROR(tslp_tsk((timeout * 1000)));
	}
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_disable_terminate(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	result = (mrb_int)SVC_PERROR(dis_ter());
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_enable_terminate(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	result = (mrb_int)SVC_PERROR(ena_ter());
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_start_cyclic(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(sta_cyc(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_stop_cyclic(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(stp_cyc(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_sta_alm(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int alarm_id, time;
	mrb_int ret = mrb_get_args(mrb, "ii", &alarm_id, &time);
	result = (mrb_int)SVC_PERROR(sta_alm(alarm_id, (time * 1000)));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_stp_alm(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int alarm_id;
	mrb_int ret = mrb_get_args(mrb, "i", &alarm_id);
	result = (mrb_int)SVC_PERROR(stp_alm(alarm_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_rel_wai(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(rel_wai(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_ras_ter(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(ras_ter(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_rsm_tsk(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(rsm_tsk(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_rot_rdq(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int priority;
	mrb_int ret = mrb_get_args(mrb, "i", &priority);
	result = (mrb_int)SVC_PERROR(rot_rdq(priority));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_ter_tsk(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(ter_tsk(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_chg_pri(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id, priority;
	mrb_int ret = mrb_get_args(mrb, "ii", &task_id, priority);
	result = (mrb_int)SVC_PERROR(chg_pri(task_id, priority));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_get_pri(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	PRI		priority = 0;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
//TODO	result = (mrb_int)SVC_PERROR(get_pri(task_id, &priority));
	if (result >= 0) {
		result = (mrb_int)priority;
	}
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_can_wup(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(can_wup(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_sus_tsk(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(sus_tsk(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_can_act(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	mrb_int task_id;
	mrb_int ret = mrb_get_args(mrb, "i", &task_id);
	result = (mrb_int)SVC_PERROR(can_act(task_id));
	return mrb_fixnum_value(result);
}

static mrb_value
mrb_mruby_task_exit(mrb_state *mrb, mrb_value self)
{
	mrb_int result = 0;
	result = (mrb_int)SVC_PERROR(ext_tsk());
	return mrb_fixnum_value(result);
}

void
mrb_mruby_task_gem_init(mrb_state* mrb)
{
#ifdef USE_MODULE
	struct RClass * nucleo_class = mrb_module_get(mrb, "Nucleo");
#else
	struct RClass * nucleo_class = mrb_class_get(mrb, "Nucleo");
#endif
	struct RClass * task_class = mrb_define_class_under(mrb, nucleo_class, "Task", mrb->object_class);

//1 はビルドはとおったけど(Script errorにならなかった)、呼び出しのテストは未
//ok は実行できることを確認しているが、暫定的にコメントアウトしている。外せば動く。ただし、現状class method はMax10個

	mrb_define_method(mrb, task_class, "initialize", mrb_mruby_task_initialize, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, task_class, "delay", mrb_mruby_task_delay, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "sleep", mrb_mruby_task_sleep, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "wakeup", mrb_mruby_task_wakeup, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "active", mrb_mruby_task_active, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "exit", mrb_mruby_task_exit, MRB_ARGS_NONE());
//1	mrb_define_class_method(mrb, task_class, "disable_terminate", mrb_mruby_task_disable_terminate, MRB_ARGS_NONE());
//1	mrb_define_class_method(mrb, task_class, "enable_terminate", mrb_mruby_task_enable_terminate, MRB_ARGS_NONE());
	mrb_define_class_method(mrb, task_class, "start_cyclic", mrb_mruby_task_start_cyclic, MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, task_class, "stop_cyclic", mrb_mruby_task_stop_cyclic, MRB_ARGS_REQ(1));

//1	mrb_define_class_method(mrb, task_class, "start_alarm", mrb_mruby_task_sta_alm, MRB_ARGS_REQ(2));
//1	mrb_define_class_method(mrb, task_class, "stop_alarm", mrb_mruby_task_stp_alm, MRB_ARGS_REQ(1));

//1	mrb_define_class_method(mrb, task_class, "release_wait", mrb_mruby_task_rel_wai, MRB_ARGS_REQ(1));
//1	mrb_define_class_method(mrb, task_class, "raise_termination", mrb_mruby_task_ras_ter, MRB_ARGS_REQ(1));

//1	mrb_define_class_method(mrb, task_class, "resume", mrb_mruby_task_rsm_tsk, MRB_ARGS_REQ(1));
//1	mrb_define_class_method(mrb, task_class, "rotate_ready_queue", mrb_mruby_task_rot_rdq, MRB_ARGS_REQ(1));

//1	mrb_define_class_method(mrb, task_class, "termination", mrb_mruby_task_ter_tsk, MRB_ARGS_REQ(1));
//1	mrb_define_class_method(mrb, task_class, "change_priority", mrb_mruby_task_chg_pri, MRB_ARGS_REQ(2));
//NG コメントアウトを外すとScript error	mrb_define_class_method(mrb, task_class, "get_priority", mrb_mruby_task_get_pri, MRB_ARGS_REQ(1));
//1	mrb_define_class_method(mrb, task_class, "cancel_wakeup", mrb_mruby_task_can_wup, MRB_ARGS_REQ(1));

//1	mrb_define_class_method(mrb, task_class, "suspend", mrb_mruby_task_sus_tsk, MRB_ARGS_REQ(1));
//1	mrb_define_class_method(mrb, task_class, "cancel_activetion", mrb_mruby_task_can_act, MRB_ARGS_REQ(1));
}

void
mrb_mruby_task_gem_final(mrb_state* mrb)
{
}

