class TestUnit_SObservableArray : STestUnit {
	
	
	ref SObservableArray<int> a = new SObservableArray<int>();
	bool onChangeCalled;
	bool onInsertCalled;
	bool onPreRemoveCalled;
	bool onClearCalled;
	bool onSetCalled;
	ref Param argumentsReceived;
	
	override void init() {

		registerBeforeCallbacks({
			"reset",
			"initListeners"
		});
		
		registerTestCases({
			"testCallbacks_clear",
			"testCallbacks_set",
			"testCallbacks_insert",
			"testCallbacks_insertAt",
			"testCallbacks_remove"
		});
	}
	
	
	void reset() {
		onChangeCalled = false;
		onInsertCalled = false;
		onPreRemoveCalled = false;
		onClearCalled = false;
		onSetCalled = false;
		
		argumentsReceived = null;
		
		a = new SObservableArray<int>({0, 1, 2, 3, 4});
	}
	
	void initListeners() {
		a.addOnChangeListener(new SArrayChangeListener(this, "onChangeListener"));
		a.addOnInsertListener(new SArrayInsertListener(this, "onInsertListener"));
		a.addOnPreRemoveListener(new SArrayPreRemoveListener(this, "onPreRemoveListener"));
		a.addOnClearListener(new SArrayClearListener(this, "onClearListener"));
		a.addOnSetListener(new SArraySetListener(this, "onSetListener"));
	}
	
	
	
	void onChangeListener() {
		SLog.d("onChangeListener");
		onChangeCalled = true;
		argumentsReceived = null;
	}
	
	void onInsertListener(int value, int position) {
		onInsertCalled = true;
		argumentsReceived = new Param2<int, int>(value, position);
	}
	
	void onPreRemoveListener(int indexToBeRemoved) {
		onPreRemoveCalled = true;
		argumentsReceived = new Param1<int>(indexToBeRemoved);
	}
	
	void onClearListener() {
		onClearCalled = true;
	}
	
	void onSetListener(int index) {
		onSetCalled = true;
		argumentsReceived = new Param1<int>(index);
	}
	
	
	
	void testCallbacks_clear() {
		a.clear();
		bool otherCallbacksCalled = onInsertCalled || onPreRemoveCalled || onSetCalled ;
		assertTrue(onChangeCalled && onClearCalled && !otherCallbacksCalled);
	}
	
	void testCallbacks_set() {
		a.set(1, 69);
		bool otherCallbacksCalled = onInsertCalled || onPreRemoveCalled || onClearCalled ;
		assertTrue(onChangeCalled && onSetCalled && !otherCallbacksCalled);
	}
	
	void testCallbacks_insert() {
		a.insert(69);
		bool otherCallbacksCalled = onSetCalled || onPreRemoveCalled || onClearCalled ;
		assertTrue(onChangeCalled && onInsertCalled && !otherCallbacksCalled);
	}
	
	void testCallbacks_insertAt() {
		a.insertAt(2, 2);
		bool otherCallbacksCalled = onSetCalled || onPreRemoveCalled || onClearCalled ;
		assertTrue(onChangeCalled && onInsertCalled && !otherCallbacksCalled);
	}
	
	void testCallbacks_remove() {
		a.remove(1);
		bool otherCallbacksCalled = onSetCalled || onInsertCalled || onClearCalled ;
		assertTrue(onChangeCalled && onPreRemoveCalled && !otherCallbacksCalled);
	}
	
}