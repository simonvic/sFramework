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
			"clear_testCallbacks",
			"set_testCallbacks",
			"set_testArgumentsReceived",
			"insert_testCallbacks",
			"insert_testArgumentsReceived",
			"insertAt_testCallbacks",
			"remove_testCallbacks",
			"remove_testArgumentsReceived",
			"removeItem_testArgumentsReceived"
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
	
	
	
	void clear_testCallbacks() {
		a.clear();
		bool otherCallbacksCalled = onInsertCalled || onPreRemoveCalled || onSetCalled ;
		assertTrue(onChangeCalled && onClearCalled && !otherCallbacksCalled);
	}
	
	void set_testCallbacks() {
		a.set(1, 69);
		bool otherCallbacksCalled = onInsertCalled || onPreRemoveCalled || onClearCalled ;
		assertTrue(onChangeCalled && onSetCalled && !otherCallbacksCalled);
	}
	
	void set_testArgumentsReceived() {
		a.set(1, 69);
		Param1<int> arguments = Param1<int>.Cast(argumentsReceived);
		if (!arguments) {
			fail("int", string.Format("%1", argumentsReceived));
			return;
		}
		
		assertEqual(1, arguments.param1);
	}
	
	void insert_testCallbacks() {
		a.insert(69);
		bool otherCallbacksCalled = onSetCalled || onPreRemoveCalled || onClearCalled ;
		assertTrue(onChangeCalled && onInsertCalled && !otherCallbacksCalled);
	}
	
	void insert_testArgumentsReceived() {
		a.insert(69);
		a.insert(420);
				
		Param2<int,int> arguments = Param2<int,int>.Cast(argumentsReceived);
		if (!arguments) {
			fail("int, int", string.Format("%1", argumentsReceived));
			return;
		}
		
		if (420 != arguments.param1) {
			fail("420", ""+arguments.param1);
			return;
		}
		
		if (6 != arguments.param2) {
			fail("5", ""+arguments.param2);
			return;
		}
		
		pass("","", "Arguments are ok");
	}
	
	void insertAt_testCallbacks() {
		a.insertAt(2, 2);
		bool otherCallbacksCalled = onSetCalled || onPreRemoveCalled || onClearCalled ;
		assertTrue(onChangeCalled && onInsertCalled && !otherCallbacksCalled);
	}
	
	void remove_testCallbacks() {
		a.remove(1);
		bool otherCallbacksCalled = onSetCalled || onInsertCalled || onClearCalled ;
		assertTrue(onChangeCalled && onPreRemoveCalled && !otherCallbacksCalled);
	}
	
	void remove_testArgumentsReceived() {
		a.remove(2);
		
		Param1<int> arguments = Param1<int>.Cast(argumentsReceived);
		if (!arguments) {
			fail("int", string.Format("%1", argumentsReceived));
			return;
		}
		
		if (2 != arguments.param1) {
			fail("2", ""+arguments.param1);
			return;
		}
		
		pass("2", ""+arguments.param1);
	}
	
	void removeItem_testArgumentsReceived() {
		a.removeItem(2);
		
		Param1<int> arguments = Param1<int>.Cast(argumentsReceived);
		if (!arguments) {
			fail("int", string.Format("%1", argumentsReceived));
			return;
		}
		
		if (2 != arguments.param1) {
			fail("2", ""+arguments.param1);
			return;
		}
		
		pass("2", ""+arguments.param1);
	}
	
}