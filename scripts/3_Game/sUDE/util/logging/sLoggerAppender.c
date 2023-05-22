class SLoggerAppender {
	
	static const string CFG_BASEPATH = "Appenders";
	
	protected ref array<string> m_header;
	protected ref array<string> m_footer;
	ref SLoggerFormatter m_formatter;
	ref array<ref SLoggerWriter> m_writers;
	
	protected bool m_headerPrinted;
	
	private void SLoggerAppender() {
	}

	static SLoggerAppender fromConfig(string className) {
		string path = SLogger.getCfgPathOrDefault(CFG_BASEPATH, className, "Default");
		SLoggerAppender a = new SLoggerAppender();
		a.m_header = SGameConfig.getStringArray(path + " header");
		a.m_footer = SGameConfig.getStringArray(path + " footer");
		a.m_formatter = SLoggerFormatter.fromConfig(SGameConfig.getString(path + " formatter"));
		
		array<string> writersClassnames = SGameConfig.getStringArray(path + " writers");
		a.m_writers = {};
		foreach (string writerClassname : writersClassnames) {
			a.m_writers.Insert(SLoggerWriter.fromConfig(writerClassname));
		}
		return a;
	}
	
	void append(SLoggerContext ctx) {
		foreach (SLoggerWriter writer : m_writers) {
			writer.write(ctx, m_formatter.format(ctx));
		}
	}
	
	void appendHeader(SLoggerContext ctx) {
		if (m_header.Count() <= 0) return;
		string header;
		foreach (string headerLine : m_header) {
			header += headerLine + "\n";
		}
		foreach (SLoggerWriter w : m_writers) {
			w.write(ctx, ctx.injected(header));
		}
	}
	
	void appendFooter(SLoggerContext ctx) {
		if (m_footer.Count() <= 0) return;
		Print(this);
		string footer;
		foreach (string footerLine : m_footer) {
			footer += footerLine + "\n";
		}
		foreach (SLoggerWriter w : m_writers) {
			w.write(ctx, ctx.injected(footer));
		}
	}
	
}