#include "FooBar.h"

using namespace Marbles::Reflection;

BOOST_AUTO_TEST_SUITE( reflection_object )

BOOST_AUTO_TEST_CASE( reflection_object_field )
{
	BOOST_MESSAGE( "reflection.object.field" );

	Foo foo;
	float half = 0.5f;
	double tiny = std::numeric_limits<double>::min();
	int negative = -1; 
	bool question = false; 
	unsigned long long huge = std::numeric_limits<unsigned long long>::max(); 

	Object foo_obj(foo);
	Object half_obj(half);
	Object tiny_obj(tiny);
	Object negative_obj(negative);
	Object question_obj(question);
	Object huge_obj(huge);

	BOOST_CHECK_EQUAL(half_obj.As<float>(), half);
	BOOST_CHECK_EQUAL(tiny_obj.As<double>(), tiny);
	BOOST_CHECK_EQUAL(negative_obj.As<int>(), negative);
	BOOST_CHECK_EQUAL(question_obj.As<bool>(), question);
	BOOST_CHECK_EQUAL(huge_obj.As<unsigned long long>(), huge);

	half_obj = 0.25f;
	BOOST_CHECK_EQUAL(half, 0.25f);
	tiny_obj = 0.0000000001;
	BOOST_CHECK_EQUAL(tiny, 0.0000000001);
	negative_obj = -25;
	BOOST_CHECK_EQUAL(negative, -25);
	question_obj = 0 > negative;
	BOOST_CHECK_EQUAL(question, true);
	huge_obj = 0ull;
	BOOST_CHECK_EQUAL(huge, 0);


	BOOST_CHECK_EQUAL(foo_obj.At("X").As<int>(), foo.x);
	BOOST_CHECK_EQUAL(foo_obj.At("Y").As<float>(), foo.y);
	BOOST_CHECK_EQUAL(foo_obj.At("Z").As<Marbles::uint64_t>(), foo.z);

	Object bar_obj = foo_obj.At("Bar");
	bar_obj.At("reference_foo") = &foo;
	BOOST_CHECK_EQUAL(foo.bar.reference_foo, &foo);
	Object new_foo_obj = TypeOf<Foo>()->Create();
	bar_obj.At("shared_foo") = new_foo_obj;
	BOOST_CHECK_EQUAL(new_foo_obj.As<Foo*>(), bar_obj.At("shared_foo").As<Foo*>());
	bar_obj.At("weak_foo") = bar_obj.At("shared_foo");
	BOOST_CHECK_EQUAL(new_foo_obj.As<Foo*>(), bar_obj.At("weak_foo").As<Foo*>());

	

}

BOOST_AUTO_TEST_SUITE_END()
