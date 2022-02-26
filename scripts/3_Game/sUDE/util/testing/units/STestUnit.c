class STestUnit : Managed {	
	
	protected bool skipAfterFail;
	
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
	
	void run() {
		foreach (STestCase testCase : m_testCases) {
			m_currentTestCaseTested = testCase;
			if (shouldSkipAfterFail()) {
				skip();
			} else {
				GetGame().GameScript.CallFunctionParams( testCase.getClass(), testCase.getFunction(), null, null);
			}
			m_lastTestCaseTested = testCase;
		}
		m_currentTestCaseTested = null;
		m_lastTestCaseTested = null;
	}
	
	protected bool shouldSkipAfterFail() {
		return skipAfterFail && m_lastTestCaseTested && (m_lastTestCaseTested.getStatus() == eSTestCaseStatus.FAILED || m_lastTestCaseTested.getStatus() == eSTestCaseStatus.SKIPPED);
	}
	
	protected void assertEqual(array<float> expected, array<float> actual) {
		if (SMath.equal(expected, actual)) {
			passed(string.Format("%1", expected), string.Format("%1", actual));
		} else {
			failed(string.Format("%1", expected), string.Format("%1", actual));
		}
	}
	
	protected void assertEqual(float expected, float actual) {
		if (SMath.equal(expected, actual)) {
			passed(expected.ToString(), actual.ToString());
		} else {
			failed(expected.ToString(), actual.ToString());
		}
	}
	
	protected void assertEqual(int expected, int actual) {
		if (expected == actual) {
			passed(expected.ToString(), actual.ToString());
		} else {
			failed(expected.ToString(), actual.ToString());
		}
	}
	
	protected void assertEqual(string expected, string actual) {
		if (expected == actual) {
			passed(expected, actual);
		} else {
			failed(expected, actual);
		}
	}
	
	protected void assertEqual(bool expected, bool actual) {
		if (expected == actual) {
			passed(expected.ToString(), actual.ToString());
		} else {
			failed(expected.ToString(), actual.ToString());
		}
	}
	
	protected void passed(string expected, string actual) {
		m_currentTestCaseTested.setExpected(expected);
		m_currentTestCaseTested.setActual(actual);
	}
	
	protected void failed(string expected, string actual) {
		m_currentTestCaseTested.setFailed();
		m_currentTestCaseTested.setExpected(expected);
		m_currentTestCaseTested.setActual(actual);
	}
	
	protected void skip() {
		m_currentTestCaseTested.setSkipped();
	}
}