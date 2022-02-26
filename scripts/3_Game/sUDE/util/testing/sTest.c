class STest : Managed {

	static int verbosity = 3;
	static string PASSED_OUTPUT  = "[ ✓ ] PASSED  - %1";
	static string FAILED_OUTPUT  = "[ × ] FAILED  - %1";
	static string SKIPPED_OUTPUT = "[ - ] SKIPPED - %1";
	
	protected ref array<ref STestUnit> toTest = new array<ref STestUnit>();
	protected ref map<eSTestCaseStatus, int> results = new map<eSTestCaseStatus, int>();
	
	static void run(typename testUnit) {
		STest t = new STest();
		t.addTestUnit(testUnit);
		t.run();
	}
	
	static void run(array<typename> testUnits) {
		STest t = new STest();
		t.addTestUnits(testUnits);
		t.run();
	}
	
	void clearTestUnits() {
		toTest.Clear();
	}
		
	void addTestUnit(typename testUnit) {
		if (!testUnit.IsInherited(STestUnit)) return;
		toTest.Insert(STestUnit.Cast(testUnit.Spawn()));
	}
	
	void addTestUnits(array<typename> testUnits) {
		foreach (typename testUnit : testUnits) {
			addTestUnit(testUnit);
		}
	}
	
	void run() {
		clearResults();
		
		SLog.d("===================================");
		SLog.d("Running tests...");
		foreach (STestUnit unit : toTest) {
			unit.run();
		}
		
		SLog.d("-----------------------------------");
		SLog.d("------------ RESULTS --------------");
		foreach (STestUnit unit2 : toTest) {
			SLog.d(unit2.ClassName());
			array<ref STestCase> testCases = unit2.getTestCases();
			foreach (STestCase testCase : testCases) {
				switch (testCase.getStatus()) {
					case eSTestCaseStatus.PASSED: passed(testCase); break;
					case eSTestCaseStatus.FAILED: failed(testCase); break;
					case eSTestCaseStatus.SKIPPED: skipped(testCase); break;
				}
			}
		}
		SLog.d("-----------------------------------");
		SLog.d("PASSED:  " + results.Get(eSTestCaseStatus.PASSED));
		SLog.d("FAILED:  " + results.Get(eSTestCaseStatus.FAILED));
		SLog.d("SKIPPED: " + results.Get(eSTestCaseStatus.SKIPPED));
		SLog.d("===================================");
	}
	
	protected void passed(STestCase testCase) {
		results.Set(eSTestCaseStatus.PASSED, results.Get(eSTestCaseStatus.PASSED) + 1);
		if (verbosity >= 2) {
			SLog.d(string.Format(PASSED_OUTPUT, testCase.getFunction()),"", 1);
			if (verbosity >= 3) {
				SLog.d("Expected: " + testCase.getExpected(),"",2);
				SLog.d("Actual:   " + testCase.getActual(),"",2);
			}
		}
	}
	
	protected void failed(STestCase testCase) {
		results.Set(eSTestCaseStatus.FAILED, results.Get(eSTestCaseStatus.FAILED) + 1);
		SLog.d(string.Format(FAILED_OUTPUT, testCase.getFunction()), "", 1);
		SLog.d("Expected: " + testCase.getExpected(), "", 2);
		SLog.d("Actual:   " + testCase.getActual(), "", 2);
		SLog.d("Message:  " + testCase.getMessage(), "", 2, testCase.getMessage() != string.Empty);
	}
	
	protected void skipped(STestCase testCase) {
		results.Set(eSTestCaseStatus.SKIPPED, results.Get(eSTestCaseStatus.SKIPPED) + 1);
		if (verbosity >= 1) {
			SLog.d(string.Format(SKIPPED_OUTPUT, testCase.getFunction()),"", 1);
		}
	}
	
	protected void clearResults() {
		results.Clear();
		results.Set(eSTestCaseStatus.PASSED, 0);
		results.Set(eSTestCaseStatus.FAILED, 0);
		results.Set(eSTestCaseStatus.SKIPPED, 0);
	}
	
}

