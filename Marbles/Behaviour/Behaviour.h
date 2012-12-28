#pragma once

#include <vector>
#include <memory>
#include <Behaviour/bag.h>

namespace Marbles
{

class Behaviour;
typedef std::shared_ptr<Behaviour> shared_behaviour;
typedef std::weak_ptr<Behaviour> weak_behaviour;

class Behaviour
{
public:
	class Tree;
	class List;
	class Select;
	template<typename > class Stack;
	typedef std::shared_ptr<const Tree> shared_tree;
	typedef std::weak_ptr<const Tree> weak_tree;

	static const int MaxDepth = 16;

	Behaviour() { mName[0] = '\0'; }
	virtual ~Behaviour() {}

	virtual void Register(Bag::Builder& builder) = 0;
	virtual void Initialize(Bag& ) const {}
	
	virtual bool CanStart(const Bag& ) const = 0;
			void Start(Bag& bag) const { OnStart(bag); }
			bool Update(Bag& bag) const { return OnUpdate(bag); }
			void Interrupt(Bag& bag) const { OnInterrupt(bag); }
			void Done(Bag& bag) const { OnDone(bag); }

	const char* Name() const { return mName; }
	const char* Name(const char* name)
	{
		const int len = strnlen(name, NameLength - 1);
		strcpy_s(mName, len, name);
		return mName;
	}
	shared_tree GetTree() const
	{
		return mTree.lock();
	}
protected:
	static const int NameLength = 32;

private:
	virtual void OnStart(Bag& ) const { }
	virtual bool OnUpdate(Bag& ) const = 0;
	virtual void OnInterrupt(Bag& ) const { }
	virtual void OnDone(Bag& ) const { }

	char mName[NameLength];
	weak_tree mTree;
};

class Behaviour::Tree : public Behaviour
{
protected:
	typedef std::vector< shared_behaviour > list; // should be unordered_set

public:
	typedef std::shared_ptr<const Tree> shared_const_tree;
	Tree()
	{
		mBagBuilder.Clear();
		mBagBuilder.GetHandle<Behaviour::shared_tree>("Tree");
	}

	virtual void Register(Bag::Builder& builder)
	{ 
		mTreeBag = builder.GetHandle<Bag::shared_ptr>(Name());
	}

	virtual void Initialize(Bag& bag) const 
	{
		Bag::shared_ptr treeBag = *bag.Get<Bag::shared_ptr>(mTreeBag);
		treeBag = mBagBuilder.Create();
		treeBag->GetAt<Behaviour::shared_tree>(0)->reset(this);

		for (list::const_iterator i = mBehaviours.begin(); i != mBehaviours.end(); ++i)
		{
			(*i)->Initialize(*treeBag);
		}
	}
	
	virtual bool CanStart(const Bag& bag) const
	{
		return Root()->CanStart(bag);
	}

	// Collection interface
	void Clear()
	{
		mBehaviours.clear();
		mRootIndex = -1;
	}

	void Add(shared_behaviour& behaviour)
	{
		mBehaviours.push_back(behaviour);
		behaviour->Register(mBagBuilder);
		mRootIndex = 0 < mRootIndex ? mRootIndex : 0; // Max(0, mRootIndex);
	}

	const Behaviour* operator[](const size_t i) const
	{ 
		return mBehaviours[i].get();
	}
	const Behaviour* At(const size_t i) const
	{ 
		return mBehaviours[i].get();
	}

	int Find(const char* name) const
	{
		list::size_type i = 0;
		size_t len = strnlen(name, 64);
		while(i < mBehaviours.size() && 0 != _strnicmp(mBehaviours[i]->Name(), name, len))
		{
			++i;
		}
		return i < mBehaviours.size() ? static_cast<int>(i) : -1;
	}
protected:
	virtual void OnStart(Bag& bag) const 
	{ 
		Root()->Start(bag); 
	}
	virtual bool OnUpdate(Bag& bag) const
	{ 
		const bool result = Root()->Update(bag); 
		return result;
	}
	virtual void OnInterrupt(Bag& bag) const
	{ 
		Root()->Interrupt(bag); 
	}
	virtual void OnDone(Bag& bag) const
	{ 
		Root()->Done(bag); 
	}

	Behaviour* Root() { return mBehaviours[mRootIndex].get(); }
	const Behaviour* Root() const { return mBehaviours[mRootIndex].get(); }

private:
	Bag::Builder mBagBuilder;
	list mBehaviours;
	int mRootIndex;
	int mTreeBag;
};

template<typename T>
class Behaviour::Stack
{
public:
	typedef T value_type;
	static const int MaxDepth = 16;

	Stack() : depth(0) { memset( stack, 0, sizeof(stack)); }
	inline value_type& At(int index) { /*ASSERT(0 <= index && index < MaxDepth);*/ return stack[index]; }
	inline const value_type& At(int index) const { /*ASSERT(0 <= index && index < MaxDepth);*/ return stack[index]; }
	inline value_type& Pop() { /*ASSERT(depth + 1 < MaxDepth);*/ return stack[depth++]; }
	inline const value_type& Pop() const { /*ASSERT(depth + 1 < MaxDepth);*/ return stack[depth++]; }
	inline void Push(const value_type& value) { /*ASSERT(0 < depth);*/ stack[--depth] = value; }
	inline void Push() const { /*ASSERT(0 < depth);*/ --depth; }
	inline void Reset() { depth = 0; }
	
	inline value_type& operator[](int index) { return At(index); }
	inline const value_type& operator[](int index) const { return At(index); }
private:
	value_type stack[MaxDepth];
	mutable int depth;
};

class Behaviour::List : public Behaviour
{
protected:
	typedef std::vector< int > list;
	typedef list::iterator iterator;
	typedef list::const_iterator const_iterator;
	typedef Behaviour::Stack< list > stack_type;

public:
	virtual void Register(Bag::Builder& builder)
	{ 
		mStackHandle = builder.GetHandle<stack_type>("ListStack");
	}
	virtual void Initialize(Bag& bag) const
	{ 
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		stack->Pop().reserve(mBehaviours.size());
		stack->Push();
	}
	virtual bool CanStart(const Bag& bag) const 
	{
		bool result = false;
		shared_tree tree = GetTree();
		for(const_iterator i = mBehaviours.begin(); !result && i != mBehaviours.end(); ++i)
		{
			result |= (*tree)[*i]->CanStart(bag);
		}
		return result;
	}

	// Collection interface
	void Clear()
	{
		mBehaviours.clear();
	}

	void Add(const char* behaviourName)
	{
		Behaviour::shared_tree tree = GetTree();
		int behaviour = tree->Find(behaviourName);
		mBehaviours.push_back(behaviour);
	}

protected:
	virtual void OnStart(Bag& bag) const
	{
		Behaviour::shared_tree tree = GetTree();
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		list& activeList = stack->Pop();

		for(const_iterator i = mBehaviours.begin(); i != mBehaviours.end(); ++i)
		{
			const Behaviour* behaviour = (*tree)[*i];
			if (behaviour->CanStart(bag))
			{
				behaviour->Start(bag);
				activeList.push_back(*i);
			}
		}
		stack->Push();
	}
	virtual bool OnUpdate(Bag& bag) const
	{
		Behaviour::shared_tree tree = GetTree();
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		list& activeList = stack->Pop();

		iterator i = activeList.begin();
		while (i != activeList.end())
		{
			const Behaviour* behaviour = (*tree)[*i];
			if (!behaviour->Update(bag))
			{
				behaviour->Done(bag);
				activeList.erase(i);
			}
			else
			{
				++i;
			}
		}
		stack->Push();
		return 0 != activeList.size();
	}
	virtual void OnInterrupt(Bag& bag) const
	{
		Behaviour::shared_tree tree = GetTree();
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		list& activeList = stack->Pop();

		iterator i = activeList.begin();
		while (i != activeList.end())
		{
			(*tree)[*i]->Interrupt(bag);
			activeList.erase(i);
		}
		stack->Push();
	}
	virtual void OnDone(Bag& bag) const
	{
		Behaviour::shared_tree tree = GetTree();
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		list& activeList = stack->Pop();
		activeList.clear();
		stack->Push();
	}

	list mBehaviours;
	Bag::Handle mStackHandle;
};

class Behaviour::Select : public Behaviour::List
{
	typedef Behaviour::Stack<int> stack_type;
public:
	Select() : mReselect(false)
	{
	}
	virtual void Register(Bag::Builder& builder)
	{ 
		mStackHandle = builder.GetHandle<stack_type>("SelectStack");
	}
	virtual void Initialize(Bag& bag) const
	{ 
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		for(int i = 0; i < stack_type::MaxDepth; ++i)
		{
			(*stack)[i] = -1;
		}
		stack->Reset();
	}
	virtual bool CanStart(const Bag& bag) const 
	{ 
		const stack_type* stack = bag.Get<stack_type>(mStackHandle);
		const int focus = stack->Pop();
		bool canStart = 0 <= FindSelection(bag, focus);
		stack->Push(); // Maintains focus on the stack
		return canStart;
	}

protected:
	virtual void OnStart(Bag& bag) const
	{
		Behaviour::shared_tree tree = GetTree();
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		const int focus = stack->Pop();
		int selected = FindSelection(bag, focus);
		if (0 <= selected)
		{
			tree->At(selected)->Start(bag);
		}
		stack->Push(selected);
	}
	virtual bool OnUpdate(Bag& bag) const
	{
		Behaviour::shared_tree tree = GetTree();
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		int focus = stack->Pop();
		// ASSERT(0 <= focus);
		while (0 <= focus && !tree->At(focus)->Update(bag))
		{
			tree->At(focus)->Done(bag);
			stack->Push();
			Start(bag);
			focus = stack->Pop();
		}
		stack->Push(focus);
		return 0 <= focus;
	}
	virtual void OnInterrupt(Bag& bag) const
	{
		Behaviour::shared_tree tree = GetTree();
		stack_type* stack = bag.Get<stack_type>(mStackHandle);
		const int focus = stack->Pop();
		// ASSERT(0 <= focus);
		tree->At(focus)->Interrupt(bag);
		stack->Push(focus);
	}
	virtual void OnDone(Bag& ) const
	{
	}
	stack_type::value_type FindSelection(const Bag& bag, stack_type::value_type focus = -1) const
	{
		Behaviour::shared_tree tree = GetTree();
		list::size_type count = mBehaviours.size();
		int selected = Max(0, focus + 1);
		while(selected != focus && !(*tree)[selected]->CanStart(bag))
		{
			selected += 1;
			selected %= count;
		}
		if (selected != focus || !mReselect || !(*tree)[selected]->CanStart(bag))
		{
			selected = -1;
		}
		return selected;
	}
	size_t mStackHandle;
	bool mReselect;
};

} // namespace Marbles