#include <Common/CircularBuffer.h>

BOOST_AUTO_TEST_SUITE( CircularBuffer )

BOOST_AUTO_TEST_CASE( basic_operations )
{
	BOOST_MESSAGE( "CircularBuffer.basic_operations" );

	const int size = 10;
	Marbles::CircularBuffer<int, size> buffer;

	int pop = 0;
	int push = 0;

	BOOST_CHECK(buffer.empty());

	while (buffer.try_push(push))
	{
		++push;
		BOOST_CHECK_EQUAL(push, buffer.size());
	}

	BOOST_CHECK(buffer.full());
	BOOST_CHECK_EQUAL(push, size);

	while (buffer.try_pop(pop))
	{
		BOOST_CHECK_EQUAL(pop, size - buffer.size() - 1);
	}

	BOOST_CHECK(buffer.empty());

	while (buffer.try_push(push))
	{
		++push;
		if (push % 2)
		{
			BOOST_CHECK_EQUAL(++pop, buffer.pop());
		}
	}

	BOOST_CHECK(buffer.full());

	int dummy;
	while (buffer.try_pop(dummy))
	{
		BOOST_CHECK_EQUAL(++pop, dummy);
	}

	BOOST_CHECK(buffer.empty());
}

namespace 
{
	static const int bufferSize = 10;
	typedef Marbles::CircularBuffer<int, bufferSize> Buffer_t;
	static std::vector< Buffer_t > buffers;
	static Buffer_t accumulation;
}

void marshaller()
{
	int value;
	int index = 0;
	for(int i = buffers.size(); i--; )
	{
		value = buffers[index].pop();
		accumulation.push(value);
	}

	int timeout;
	do 
	{
		timeout = 100*buffers.size();
		do
		{
			index += 1;
			index %= buffers.size();
			--timeout;
		} while(buffers[index].empty() && 0 < timeout);

		if (buffers[index].try_pop(value))
		{
			accumulation.push(value);
		}
	} while(0 <= value);
}

void producer(Buffer_t* buffer, int value, int amount)
{
	for(int i = amount; i--; )
	{
		buffer->push(value);
	}
}

BOOST_AUTO_TEST_CASE( multi_thread_usage )
{
	BOOST_MESSAGE( "CircularBuffer.multi_thread_usage" );
	const int numProducers = 15;
	const int amountProduced = bufferSize * 10;
	typedef std::array<boost::thread, numProducers> ThreadArray;
	typedef std::array<int, numProducers> AmountArray;

	AmountArray amounts;
	ThreadArray threads;

	buffers.resize((numProducers + 1)/2);

	AmountArray::iterator amount = amounts.begin();
	while(amount != amounts.end())
	{
		*amount++ = 0;
	}

	boost::thread marshallerThread(marshaller);
	boost::thread marshaller2Thread(marshaller);
	for(unsigned i = numProducers; i--;)
	{
		boost::thread action(std::bind(producer, &buffers[i%buffers.size()], i, amountProduced));
		threads[i].swap(action);
	}

	int sum = 0;
	do
	{
		int value;
		if (accumulation.try_pop(value))
		{
			++amounts[value];
			++sum;
		}
	} while(sum < numProducers*amountProduced);

	// Push an exit for each marchal thread
	const int exit = -1;
	buffers.front().push(exit);
	buffers.front().push(exit);

	amount = amounts.begin();
	while(amount != amounts.end())
	{
		BOOST_CHECK_EQUAL(*amount++, amountProduced);
	}

	// Clean up
	marshallerThread.join();
	marshaller2Thread.join();

	accumulation.clear();
	buffers.resize(0);
}

BOOST_AUTO_TEST_SUITE_END()