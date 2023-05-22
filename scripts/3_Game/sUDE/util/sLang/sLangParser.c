class SLangParserCursor {
	int m_pos;
	protected ref array<ref SLangToken> m_tokens;
	
	void SLangParserCursor(array<ref SLangToken> tokens, int pos = 0) {
		m_pos = pos;
		m_tokens = tokens;
		if (!m_tokens) {
			m_tokens = {};
		}
	}
	
	int pos() {
		return m_pos;
	}
	
	bool atEnd() {
		return m_pos == m_tokens.Count();
	}
	
	bool atBeginning() {
		return m_pos == -1;
	}
	
	bool consume(out SLangParserResult result) {
		if (atEnd()) return false;
		result = new SLangParserResult(m_pos, m_pos + 1, new SLangAST(m_tokens[m_pos].text));
		m_pos++;
		return true;
	}
	
	bool vomit(int amount = 1) {
		m_pos -= amount;
		return true;
	}
	
	bool peek(out SLangToken token, int amount = 0) {
		int peekPos = m_pos + amount;
		if (!atEnd() && Math.IsInRangeInt(peekPos, 0, m_tokens.Count() - 1)) {
			token = m_tokens[peekPos];
		}
		return token != null;
	}
	
	
	static bool log = false;
	
	bool consumeIf(eSLangToken expected, out SLangParserResult result) {
		SLog.d("------------------", "", 2, log);
		SLog.d("Consuming if expected: " + typename.EnumToString(eSLangToken, expected), "", 2, log);
		SLangToken token;
		if (!peek(token)) return false;
//		SLog.d("found token " + token.rawName(), "", 3, log);
		if (token.raw() == expected) {
			SLog.d("found " + token.rawName(), "", 3, log);
			SLog.d("------------------", "", 2, log);
			return consume(result);
		}
		SLog.d("not found! Found instead " + token.rawName(), "", 3, log);
		SLog.d("------------------", "", 2, log);
		return false;
	}
	
	bool consumeIf(array<eSLangToken> expected, out SLangParserResult result) {
		SLog.d("------------------", "", 2, log);
		string rawNames;
		foreach (eSLangToken t : expected) {
			rawNames += typename.EnumToString(eSLangToken, t) + " ";
		}
		SLog.d("Consuming if expected: " + rawNames, "", 2, log);
		SLangToken token;
		if (!peek(token)) return false;
//		SLog.d("found token " + token.rawName(), "", 3, log);
		if (expected.Find(token.raw()) != -1) {
			SLog.d("found " + token.rawName(), "", 3, log);
			SLog.d("------------------", "", 2, log);
			return consume(result);
		}
		SLog.d("not found! Found instead " + token.rawName(), "", 3, log);
		SLog.d("------------------", "", 2, log);
		return false;
	}
	
}

class SLangParserResult {
	int start;
	int end;
	ref SLangAST ast;
	
	void SLangParserResult(int s, int e, SLangAST a) {
		start = s;
		end = e;
		ast = a;
	}
	
	string text() {
		return ast.getData();
	}
	
}


class SLangParser {
	
	ref array<ref SLangToken> m_tokens;
	ref SLangParserCursor m_cursor;
	int m_length;
	ref SLangAST m_root;	
	
	string textOf(SLangParserResult r) {
		return "";
	}
	
	SLangAST parse(array<ref SLangToken> tokens) {
		m_tokens = tokens;
		m_cursor = new SLangParserCursor(tokens);
		m_root = new SLangAST("root");
		SLangParserResult begin = parseBegin();
		if (begin) {
			m_root.addChild(begin.ast);
		}
		return m_root;
		
	}
	
	SLangParserResult parseBegin(int start = 0) {
		string text;
		
		SLangParserResult expr = parseExpression(start);
		if (!expr) return null;
		
		SLangParserResult eof = parseEOF(start + expr.end);
		if (!eof) return null;
		
		auto beginContext = new SLangASTContext_begin(expr.text() + eof.text());
		beginContext.addChild(expr.ast);
		beginContext.addChild(eof.ast);
		return new SLangParserResult(start, eof.end, beginContext);
		
	}
	
	SLangParserResult parseEOF(int start) {
		SLangParserResult eof;
		if (m_cursor.consumeIf(eSLangToken.EOF, eof)) {
			auto eofContext = new SLangASTContext_literal("EOF");
			return new SLangParserResult(start, m_cursor.pos(), eofContext);
		}
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parseExpression(int start) {
		SLangParserResult expression;
		
		expression = parseParenthesizedExpression(start);
		if (expression) {
			return expression;
		}
		
		expression = parseDotExpression(start);
		if (expression) {
			return expression;
		}
		
		expression = parseBinaryExpression(start);
		if (expression) {
			return expression;
		}
		
		expression = parsePrefixedExpression(start);
		if (expression) {
			return expression;
		}
		
		expression = parseLiteral(start);
		if (expression) {
			return expression;
		}
		
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parseSimpleExpression(int start) {
		SLangParserResult expression;
		
		expression = parseParenthesizedExpression(start);
		if (expression) {
			return expression;
		}
		
		expression = parseDotExpression(start);
		if (expression) {
			return expression;
		}
		
		expression = parsePrefixedExpression(start);
		if (expression) {
			return expression;
		}
		
		expression = parseLiteral(start);
		if (expression) {
			return expression;
		}
		
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parseParenthesizedExpression(int start) {
		SLangParserResult lparen;
		if (m_cursor.consumeIf(eSLangToken.LPAREN, lparen)) {
			SLangParserResult expr = parseExpression(m_cursor.pos());
			if (expr) {
				SLangParserResult rparen;
				if (m_cursor.consumeIf(eSLangToken.RPAREN, rparen)) {
					auto parenthesizedContext = new SLangASTContext_parenthesizedExpression(lparen.text() + expr.text() + rparen.text());
					parenthesizedContext.addChild(lparen.ast);
					parenthesizedContext.addChild(expr.ast);
					parenthesizedContext.addChild(rparen.ast);
					return new SLangParserResult(start, m_cursor.pos(), parenthesizedContext);
				}
			}
		}
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parseDotExpression(int start) {
		SLangParserResult identifier;
		if (m_cursor.consumeIf(eSLangToken.LITERAL_IDENTIFIER, identifier)) {
			SLangParserResult dot;
			if (m_cursor.consumeIf(eSLangToken.OP_DOT, dot)) {
				SLangParserResult expr = parseExpression(m_cursor.pos());
				if (expr) {
					auto dotContext = new SLangASTContext_dotExpression(dot.text() + expr.text());
					return new SLangParserResult(start, m_cursor.pos(), dotContext);
				}
			}
		}
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parseBinaryExpression(int start) {
		SLangParserResult lhs = parseSimpleExpression(start);
		if (lhs) {
			SLangParserResult bop = parseBinaryOperator(m_cursor.pos());
			if (bop) {
				SLangParserResult rhs = parseExpression(m_cursor.pos());
				if (rhs) {
					auto binaryContext = new SLangASTContext_binaryExpression(lhs.text() + bop.text() + rhs.text());
					binaryContext.addChild(lhs.ast);
					binaryContext.addChild(bop.ast);
					binaryContext.addChild(rhs.ast);
					return new SLangParserResult(start, m_cursor.pos(), binaryContext);
				}
			}
		}
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parseBinaryOperator(int start) {
		array<eSLangToken> expected = {
			eSLangToken.OP_ACCESS
			eSLangToken.OP_MULT
			eSLangToken.OP_POWER
			eSLangToken.OP_DIV
			eSLangToken.OP_MODULO
			eSLangToken.OP_ADD
			eSLangToken.OP_MINUS
			eSLangToken.OP_LESSEQUAL
			eSLangToken.OP_GREATEREQUAL
			eSLangToken.OP_GREATER
			eSLangToken.OP_LESS
			eSLangToken.OP_EQUAL
			eSLangToken.OP_NOTEQUAL
			eSLangToken.OP_CONTAINED
			eSLangToken.OP_CONTAINS
			eSLangToken.OP_AND
			eSLangToken.OP_OR
		};
		SLangParserResult bop;
		if (m_cursor.consumeIf(expected, bop)) {
			return new SLangParserResult(start, m_cursor.pos(), bop.ast);
		}
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parsePrefixedExpression(int start) {
		array<eSLangToken> expected = {
			eSLangToken.OP_MINUS
			eSLangToken.OP_NOT
		};
		SLangParserResult prefixResult;
		if (m_cursor.consumeIf(expected, prefixResult)) {
			SLangParserResult expressionResult = parseExpression(m_cursor.pos());
			if (expressionResult) {
				auto prefixedContext = new SLangASTContext_prefixedExpression(prefixResult.text() + expressionResult.text());
				prefixedContext.addChild(prefixResult.ast);
				prefixedContext.addChild(expressionResult.ast);
				return new SLangParserResult(start, m_cursor.pos(), prefixedContext);
			}
		}
		m_cursor.m_pos = start;
		return null;
	}
	
	SLangParserResult parseLiteral(int start) {
		SLangParserResult literal;
		if (m_cursor.consumeIf(eSLangToken.LITERAL_INT, literal)) {
			return new SLangParserResult(start, m_cursor.pos(), new SLangASTContext_literalInteger(literal.text()));
		}
		
		if (m_cursor.consumeIf(eSLangToken.LITERAL_FLOAT, literal)) {
			return new SLangParserResult(start, m_cursor.pos(), new SLangASTContext_literalFloat(literal.text()));
		}
		
		if (m_cursor.consumeIf(eSLangToken.LITERAL_BOOL, literal)) {
			return new SLangParserResult(start, m_cursor.pos(), new SLangASTContext_literalBool(literal.text()));
		}
		
		if (m_cursor.consumeIf(eSLangToken.LITERAL_STRING, literal)) {
			return new SLangParserResult(start, m_cursor.pos(), new SLangASTContext_literalString(literal.text()));
		}
		
		if (m_cursor.consumeIf(eSLangToken.LITERAL_IDENTIFIER, literal)) {
			return new SLangParserResult(start, m_cursor.pos(), new SLangASTContext_literalIdentifier(literal.text()));
		}
		
		m_cursor.m_pos = start;
		return null;
		
	}
	
}