// Format.h
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Serialization
{

// --------------------------------------------------------------------------------------------------------------------
class Format
{
	bool mEndianSwap;
	Reflection::Object mRoot;
public:
	Format(Object root, bool endianSwap) : mEndianSwap(endianSwap), mRoot(root) {}

	bool					EndianSwap() const { return mEndianSwap; }

	// Put put interface
	virtual std::ostream&	Write(std::ostream& os, const bool& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::uint8_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::uint16_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::uint32_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::uint64_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::int8_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::int16_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::int32_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const Marbles::int64_t& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const std::string& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const double& value) const = 0;
	virtual std::ostream&	Write(std::ostream& os, const float& value) const = 0;

	virtual std::ostream&	TypeInfo(std::ostream& os, const Object& root) const = 0;
	virtual std::ostream&	Label(std::ostream& os, const Object& root) const = 0;
	virtual std::ostream&	OpenEnumeration(std::ostream& os) const = 0;
	virtual std::ostream&	CloseEnumeration(std::ostream& os) const = 0;
	virtual std::ostream&	OpenMap(std::ostream& os) const = 0;
	virtual std::ostream&	CloseMap(std::ostream& os) const = 0;

	virtual std::ostream&	Seperator(std::ostream& os) const = 0;
	virtual std::ostream&	Indent(std::ostream& os) const = 0;
	virtual std::ostream&	NewLine(std::ostream& os) const = 0;

	// Read Interface
	virtual bool			Read(std::istream& is, Object& value) const = 0;

	virtual bool			TypeInfo(std::istream& is, Object& value) const = 0;
	virtual std::string		Label(std::istream& is) const = 0;
	virtual bool			OpenEnumeration(std::istream& is) const = 0;
	virtual bool			CloseEnumeration(std::istream& is) const = 0;
	virtual bool			OpenMap(std::istream& is) const = 0;
	virtual bool			CloseMap(std::istream& is) const = 0;

	virtual bool			Consume(std::istream& is) const = 0;
	virtual bool			Seperator(std::istream& is) const = 0;
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Serialization
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
