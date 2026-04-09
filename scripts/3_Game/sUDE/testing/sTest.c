#ifdef DIAG_DEVELOPER

class STest : Managed {

	/**
	*	@brief Higher values will means more verbosity in logging
	*/
	static int verbosity = 3;

	/**
	*	@brief If set to true, next TestUnits will be run
	*/
	static bool shouldContinueAtFail = false;

	static string PASSED_OUTPUT  = "[ + ] PASSED  - %1";
	static string FAILED_OUTPUT  = "[ - ] FAILED  - %1";
	static string SKIPPED_OUTPUT = "[   ] SKIPPED - %1";

	protected ref array<ref STestUnit> toTest = new array<ref STestUnit>();
	protected ref map<eSTestCaseStatus, int> results = new map<eSTestCaseStatus, int>();

	/**
	*	@brief Run predefined set of tests, meant for sUDE mods
	*/
	static void runSUDE() {
		array<typename> tests = {
			//TestUnit_STest,
			TestUnit_SMath,
			TestUnit_SFlagOperator,
			TestUnit_SColor,
			TestUnit_SFileHelper,
			TestUnit_SConstraints,
			TestUnit_SUserConfigOption,
			TestUnit_SUserConfig,
			TestUnit_SObservableArray,
			TestUnit_SLOG
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

		SLOG.d("=======================================================================");
		SLOG.d("Running tests...");

		foreach (STestUnit unit : toTest) {
			unit.run();
			if (unit.hasFailed() && !shouldContinueAtFail) break;
		}

		SLOG.d("-----------------------------------------------------------------------");
		foreach (STestUnit unit2 : toTest) {
			if (unit2.hasFailed()) {
				SLOG.d(string.Format(FAILED_OUTPUT, unit2.ClassName()));
			} else {
				SLOG.d(string.Format(PASSED_OUTPUT, unit2.ClassName()));
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
		SLOG.d("-----------------------------------------------------------------------");
		SLOG.d("                   PASSED    |    FAILED    |    SKIPPED");
		SLOG.d("                     "+results.Get(eSTestCaseStatus.PASSED) + "              " + results.Get(eSTestCaseStatus.FAILED) + "               " + results.Get(eSTestCaseStatus.SKIPPED));
		SLOG.d("=======================================================================");
		if (verbosity == 69) SLOG.c("| NICE! |");
	}


	/**
	*	@brief Update results and log the output when a TestCase has passed
	*	@param testCase \p STestCase - test case that has passed
	*/
	protected void passed(STestCase testCase) {
		results.Set(eSTestCaseStatus.PASSED, results.Get(eSTestCaseStatus.PASSED) + 1);
		if (verbosity >= 2) {
			SLOG.d(1, string.Format(PASSED_OUTPUT, testCase.getFunction()));
			if (verbosity >= 3) {
				if (testCase.getExpected() != string.Empty) {
					SLOG.d(2, "Expected: " + testCase.getExpected());
				}
				if (testCase.getActual() != string.Empty) {
					SLOG.d(2, "Actual:   " + testCase.getActual());
				}
				if (testCase.getMessage() != string.Empty) {
					SLOG.d(2, "Message:  " + testCase.getMessage());
				}
			}
		}
	}

	/**
	*	@brief Update results and log the output when a TestCase has failed
	*	@param testCase \p STestCase - test case that has failed
	*/
	protected void failed(STestCase testCase) {
		results.Set(eSTestCaseStatus.FAILED, results.Get(eSTestCaseStatus.FAILED) + 1);
		SLOG.d(1, string.Format(FAILED_OUTPUT, testCase.getFunction()));
		SLOG.d(2, "Expected: " + testCase.getExpected());
		SLOG.d(2, "Actual:   " + testCase.getActual());
		if (testCase.getMessage() != string.Empty) {
			SLOG.d(2, "Message:  " + testCase.getMessage());
		}
	}

	/**
	*	@brief Update results and log the output when a TestCase has been skipped
	*	@param testCase \p STestCase - test case that has been skipped
	*/
	protected void skipped(STestCase testCase) {
		results.Set(eSTestCaseStatus.SKIPPED, results.Get(eSTestCaseStatus.SKIPPED) + 1);
		if (verbosity >= 1) {
			SLOG.d(1, string.Format(SKIPPED_OUTPUT, testCase.getFunction()));
			if (testCase.getMessage() != string.Empty) {
				SLOG.d(2, "Message:  " + testCase.getMessage());
			}
		}
	}

	protected void clearResults() {
		results.Clear();
		results.Set(eSTestCaseStatus.PASSED, 0);
		results.Set(eSTestCaseStatus.FAILED, 0);
		results.Set(eSTestCaseStatus.SKIPPED, 0);
	}

}



#endif
