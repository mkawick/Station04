#pragma once

// --------------------------------------------------------------------------------------------------------------------
#define FN_COMMA				,
#define FN_TOSTRING2(a)			(#a)
#define FN_TOSTRING(a)			FN_TOSTRING2(a)
#define FN_JOIN2(a,b)			a##b
#define FN_JOIN(a,b)			FN_JOIN2(a,b)
#define FN_EXPAND(beg,mid,end)  FN_JOIN(FN_JOIN(beg,mid),end)

// --------------------------------------------------------------------------------------------------------------------
#define FN_LIST_0(r0,t0,sp)							
#define FN_LIST_1(r0,t0,sp)								FN_EXPAND(r0,0,t0)
#define FN_LIST_2(r0,t0,sp)		FN_LIST_1(r0,t0,sp)	sp	FN_EXPAND(r0,1,t0)
#define FN_LIST_3(r0,t0,sp)		FN_LIST_2(r0,t0,sp)	sp	FN_EXPAND(r0,2,t0)
#define FN_LIST_4(r0,t0,sp)		FN_LIST_3(r0,t0,sp)	sp	FN_EXPAND(r0,3,t0)
#define FN_LIST_5(r0,t0,sp)		FN_LIST_4(r0,t0,sp)	sp	FN_EXPAND(r0,4,t0)
#define FN_LIST_6(r0,t0,sp)		FN_LIST_5(r0,t0,sp)	sp	FN_EXPAND(r0,5,t0)
#define FN_LIST_7(r0,t0,sp)		FN_LIST_6(r0,t0,sp)	sp	FN_EXPAND(r0,6,t0)
#define FN_LIST_8(r0,t0,sp)		FN_LIST_7(r0,t0,sp)	sp	FN_EXPAND(r0,7,t0)
#define FN_LIST_9(r0,t0,sp)		FN_LIST_8(r0,t0,sp)	sp	FN_EXPAND(r0,8,t0)
#define FN_LIST_10(r0,t0,sp)	FN_LIST_9(r0,t0,sp)	sp	FN_EXPAND(r0,9,t0)
#define FN_LIST_11(r0,t0,sp)	FN_LIST_10(r0,t0,sp)sp	FN_EXPAND(r0,10,t0)
#define FN_LIST_12(r0,t0,sp)	FN_LIST_11(r0,t0,sp)sp	FN_EXPAND(r0,11,t0)
#define FN_LIST(N,r0,t0,sp)		FN_JOIN(FN_LIST_,N)(r0,t0,sp)

// --------------------------------------------------------------------------------------------------------------------
#define FN_LIST_PAIR_0(r0,t0,r1,t1,sp)												
#define FN_LIST_PAIR_1(r0,t0,r1,t1,sp)										FN_EXPAND(r0,0,t0)	FN_EXPAND(r1,0,t1)
#define FN_LIST_PAIR_2(r0,t0,r1,t1,sp)	FN_LIST_PAIR_1(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,1,t0)	FN_EXPAND(r1,1,t1)
#define FN_LIST_PAIR_3(r0,t0,r1,t1,sp)	FN_LIST_PAIR_2(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,2,t0)	FN_EXPAND(r1,2,t1)
#define FN_LIST_PAIR_4(r0,t0,r1,t1,sp)	FN_LIST_PAIR_3(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,3,t0)	FN_EXPAND(r1,3,t1)
#define FN_LIST_PAIR_5(r0,t0,r1,t1,sp)	FN_LIST_PAIR_4(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,4,t0)	FN_EXPAND(r1,4,t1)
#define FN_LIST_PAIR_6(r0,t0,r1,t1,sp)	FN_LIST_PAIR_5(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,5,t0)	FN_EXPAND(r1,5,t1)
#define FN_LIST_PAIR_7(r0,t0,r1,t1,sp)	FN_LIST_PAIR_6(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,6,t0)	FN_EXPAND(r1,6,t1)
#define FN_LIST_PAIR_8(r0,t0,r1,t1,sp)	FN_LIST_PAIR_7(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,7,t0)	FN_EXPAND(r1,7,t1)
#define FN_LIST_PAIR_9(r0,t0,r1,t1,sp)	FN_LIST_PAIR_8(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,8,t0)	FN_EXPAND(r1,8,t1)
#define FN_LIST_PAIR_10(r0,t0,r1,t1,sp)	FN_LIST_PAIR_9(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,9,t0)	FN_EXPAND(r1,9,t1)
#define FN_LIST_PAIR_11(r0,t0,r1,t1,sp)	FN_LIST_PAIR_10(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,10,t0)	FN_EXPAND(r1,10,t1)
#define FN_LIST_PAIR_12(r0,t0,r1,t1,sp)	FN_LIST_PAIR_11(r0,t0,r1,t1,sp)	sp	FN_EXPAND(r0,11,t0)	FN_EXPAND(r1,11,t1)
#define FN_LIST_PAIR(N,r0,t0,r1,t1,sp)	FN_JOIN(FN_LIST_PAIR_,N)(r0,t0,r1,t1,sp)

// --------------------------------------------------------------------------------------------------------------------
#define FN_TYPENAME(N)		FN_LIST(N,typename A,,FN_COMMA) // typename A0,typename A1,typename A2,...
#define FN_TYPES(N)			FN_LIST(N,A,,FN_COMMA)			// A0,A1,A2,...
#define FN_DECLARE(N)		FN_LIST_PAIR(N,A, ,a,;,)		// A0 a0;A1 a1;A2 a2;...
#define FN_PARAMETER(N)		FN_LIST_PAIR(N,A, ,a,,FN_COMMA)	// A0 a0,A1 a1,A2 a2,...
#define FN_REF_DECLARE(N)	FN_LIST_PAIR(N,A,& ,a,;,)		// A0& a0;A1& a1;A2& a2;...
#define FN_REF_PARAMETER(N)	FN_LIST_PAIR(N,A,& ,a,,FN_COMMA)// A0& a0,A1& a1,A2& a2,...
#define FN_ARGUMENTS(N)		FN_LIST(N,a,,FN_COMMA)			// a0,a1,a2,...
#define FN_TYPEDEF(N)		FN_LIST_PAIR(N,typedef A,, arg,_type;,)	// typedef A0 arg0_type;typedef A1 arg1_type;...

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
// --------------------------------------------------------------------------------------------------------------------
template<typename sig>
struct function_traits;

template<typename R>
struct function_traits<R ()>
{
	static const size_t count = 0;
	typedef R return_type;
};

// --------------------------------------------------------------------------------------------------------------------
#define FN_SIG_TRAITS(N) \
	template<typename R, FN_TYPENAME(N)> \
	struct function_traits<R (FN_TYPES(N))> \
	{ \
		static const size_t count = N; \
		typedef R return_type; \
		FN_TYPEDEF(N) \
	};

FN_SIG_TRAITS(1)
FN_SIG_TRAITS(2)
FN_SIG_TRAITS(3)
FN_SIG_TRAITS(4)
FN_SIG_TRAITS(5)
FN_SIG_TRAITS(6)
FN_SIG_TRAITS(7)
FN_SIG_TRAITS(8)
FN_SIG_TRAITS(9)
FN_SIG_TRAITS(10)
FN_SIG_TRAITS(11)
FN_SIG_TRAITS(12)

#undef FN_SIG_TRAITS

// --------------------------------------------------------------------------------------------------------------------
#define FN_MEMBER_TRAITS(N) \
	template<typename R, typename C, FN_TYPENAME(N)> \
	struct function_traits<R (C::*)(FN_TYPES(N))> \
	{ \
		static const size_t count = N; \
		typedef C member_type; \
		typedef R return_type; \
		FN_TYPEDEF(N) \
	};

template<typename R, typename C>
struct function_traits<R (C::*)()>
{
	static const size_t count = 0;
	typedef C member_type;
	typedef R return_type;
};

FN_MEMBER_TRAITS(1)
FN_MEMBER_TRAITS(2)
FN_MEMBER_TRAITS(3)
FN_MEMBER_TRAITS(4)
FN_MEMBER_TRAITS(5)
FN_MEMBER_TRAITS(6)
FN_MEMBER_TRAITS(7)
FN_MEMBER_TRAITS(8)
FN_MEMBER_TRAITS(9)
FN_MEMBER_TRAITS(10)
FN_MEMBER_TRAITS(11)
FN_MEMBER_TRAITS(12)

#undef FN_MEMBER_TRAITS

// --------------------------------------------------------------------------------------------------------------------
#define FN_TRAITS(N) \
	template<typename R, FN_TYPENAME(N)> \
	struct function_traits<R (*)(FN_TYPES(N))> \
	{ \
		static const size_t count = N; \
		typedef R return_type; \
		FN_TYPEDEF(N) \
	};

template<typename R>
struct function_traits<R (*)()>
{
	static const size_t count = 0;
	typedef R return_type;
};

FN_TRAITS(1)
FN_TRAITS(2)
FN_TRAITS(3)
FN_TRAITS(4)
FN_TRAITS(5)
FN_TRAITS(6)
FN_TRAITS(7)
FN_TRAITS(8)
FN_TRAITS(9)
FN_TRAITS(10)
FN_TRAITS(11)
FN_TRAITS(12)

#undef FN_TRAITS

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of File --------------------------------------------------------------------------------------------------------
