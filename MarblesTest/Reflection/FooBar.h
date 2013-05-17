#include <Reflection.h>
#include <memory>
#include <sstream>

struct Foo;
struct Bar
{
	Foo*					reference_zero;
	Foo*					reference_foo;
	Foo**					reference2_foo;
	// std::unique_ptr<Foo>	unique_foo;
	std::shared_ptr<Foo>	shared_foo;
	std::weak_ptr<Foo>		weak_foo;

private:
	friend struct Foo;
	Bar(Foo* foo)
	: reference_zero(NULL)
	, reference_foo(NULL) 
	, reference2_foo(NULL)
	{}
};

struct Foo
{
	Foo() : bar(0) {}
	Bar					bar;
	int					x;
	float				y;
	Marbles::uint64_t	z;

	float				GetX() const				{ return static_cast<float>(x); }
	void				SetX(const float val)		{ x = static_cast<int>(val); }
	unsigned			GetY() const				{ return static_cast<unsigned>(y); }
	void				SetY(const unsigned val)	{ y = static_cast<float>(val); }
	std::string			GetZ() const				{ std::stringstream ss; ss << z; return ss.str(); }
	void				SetZ(const std::string val)	{ std::stringstream ss(val); ss >> z; }

	Foo*				GetFooBarFoo() const		{ return bar.reference_foo; }
};

REFLECT_TYPE(Foo,	
	REFLECT_CREATOR()
	REFLECT_MEMBER("Bar", &Foo::bar, "")
	REFLECT_MEMBER("X", &Foo::x, "")
	REFLECT_MEMBER("Y", &Foo::y, "")
	REFLECT_MEMBER("Z", &Foo::z, "")
	)

REFLECT_TYPE(Bar,
	REFLECT_MEMBER("reference_zero", &Bar::reference_zero, "")
	REFLECT_MEMBER("reference_foo", &Bar::reference_foo, "")
	// REFLECT_MEMBER("reference2_foo", &Bar::reference2_foo, "")
	REFLECT_MEMBER("shared_foo", &Bar::shared_foo, "")
	REFLECT_MEMBER("weak_foo", &Bar::weak_foo, "")
	)
