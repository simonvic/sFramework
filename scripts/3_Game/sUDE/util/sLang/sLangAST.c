/**
parser grammar SLangParser;
options { tokenVocab=SLangLexer; }

begin
	: expression* EOF
	;

expression
	: '(' expression ')'                #parenthesizedExpression
	| LITERAL_IDENTIFIER '.' expression #dotExpression
	| expression bop expression         #binaryExpression
	| prefixed                          #prefixedExpression
	| literal                           #literalExpression
	;

bop
    : '@'
	| '*'
	| '^'
	| '/'
	| '%'
	| '+'
	| '-'
	| '<='
	| '>='
	| '>'
	| '<'
	| '='
	| '!='
	| '~='
	| '=~'
	| '&'
	| '|'
	;

prefixed
	: prefix = '-' expression    #negativeExpression
	| prefix = '!' expression    #notExpression
	;

literal
	: LITERAL_INTEGER
	| LITERAL_FLOAT
	| LITERAL_BOOL
	| LITERAL_STRING
	| LITERAL_IDENTIFIER
	;

*/

class SLangAST : STreeNode<string> {
	void SLangAST(string code) {
		
	}
	
	SLangASTContext_begin begin() {
		return SLangASTContext_begin.Cast(getChildren()[0]);
	}
	
	SLangAST getChild(int i) {
		return SLangAST.Cast(m_children[i]);
	}
	
	string getText() {
		return getData();
	}
	
	void printDebug(int maxDepth = -1, int depth = 0) {
		SLog.d(getData(), ""+Type(), depth);
		if (maxDepth >= 0 && depth > maxDepth) return;
		foreach (STreeNode<string> child : m_children) {
			SLangAST.Cast(child).printDebug(maxDepth, depth + 1);
		}
	}
}

class SLangASTToken : SLangAST {}
class SLangASTContext : SLangAST {}

class SLangASTContext_begin : SLangASTContext  {
	array<ref SLangASTContext_expression> expressions() {
		array<ref SLangASTContext_expression> x = {};
		foreach (auto child : m_children) {
			SLangASTContext_expression expr = SLangASTContext_expression.Cast(child);
			if (expr) {
				x.Insert(expr);
			}
		}
		return x;
	}
	
	SLangASTContext_expression expression(int index) {
		return expressions()[index];
	}
}

class SLangASTContext_expression : SLangASTContext {}

class SLangASTContext_parenthesizedExpression : SLangASTContext_expression {
	SLangASTContext_expression expression() {
		return SLangASTContext_expression.Cast(getChild(1));
	}
}

class SLangASTContext_primalExpression : SLangASTContext_expression {
	SLangASTContext_primal primal() {
		return SLangASTContext_primal.Cast(getChild(0));
	}
}

class SLangASTContext_prefixedExpression : SLangASTContext_expression { 
	string prefix() {
		return getChild(0).getText();
	}
	
	SLangASTContext_expression expression() {
		return SLangASTContext_expression.Cast(getChild(1));
	}
}
class SLangASTContext_notExpression : SLangASTContext_prefixedExpression {}
class SLangASTContext_negativeExpression : SLangASTContext_prefixedExpression {}

class SLangASTContext_binaryExpression : SLangASTContext_expression {
	SLangASTContext_expression lhs() {
		return SLangASTContext_expression.Cast(getChild(0));
	}
	
	string bop() {
		return getChild(1).getText();
	}
	
	SLangASTContext_expression rhs() {
		return SLangASTContext_expression.Cast(getChild(2));
	}
}

class SLangASTContext_dotExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_accessExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_powerExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_multiplicativeExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_additiveExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_relationalExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_equalityExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_containmentExpression : SLangASTContext_binaryExpression {}
class SLangASTContext_boolExpression : SLangASTContext_binaryExpression {}


class SLangASTContext_primal : SLangASTContext_expression {}
class SLangASTContext_identifier : SLangASTContext_primal {
	string identifier() {
		return getData();
	}
}
class SLangASTContext_literal : SLangASTContext_primal {}
class SLangASTContext_literalInteger : SLangASTContext_literal {
	int intValue() {
		return getData().ToInt();
	}
}
class SLangASTContext_literalFloat : SLangASTContext_literal {
	float floatValue() {
		return getData().ToFloat();
	}
}
class SLangASTContext_literalBool : SLangASTContext_literal {
	bool boolValue() {
		return getData() == "true";
	}
}
class SLangASTContext_literalString : SLangASTContext_literal {
	string stringValue() {
		return getData();
	}
}
class SLangASTContext_literalIdentifier : SLangASTContext_literal {
	string identifier() {
		return getData();
	}
}