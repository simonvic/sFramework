//lol :)
class TestUnit_STest : STestUnit {
	
	override void init() {
		
		continueAfterFail(true);
		
		registerBeforeClassCallbacks({
			"beforeClass"
		});
		
		registerBeforeCallbacks({
			"before"
		});
		
		registerTestCases({
			"testAssertEqual",
			"testAssertTrue",
			"shouldFail",
			"testAssertFalse",
			"testMultipleAssertion",
			"shouldSkip",
			"testAssertNull",
			"testAssertNotNull"
		});
		
		registerAfterCallbacks({
			"after"
		});
		
		registerAfterClassCallbacks({
			"afterClass"
		});
	}
	
	void beforeClass() {
		SLog.d("before class",""+this);
	}
	
	void before() {
		SLog.d("before",""+this);
	}
	
	void after() {
		SLog.d("after",""+this);
	}
	
	void afterClass() {
		SLog.d("after class",""+this);
	}
	
	void shouldFail() {
		assertEqual(0, 1);
	}

	void shouldSkip() {
		// do nothing
	}
	
	void testMultipleAssertion() {
		if (69 < 420) {
			fail(false.ToString(), true.ToString(), "Damn man... 69 is smaller than 420");
		} else {
			assertTrue(69 > -69);
		}
	}
	
	void testAssertEqual() {
		assertEqual(69, 69);
	}
	
	void testAssertTrue() {
		assertTrue(true);
	}
	
	void testAssertFalse() {
		assertFalse(false);
	}
	
	void testAssertNull() {
		assertNull(null);
	}
	
	void testAssertNotNull() {
		assertNotNull(new SColor());
	}
}