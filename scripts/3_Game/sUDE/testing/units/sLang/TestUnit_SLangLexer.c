#ifdef DIAG_DEVELOPER



class TestUnit_SLangLexer : STestUnit {
	override void init() {
		array<ref Param> argumentsList = {
			new Param2<string, eSLangToken>("!", eSLangToken.OP_NOT),
			new Param2<string, eSLangToken>("*", eSLangToken.OP_MULT),
			new Param2<string, eSLangToken>("+", eSLangToken.OP_ADD),
			new Param2<string, eSLangToken>("-", eSLangToken.OP_MINUS),
			new Param2<string, eSLangToken>(".", eSLangToken.OP_DOT),
			new Param2<string, eSLangToken>("/", eSLangToken.OP_DIV),
			new Param2<string, eSLangToken>(">", eSLangToken.OP_GREATER),
			new Param2<string, eSLangToken>("<", eSLangToken.OP_LESS),
			new Param2<string, eSLangToken>("@", eSLangToken.OP_ACCESS),
			new Param2<string, eSLangToken>("^", eSLangToken.OP_POWER),
			new Param2<string, eSLangToken>("&", eSLangToken.OP_AND),
			new Param2<string, eSLangToken>("|", eSLangToken.OP_OR)
		};
		registerTestCaseParameterized("test_unaryRawToken", argumentsList);
		
		
		registerTestCases({
			"test_unaryRawTokensOnSameLine"
			"test_mixedRawTokens"
			"test_alphanumericRawTokens"
		});
	}

	
	void test_unaryRawToken(string char, eSLangToken expected) {
		auto lexer = new SLangLexer();
		assertEqual(expected, lexer.tokenize(char)[0].rawToken);
	}
	
	void test_unaryRawTokensOnSameLine() {
		string code = "!*+-./><@^&|";
		array<eSLangToken> expected = {
			eSLangToken.OP_NOT
			eSLangToken.OP_MULT
			eSLangToken.OP_ADD
			eSLangToken.OP_MINUS
			eSLangToken.OP_DOT
			eSLangToken.OP_DIV
			eSLangToken.OP_GREATER
			eSLangToken.OP_LESS
			eSLangToken.OP_ACCESS
			eSLangToken.OP_POWER
			eSLangToken.OP_AND
			eSLangToken.OP_OR
			eSLangToken.EOF
		};
		auto lexer = new SLangLexer();
		lexer.tokenize(code);
		auto actual = lexer.getRawTokens();
		assertEqual(expected, actual);
	}
	
	void test_mixedRawTokens() {
		string code = "!=< = <==>>=~= =~";
		array<eSLangToken> expected = {
			eSLangToken.OP_NOT
			eSLangToken.OP_EQUAL			
			eSLangToken.OP_LESS
			eSLangToken.OP_EQUAL
			eSLangToken.OP_LESSEQUAL
			eSLangToken.OP_EQUAL
			eSLangToken.OP_GREATER
			eSLangToken.OP_GREATEREQUAL
			eSLangToken.OP_CONTAINED
			eSLangToken.OP_CONTAINS
			eSLangToken.EOF
		};
		auto lexer = new SLangLexer();
		lexer.tokenize(code);
		auto actual = lexer.getRawTokens();
		assertEqual(expected, actual);
	}
	
	void test_alphanumericRawTokens() {
		string code = "69 420.69 hello hello_world hello69_420 true false true_ false_ 'hello world'";
		array<eSLangToken> expected = {
			eSLangToken.LITERAL_INT
			eSLangToken.LITERAL_FLOAT
			eSLangToken.LITERAL_IDENTIFIER
			eSLangToken.LITERAL_IDENTIFIER
			eSLangToken.LITERAL_IDENTIFIER
			eSLangToken.LITERAL_BOOL
			eSLangToken.LITERAL_BOOL
			eSLangToken.LITERAL_IDENTIFIER
			eSLangToken.LITERAL_IDENTIFIER
			eSLangToken.LITERAL_STRING
			eSLangToken.EOF
		};
		auto lexer = new SLangLexer();
		lexer.tokenize(code);
		auto actual = lexer.getRawTokens();
		assertEqual(expected, actual);
	}
}

#endif
