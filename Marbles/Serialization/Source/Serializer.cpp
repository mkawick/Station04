// --------------------------------------------------------------------------------------------------------------------
#include <Serialization/Serializer.h>
#include <Serialization/Reader.h>
#include <Serialization/Writer.h>
#include <Serialization/Format.h>

#include <locale>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Serialization
{
namespace 
{
// --------------------------------------------------------------------------------------------------------------------
const uint32_t EndianSwap(const uint32_t value)
{
	return ((value << 24) & 0xFF000000)
		| ((value << 16) & 0x00FF0000)
		| ((value >> 16) & 0x0000FF00) 
		| ((value >> 24) & 0x000000FF);
}

// --------------------------------------------------------------------------------------------------------------------
static const uint32_t text_big		= 'type';
static const uint32_t text_little	= 'epyt';
static const uint32_t binary_big	= 'rsb0';
static const uint32_t binary_little	= '0bsr';
static const uint32_t memory_big	= 'rsm0';
static const uint32_t memory_little	= '0msr';

// --------------------------------------------------------------------------------------------------------------------
struct TextFormat : public Format
{
	std::ostream& Write(std::ostream& os, const bool& value) const
	{ return os << std::ios::boolalpha << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::uint8_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::uint16_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::uint32_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::uint64_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::int8_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::int16_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::int32_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const Marbles::int64_t& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const float& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const double& value) const
	{ return os << value; } 
	std::ostream& Write(std::ostream& os, const std::string& value) const
	{ return os << "\"" << value << "\"" ; } 

	std::ostream& TypeInfo(std::ostream& os, const Object& obj) const
	{
		ASSERT(obj.IsValid());
		return os << "type<" << obj.TypeInfo()->Name() << ">";
	}
	std::ostream& Label(std::ostream& os, const Object& obj) const
	{
		ASSERT(obj.IsValid());
		return os << obj.MemberInfo()->Name() << '=';
	}
	std::ostream& OpenEnumeration(std::ostream& os) const
	{ return os << " ["; }
	std::ostream& CloseEnumeration(std::ostream& os) const
	{ return os << "]"; }
	std::ostream& OpenMap(std::ostream& os) const
	{ return os << " {"; }
	std::ostream& CloseMap(std::ostream& os) const
	{ return os << "}"; }
	std::ostream& Seperator(std::ostream& os) const
	{ return os << ","; }
	std::ostream& Indent(std::ostream& os) const
	{ return os << "  "; }
	std::ostream& NewLine(std::ostream& os) const
	{ return os << std::endl; }

	// Reader interface
	bool Read(std::istream& is, Object& value) const
	{
		shared_type type = value.TypeInfo();
		std::string readValue;
		std::istream::pos_type pos = is.tellg();
		std::locale prev = is.getloc();

		is.imbue(mReadStop);
		char peek = static_cast<char>(is.peek());
		if ('"' == peek || '\'' == peek)
		{
			// Read open quote
			ReadIf(is, '"') || ReadIf(is, '\'');

			is.imbue(mStringStop);
			is >> value.As<std::string>();
			is.imbue(mReadStop);

			// Read closing quote
			ReadIf(is, '"') || ReadIf(is, '\'');
		}
		else if ('-' == peek || std::isdigit(peek, mReadStop))
		{	// Read a number
			if (*type == *TypeOf<Marbles::uint8_t>())		is >> value.As<Marbles::uint8_t>();
			else if (*type == *TypeOf<Marbles::uint16_t>())	is >> value.As<Marbles::uint16_t>();
			else if (*type == *TypeOf<Marbles::uint32_t>())	is >> value.As<Marbles::uint32_t>();
			else if (*type == *TypeOf<Marbles::uint64_t>())	is >> value.As<Marbles::uint64_t>();
			else if (*type == *TypeOf<Marbles::int8_t>())	is >> value.As<Marbles::int8_t>();
			else if (*type == *TypeOf<Marbles::int16_t>())	is >> value.As<Marbles::int16_t>();
			else if (*type == *TypeOf<Marbles::int32_t>())	is >> value.As<Marbles::int32_t>();
			else if (*type == *TypeOf<Marbles::int64_t>())	is >> value.As<Marbles::int64_t>();
			else if (*type == *TypeOf<float>())				is >> value.As<float>();
			else if (*type == *TypeOf<double>())			is >> value.As<double>();
			else { ASSERT(!"Unknown numeric type!"); }
		}
		else if ('t' == std::tolower(peek, prev) || 'f' == std::tolower(peek, prev))
		{
			std::string boolean;
			is >> boolean;
			value.As<bool>() = "true" == boolean;
		}
		else 
		{	
			ASSERT(!"Unable to read");
		}
		if (pos != is.tellg())
		{
			//hash_t hashName = 0;
			//for (ObjectList::iterator i = mPath.begin(); i != mPath.end(); ++i)
			//{
			//	hashName += i->HashName();
			//}
			//mElements[hashName] = value;
		}
		is.imbue(prev);
		return pos != is.tellg();
	}

	bool TypeInfo(std::istream& is, Object& value) const
	{
		char name[1024];
		shared_type type;
		std::istream::pos_type start = is.tellg();
		std::locale prev = is.getloc();

		// Read formatted type information
		is.imbue(mReadStop);
		//is >> std::ios::uppercase;
		is >> name;
		// TODO(danc): this should be case insensative
		if (0 == std::char_traits<char>::compare("type", name, 4)) 
		{
			is >> name;
			type = Type::Find(name);
		}
		else
		{
			is.seekg(start, std::ios::beg);
			type = value.TypeInfo();
		}
		is.imbue(prev);

		const bool createRequired = !value.IsValid();
		const bool canCreate = type && (!value.IsValid() || type->Implements(value.TypeInfo()));

		if (createRequired && canCreate)
		{
			value = type->Create();
		}
		return !createRequired;
	}

	bool ReadReference(std::istream& is, Object& value)
	{
		std::istream::pos_type start = is.tellg();
		if (value.IsReference())
		{
			ConsumeWhitespace(is);

			std::locale prev = is.getloc();
			is.imbue(mReadStop);

			char path[1024];
			if (std::isdigit(is.peek(), mReadStop))
			{	// NULL value
				is >> path; // consumes 0
				(*value).As<void*>() = 0;
			}
			else if (std::isalpha(is.peek(), mReadStop))
			{	// Look up reference
				is >> path;
				Path hashPath(path);
				

				//mElements.find(hashPath.HashName());

				(*value).As<void*>() = 0;
			}
			else
			{
				const bool createRequired = value.IsReference() && 0 == (*value).As<void*>();
				if (createRequired)
				{
					value = value.TypeInfo()->Create();
				}
			}
			is.imbue(prev);
		}
		return start != is.tellg();
	}

	std::string Label(std::istream& is) const
	{
		std::istream::pos_type start = is.tellg();
		std::locale prev = is.getloc();
		std::string label;
		is.imbue(mReadStop);
		is >> label;
		is.imbue(prev);

		return label;
	}

	bool OpenEnumeration(std::istream& is) const
	{
		return ReadIf(is, '[');
	}

	bool CloseEnumeration(std::istream& is) const
	{
		return ReadIf(is, ']');
	}

	bool OpenMap(std::istream& is) const
	{
		return ReadIf(is, '{');
	}

	bool CloseMap(std::istream& is) const
	{
		return ReadIf(is, '}');
	}
	
	bool Consume(std::istream& is) const
	{
		std::istream::pos_type start = is.tellg();
		std::locale prev = is.getloc();
		std::string consume;
		int count = 0; 

		is.imbue(mReadStop);
		do 
		{
			if (ReadIf(is, '[') || ReadIf(is, '{'))
			{
				++count;
			}
			else if (ReadIf(is, ']') || ReadIf(is, '}'))
			{
				--count;
			}
			else
			{
				is >> consume;
			}
		} while (0 != count);

		is.imbue(prev);
		return true;
	}

	bool Seperator(std::istream& is) const
	{
		return ReadIf(is, ',');
	}

	TextFormat(Object root, const bool endianSwap) 
	: Format(root, endianSwap)
	, mReadStop(std::locale(), new read_stop())
	{}

private:
	bool ReadIf(std::istream& is, char character) const
	{
		ConsumeWhitespace(is);
		const bool isCharacter = character == static_cast<char>(is.peek());
		if (isCharacter)
		{
			char value;
			is.read(&value, sizeof(value));
		}
		return isCharacter;
	}

	void ConsumeWhitespace(std::istream& is) const
	{
		char peek = static_cast<char>(is.peek());
		while (std::isspace(peek, mReadStop))
		{
			is.read(&peek, sizeof(peek));
			peek = static_cast<char>(is.peek());
		}
	}

	struct read_stop : public std::ctype<char>
	{
	public:
		read_stop()
		: std::ctype<char>(generate(), false, sizeof(_table))
		{}

	private:
		virtual ~read_stop()
		{}

		std::ctype_base::mask _table[256];
		const std::ctype_base::mask *generate()
		{
			const std::ctype<char>& stdFacet = std::use_facet<std::ctype<char>>(std::locale());
			const int tableSize = sizeof(_table) / sizeof(_table[0]);
			char keys[tableSize];
			for (int ch = 0; ch < tableSize; ++ch)
				keys[ch] = static_cast<char>(ch);
			stdFacet.is(&keys[0], &keys[0] + tableSize, &_table[0]);

			_table['\n'] = _table['\r'] = _table['\t'] = _table[' '] = std::ctype<char>::space;
			_table['<'] = _table['>'] = _table[':'] = _table['='] = std::ctype<char>::space;
			_table[','] = std::ctype<char>::space;
			// _table['['] = _table[']'] = _table['{'] = _table['}'] = std::ctype<char>::space;
			return &_table[0];
		}
	};
	struct string_stop : public std::ctype<char>
	{
	public:
		string_stop()
		: std::ctype<char>(generate(), false, sizeof(_table))
		{}

	private:
		virtual ~string_stop()
		{}

		std::ctype_base::mask _table[256];
		const std::ctype_base::mask *generate()
		{
			const int tableSize = sizeof(_table) / sizeof(_table[0]);
			for (int ch = 0; ch < tableSize; ++ch)
				_table[ch] = std::ctype<char>::punct;

			_table['\''] = _table['"'] = std::ctype<char>::space;
			return &_table[0];
		}
	};
	typedef std::map<hash_t, Object> ObjectMap;
	std::locale mReadStop;
	std::locale mStringStop;
	Object mRoot;
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace <>

// --------------------------------------------------------------------------------------------------------------------
bool Serializer::Text(std::ostream& os, const Object& root, const Object& sub)
{	// start serialization
	Writer<TextFormat> writer(root);
	writer.Include(sub);
	return writer.Write(os);
}

// --------------------------------------------------------------------------------------------------------------------
bool Serializer::From(std::istream& is, Object& root)
{
	uint32_t header;
	bool littleEndian = false;
	std::ios::pos_type pos = is.tellg();
	is.read(reinterpret_cast<char*>(&header), sizeof(header));

	switch(header)
	{
	case 'epyt': 
		littleEndian = true;
	case 'type':
		{	// text format
			Reader<TextFormat> reader(littleEndian);
			is.seekg(0, std::ios::beg);
			reader.Read(is, root);
		}
		break;
	}

	return pos != is.tellg();
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Serialization
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
