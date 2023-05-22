/**

lexer grammar SLangLexer;

OP_NOT          : '!' ;
OP_MODULO       : '%' ;
OP_AND          : '&' ;
QUOTE           : '\'' ;
LPAREN          : '(' ;
RPAREN          : ')' ;
OP_MULT         : '*' ;
OP_ADD          : '+' ;
OP_MINUS        : '-' ;
OP_DOT          : '.' ;
OP_DIV          : '/' ;
OP_LESS         : '<' ;
OP_EQUAL        : '=' ;
OP_GREATER      : '>' ;
OP_ACCESS       : '@' ;
OP_POWER        : '^' ;
OP_NOTEQUAL     : '!=' ;
OP_LESSEQUAL    : '<=' ;
OP_GREATEREQUAL : '>=' ;
OP_OR           : '|' ;
OP_CONTAINED    : '~=' ;
OP_CONTAINS     : '=~' ;

LITERAL_INTEGER    : [0-9]+ ;
LITERAL_FLOAT      : [0-9]+ '.' [0-9]+ ;
LITERAL_IDENTIFIER : [a-zA-Z_][a-zA-Z_0-9]* ;
LITERAL_STRING     : '\'' .*? '\'' ;
LITERAL_BOOL       : 'true' | 'false' ;
WS                 : [ \t\n\r\f]+ -> skip ;

*/
enum eSLangToken {
	UNKOWN = -1,
	LITERAL_IDENTIFIER,
	LITERAL_INT,
	LITERAL_FLOAT,
	LITERAL_BOOL,
	LITERAL_STRING,
	EOF,
	OP_NOT          = 33,  // "!".ToAscii(),
	OP_MODULO       = 37,  // "%".ToAscii(),
	OP_AND          = 38,  // "&".ToAscii(),
	QUOTE           = 39,  // "'".ToAscii(),
	LPAREN          = 40,  // "(".ToAscii(),
	RPAREN          = 41,  // ")".ToAscii(),
	OP_MULT         = 42,  // "*".ToAscii(),
	OP_ADD          = 43,  // "+".ToAscii(),
	OP_MINUS        = 45,  // "-".ToAscii(),
	OP_DOT          = 46,  // ".".ToAscii(),
	OP_DIV          = 47,  // "/".ToAscii(),
	OP_LESS         = 60,  // "<".ToAscii(),
	OP_EQUAL        = 61,  // "=".ToAscii(),
	OP_GREATER      = 62,  // ">".ToAscii(),
	OP_ACCESS       = 64,  // "@".ToAscii(),
	OP_POWER        = 94,  // "^".ToAscii(),
	OP_NOTEQUAL     = 94,  // "!".ToAscii() + "=".ToAscii() + 1,
	OP_LESSEQUAL    = 121, // "<".ToAscii() + "=".ToAscii(),
	OP_GREATEREQUAL = 123, // ">".ToAscii() + "=".ToAscii(),
	OP_OR           = 124  // "|".ToAscii(),
	OP_CONTAINED    = 187, // "~".ToAscii() + "=".ToAscii(),
	OP_CONTAINS     = 188, // "=".ToAscii() + "~".ToAscii() + 1
}

class SLangToken {
	eSLangToken rawToken;
	string text;
	int column;
	int row;
	
	void SLangToken(eSLangToken tk, string t, int c, int r) {
		rawToken = tk;
		text = t;
		column = c;
		row = r;
	}
	
	eSLangToken raw() {
		return rawToken;
	}
	
	string rawName() {
		return typename.EnumToString(eSLangToken, rawToken);
	}
}

class SLangLexer {
	
	int length;
	string code;
	int cursor = -1;
	int column;
	int row;
	ref array<ref SLangToken> tokens = {};
	ref array<eSLangToken> rawTokens = {};
	
	
	array<ref SLangToken> tokenize(string c) {
		rawTokens = {};
		tokens = {};
		code = c;
		length = c.Length();
		cursor = -1;
		
		string s;
		string char;
		eSLangToken token = eSLangToken.UNKOWN;
		while (peekForward(char)) {
			if (isNewline(char)) {
				row++;
				column = 0;
				continue;
			}
			if (isWhitespace(char)) continue;
			if (isNumber(char)) {
				s += char;
				while (peekForward(char)) {
					if (isNumber(char) || char == ".") {
						s += char;
					} else {
						peekBackward(char);
						break;
					}
				}
				if (s.Contains(".")) {
					token = eSLangToken.LITERAL_FLOAT;
				} else {
					token = eSLangToken.LITERAL_INT;
				}
			} else if (isAlphabetic(char)) {
				s += char;
				while (peekForward(char)) {
					if (isAlphanumeric(char) || char == "_") {
						s += char;
					} else {
						peekBackward(char);
						break;
					}
				}
				if (s == "true" || s == "false") {
					token = eSLangToken.LITERAL_BOOL;
				} else {
					token = eSLangToken.LITERAL_IDENTIFIER;
				}
			} else {
				switch (char) {
					case "'":
					s = char;
					token = eSLangToken.LITERAL_STRING;
					while (peekForward(char)) {
						s += char;
						if (char == "'") {
							break;
						}
					}
					break;
					
					case "<":
					if (peekForward(char) && char == "=") {
						token = eSLangToken.OP_LESSEQUAL;
					} else {
						token = eSLangToken.OP_LESS;
						peekBackward(char);
					}
					break;
					
					case ">":
					if (peekForward(char) && char == "=") {
						token = eSLangToken.OP_GREATEREQUAL;
					} else {
						token = eSLangToken.OP_GREATER;
						peekBackward(char);
					}
					break;
					
					case "~":
					if (peekForward(char) && char == "=") {
						token = eSLangToken.OP_CONTAINED;
					} else {
						peekBackward(char);
					}
					break;
					
					case "=":
					if (peekForward(char) && char == "~") {
						token = eSLangToken.OP_CONTAINS;
					} else {
						token = eSLangToken.OP_EQUAL;
						peekBackward(char);
					}
					break;
					
					default:
					token = char.ToAscii();
					s = char;
				}
			}
			rawTokens.Insert(token);
			tokens.Insert(new SLangToken(token, s, column, row));
			s = "";
		}
		rawTokens.Insert(eSLangToken.EOF);
		tokens.Insert(new SLangToken(eSLangToken.EOF, "EOF", column + 1, row));
		return tokens;
	}
	
	array<eSLangToken> getRawTokens() {
		return rawTokens;
	}
	
	bool peekForward(out string char, int step = 1) {
		column++;
		cursor++;
		if (cursor >= length) {
			char = "";
		} else {
			char = code[cursor];
		}
		return char != "";
	}
	
	bool peekBackward(out string char, int step = 1) {
		column--;
		cursor--;
		if (cursor < 0) {
			char = "";
		} else {
			char = code[cursor];
		}
		return char != "";
	}
	
	static bool isNewline(string char) {
		return char == "\n";
	}
	
	static bool isWhitespace(string char) {
		return char == " " || char == "\t" || isNewline(char);
	}
	
	static bool isAlphanumeric(string char) {
		return isAlphabetic(char) || isNumber(char);
	}
	
	static bool isAlphabetic(string char) {
		int ascii = char.ToAscii();
		return Math.IsInRangeInt(ascii, 65, 90) || Math.IsInRangeInt(ascii, 97, 121);
	}
	
	static bool isNumber(string char) {
		return Math.IsInRangeInt(char.ToAscii(), 48, 57);
	}
	
}
