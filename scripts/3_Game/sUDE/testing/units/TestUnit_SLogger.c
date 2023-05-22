#ifdef DIAG_DEVELOPER
class TestUnit_SLogger : STestUnit {
	
	static ref SLogger logger = SLogger.of(TestUnit_SLogger);
	
	override void init() {
		logger.info("hello world!");
	}
}
#endif