
/**
*	
*/
class SLoggerContext {
	
	protected ref map<string, string> m_data;
	
	void SLoggerContext() {
		m_data = new map<string, string>();
	}
	
	
	void store(string identifier, string value) {
		m_data.Set(identifier, value);
	}
	
	string fetch(string identifier) {
		if (!m_data.Contains(identifier)) return "";
		return m_data.Get(identifier);
	}
	
	
	/**
	*	value
	*	value.value
	*	value:value
	*
	string fetchVariable(string name) {
		int i = name.IndexOf("date:");
		if (i != -1) {
			return SDate.now().format(name.Substring(5, name.Length() - 5));
		}
		string v;
		fetch(name, v);
		return v;
		i = name.IndexOf(".");
		if (i != -1) {
			string lhs = name.Substring(0, i);
			Managed tmp;
			SLoggerContext ctx;
			if (!fetch(lhs, tmp) || !Class.CastTo(ctx, tmp)) return "";
			string rhs = name.Substring(i + 1, name.Length() - i - 1);
			return ctx.fetchVariable(rhs);
		}
		
		return "" + fetch(name).toString();
	}
	*/
	
	string injected(string text) {
		return SLoggerContext.inject(this, text);
	}
	
	void inject(out string text) {
		text = SLoggerContext.inject(this, text);
	}
	
	/**
	*	${value}
	*	${value.value}
	*	${date:format}
	*/
	static string inject(SLoggerContext ctx, string text) {
		int opener;
		int closer;
		string variable;
		while (opener != -1) {
			opener = text.IndexOf("${");
			closer = text.IndexOfFrom(opener, "}");
			if (closer == -1) return text;
			variable = text.Substring(opener + 2, closer - opener - 2);
			// TODO: hardcoded for now KISS :3
			if (variable.IndexOf("date:") != -1) {
				variable = SDate.now().format(variable.Substring(5, variable.Length() - 5));
			} else {
				variable = ctx.fetch(variable);
			}
			text = text.Substring(0, opener) + variable + text.Substring(closer + 1, text.Length() - closer - 1);
		}
		return text;
	}
}