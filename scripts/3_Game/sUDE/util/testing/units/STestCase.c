enum eSTestCaseStatus {
	PASSED,
	FAILED,
	SKIPPED
}

class STestCase : Managed {
	
	protected Class clazz;
	protected string function;
	protected eSTestCaseStatus status = eSTestCaseStatus.PASSED;
	protected string expected;
	protected string actual;
	
	
	void STestCase(Class classInstance, string functionName) {
		clazz = classInstance;
		function = functionName;
	}
	
	Class getClass() {
		return clazz;
	}
	
	string getFunction() {
		return function;
	}
	
	void setFailed() {
		status = eSTestCaseStatus.FAILED;
	}
	
	void setSkipped() {
		status = eSTestCaseStatus.SKIPPED;
	}
	
	eSTestCaseStatus getStatus() {
		return status;
	}
	
	string getExpected() {
		return expected;
	}
	
	void setExpected(string value) {
		expected = value;
	}
	
	string getActual() {
		return actual;
	}
	
	void setActual(string value) {
		actual = value;
	}

}
