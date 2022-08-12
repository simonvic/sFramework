#ifdef DIAG_DEVELOPER

class TestUnit_SMath : STestUnit {
	
	override void init() {
		registerTestCases({
			"floatsShouldBeEqual",
			"floatsShouldNotBeEqual",
			"smallFloatsShouldBeEqual",
			"smallerFloatsShouldNotBeEqual",
			//"shouldSkip",
			"smallFloatsWithReducedEpsilonShouldBeEqual",
			"testIsInRange1",
			"testIsInRange2",
			"testIsInRange3",
			//"shouldFail",
			"arraysShouldBeEqual",
			"arraysShouldNotBeEqual",
			"arraysWithDifferentSizeShouldNotBeEqual",
			"arraysWithDifferentSizeShouldNotBeEqual2",
			"testArrayClamp1",
			"testArrayClamp2",
			"testArrayClamp3"
		});
	}
	
	void floatsShouldBeEqual() {
		assertTrue(SMath.equal(0.1, 0.1));
	}
	
	void floatsShouldNotBeEqual() {
		assertFalse(SMath.equal(0.5, 0.1));
	}
	
	void smallFloatsShouldBeEqual() {
		assertTrue(SMath.equal(0.00000001, 0.00000001));
	}
	
	void smallerFloatsShouldNotBeEqual() {
		assertFalse(SMath.equal(0.000000000000001, 0.000000000000002));
	}
	
	void smallFloatsWithReducedEpsilonShouldBeEqual() {
		assertTrue(SMath.equal(0.000000000000001, 0.000000000000002, 0.0001));
	}
	
	void testIsInRange1() {
		assertTrue(SMath.isInRange(0.5, 0.0, 1.0));
	}
	
	void testIsInRange2() {
		assertFalse(SMath.isInRange(-100, 0.0, 1.0));
	}
	
	void testIsInRange3() {
		assertFalse(SMath.isInRange(100, 0.0, 1.0));
	}
	
	void arraysShouldBeEqual() {
		TFloatArray a1 = {0, 1, 2, 3};
		TFloatArray a2 = {0, 1, 2, 3};
		assertTrue(SMath.equal(a1, a2));
	}
	
	void arraysShouldNotBeEqual() {
		TFloatArray a1 = {0, 0, 0, 0};
		TFloatArray a2 = {0, 1, 2, 3};
		assertFalse(SMath.equal(a1, a2));
	}
	
	void arraysWithDifferentSizeShouldNotBeEqual() {
		TFloatArray a1 = {0, 1, 2};
		TFloatArray a2 = {0, 1, 2, 3};
		assertFalse(SMath.equal(a1, a2));
	}
	
	void arraysWithDifferentSizeShouldNotBeEqual2() {
		TFloatArray a1 = {0, 1, 2, 3};
		TFloatArray a2 = {0, 1, 2};
		assertFalse(SMath.equal(a1, a2));
	}
		
	void testArrayClamp1() {
		TFloatArray toClamp   = {0.5, 5,  50,  500};
		TFloatArray min       = {0,   1,  10,  100};
		TFloatArray max       = {1,   10, 100, 1000};
		TFloatArray expected  = toClamp;
		
		SMath.clamp(toClamp, min, max);
		if (!SMath.equal(expected, toClamp)) {
			fail(string.Format("%1", expected), string.Format("%1", toClamp), "Failed when comparing the arrays");
		} else {
			assertTrue(SMath.isInRange(toClamp, min, max));
		}
	}
	
	void testArrayClamp2() {
		TFloatArray toClamp  = {2,   5,  50,  500};
		TFloatArray min      = {0,   1,  10,  100};
		TFloatArray max      = {1,   10, 100, 1000};
		TFloatArray expected = {1,   5,  50,  500};
		
		SMath.clamp(toClamp, min, max);
		if (!SMath.equal(expected, toClamp)) {
			fail(string.Format("%1", expected), string.Format("%1", toClamp), "Failed when comparing the arrays");
		} else {
			assertTrue(SMath.isInRange(toClamp, min, max));
		}
		
	}
	
	void testArrayClamp3() {
		TFloatArray toClamp  = {2,   5,  5000,  0.1};
		TFloatArray min      = {0,   1,  10,  100};
		TFloatArray max      = {1,   10, 100, 1000};
		TFloatArray expected = {1,   5,  100,  100};
		
		SMath.clamp(toClamp, min, max);
		if (!SMath.equal(expected, toClamp)) {
			fail(string.Format("%1", expected), string.Format("%1", toClamp), "Failed when comparing the arrays");
		} else {
			assertTrue(SMath.isInRange(toClamp, min, max));
		}
	}
	
	
	
	void shouldFail() {
		assertEqual(0, 1);
	}

	void shouldSkip() {
		// do nothing
	}
}

#endif