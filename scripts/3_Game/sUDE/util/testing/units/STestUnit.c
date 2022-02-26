class STestUnit : Managed {	
	
	protected bool skipAfterFail;
	protected bool hasFailed;
	
	protected ref array<ref STestCase> m_testCases = new array<ref STestCase>();
	
	protected STestCase m_currentTestCaseTested;
	protected STestCase m_lastTestCaseTested;
	
	void STestUnit(bool shouldSkipAfterFail = true) {
		skipAfterFail = shouldSkipAfterFail;
		init();
	}
		
	protected void init();
		
	protected void registerTestCases(TStringArray functionsNames) {
		foreach (string functionName : functionsNames) {
			m_testCases.Insert(new STestCase(this, functionName));
		}
	}
	
	array<ref STestCase> getTestCases() {
		return m_testCases;
	}
	
	bool hasFailed() {
		return hasFailed;
	}
	
	void run() {
		foreach (STestCase testCase : m_testCases) {
			m_currentTestCaseTested = testCase;
			if (hasFailed && skipAfterFail) {
				skip();
			} else {
				GetGame().GameScript.CallFunctionParams( testCase.getClass(), testCase.getFunction(), null, null);
			}
			m_lastTestCaseTested = testCase;
		}
		m_currentTestCaseTested = null;
		m_lastTestCaseTested = null;
	}

	protected void assertEqual(array<float> expected, array<float> actual) {
		if (SMath.equal(expected, actual)) {
			pass(string.Format("%1", expected), string.Format("%1", actual));
		} else {
			fail(string.Format("%1", expected), string.Format("%1", actual));
		}
	}
	
	protected void assertEqual(float expected, float actual) {
		if (SMath.equal(expected, actual)) {
			pass(expected.ToString(), actual.ToString());
		} else {
			fail(expected.ToString(), actual.ToString());
		}
	}
	
	protected void assertEqual(int expected, int actual) {
		if (expected == actual) {
			pass(expected.ToString(), actual.ToString());
		} else {
			fail(expected.ToString(), actual.ToString());
		}
	}
	
	protected void assertEqual(string expected, string actual) {
		if (expected == actual) {
			pass(expected, actual);
		} else {
			fail(expected, actual);
		}
	}
	
	protected void assertEqual(bool expected, bool actual) {
		if (expected == actual) {
			pass(expected.ToString(), actual.ToString());
		} else {
			fail(expected.ToString(), actual.ToString());
		}
	}
	
	protected void assertTrue(bool actual) {
		if (actual) {
			pass("true", actual.ToString());
		} else {
			fail("true", actual.ToString());
		}
	}
	
	protected void assertFalse(bool actual) {
		if (!actual) {
			pass("true", actual.ToString());
		} else {
			fail("true", actual.ToString());
		}
	}
	
	protected void assertNull(Class actual) {
		if (actual == null) {
			pass("null", actual.ToString());
		} else {
			fail("null", actual.ToString());
		}
	}
	
	protected void assertNotNull(Class actual) {
		if (actual != null) {
			pass("null", actual.ToString());
		} else {
			fail("null", actual.ToString());
		}
	}
	
	protected void pass(string expected, string actual) {
		m_currentTestCaseTested.setPassed();
		m_currentTestCaseTested.setExpected(expected);
		m_currentTestCaseTested.setActual(actual);
	}
	
	protected void fail(string expected, string actual, string message = string.Empty) {
		hasFailed = true;
		m_currentTestCaseTested.setFailed();
		m_currentTestCaseTested.setExpected(expected);
		m_currentTestCaseTested.setActual(actual);
		m_currentTestCaseTested.setMessage(message);
		
	}	
	
	protected void skip() {
		m_currentTestCaseTested.setSkipped();
	}
}