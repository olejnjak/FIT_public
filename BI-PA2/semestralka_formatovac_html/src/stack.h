//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef STACK_H
#define STACK_H
#include <iostream>

//! Template type T is expected to have copy constructor implemented.
//! \brief Class implements classic abstract data type Stack as linked list.\n
template <class T>
class Stack {
protected:
	//! \brief Structure to save stack items.
	struct Item {
		T m_Value; ///< Stored value.
		Item * m_Next; ///< Pointer to next stack item.

		//! \brief Constructor only sets the value, follower is NULL for now.
		Item (T value) : m_Value(value) {
			m_Next = NULL;
		}
	};

	//! \name pointers to the beginning/end of the stack
	//! @{
	Item * m_Begin;
	Item * m_End;
	//! @}

	void copy(const Stack& src);
	void free(void);

public:
	//! Indicates Push() or Pop() called on empty stack.
	//! \brief Class for throwing structured exceptions.
	class StackEmptyException : public std::exception {
	};

	Stack (void);
	~Stack (void);
	Stack (const Stack& src);
	T Pop(void);
	T Top(void) const;
	void Push (T item);
	bool IsEmpty(void) const;

	Stack operator= (const Stack& right);
	//another possible operator overloads(+= - Push, -= Pop, etc.), but in my opinion standard program uses methods as implemented
};

//-------------------------------------------------------------------------------------------
//! \brief Constructor initialises empty instance.
template <class T>
Stack<T>::Stack (void) {
	m_Begin = NULL;
	m_End = NULL;
}

//-------------------------------------------------------------------------------------------
template <class T>
Stack<T>::~Stack (void) {
	free();
}

//-------------------------------------------------------------------------------------------
//! \brief Copy constructor.
template <class T>
Stack<T>::Stack (const Stack& src) {
	copy(src);
}

//-------------------------------------------------------------------------------------------
//! \brief Operator= overload.
template <class T>
Stack<T> Stack<T>::operator= (const Stack& right) {
	if (&right == this) return *this;
	free();
	copy(right);
	return *this;
}

//-------------------------------------------------------------------------------------------
//! Primarily written to be called in CStack(const CStack& src) and operator=() overload.
//! \brief Method makes hard copy of source stack.
//! \param src Source instance of CStack to be copied from.
template <class T>
void Stack<T>::copy (const Stack& src) {
	T * begin = src.m_Begin;
	Item * previous = NULL; //I need to remember previous list values, so I can adjust their m_Next values.

	while (begin) { //Go through the item list of source stack.
		Item * current = new Item(begin->m_Value); //Make new item with value of the source item, follower is set to NULL.

		if (previous) previous->m_Next = current; //If there is some previous item, I'll adjust 'm_Next' pointer to 'current'.
		else m_Begin = current; //If there is no previous item, I'm on the very beginning of the list, so I need to set 'm_Begin'

		previous = current; //Now the 'current' item is the 'previous' for the next iteration.
		begin = begin->m_Next; //I need to go through the whole list, so I'm adjusting the beginning to the next item.
	}
	m_End = previous; //The last item is saved in 'previous' so I'll mark it as the end.
}

//-------------------------------------------------------------------------------------------
//! Primarily written to be called in in CStack(const CStack& src) and destructor.
//! \brief Method is called to free allocated memory space.
template <class T>
void Stack<T>::free (void) {
	while(m_Begin) { //Go through the list.
		Item * to_delete = m_Begin; //Store 'm_Begin' to be able to delete it.
		m_Begin = m_Begin->m_Next; //Set the new beginning.
		delete to_delete; //Delete the old one.
	}

	m_Begin = NULL; //Reinitialise member variables.
	m_End = NULL;
}

//-------------------------------------------------------------------------------------------
//! \brief Method inserts an item to the top of the stack.
template <class T>
void Stack<T>::Push (T insert) {
	Item * ins = new Item(insert);
	ins->m_Next = m_Begin;
	m_Begin = ins;
}

//-------------------------------------------------------------------------------------------
//! \exception StackEmptyException If the stack is empty.
//! \brief Method takes and deletes item on the top of the stack.
//! \returns The value of the item on the top of the stack.
template <class T>
T Stack<T>::Pop (void) {
	if (IsEmpty()) throw StackEmptyException(); //If the stack is empty, throw an exception.

	T ret = m_Begin->m_Value;
	Item * to_delete = m_Begin;
	m_Begin = m_Begin->m_Next;
	delete to_delete;

	return ret;
}

//-------------------------------------------------------------------------------------------
//! \exception StackEmptyException If the stack is empty.
//! \brief Method only returns the value of the item on the top of the stack without taking it.
//! \returns The value of the item on the top of the stack.
template <class T>
T Stack<T>::Top (void) const {
	if (IsEmpty()) throw StackEmptyException(); //If the stack is empty, throw an exception.

	return m_Begin->m_Value;
}

//-------------------------------------------------------------------------------------------
//! \brief Method decides whether the stack is empty or not.
//! \retval true If the stack is empty.
//! \retval false If the stack isn't empty.
template <class T>
bool Stack<T>::IsEmpty (void) const {
	return m_Begin == NULL;
}
#endif
