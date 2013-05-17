#include "FooBar.h"
#include <Serialization/Serializer.h>

BOOST_AUTO_TEST_SUITE( serialization )

BOOST_AUTO_TEST_CASE( text_serialization )
{
	BOOST_MESSAGE( "Serialization.format.text" );
	Foo foo;
	foo.x = 3;
	foo.y = 2;
	foo.z = 1;

	// Serialize same data should yield same result
	std::stringstream ss1;
	std::stringstream ss2;
	BOOST_CHECK(Marbles::Serialization::Serializer::Text(ss1, foo));
	BOOST_CHECK(Marbles::Serialization::Serializer::Text(ss2, foo));
	BOOST_CHECK_EQUAL(ss1.str(), ss2.str());
	
	// Serialize different should yield different result
	foo.x = 1;
	std::stringstream ss3;
	BOOST_CHECK(Marbles::Serialization::Serializer::Text(ss3, foo));
	BOOST_CHECK_NE(ss1.str(), ss3.str());

	// Reading outputed data should yield same result
	Foo* foo_ref = NULL;
	Foo foo2;
	foo2.bar.reference2_foo = &foo_ref;
	foo2.bar.reference_foo = &foo;
	foo2.bar.reference_zero = NULL;
	foo2.bar.shared_foo = std::make_shared<Foo>();
	foo2.bar.weak_foo = foo2.bar.shared_foo;
	BOOST_CHECK(Marbles::Serialization::Serializer::From(ss1, foo2));
	BOOST_CHECK(Marbles::Serialization::Serializer::From(ss2, foo_ref));
	BOOST_CHECK_NE(static_cast<Foo*>(NULL), foo_ref);
	if (NULL != foo_ref)
	{
		BOOST_CHECK_EQUAL(foo_ref->x, foo2.x);
		BOOST_CHECK_EQUAL(foo_ref->y, foo2.y);
		BOOST_CHECK_EQUAL(foo_ref->z, foo2.z);
		delete foo_ref;
	}
}

BOOST_AUTO_TEST_SUITE_END()
