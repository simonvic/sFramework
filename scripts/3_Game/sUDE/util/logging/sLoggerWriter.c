class SLoggerWriter {
	static const string CFG_BASEPATH = "Writers";
	static SLoggerWriter fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "Writer");
		string type = SGameConfig.getString(path + " type");
		switch (type) {
			case "Console": return SLoggerWriterConsole.fromConfig(className);
			case "File": return SLoggerWriterFile.fromConfig(className);
			case "Remote": return SLoggerWriterRemote.fromConfig(className);
		}
		return new SLoggerWriter();
	}
	
	void write(SLoggerContext ctx, string message);
}

class SLoggerWriterConsole : SLoggerWriter {
	
	override static SLoggerWriterConsole fromConfig(string className) {
		return new SLoggerWriterConsole();
	}
	
	override void write(SLoggerContext ctx, string message) {
		PrintFormat("%1", message);
	}
}


class SLoggerWriterFile : SLoggerWriter {
	protected string m_logsFolder;
	protected string m_logFilename;
	
	override static SLoggerWriterFile fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "File");
		SLoggerWriterFile w = new SLoggerWriterFile();
		w.m_logsFolder = SGameConfig.getString(path + " logsFolder");
		w.m_logFilename = SGameConfig.getString(path + " logFilename");
		return w;
	}
	
	override void write(SLoggerContext ctx, string message) {
		string filePath = m_logsFolder + "\\" + m_logFilename;
		ctx.inject(filePath);
		SFileHelper.touch(filePath);
		FileHandle file = OpenFile(filePath, FileMode.APPEND);
		// TODO: handle error
		// TODO: cache file handle
		if (file != 0) {
			FPrintln(file, message);
		}
		CloseFile(file);
	}
}


class SLoggerWriterRemote : SLoggerWriter {

	protected string m_endpoint;
	protected ref array<string> m_header;
	
	override static SLoggerWriterRemote fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "Remote");
		SLoggerWriterRemote w = new SLoggerWriterRemote();
		w.m_endpoint = SGameConfig.getString(path + " endpoint");
		w.m_header = SGameConfig.getStringArray(path + " header");
		return w;
	}
	
	override void write(SLoggerContext ctx, string message) {
		RestContext rest = GetRestApi().GetRestContext(m_endpoint);
		string h;
		foreach (string header : m_header) {
			h += header;
		}
		rest.SetHeader(h);
		rest.POST(null, "", message);
	}
}