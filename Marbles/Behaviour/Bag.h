#pragma once 

#include <common/hash.h>
#include <common/common.h>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

class Bag
{
public:
	class Builder;
	friend class Builder;
	struct Item
	{
		static const int NameLength = 32;
		char		name[NameLength];
		hash_t		hashname;
		size_t		offset;
		void		(*destruct)(void*);
	};
	typedef size_t Handle;
	typedef std::tr1::shared_ptr<Bag> shared_ptr;

	static inline hash_t HashName(const char* name) 
	{
		char lower[Item::NameLength];
		int len = strnlen(name, Item::NameLength);
		std::transform(name, name + len, lower, tolower);
		return Hash::djb2(lower); 
	}

	template<typename T> inline const T* Get(Handle handle) const 
	{ 
		return reinterpret_cast<const T*>(reinterpret_cast<const char*>(this) + handle); 
	}
	template<typename T> inline T* Get(Handle handle) 
	{ 
		return reinterpret_cast<T*>(reinterpret_cast<char*>(this) + handle); 
	}

	template<typename T> inline T* GetAt(int index)
	{
		return Get<T>(mItems[index].offset);
	}
	template<typename T> inline const T* GetAt(int index) const
	{
		return Get<T>(mItems[index].offset);
	}
	
	~Bag()
	{
		Item* start = &mItems[0];
		Item* end = Get<Item>(start->offset);
		for(Item* i = &mItems[0]; i != end; ++i)
		{
			(*i->destruct)(Get<void>(i->offset));
		}
	}
private:
	Bag();
	Bag(const Bag& rhs);
	Bag& operator=(const Bag& rhs);

	// [Bag][mItems][mData ... mSize]
	size_t	mData;
	size_t	mSize;
	Item	mItems[1];
};

class Bag::Builder
{
public:
	struct Item : public Bag::Item
	{
		size_t size;
		void (*construct)(void*);
	};
	template<typename T> Bag::Handle GetHandle(const char* name) 
	{
		Bag::Handle handle = Find(name);
		if (0 != handle)
		{
			const Item& last = mItems.back();
			size_t header = 2*sizeof(size_t);
			size_t offset = header + mItems.size() * sizeof(Bag::Item);
			size_t size = offset + last.offset + last.size;

			Item item;
			memset(&item, 0, sizeof(Item));
			strncpy_s(item.name, sizeof(item.name), name, sizeof(item.name) - 1);
			item.hashname = Bag::HashName(item.name);
			item.offset = AlignTo<T>(size);
			item.size = sizeof(T);
			item.construct = Construct<T>;
			item.destruct = Destruct<T>;

			handle = item.offset;
		}
		return handle;
	}

	Bag::shared_ptr Create() const
	{
		const Item& last = mItems.back();
		size_t header = 2*sizeof(size_t);
		size_t offset = header + mItems.size() * sizeof(Bag::Item);
		size_t size = offset + last.offset + last.size;

		Bag::shared_ptr bag(reinterpret_cast<Bag*>(new char[size]));
		bag->mSize = size;
		bag->mData = offset;
		for (size_t i = 0; i < mItems.size(); ++i)
		{
			bag->mItems[i] = mItems[i];
			(*mItems[i].construct)(bag->Get<char*>(bag->mItems[i].offset));
		}
		return bag;
	}

	void Clear() 
	{
		mItems.clear();
	}

private:
	template<typename T> static void Construct(void* p) { new (p) T(); }
	template<typename T> static void Destruct(void* p)	{ (void)p; reinterpret_cast<T*>(p)->~T(); }

	Bag::Handle Find(const char* name) const
	{
		hash_t hashname = Bag::HashName(name);
		std::vector<Item>::size_type i = 0;
		while(i < mItems.size() && hashname != mItems[i].hashname)
		{
			++i;
		}
		return i < mItems.size() ? mItems[i].offset : 0;
	}

	std::vector<Item> mItems;
};
