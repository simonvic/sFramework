class SLang {
	
	static SVariable run(string code, bool logs = false) {
		auto lexer = new SLangLexer();
		auto parser = new SLangParser();
		auto interpreter = new SLangInterpreter();
		SLog.d("============================================================================ sLang", "", 0, logs);
		SLog.d(code, "input", 0, logs);
		
		SLog.d("---------------------------------------------------------------------------- LEXER", "", 0, logs);
		array<ref SLangToken> tokens = lexer.tokenize(code);
		SLog.d("Tokens", "", 0, logs);
		foreach (SLangToken token : tokens) {
			SLog.d(string.Format("- %2 %1", "\t" + token.text, typename.EnumToString(eSLangToken, token.raw())), "", 0, logs);
		}
		
		SLog.d("---------------------------------------------------------------------------- PARSER", "", 0, logs);
		SLangAST ast = parser.parse(tokens);
		if (logs) ast.printDebug();
		
		SLog.d("---------------------------------------------------------------------------- INTERPRETER", "", 0, logs);
		SVariable v = interpreter.interpret(ast);
		SLog.d(v.toString(), ""+ v.Type());
		SLog.d("============================================================================", "", 0, logs);
		return v;
	}
}