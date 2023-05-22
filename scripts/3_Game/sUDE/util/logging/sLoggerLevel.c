class SLoggerLevel {
	static const string CFG_BASEPATH = "Levels";
	
	protected int m_severity;
	protected string m_label;
	protected string m_shortLabel;
	
	private void SLoggerLevel() {
	}
	
	static SLoggerLevel fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "Level");
		SLoggerLevel l = new SLoggerLevel();
		l.m_severity = SGameConfig.getChildrenNames(SGameConfig.CFG_BASENAME + " Logging " + CFG_BASEPATH).Find(className);
		l.m_label = SGameConfig.getString(path + " label");
		l.m_shortLabel = SGameConfig.getString(path + " shortLabel");
		return l;
	}
	
	int getSeverity() {
		return m_severity;
	}
	
	string getLabel() {
		return m_label;
	}
	
	string getShortLabel() {
		return m_shortLabel;
	}
}