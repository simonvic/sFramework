class STreeNode<Class T> : Managed {
	
	protected T m_data;
	protected ref array<ref STreeNode<T>> m_children;
	
	void STreeNode(T data) {
		m_data = data;
		m_children = new array<ref STreeNode<T>>;
	}
	
	T getData() {
		return m_data;
	}
	
	void setData(T data) {
		m_data = data;
	}
	
	array<ref STreeNode<T>> getChildren() {
		return m_children;
	}
	
	void addChildren(array<T> dataArray) {
		foreach (T data : dataArray) {
			addChild(data);
		}
	}
	
	void addChild(T data) {
		addChild(new STreeNode<T>(data));
	}
	
	void addChildren(array<STreeNode<T>> children) {
		foreach (STreeNode<T> child : children) {
			addChild(child);
		}
	}
	
	void addChild(STreeNode<T> child) {
		m_children.Insert(child);
	}
	
	
	void debugPrint(int depth = 0){
		SLog.d(getData(), "", depth);
		foreach (STreeNode<T> child : m_children) {
			child.debugPrint(depth+1);
		}
	}
		
		
	
}