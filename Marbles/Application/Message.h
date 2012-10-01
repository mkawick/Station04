#pragma once

#include<boost\thread\mutex.hpp>
#include<boost\interprocess\sync\scope_lock.hpp>
#include<boost\circular_buffer.hpp>
//#include<boost\interprocess\detail\atomic.hpp>

namespace Marbles
{
	class MessageBox
	{
	public:
		void ClearMessages();

		template<typename T> void OnMessage(T&& fn)
		{
		}

	private:
		MessageBox()
		{
		}
	};

	class MessageService
	{
	public:
		MessageService()
		{
			Register(this);
		}
		~MessageService()
		{
			Unregister(this);
		}

		template<typename T> bool Post(T* msg)
		{
			boost::scoped_lock lock(mBoxMutex);
			MessageBoxList::iterator i = mMessageBoxes.begin(); 
			while(i != mMessageBoxes.end())
			{
				i->Post(msg);
				++i;
			}
		}

	private:
		friend class MessageBox;
		void Register(MessageService::MessageBox* box)
		{
			boost::scoped_lock lock(mBoxMutex);
			mMessageBoxes.push_back(box);
		}
		void Unregister(MessageService::MessageBox* box)
		{
			boost::scoped_lock lock(mBoxMutex);
			MessageBoxList::iterator i = mMessageBoxes.begin(); 
			while(i != mMessageBoxes.end())
			{
				if (*i == box)
				{
					mMessageBoxes.remove(i);
					break;
				}
				++i;
			}
		}
		typedef std::vector<MessageBox*> MessageBoxList;
		MessageBoxList mMessageBoxes;
		boost::mutex mBoxMutex;

		static void Register(MessageService* route)
		{
			boost::scoped_lock lock(smServiceMutex);
			smServices.push_back(route);
		}

		static void Unregister(MessageService* route)
		{
			boost::scoped_lock lock(smServiceMutex);
			MessageBoxList::iterator i = smServices.begin(); 
			while(i != smServices.end())
			{
				if (*i == route)
				{
					smServices.remove(i);
					break;
				}
				++i;
			}
		}

		typedef std::vector<MessageService*> ServiceList;
		static boost::mutex smServiceMutex;
		static ServiceList smServices;
	}

	template<size_t MaxMessages> 
	class MailBox : public MessageBox
	{
	public:
		MailBox()
			: MessageBox(mSlots, MaxMessages)
		{
			Task::ActiveTask()->GetMailer()->Register(this);
		}
		~MailBox()
		{
			Task::ActiveTask()->GetMailer()->Unregister(this);
		}

		template<typename T> bool Post(T* message)
		{
			// static_assert<std::is_pod<T>::value>>::value;
		}

		template<typename T> void OnMessage(std::function<void (T&)>&& fn)
		{
			
		}
		
	private:

		
		std::tr1::unordered_set<hash_t, boost::bind> mHandlers;
		boost::cirular_buffer mMail; // add/remove must be thread safe
	};

} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
