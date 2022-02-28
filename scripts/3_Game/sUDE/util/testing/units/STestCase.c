#ifdef DIAG_DEVELOPER

enum eSTestCaseStatus {
	PASSED,
	FAILED,
	SKIPPED
}

class STestCase : Managed {
	
	protected Class clazz;
	protected string function;
	protected eSTestCaseStatus status = eSTestCaseStatus.SKIPPED;
	protected string expected;
	protected string actual;
	protected string message;
	
	
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
	
	void setPassed() {
		status = eSTestCaseStatus.PASSED;
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
	
	string getMessage() {
		return message;
	}
	
	void setMessage(string cause) {
		message = cause;
	}

}

#endif