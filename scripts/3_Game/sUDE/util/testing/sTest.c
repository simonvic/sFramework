class STest : Managed {

	static int verbosity = 3;
	static string PASSED_OUTPUT  = "[ ✓ ] PASSED  - %1";
	static string FAILED_OUTPUT  = "[ × ] FAILED  - %1";
	static string SKIPPED_OUTPUT = "[ - ] SKIPPED - %1";
	
	protected static ref array<ref STestUnit> toTest = new array<ref STestUnit>();
	protected static ref map<eSTestCaseStatus, int> results = new map<eSTestCaseStatus, int>();
	
	static void init() {
		results.Clear();
		results.Set(eSTestCaseStatus.PASSED, 0);
		results.Set(eSTestCaseStatus.FAILED, 0);
		results.Set(eSTestCaseStatus.SKIPPED, 0);
		
		toTest.Clear();
		toTest.Insert(new TestUnit_SMath());
		toTest.Insert(new TestUnit_SConstraints());
		toTest.Insert(new TestUnit_SUserConfig());
	}
	
	static void run() {
		init();
		
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
	
	static void passed(STestCase testCase) {
		results.Set(eSTestCaseStatus.PASSED, results.Get(eSTestCaseStatus.PASSED) + 1);
		if (verbosity >= 2) {
			SLog.d(string.Format(PASSED_OUTPUT, testCase.getFunction()),"", 1);
			if (verbosity >= 3) {
				SLog.d("Expected: " + testCase.getExpected(),"",2);
				SLog.d("Actual:   " + testCase.getActual(),"",2);
			}
		}
	}
	
	static void failed(STestCase testCase) {
		results.Set(eSTestCaseStatus.FAILED, results.Get(eSTestCaseStatus.FAILED) + 1);
		SLog.d(string.Format(FAILED_OUTPUT, testCase.getFunction()),"", 1);
		SLog.d("Expected: " + testCase.getExpected(),"",2);
		SLog.d("Actual:   " + testCase.getActual(),"",2);
	}
	
	static void skipped(STestCase testCase) {
		results.Set(eSTestCaseStatus.SKIPPED, results.Get(eSTestCaseStatus.SKIPPED) + 1);
		if (verbosity >= 1) {
			SLog.d(string.Format(SKIPPED_OUTPUT, testCase.getFunction()),"", 1);
		}
	}
	
}

