class SStack<Class T> {
	protected ref array<T> m_stack;
	
	void SStack(){
		m_stack = new array<T>();
	}
	
	void push(T item){
		m_stack.InsertAt(item, 0);
	}
	
	T pop(){
		T popped = m_stack.Get(0);
		m_stack.RemoveOrdered(0);
		return popped;
	}
	
	T peek(){
		return m_stack.Get(0);
	}
	
}