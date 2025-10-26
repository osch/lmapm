/*
* lmapm.c
* big-number library for Lua based on the MAPM library
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 24 Jul 2018 14:57:26
* This code is hereby placed in the public domain and also under the MIT license
*/

#include <stdlib.h>

#include "m_apm.h"

#include "lua.h"
#include "lauxlib.h"
#include "mycompat.h"

#define MYNAME		"mapm"
#define MYVERSION	MYNAME " library for " LUA_VERSION " / Jul 2018 / "\
			"using MAPM " MAPM_LIB_SHORT_VERSION
#define MYTYPE		MYNAME " bignumber"

static lua_State *LL=NULL;

void M_apm_log_error_msg(int fatal, char *message)
{
 (void) fatal;
 luaL_error(LL,"(MAPM) %s",message);
}

static int digits=64;

static int Pdig(lua_State *L, int i)
{
 return luaL_optinteger(L,i,digits);
}

static M_APM Pnew(lua_State *L)
{
 M_APM x=m_apm_init();
 luaL_boxpointer(L,x);
 luaL_setmetatable(L,MYTYPE);
 return x;
}

static M_APM Pget(lua_State *L, int i)
{
 LL=L;
 luaL_checkany(L,i);
 switch (lua_type(L,i))
 {
  case LUA_TNUMBER:
  {
   M_APM x=Pnew(L);
   m_apm_set_double(x,lua_tonumber(L,i));
   lua_replace(L,i);
   return x;
  }
  case LUA_TSTRING:
  {
   M_APM x=Pnew(L);
   m_apm_set_string(x,(char*)lua_tostring(L,i));
   lua_replace(L,i);
   return x;
  }
  default:
   return luaL_unboxpointer(L,i,MYTYPE);
 }
 return NULL;
}

static int Lnew(lua_State *L)			/** new(x) */
{
 Pget(L,1);
 lua_settop(L,1);
 return 1;
}

static int Pdo0(lua_State *L, void (*f)(M_APM y, M_APM x))
{
 M_APM a=Pget(L,1);
 M_APM c=Pnew(L);
 f(c,a);
 return 1;
}

static int Pdo1(lua_State *L, void (*f)(M_APM y, int n, M_APM x))
{
 M_APM a=Pget(L,1);
   int n=Pdig(L,2);
 M_APM c=Pnew(L);
 f(c,n,a);
 return 1;
}

static int Pdo2(lua_State *L, void (*f)(M_APM z, M_APM x, M_APM y))
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
 M_APM c=Pnew(L);
 f(c,a,b);
 return 1;
}

static int Pdo3(lua_State *L, void (*f)(M_APM z, int n, M_APM x, M_APM y))
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
   int n=Pdig(L,3);
 M_APM c=Pnew(L);
 f(c,n,a,b);
 return 1;
}

static int Ldigits(lua_State *L)		/** digits([n]) */
{
 lua_pushinteger(L,digits);
 digits=Pdig(L,1);
 return 1;
}

static int Ltostring(lua_State *L)		/** tostring(x,[n]) */
{
 char *s;
 M_APM a=Pget(L,1);
   int n=Pdig(L,2);
 int m= (n<0) ? m_apm_significant_digits(a) : n;
 s=malloc(m+16);
 if (s==NULL) return 0;
 m_apm_to_string(s,n,a);
 lua_pushstring(L,s);
 free(s);
 return 1;
}

static int Ltofixed(lua_State *L)		/** tofixed(x,[n]) */
{
 M_APM a=Pget(L,1);
   int n=Pdig(L,2);
 char *s=m_apm_to_fixpt_stringexp(n,a,'.',0,0);
 if (s==NULL) return 0;
 lua_pushstring(L,s);
 free(s);
 return 1;
}

static int Ltonumber(lua_State *L)		/** tonumber(x) */
{
 lua_settop(L,1);
 lua_pushinteger(L,32);				/* enough for long doubles */
 Ltostring(L);
 lua_pushnumber(L,lua_tonumber(L,-1));
 return 1;
}

static int Lround(lua_State *L)			/** round(x) */
{
 return Pdo1(L,m_apm_round);
}

static int Linv(lua_State *L)			/** inv(x) */
{
 return Pdo1(L,m_apm_reciprocal);
}

static int Lsqrt(lua_State *L)			/** sqrt(x) */
{
 return Pdo1(L,m_apm_sqrt);
}

static int Lcbrt(lua_State *L)			/** cbrt(x) */
{
 return Pdo1(L,m_apm_cbrt);
}

static int Llog(lua_State *L)			/** log(x) */
{
 return Pdo1(L,m_apm_log);
}

static int Llog10(lua_State *L)			/** log10(x) */
{
 return Pdo1(L,m_apm_log10);
}

static int Lexp(lua_State *L)			/** exp(x) */
{
 return Pdo1(L,m_apm_exp);
}

static int Lsin(lua_State *L)			/** sin(x) */
{
 return Pdo1(L,m_apm_sin);
}

static int Lcos(lua_State *L)			/** cos(x) */
{
 return Pdo1(L,m_apm_cos);
}

static int Ltan(lua_State *L)			/** tan(x) */
{
 return Pdo1(L,m_apm_tan);
}

static int Lasin(lua_State *L)			/** asin(x) */
{
 return Pdo1(L,m_apm_asin);
}

static int Lacos(lua_State *L)			/** acos(x) */
{
 return Pdo1(L,m_apm_acos);
}

static int Latan(lua_State *L)			/** atan(x) */
{
 return Pdo1(L,m_apm_atan);
}

static int Lsincos(lua_State *L)		/** sincos(x) */
{
 M_APM a=Pget(L,1);
   int n=Pdig(L,2);
 M_APM s=Pnew(L);
 M_APM c=Pnew(L);
 m_apm_sin_cos(s,c,n,a);
 return 2;
}

static int Latan2(lua_State *L)			/** atan2(y,x) */
{
 return Pdo3(L,m_apm_atan2);
}

static int Lsinh(lua_State *L)			/** sinh(x) */
{
 return Pdo1(L,m_apm_sinh);
}

static int Lcosh(lua_State *L)			/** cosh(x) */
{
 return Pdo1(L,m_apm_cosh);
}

static int Ltanh(lua_State *L)			/** tanh(x) */
{
 return Pdo1(L,m_apm_tanh);
}

static int Lasinh(lua_State *L)			/** asinh(x) */
{
 return Pdo1(L,m_apm_asinh);
}

static int Lacosh(lua_State *L)			/** acosh(x) */
{
 return Pdo1(L,m_apm_acosh);
}

static int Latanh(lua_State *L)			/** atanh(x) */
{
 return Pdo1(L,m_apm_atanh);
}

static int Labs(lua_State *L)			/** abs(x) */
{
 return Pdo0(L,m_apm_absolute_value);
}

static int Lneg(lua_State *L)			/** neg(x) */
{
 return Pdo0(L,m_apm_negate);
}

static int Lfactorial(lua_State *L)		/** factorial(x) */
{
 return Pdo0(L,m_apm_factorial);
}

static int Lfloor(lua_State *L)			/** floor(x) */
{
 return Pdo0(L,m_apm_floor);
}

static int Lceil(lua_State *L)			/** ceil(x) */
{
 return Pdo0(L,m_apm_ceil);
}

static int Ladd(lua_State *L)			/** add(x,y) */
{
 return Pdo2(L,m_apm_add);
}

static int Lsub(lua_State *L)			/** sub(x,y) */
{
 return Pdo2(L,m_apm_subtract);
}

static int Lmul(lua_State *L)			/** mul(x,y) */
{
 return Pdo2(L,m_apm_multiply);
}

static int Ldiv(lua_State *L)			/** div(x,y) */
{
 return Pdo3(L,m_apm_divide);
}

static int Lidiv(lua_State *L)			/** idiv(x,y) */
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
 M_APM r=Pnew(L);
 m_apm_integer_divide(r,a,b);
 return 1;
}

static int Lquotrem(lua_State *L)		/** quotrem(x,y) */
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
 M_APM q=Pnew(L);
 M_APM r=Pnew(L);
 m_apm_integer_div_rem(q,r,a,b);
 return 2;
}

static int Lmod(lua_State *L)			/** mod(x,y) */
{
 Lquotrem(L);
 return 1;
}

static int Lpow(lua_State *L)			/** pow(x,y) */
{
 return Pdo3(L,m_apm_pow);
}

static int Lipow(lua_State *L)			/** ipow(x,n) */
{
 M_APM a=Pget(L,1);
   int e=luaL_checkinteger(L,2);
   int n=Pdig(L,3);
 M_APM r=Pnew(L);
 m_apm_integer_pow(r,n,a,e);
 return 1;
}

static int Lgcd(lua_State *L)			/** gcd(x,y) */
{
 return Pdo2(L,m_apm_gcd);
}

static int Llcm(lua_State *L)			/** lcm(x,y) */
{
 return Pdo2(L,m_apm_lcm);
}

static int Lcompare(lua_State *L)		/** compare(x,y) */
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
 lua_pushinteger(L,m_apm_compare(a,b));
 return 1;
}

static int Leq(lua_State *L)
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
 lua_pushboolean(L,m_apm_compare(a,b)==0);
 return 1;
}

static int Llt(lua_State *L)
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
 lua_pushboolean(L,m_apm_compare(a,b)<0);
 return 1;
}

static int Lle(lua_State *L)
{
 M_APM a=Pget(L,1);
 M_APM b=Pget(L,2);
 lua_pushboolean(L,m_apm_compare(a,b)<=0);
 return 1;
}

static int Lsign(lua_State *L)			/** sign(x) */
{
 M_APM a=Pget(L,1);
 lua_pushinteger(L,m_apm_sign(a));
 return 1;
}

static int Lexponent(lua_State *L)		/** exponent(x) */
{
 M_APM a=Pget(L,1);
 lua_pushinteger(L,m_apm_exponent(a));
 return 1;
}

static int Lisinteger(lua_State *L)		/** isinteger(x) */
{
 M_APM a=Pget(L,1);
 lua_pushboolean(L,m_apm_is_integer(a));
 return 1;
}

static int Liseven(lua_State *L)		/** iseven(x) */
{
 M_APM a=Pget(L,1);
 lua_pushboolean(L,m_apm_is_integer(a) && m_apm_is_even(a));
 return 1;
}

static int Lisodd(lua_State *L)			/** isodd(x) */
{
 M_APM a=Pget(L,1);
 lua_pushboolean(L,m_apm_is_integer(a) && m_apm_is_odd(a));
 return 1;
}

static int Ldigitsin(lua_State *L)		/** digitsin(x) */
{
 M_APM a=Pget(L,1);
 lua_pushinteger(L,m_apm_significant_digits(a));
 return 1;
}

static int Lgc(lua_State *L)
{
 M_APM a=Pget(L,1);
 m_apm_free(a);
 lua_pushnil(L);
 lua_setmetatable(L,1);
 return 0;
}

static const luaL_Reg R[] =
{
	{ "__add",	Ladd	},		/** __add(x,y) */
	{ "__div",	Ldiv	},		/** __div(x,y) */
	{ "__eq",	Leq	},		/** __eq(x,y) */
	{ "__gc",	Lgc	},
	{ "__le",	Lle	},		/** __le(x,y) */
	{ "__lt",	Llt	},		/** __lt(x,y) */
	{ "__mod",	Lmod	},		/** __mod(x,y) */
	{ "__mul",	Lmul	},		/** __mul(x,y) */
	{ "__pow",	Lpow	},		/** __pow(x,y) */
	{ "__sub",	Lsub	},		/** __sub(x,y) */
	{ "__tostring",	Ltostring},		/** __tostring(x) */
	{ "__unm",	Lneg	},		/** __unm(x) */
	{ "abs",	Labs	},
	{ "acos",	Lacos	},
	{ "acosh",	Lacosh	},
	{ "add",	Ladd	},
	{ "asin",	Lasin	},
	{ "asinh",	Lasinh	},
	{ "atan",	Latan	},
	{ "atan2",	Latan2	},
	{ "atanh",	Latanh	},
	{ "cbrt",	Lcbrt	},
	{ "ceil",	Lceil	},
	{ "compare",	Lcompare},
	{ "cos",	Lcos	},
	{ "cosh",	Lcosh	},
	{ "digits",	Ldigits	},
	{ "digitsin",	Ldigitsin},
	{ "div",	Ldiv	},
	{ "exp",	Lexp	},
	{ "exponent",	Lexponent},
	{ "factorial",	Lfactorial},
	{ "floor",	Lfloor	},
	{ "gcd",	Lgcd	},
	{ "idiv",	Lidiv	},
	{ "inv",	Linv	},
	{ "ipow",	Lipow	},
	{ "iseven",	Liseven	},
	{ "isinteger",	Lisinteger},
	{ "isodd",	Lisodd	},
	{ "lcm",	Llcm	},
	{ "log",	Llog	},
	{ "log10",	Llog10	},
	{ "mod",	Lmod	},
	{ "mul",	Lmul	},
	{ "neg",	Lneg	},
	{ "new",	Lnew	},
	{ "pow",	Lpow	},
	{ "quotrem",	Lquotrem},
	{ "round",	Lround	},
	{ "sign",	Lsign	},
	{ "sin",	Lsin	},
	{ "sincos",	Lsincos	},
	{ "sinh",	Lsinh	},
	{ "sqrt",	Lsqrt	},
	{ "sub",	Lsub	},
	{ "tan",	Ltan	},
	{ "tanh",	Ltanh	},
	{ "tofixed",	Ltofixed},
	{ "tonumber",	Ltonumber},
	{ "tostring",	Ltostring},
	{ NULL,		NULL	}
};

LUALIB_API int luaopen_mapm(lua_State *L)
{
 luaL_newmetatable(L,MYTYPE);
 luaL_setfuncs(L,R,0);
 lua_pushliteral(L,"version");			/** version */
 lua_pushliteral(L,MYVERSION);
 lua_settable(L,-3);
 lua_pushliteral(L,"__index");
 lua_pushvalue(L,-2);
 lua_settable(L,-3);
 return 1;
}
