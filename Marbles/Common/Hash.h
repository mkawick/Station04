// --------------------------------------------------------------------------------------------------------------------

#pragma once
#pragma warning(disable:4706)
// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
typedef size_t hash_t;

// --------------------------------------------------------------------------------------------------------------------
class Hash
{
public:
	// http://www.cse.yorku.ca/~oz/hash.html
	static inline hash_t sdbm(const char* string)
	{
        hash_t hash = 0;
        int c;

        while (c = *string++)
		{
            hash = c + (hash << 6) + (hash << 16) - hash;
		}

        return hash;
	}

	static inline hash_t sdbm(const void* data, size_t size)
	{
		const unsigned char* pos = static_cast<const unsigned char*>(data);
		const unsigned char* end = pos + size;
        hash_t hash = 0;
        int c;

        while (pos != end && (c = *pos++))
		{
            hash = c + (hash << 6) + (hash << 16) - hash;
		}

        return hash;
	}

	// http://www.cse.yorku.ca/~oz/hash.html
    static inline hash_t djb2(const char *string)
    {
        hash_t hash = 5381;
        int c;

        while (c = *string++)
		{
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		}

        return hash;
    }

	static inline hash_t djb2(const void *data, size_t size)
    {
		const unsigned char* pos = static_cast<const unsigned char*>(data);
		const unsigned char* end = pos + size;
        hash_t hash = 5381;

        while (pos != end)
		{
			int c = *pos++;
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		}

        return hash;
    }
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

#pragma warning(default:4706)

// End of file --------------------------------------------------------------------------------------------------------
