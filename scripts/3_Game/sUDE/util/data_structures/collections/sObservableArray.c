typedef Param2<Class,string> SArrayChangeListener;
typedef Param2<Class,string> SArrayInsertListener;
typedef Param2<Class,string> SArrayPreRemoveListener;
typedef Param2<Class,string> SArrayClearListener;
typedef Param2<Class,string> SArraySetListener;

class SObservableArray<Class T> : Managed {
	
	protected ref array<T> m_list;
	
	protected ref array<ref SArrayChangeListener> m_onChangeListeners;
	protected ref array<ref SArrayInsertListener> m_onInsertListeners;
	protected ref array<ref SArrayPreRemoveListener> m_onPreRemoveListeners;
	protected ref array<ref SArrayClearListener> m_onClearListeners;
	protected ref array<ref SArraySetListener> m_onSetListeners;
	
	void SObservableArrayList(array<T> init = null) {
		if (!init) {
			m_list = new array<T>();
		} else {
			m_list = init;
		}
		m_onChangeListeners = new array<ref SArrayChangeListener>();
		m_onInsertListeners = new array<ref SArrayInsertListener>();
		m_onPreRemoveListeners = new array<ref SArrayPreRemoveListener>();
		m_onClearListeners = new array<ref SArrayClearListener>();
		m_onSetListeners = new array<ref SArraySetListener>();
	}
		
	void addOnChangeListener(SArrayChangeListener listener) {
		m_onChangeListeners.Insert(listener);
	}
	
	void addOnInsertListener(SArrayInsertListener listener) {
		m_onInsertListeners.Insert(listener);
	}
	
	void addOnPreRemoveListener(SArrayPreRemoveListener listener) {
		m_onPreRemoveListeners.Insert(listener);
	}
	
	void addOnClearListener(SArrayClearListener listener) {
		m_onClearListeners.Insert(listener);
	}
	
	void addOnSetListener(SArraySetListener listener) {
		m_onSetListeners.Insert(listener);
	}
	
	protected void onChange() {
		foreach (SArrayChangeListener listener : m_onChangeListeners) {
			GetGame().GameScript.CallFunction(listener.param1, listener.param2, null, null);
		}
	}
	
	protected void onInsert(T value, int insertPosition) {
		onChange();
		foreach (SArrayInsertListener listener : m_onInsertListeners) {
			GetGame().GameScript.CallFunctionParams(listener.param1, listener.param2, null, new Param2<T,int>(value, insertPosition));
		}
	}
	
	protected void onPreRemove(T index) {
		onChange();
		foreach (SArrayPreRemoveListener listener : m_onPreRemoveListeners) {
			GetGame().GameScript.CallFunctionParams(listener.param1, listener.param2, null, new Param1<int>(index));
		}
	}
	
	protected void onClear() {
		onChange();
		foreach (SArrayClearListener listener : m_onClearListeners) {
			GetGame().GameScript.CallFunctionParams(listener.param1, listener.param2, null, null);
		}
	}
	
	protected void onSet(int index, T value) {
		onChange();
		foreach (SArraySetListener listener : m_onSetListeners) {
			GetGame().GameScript.CallFunctionParams(listener.param1, listener.param2, null, new Param2<int,T>(index,value));
		}
	}
	
	
	
	
	
		
	int init(T init[]) {
		return m_list.Init(init);
	}
	
	void set(int index, T value) {
		m_list.Set(index, value);
		if (index >= 0 && index < m_list.Count()) {
			onSet(index, value);
		}
	}
	
	int insert(T value) {
		int insertPosition = m_list.Insert(value);
		onInsert(value, insertPosition);
		return insertPosition;
	}
	
	int insertAt(T value, int index) {
		int newElementCount = m_list.InsertAt(value, index);
		if (index >= m_list.Count() || index < 0) {
			index = m_list.Count();
		}
		onInsert(value, index);
		return newElementCount;
	}
	
	void remove(int index) {
		onPreRemove(index);
		m_list.Remove(index);
	}
	
	void removeOrdered(int index) {
		if (index >= 0 && index < m_list.Count()) {
			onPreRemove(index);
		}
		m_list.RemoveOrdered(index);
	}
	
	void removeItem(T value) {
		int removeIndex = m_list.Find(value);
		if ( removeIndex >= 0 ) {
			removeOrdered(removeIndex);
		}
	}
	
	void removeItemUnOrdered(T value) {
		int removeIndex = m_list.Find(value);
		if ( removeIndex >= 0 ) {
			remove(removeIndex);
		}
	}
	
	void swap(notnull array<T> other) {
		m_list.Swap(other);
		onChange();
	}
	
	void swapItems(int item1_index, int item2_index) {
		T item1 = m_list.Get(item1_index);
		this.set(item1_index, m_list.Get(item2_index));
		this.set(item2_index, item1);
		onChange();
	}
	
	int copy(notnull array<T> from) {
		int copyCount = m_list.Copy(from);
		onChange();
		return copyCount;
	}
	
	void clear() {
		m_list.Clear();
		onClear();
	}
	
	void sort(bool reverse = false) {
		m_list.Sort(reverse);
		onChange();
	}
	
	void shuffleArray() {
		for (int i = 0; i < m_list.Count(); i++) {
			swapItems(i, m_list.GetRandomIndex());
		}
	}
	
	void invert() {
		int left = 0;
		int right = m_list.Count() - 1;
		if( right > 1 ) {
			while( left < right ) {
				T temp = m_list.Get(left);
				this.set(left++, m_list.Get(right));
				this.set(right--, temp);
			}
		}
	}
	
	
	
	//////////////////////////////////////////////////////
	
	
	int count() {
		return m_list.Count();
	}
	
	int find(T value) {
		return m_list.Find(value);
	}

	T get(int index) {
		return m_list.Get(index);
	}
	
	void insertAll(notnull array<T> from) {
		foreach (T item : from) {
			insert(item);
		}
	}
	
	void insertArray(array<T> other) {
		foreach (T item : other) {
			insert(item);
		}
	}
	
	void resize(int newSize) {
		m_list.Resize(newSize);
	}
	
	void reserve(int newSize) {
		m_list.Reserve(newSize);
	}

	bool isValidIndex( int index ) {
		return m_list.IsValidIndex(index);
	}
	
	int getRandomIndex() {
		return m_list.GetRandomIndex();
	}

	T getRandomElement() {
		return m_list.GetRandomElement();
	}

	int moveIndex(int curr_index, int move_number) {
		return m_list.MoveIndex(curr_index, move_number);
	}
	
}