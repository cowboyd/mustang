#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_string.h"
#include "v8_integer.h"
#include "v8_array.h"
#include "v8_function.h"
#include "v8_context.h"

using namespace v8;

#ifndef rb_sym_to_s
#define rb_sym_to_s(sym) rb_funcall2(sym, rb_intern("to_s"), 0, NULL)
#endif

Handle<Value> to_v8(VALUE value)
{
  switch (TYPE(value)) {
  case T_NIL:
    return Null();
  case T_TRUE:
    return True();
  case T_FALSE:
    return False();
  case T_SYMBOL:
    return to_v8(rb_sym_to_s(value));
  case T_FIXNUM:
    return v8_integer_cast(value);
  case T_STRING:
    return v8_string_cast(value);
  case T_ARRAY:
    return v8_array_cast(value);
  default:
    if (rb_obj_is_kind_of(value, rb_cMustangV8String)) {
      return v8_ref_get<String>(value);
    } else if (rb_obj_is_kind_of(value, rb_cMustangV8Integer)) {
      return v8_ref_get<Integer>(value);
    } else if (rb_obj_is_kind_of(value, rb_cMustangV8Array)) {
      return v8_ref_get<Array>(value);
    } else if (rb_obj_is_kind_of(value, rb_cMustangV8Function)) {
      return v8_ref_get<Function>(value);
    } else if (rb_obj_is_kind_of(value, rb_cMustangV8Object)) {
      return v8_ref_get<Object>(value);
    } else {
      return Undefined();
    }
  }
}

VALUE to_ruby(Handle<Value> value)
{
  if (value.IsEmpty() || value->IsUndefined() || value->IsNull()) {
    return Qnil;
  }
  if (value->IsBoolean()) {
    return value->BooleanValue() ? Qtrue : Qfalse;
  }
  if (value->IsUint32() || value->IsInt32()) {
    return v8_integer_cast(value);
  }
  if (value->IsString()) {
    return v8_string_cast(value);
  }
  if (value->IsFunction()) {
    return v8_function_cast(value);
  }
  if (value->IsArray()) {
    return v8_array_cast(value);
  }

  return Qnil;
}

OVERLOADED_V8_TO_RUBY_CAST(Object);
OVERLOADED_V8_TO_RUBY_CAST(String);
OVERLOADED_V8_TO_RUBY_CAST(Integer);
OVERLOADED_V8_TO_RUBY_CAST(Function);
OVERLOADED_V8_TO_RUBY_CAST(Array);

VALUE to_ruby(bool value)     { return value ? Qtrue : Qfalse; }
VALUE to_ruby(double value)   { return rb_float_new(value); }
VALUE to_ruby(char *value)    { return rb_str_new2(value); }
VALUE to_ruby(int64_t value)  { return LONG2NUM(value); }
VALUE to_ruby(uint32_t value) { return UINT2NUM(value); }
VALUE to_ruby(int32_t value)  { return INT2FIX(value); }
