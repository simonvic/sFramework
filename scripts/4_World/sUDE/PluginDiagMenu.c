#ifdef DIAG_DEVELOPER
modded class PluginDiagMenu {

	protected int m_id_sUDE;
	protected int m_id_sUDE_sFramework;
	protected int m_id_sUDE_sFramework_testsVerbosity;
	protected int m_id_sUDE_sFramework_continueTestsAtFail;
	protected int m_id_sUDE_sFramework_runTests;
	protected int m_id_sUDE_sFramework_logLevel;
	protected int m_id_sUDE_sFramework_logSimpleFloat;

	override void RegisterModdedDiagsIDs() {
		super.RegisterModdedDiagsIDs();	
		m_id_sUDE = GetModdedDiagID();
		m_id_sUDE_sFramework = GetModdedDiagID();
		m_id_sUDE_sFramework_testsVerbosity = GetModdedDiagID();
		m_id_sUDE_sFramework_continueTestsAtFail = GetModdedDiagID();
		m_id_sUDE_sFramework_runTests = GetModdedDiagID();
		m_id_sUDE_sFramework_logLevel = GetModdedDiagID();
		m_id_sUDE_sFramework_logSimpleFloat = GetModdedDiagID();
	}

	override void RegisterModdedDiags() {
		super.RegisterModdedDiags();
		DiagMenu.RegisterMenu( m_id_sUDE, "sUDE", GetModdedRootMenu());
		DiagMenu.RegisterMenu( m_id_sUDE_sFramework, "sFramework", m_id_sUDE);
		DiagMenu.RegisterRange(m_id_sUDE_sFramework_testsVerbosity,      "", "sTest | verbosity",        m_id_sUDE_sFramework, "0, 5, 3, 1", simonvicCallback_sFramework_testsVerbsity);
		DiagMenu.RegisterBool( m_id_sUDE_sFramework_continueTestsAtFail, "", "sTest | continue at fail", m_id_sUDE_sFramework, false, simonvicCallback_sFramework_continueTestsAtFail);
		DiagMenu.RegisterBool( m_id_sUDE_sFramework_runTests,            "", "sTest | Run",              m_id_sUDE_sFramework, false, simonvicCallback_sFramework_runTests);
		DiagMenu.RegisterItem( m_id_sUDE_sFramework_logLevel,            "", "sLOG  | Level",            m_id_sUDE_sFramework, "DEBUG, INFO, WARNING, ERROR, CRITICAL, NONE", simonvicCallback_sFramework_logLevel);
		DiagMenu.RegisterBool( m_id_sUDE_sFramework_logSimpleFloat,      "", "sLOG  | Simple float",     m_id_sUDE_sFramework, false, simonvicCallback_sFramework_logSimpleFloat);
	}


	protected int getSUDERootMenu() {
		return m_id_sUDE;
	}

	static void simonvicCallback_sFramework_runTests(bool value) {
		STest.runSUDE();
	}

	static void simonvicCallback_sFramework_continueTestsAtFail(bool value) {
		STest.shouldContinueAtFail = value;
	}

	static void simonvicCallback_sFramework_testsVerbsity(float value) {
		STest.verbosity = value;
	}

	static void simonvicCallback_sFramework_logLevel(int value) {
		SLOG.LEVEL = Math.Clamp(value, SLoggerLevels.DEBUG, SLoggerLevels.COUNT);
	}

	static void simonvicCallback_sFramework_logSimpleFloat(bool value) {
		SLOG.SIMPLE_FLOAT = value;
	}

}

#endif
