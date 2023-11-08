#ifdef DIAG_DEVELOPER
modded class PluginDiagMenu {
	
	protected int m_id_sUDE;
	protected int m_id_sUDE_sFramework;
	protected int m_id_sUDE_sFramework_testsVerbosity;
	protected int m_id_sUDE_sFramework_continueTestsAtFail;
	protected int m_id_sUDE_sFramework_runTests;
	
	override void RegisterModdedDiagsIDs() {
		super.RegisterModdedDiagsIDs();	
		m_id_sUDE = GetModdedDiagID();
		m_id_sUDE_sFramework = GetModdedDiagID();
		m_id_sUDE_sFramework_testsVerbosity = GetModdedDiagID();
		m_id_sUDE_sFramework_continueTestsAtFail = GetModdedDiagID();
		m_id_sUDE_sFramework_runTests = GetModdedDiagID();
	}
	
	override void RegisterModdedDiags() {
		super.RegisterModdedDiags();
		DiagMenu.RegisterMenu( m_id_sUDE, "sUDE", GetModdedRootMenu());
		DiagMenu.RegisterMenu( m_id_sUDE_sFramework, "sFramework", m_id_sUDE);
		DiagMenu.RegisterRange(m_id_sUDE_sFramework_testsVerbosity,      "", "sTest | verbosity", m_id_sUDE_sFramework, "0, 5, 3, 1", simonvicCallback_sFramework_testsVerbsity);
		DiagMenu.RegisterBool( m_id_sUDE_sFramework_continueTestsAtFail, "", "sTest | continue at fail", m_id_sUDE_sFramework, false, simonvicCallback_sFramework_continueTestsAtFail);
		DiagMenu.RegisterBool( m_id_sUDE_sFramework_runTests,            "", "sTest | Run", m_id_sUDE_sFramework, false, simonvicCallback_sFramework_runTests);
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
}

#endif