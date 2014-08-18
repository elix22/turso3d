// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "../Base/AutoPtr.h"
#include "../Base/WeakPtr.h"
#include "../IO/JSONValue.h"

namespace Turso3D
{

class Event;

/// Internal helper class for invoking event handler functions.
class TURSO3D_API EventHandler
{
public:
    /// Construct with receiver object pointer.
    EventHandler(WeakRefCounted* receiver);
    /// Destruct.
    virtual ~EventHandler();

    /// Invoke the handler function. Implemented by subclasses.
    virtual void Invoke(Event& event) = 0;

    /// Return the receiver object.
    WeakRefCounted* Receiver() const { return receiver.Get(); }

protected:
    /// Receiver object.
    WeakPtr<WeakRefCounted> receiver;
};

/// Template implementation of the event handler invoke helper, stores a function pointer of specific class.
template <class T, class U> class EventHandlerImpl : public EventHandler
{
public:
    typedef void (T::*HandlerFunctionPtr)(U&);

    /// Construct with receiver and function pointers.
    EventHandlerImpl(WeakRefCounted* receiver_, HandlerFunctionPtr function_) :
        EventHandler(receiver_),
        function(function_)
    {
        assert(function);
    }

    /// Invoke the handler function.
    virtual void Invoke(Event& event)
    {
        T* typedReceiver = static_cast<T*>(receiver.Get());
        U& typedEvent = static_cast<U&>(event);
        (typedReceiver->*function)(typedEvent);
    }

private:
    /// Pointer to the event handler function.
    HandlerFunctionPtr function;
};

/// An event to which objects can subscribe by specifying a handler function. Can be subclassed to include event-specific data.
class TURSO3D_API Event
{
public:
    /// Construct.
    Event();
    /// Destruct.
    virtual ~Event();
    
    /// Send the event.
    void Send(WeakRefCounted* sender);
    /// Subscribe to the event. The event takes ownership of the handler data. If there is already handler data for the same receiver, it is overwritten.
    void Subscribe(EventHandler* handler);
    /// Unsubscribe from the event.
    void Unsubscribe(WeakRefCounted* receiver);

    /// Return whether has at least one valid receiver.
    bool HasReceivers() const;
    /// Return whether has a specific receiver.
    bool HasReceiver(const WeakRefCounted* receiver) const;
    /// Return current sender.
    WeakRefCounted* Sender() const { return currentSender; }
    
private:
    /// Prevent copy construction.
    Event(const Event& rhs);
    /// Prevent assignment.
    Event& operator = (const Event& rhs);
    
    /// Event handlers.
    Vector<AutoPtr<EventHandler> > handlers;
    /// Current sender.
    WeakPtr<WeakRefCounted> currentSender;
};

}
