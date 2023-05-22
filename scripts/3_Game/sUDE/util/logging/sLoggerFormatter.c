class SLoggerFormatter {
	static const string CFG_BASEPATH = "Formatters";
	
	protected ref array<string> m_entries;
	
	static SLoggerFormatter fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "Default");
		string type = SGameConfig.getString(path + " type");
		switch (type) {
			case "JSON": return SLoggerFormatterJSON.fromConfig(className);
			case "CSV": return SLoggerFormatterCSV.fromConfig(className);
		}
		SLoggerFormatter f = new SLoggerFormatter();
		f.m_entries = SGameConfig.getStringArray(path + " entries");
		return f;
	}
	
	string format(SLoggerContext ctx) {
		string formatted;
		foreach (string entry : m_entries) {
			formatted += ctx.injected(entry);
		}
		return formatted;
	}
}



class SLoggerFormatterJSON : SLoggerFormatter {
	
	override static SLoggerFormatterJSON fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "JSON");
		SLoggerFormatterJSON f = new SLoggerFormatterJSON();
		f.m_entries = SGameConfig.getStringArray(path + " entries");
		return f;
	}
	
	override string format(SLoggerContext ctx) {
		string formatted;
		foreach (string entry : m_entries) {
			entry.Replace("'", "\"");
			formatted += ctx.injected(entry);
		}
		return formatted;
	}
}



class SLoggerFormatterCSV : SLoggerFormatter {
	
	protected bool m_forceDoubleQuotes;
	protected string m_separator;
	
	override static SLoggerFormatterCSV fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "CSV");
		SLoggerFormatterCSV f = new SLoggerFormatterCSV();
		f.m_entries = SGameConfig.getStringArray(path + " entries");
		f.m_forceDoubleQuotes = SGameConfig.getBool(path + " forceDoubleQuotes");
		f.m_separator = SGameConfig.getString(path + " separator");
		return f;
	}
	
	override string format(SLoggerContext ctx) {
		string formatted;
		string injected;
		for (int i = 0; i < m_entries.Count() - 1; i++) {
			injected = ctx.injected(m_entries[i]);
			if (m_forceDoubleQuotes || injected.IndexOf(m_separator) != -1) {
				injected = "\"" + injected + "\"";
			}
			formatted += injected + m_separator;
		}
		injected = ctx.injected(m_entries[i]);
		if (m_forceDoubleQuotes || injected.IndexOf(m_separator) != -1) {
			formatted += "\"" + injected + "\"";
		}
		return formatted;
	}
}

