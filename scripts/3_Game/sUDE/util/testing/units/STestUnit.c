typedef Param2<Class, string> STestBeforeCallback;
typedef Param2<Class, string> STestBeforeClassCallback;
typedef Param2<Class, string> STestAfterCallback;
typedef Param2<Class, string> STestAfterClassCallback;

class STestUnit : Managed {
	
	protected bool shouldContinueAfterFail;
	protected bool hasFailed;
	
	protected ref array<ref STestCase> m_testCases = new array<ref STestCase>();
	protected ref array<ref STestBeforeCallback> m_beforeCallbacks = new array<ref STestBeforeCallback>();
	protected ref array<ref STestBeforeClassCallback> m_beforeClassCallbacks = new array<ref STestBeforeClassCallback>();
	protected ref array<ref STestAfterCallback> m_afterCallbacks = new array<ref STestAfterCallback>();
	protected ref array<ref STestAfterClassCallback> m_afterClassCallbacks = new array<ref STestAfterClassCallback>();
	
	protected STestCase m_currentTestCaseTested;
	protected STestCase m_lastTestCaseTested;
	
	void STestUnit(bool shouldContinue = true) {
		shouldContinueAfterFail = shouldContinue;
		init();
	}
		
	protected void init();
		
	protected void registerTestCases(TStringArray functionsNames) {
		foreach (string functionName : functionsNames) {
			m_testCases.Insert(new STestCase(this, functionName));
		}
	}
	
	protected void registerBeforeCallbacks(TStringArray functionsNames) {
		foreach (string functionName : functionsNames) {
			m_beforeCallbacks.Insert(new STestBeforeCallback(this, functionName));
		}
	}
	
	protected void registerBeforeClassCallbacks(TStringArray functionsNames) {
		foreach (string functionName : functionsNames) {
			m_beforeClassCallbacks.Insert(new STestBeforeClassCallback(this, functionName));
		}
	}
	
	protected void registerAfterCallbacks(TStringArray functionsNames) {
		foreach (string functionName : functionsNames) {
			m_afterCallbacks.Insert(new STestAfterCallback(this, functionName));
		}
	}
	
	protected void registerAfterClassCallbacks(TStringArray functionsNames) {
		foreach (string functionName : functionsNames) {
			m_afterClassCallbacks.Insert(new STestAfterClassCallback(this, functionName));
		}
	}
	
	array<ref STestCase> getTestCases() {
		return m_testCases;
	}
	
	bool hasFailed() {
		return hasFailed;
	}
	
	protected void continueAfterFail(bool shouldContinue) {
		shouldContinueAfterFail = shouldContinue;
	}
	
	void run() {
		executeBeforeClassCallbacks();
		foreach (STestCase testCase : m_testCases) {
			m_currentTestCaseTested = testCase;
			executeBeforeCallbacks(testCase);
			if (hasFailed && !shouldContinueAfterFail) {
				skip();
			} else {
				executeTestCase(testCase);
			}
			executeAfterCallbacks(testCase);
			m_lastTestCaseTested = testCase;
		}
		executeAfterClassCallbacks();
		m_currentTestCaseTested = null;
		m_lastTestCaseTested = null;
	}
	
	protected void executeBeforeClassCallbacks() {
		foreach (STestBeforeClassCallback beforeClassCallback : m_beforeClassCallbacks) {
			GetGame().GameScript.CallFunctionParams( beforeClassCallback.param1, beforeClassCallback.param2, null, null);
		}
	}
	
	protected void executeBeforeCallbacks(STestCase nextTestCase) {
		foreach (STestBeforeCallback beforeCallback : m_beforeCallbacks) {
			GetGame().GameScript.CallFunction( beforeCallback.param1, beforeCallback.param2, null, nextTestCase);
		}
	}
	
	protected void executeTestCase(STestCase testCase) {
		GetGame().GameScript.CallFunctionParams( testCase.getClass(), testCase.getFunction(), null, null);
	}
	
	protected void executeAfterCallbacks(STestCase lastTestCase) {
		foreach (STestAfterCallback afterCallback : m_afterCallbacks) {
			GetGame().GameScript.CallFunction( afterCallback.param1, afterCallback.param2, null, lastTestCase);
		}
	}
	
	protected void executeAfterClassCallbacks() {
		foreach (STestAfterClassCallback afterClassCallback : m_afterClassCallbacks) {
			GetGame().GameScript.CallFunctionParams( afterClassCallback.param1, afterClassCallback.param2, null, null);
		}
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