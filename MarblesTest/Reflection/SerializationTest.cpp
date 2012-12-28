#include <Reflection/Reflection.h>
#include <sstream>

struct Foo
{
	int x;
	int y;
	int z;
};

REFLECT_TYPE(Foo)
	REFLECT_MEMBER("X", x, "")
	REFLECT_MEMBER("Y", y, "")
	REFLECT_MEMBER("Z", z, "")
REFLECT_END()

BOOST_AUTO_TEST_SUITE( serialization )

BOOST_AUTO_TEST_CASE( text_serialization )
{
	BOOST_MESSAGE( "Serialization.reflection_text" );
	Foo foo;
	foo.x = 3;
	foo.y = 2;
	foo.z = 1;

	// Serialize same data should yield same result
	std::stringstream ss1;
	std::stringstream ss2;
	BOOST_CHECK(Marbles::Reflection::Serialize::Text(ss1, foo));
	BOOST_CHECK(Marbles::Reflection::Serialize::Text(ss2, foo));
	BOOST_CHECK_EQUAL(ss1.str(), ss2.str());
	
	// Serialize different should yield different result
	foo.x = 1;
	ss1.clear();
	BOOST_CHECK(Marbles::Reflection::Serialize::Text(ss1, foo));
	BOOST_CHECK_NE(ss1.str(), ss2.str());

	// Reading outputed data should yield same result
	Foo foo2;
	BOOST_CHECK(Marbles::Reflection::Serialize::From(ss1, foo2));
	BOOST_CHECK_EQUAL(foo.x, foo2.x);
	BOOST_CHECK_EQUAL(foo.y, foo2.y);
	BOOST_CHECK_EQUAL(foo.z, foo2.z);
}

BOOST_AUTO_TEST_SUITE_END()
