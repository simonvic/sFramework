class STest : Managed {

	/**
	*	@brief Higher values will means more verbosity in logging
	*/
	static int verbosity = 3;
	
	/**
	*	@brief If set to true, next TestUnits will be run
	*/
	static bool shouldContinueAtFail = false;
	
	static string PASSED_OUTPUT  = "[ ✓ ] PASSED  - %1";
	static string FAILED_OUTPUT  = "[ × ] FAILED  - %1";
	static string SKIPPED_OUTPUT = "[ - ] SKIPPED - %1";
	
	protected ref array<ref STestUnit> toTest = new array<ref STestUnit>();
	protected ref map<eSTestCaseStatus, int> results = new map<eSTestCaseStatus, int>();
	
	/**
	*	@brief Run predefined set of tests, meant for sUDE mods
	*/
	static void runSUDE() {
		array<typename> tests = {
			TestUnit_STest,
			TestUnit_SMath,
			TestUnit_SConstraints,
			TestUnit_SUserConfigOption,
			TestUnit_SUserConfig,
			TestUnit_SObservableArray
		};
		STest.run(tests);
	}
	
	/**
	*	@brief Run a single TestUnit
	*	@param testUnit \p typename - Typename of TestUnit to run
	*/
	static void run(typename testUnit) {
		STest t = new STest();
		t.addTestUnit(testUnit);
		t.run();
	}
	
	/**
	*	@brief Run a single TestUnit
	*	@param testUnits \p array<typename> - array of typenames of TestUnits to run
	*/
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
		
		SLog.d("=======================================================================");
		SLog.d("Running tests...");
		
		foreach (STestUnit unit : toTest) {
			unit.run();
			if (unit.hasFailed() && !shouldContinueAtFail) break;
		}
		
		SLog.d("-----------------------------------------------------------------------");
		foreach (STestUnit unit2 : toTest) {
			if (unit2.hasFailed()) {
				SLog.d(string.Format(FAILED_OUTPUT, unit2.ClassName()));
			} else {
				SLog.d(string.Format(PASSED_OUTPUT, unit2.ClassName()));
			}
			array<ref STestCase> testCases = unit2.getTestCases();
			foreach (STestCase testCase : testCases) {
				switch (testCase.getStatus()) {
					case eSTestCaseStatus.PASSED: passed(testCase); break;
					case eSTestCaseStatus.FAILED: failed(testCase); break;
					case eSTestCaseStatus.SKIPPED: skipped(testCase); break;
				}
			}
		}
		SLog.d("-----------------------------------------------------------------------");
		SLog.d("                   PASSED    |    FAILED    |    SKIPPED");
		SLog.d("                     "+results.Get(eSTestCaseStatus.PASSED) + "              " + results.Get(eSTestCaseStatus.FAILED) + "               " + results.Get(eSTestCaseStatus.SKIPPED));
		SLog.d("=======================================================================");
		SLog.c("| NICE! |", "", 0, verbosity == 69);
	}
	
	
	/**
	*	@brief Update results and log the output when a TestCase has passed
	*	@param testCase \p STestCase - test case that has passed
	*/
	protected void passed(STestCase testCase) {
		results.Set(eSTestCaseStatus.PASSED, results.Get(eSTestCaseStatus.PASSED) + 1);
		if (verbosity >= 2) {
			SLog.d(string.Format(PASSED_OUTPUT, testCase.getFunction()),"", 1);
			if (verbosity >= 3) {
				SLog.d("Expected: " + testCase.getExpected(), "", 2, testCase.getExpected() != string.Empty);
				SLog.d("Actual:   " + testCase.getActual(), "", 2, testCase.getActual() != string.Empty);
				SLog.d("Message:  " + testCase.getMessage(), "", 2, testCase.getMessage() != string.Empty);
			}
		}
	}
	
	/**
	*	@brief Update results and log the output when a TestCase has failed
	*	@param testCase \p STestCase - test case that has failed
	*/
	protected void failed(STestCase testCase) {
		results.Set(eSTestCaseStatus.FAILED, results.Get(eSTestCaseStatus.FAILED) + 1);
		SLog.d(string.Format(FAILED_OUTPUT, testCase.getFunction()), "", 1);
		SLog.d("Expected: " + testCase.getExpected(), "", 2);
		SLog.d("Actual:   " + testCase.getActual(), "", 2);
		SLog.d("Message:  " + testCase.getMessage(), "", 2, testCase.getMessage() != string.Empty);
	}
	
	/**
	*	@brief Update results and log the output when a TestCase has been skipped
	*	@param testCase \p STestCase - test case that has been skipped
	*/
	protected void skipped(STestCase testCase) {
		results.Set(eSTestCaseStatus.SKIPPED, results.Get(eSTestCaseStatus.SKIPPED) + 1);
		if (verbosity >= 1) {
			SLog.d(string.Format(SKIPPED_OUTPUT, testCase.getFunction()),"", 1);
			SLog.d("Message:  " + testCase.getMessage(),"",2, testCase.getMessage() != string.Empty);
		}
	}
	
	protected void clearResults() {
		results.Clear();
		results.Set(eSTestCaseStatus.PASSED, 0);
		results.Set(eSTestCaseStatus.FAILED, 0);
		results.Set(eSTestCaseStatus.SKIPPED, 0);
	}
	
}

