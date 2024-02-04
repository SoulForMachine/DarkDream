
#ifndef _STACK_H_
#define _STACK_H_


template <class _Type>
class FixedStack
{
public:
	explicit FixedStack(size_t size, void* buffer = 0);
	~FixedStack();

	void Push(const _Type& t)
	{
		assert(m_top < m_size);
		new (m_buffer[m_top++]) _Type(t);
	}
	void Pop()
	{
		assert(m_top > 0);
		m_buffer[--m_top].~_Type();
	}
	void Pop(_Type& t)
	{
		assert(m_top > 0);
		--m_top;
		t = m_buffer[m_top];
		m_buffer[m_top].~_Type();
	}
	_Type& Peek()
	{
		assert(m_top > 0);
		return m_buffer[m_top - 1]; 
	}

	void Clear();

private:
	_Type* m_buffer;
	size_t m_top;
	size_t m_size;
	bool m_ownBuffer;
};


template <class _Type>
FixedStack<_Type>::FixedStack(size_t size, void* buffer = 0)
{
	m_size = size;
	m_top = 0;
	m_ownBuffer = (buffer == 0);

	if(m_ownBuffer)
		m_buffer = (_Type*)new ubyte[size * sizeof(_Type)];
	else
		m_buffer = (_Type*)buffer;
}

template <class _Type>
FixedStack<_Type>::~FixedStack()
{
	Clear();

	if(m_ownBuffer)
		Memory::Delete((ubyte*)m_buffer);
}


template <class _Type>
void FixedStack<_Type>::Clear()
{
	while(m_top)
		Pop();
}


#endif //_STACK_H_
