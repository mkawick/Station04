#pragma once

typedef size_t hash_t;

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

	static inline hash_t sdbm(const unsigned char* data, size_t size)
	{
		const unsigned char* end = data + size;
        hash_t hash = 0;
        int c;

        while (data != end && (c = *data++))
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

	static inline hash_t djb2(const unsigned char *data, size_t size)
    {
		const unsigned char* end = data + size;
        hash_t hash = 5381;

        while (data != end)
		{
			int c = *data;
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
            ++data;
		}

        return hash;
    }
};

