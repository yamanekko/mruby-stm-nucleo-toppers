// シリアル経由でlogを出力するクラス

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "serial.h"
#include "syslog.h"
#include <string.h>
#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "nucleo.h"

static mrb_value
mrb_mruby_serial_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_int port_no;
	mrb_get_args(mrb, "|i", &port_no);
	if (port_no > 0){
		mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(port_no));
	}
	return self;
}

static mrb_value
mrb_mruby_serial_with_level(mrb_state *mrb, mrb_value self)
{
/* TODO
	const char *msg;
	mrb_int len, type;
	mrb_get_args(mrb, "is", &type, &msg, &len);
	syslog((unsigned int)type, msg);
*/
	return self;
}

static mrb_value
mrb_mruby_serial_syslog(mrb_state *mrb, mrb_value self)
{
	char *msg;
	mrb_int len;
	mrb_get_args(mrb, "s", &msg, &len);
	syslog(LOG_NOTICE, msg);
	return self;
}

static mrb_value
mrb_mruby_serial_write(mrb_state *mrb, mrb_value self)
{
	char *msg;
	mrb_int len;
	mrb_get_args(mrb, "s", &msg, &len);
	mrb_int port_no = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
	serial_wri_dat(port_no, msg, len);
	return self;
}

static mrb_value
mrb_mruby_serial_read_byte(mrb_state *mrb, mrb_value self)
{
	uint8_t buf;
	mrb_int port_no = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
	ER_UINT erlen = serial_rea_dat(port_no, (char*)&buf, sizeof(buf));
	if(erlen > 0){
		return mrb_fixnum_value(buf);
	}else{
		return mrb_nil_value();
	}
}

void
mrb_mruby_serial_gem_init(mrb_state* mrb)
{
#ifdef USE_MODULE
	struct RClass * nucleo_class = mrb_module_get(mrb, "Nucleo");
#else
	struct RClass * nucleo_class = mrb_class_get(mrb, "Nucleo");
#endif

	struct RClass * serial_class = mrb_define_class_under(mrb, nucleo_class, "Serial", mrb->object_class);
	mrb_define_method(mrb, serial_class, "initialize", mrb_mruby_serial_initialize, MRB_ARGS_REQ(1));
//TODO	mrb_define_method(mrb, serial_class, "syslog_with_level", mrb_mruby_serial_with_level, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, serial_class, "syslog", mrb_mruby_serial_syslog, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, serial_class, "write", mrb_mruby_serial_write, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, serial_class, "read_byte", mrb_mruby_serial_read_byte, MRB_ARGS_NONE());
}

void
mrb_mruby_serial_gem_final(mrb_state* mrb)
{
}
