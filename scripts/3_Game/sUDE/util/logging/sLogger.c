enum eSLoggerSeverity {
	TRACE,
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL,
	FATAL
}

class SLogger {
	
	static ref SLogger defaultLogger = SLogger.of("Default");
	
	static string getCfgPathOrDefault(string basepath, string className, string defaultClassName) {
		string path = string.Format("%1 %2 %3 %4", SGameConfig.CFG_BASENAME, "Logging", basepath, className);
		if (GetGame().ConfigIsExisting(path)) {
			return path;
		}
		return string.Format("%1 %2 %3 %4", SGameConfig.CFG_BASENAME, "Logging", basepath, defaultClassName);
	}
	
	protected bool m_initialized;
	protected string m_name = "";
	protected int m_severityThreshold;
	ref array<ref SLoggerLevel> m_levels = {};
	ref array<ref SLoggerAppender> m_appenders = {};
	
	protected int m_indentation = 0;
	
	protected bool m_persistCaller;
	protected bool m_persistIndentation;
	protected bool m_muted;
	protected ref SLoggerContext m_ctx;
	
	
	private void SLogger(string name) {
		m_name = name;
	}
	
	void ~SLogger() {
		foreach (SLoggerAppender appender : m_appenders) {
			appender.appendFooter(m_ctx);
		}
	}
	
	static SLogger of(typename name) {
		return new SLogger(name.ToString());
	}
	
	static SLogger of(string name) {
		return new SLogger(name);
	}
	
	protected void init() {
		m_ctx = new SLoggerContext();
		m_ctx.store("module", Type().GetModule());
		m_ctx.store("mod.name", "sUDE");
		m_ctx.store("mod.version", "v2.0");
		m_ctx.store("logger.name", m_name);
		
		m_ctx.store("dayz.profileName", GetProfileName());
		m_ctx.store("dayz.machineName", GetMachineName());
		//m_ctx.store("dayz.averageFPS", ""+GetGame().GetFPS());
		m_ctx.store("dayz.time", ""+GetGame().GetTime());
		m_ctx.store("dayz.tickTime", ""+GetGame().GetTickTime());
		string s;
		array<ref ModInfo> mods = {};
		GetGame().GetModInfos(mods);
		foreach (ModInfo mod : mods) {
			s += mod.GetName() + ",";
		}
		m_ctx.store("dayz.mods", s);		
		
		GetGame().GetVersion(s);
		m_ctx.store("dayz.version", s);

		GetGame().GetWorldName(s);
		m_ctx.store("dayz.worldName", s);
		
		string path = SLogger.getCfgPathOrDefault("Loggers", m_name, "Default");
		m_severityThreshold = SGameConfig.getInt(path + " severityThreshold");
		
		array<string> temp = SGameConfig.getStringArray(path + " levels");
		foreach (string levelClassname : temp) {
			m_levels.Insert(SLoggerLevel.fromConfig(levelClassname));
		}
		
		temp = SGameConfig.getStringArray(path + " appenders");
		foreach (string appenderClassname : temp) {
			SLoggerAppender appender = SLoggerAppender.fromConfig(appenderClassname);
			m_appenders.Insert(appender);
			appender.appendHeader(m_ctx);
		}
		m_initialized = true;
		
	}
	
	SLogger caller(string caller, bool persistent = false) {
		if (!m_initialized) init();
		m_ctx.store("caller", caller);
		m_persistCaller = persistent;
		return this;
	}
	
	SLogger indent(int indentation = 1, bool persistent = false) {
		m_indentation += indentation;
		m_persistIndentation = persistent;
		return this;
	}
	
	void mute() {
		m_muted = true;
	}
	
	void unmute() {
		m_muted = false;
	}

	/**
	*/
	void d(string variable) {
		log(variable, eSLoggerSeverity.DEBUG);
	}
	
	void d(typename variable) {
		d(toString(variable));
	}
	
	void d(float variable) {
		d(toString(variable));
	}
	
	void d(int variable) {
		d(toString(variable));
	}
	
	void d(bool variable) {
		d(toString(variable));
	}
	
	void d(vector variable) {
		d(toString(variable));
	}
	
	void d(Class variable) {
		d(toString(variable));
	}
	
	/**
	*/
	void info(string variable) {
		log(variable, eSLoggerSeverity.INFO);
	}
	
	void info(typename variable) {
		info(toString(variable));
	}
	
	void info(float variable) {
		info(toString(variable));
	}
	
	void info(int variable) {
		info(toString(variable));
	}
	
	void info(bool variable) {
		info(toString(variable));
	}
	
	void info(vector variable) {
		info(toString(variable));
	}
	
	void info(Class variable) {
		info(toString(variable));
	}
	
		
	/**
	*/
	void warning(string variable) {
		log(variable, eSLoggerSeverity.WARNING);
	}
	
	void warning(typename variable) {
		warning(toString(variable));
	}
	
	void warning(float variable) {
		warning(toString(variable));
	}
	
	void warning(int variable) {
		warning(toString(variable));
	}
	
	void warning(bool variable) {
		warning(toString(variable));
	}
	
	void warning(vector variable) {
		warning(toString(variable));
	}
	
	void warning(Class variable) {
		warning(toString(variable));
	}
	
	/**
	*/
	void error(string variable) {
		log(variable, eSLoggerSeverity.ERROR);
	}
	
	void error(typename variable) {
		error(toString(variable));
	}
	
	void error(float variable) {
		error(toString(variable));
	}
	
	void error(int variable) {
		error(toString(variable));
	}
	
	void error(bool variable) {
		error(toString(variable));
	}
	
	void error(vector variable) {
		error(toString(variable));
	}
	
	void error(Class variable) {
		error(toString(variable));
	}
	
	/**
	*/
	void critical(string variable) {
		log(variable, eSLoggerSeverity.CRITICAL);
	}
	
	void critical(typename variable) {
		critical(toString(variable));
	}
	
	void critical(float variable) {
		critical(toString(variable));
	}
	
	void critical(int variable) {
		critical(toString(variable));
	}
	
	void critical(bool variable) {
		critical(toString(variable));
	}
	
	void critical(vector variable) {
		critical(toString(variable));
	}
	
	void critical(Class variable) {
		critical(toString(variable));
	}
	
	/**
	*/
	void fatal(string variable) {
		log(variable, eSLoggerSeverity.FATAL);
	}
	
	void fatal(typename variable) {
		fatal(toString(variable));
	}
	
	void fatal(float variable) {
		fatal(toString(variable));
	}
	
	void fatal(int variable) {
		fatal(toString(variable));
	}
	
	void fatal(bool variable) {
		fatal(toString(variable));
	}
	
	void fatal(vector variable) {
		fatal(toString(variable));
	}
	
	void fatal(Class variable) {
		fatal(toString(variable));
	}
	
	
	/**
	*/
	void log(string variable, eSLoggerSeverity severity = eSLoggerSeverity.TRACE) {
		if (!m_initialized) {
			init();
		}
		SLoggerLevel level = m_levels[severity];
		if (level) {
			m_ctx.store("level.severity", ""+level.getSeverity());
			m_ctx.store("level.label", level.getLabel());
			m_ctx.store("level.shortLabel", level.getShortLabel());
		}
		m_ctx.store("message", variable);
		
		foreach (SLoggerAppender appender : m_appenders) {
			appender.append(m_ctx);
		}
		
		if (!m_persistIndentation) m_indentation = 0;
	}
	
	void log(typename variable) {
		log(toString(variable));
	}
	
	void log(float variable) {
		log(toString(variable));
	}
	
	void log(int variable) {
		log(toString(variable));
	}
	
	void log(bool variable) {
		log(toString(variable));
	}
	
	void log(vector variable) {
		log(toString(variable));
	}
	
	void log(Class variable) {
		log(toString(variable));
	}

	string toString(typename variable) {
		return string.ToString(variable, false, false, false);
	}

	string toString(float variable) {
		return string.ToString(variable, false, false, false);
	}

	string toString(int variable) {
		return string.ToString(variable, false, false, false);
	}

	string toString(bool variable) {
		return string.ToString(variable, false, false, false);
	}

	string toString(vector variable) {
		return string.ToString(variable, false, false, false);
	}

	string toString(Class variable) {
		return string.ToString(variable, false, false, false);
	}


}