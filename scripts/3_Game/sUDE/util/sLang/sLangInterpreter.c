class SLangInterpreter {
	SVariable interpret(SLangAST ast) {
		if (ast.begin()) {
			return visit_begin(ast.begin());
		}
		return new SVariableException("!!!Empty AST!!!");
	}
		
	SVariable visit_begin(SLangASTContext_begin ctx) {
		return visit_expression(ctx.expression(0));
	}

	SVariable visit_expression(SLangASTContext_expression ctx) {
		SLangASTContext_parenthesizedExpression parenthesizedExpressionCtx;
		if (Class.CastTo(parenthesizedExpressionCtx, ctx)) {			
			return visit_expression(parenthesizedExpressionCtx.expression());
		}
		SLangASTContext_binaryExpression binaryExpressionCtx;
		if (Class.CastTo(binaryExpressionCtx, ctx)) {			
			return visit_binaryExpression(binaryExpressionCtx);
		}
		SLangASTContext_prefixedExpression prefixedExpressionCtx;
		if (Class.CastTo(prefixedExpressionCtx, ctx)) {
			return visit_prefixedExpression(prefixedExpressionCtx);
		}
		SLangASTContext_primal primalCtx;
		if (Class.CastTo(primalCtx, ctx)) {
			return visit_primal(primalCtx);
		}
		return new SVariableException("invalid expression context");
	}
	
	SVariable visit_prefixedExpression(SLangASTContext_prefixedExpression ctx) {
		switch (ctx.prefix()) {
			case "!": return visit_expression(ctx.expression()).not();
			case "-": return visit_expression(ctx.expression()).negative();
		}
		return new SVariableException("invalid prefixed expression");
	}
	
	SVariable visit_binaryExpression(SLangASTContext_binaryExpression ctx) {
		SVariable lhs = visit_expression(ctx.lhs());
		SVariable rhs = visit_expression(ctx.rhs());
		switch (ctx.bop()) {
			case "*" : return lhs.multiply(rhs);
			case "/" : return lhs.divide(rhs);
			case "%": return lhs.modulo(rhs);
			case "^": return lhs.power(rhs);
			case "+" : return lhs.plus(rhs);
			case "-": return lhs.minus(rhs);
			case "<=" : return lhs.lessEqual(rhs);
			case ">=" : return lhs.greaterEqual(rhs);
			case ">" : return lhs.greater(rhs);
			case "<": return lhs.less(rhs);
			case "=" : return lhs.equal(rhs);
			case "!=": return lhs.notEqual(rhs);
			case "~=" : return lhs.isContained(rhs);
			case "=~": return lhs.contains(rhs);
			case "&" : return lhs.and(rhs);
			case "|": return lhs.or(rhs);
		}
		return new SVariableException("invalid binary expression");
	}

	SVariable visit_primalExpression(SLangASTContext_primalExpression ctx) {
		return visit_primal(ctx.primal());
	}
	
	SVariable visit_primal(SLangASTContext_primal ctx) {
		SLangASTContext_identifier identifierCtx;
		if (Class.CastTo(identifierCtx, ctx)) {
			return visit_identifier(identifierCtx);
		}
		SLangASTContext_literal literalCtx;
		if (Class.CastTo(literalCtx, ctx)) {
			return visit_literal(literalCtx);
		}
		return new SVariableException("invalid primal");
	}
	
	SVariable visit_identifier(SLangASTContext_identifier ctx) {
		return new SVariable_string("fetch variable from context");
	}
	
	SVariable visit_literal(SLangASTContext_literal ctx) {
		SLangASTContext_literalInteger literalIntegerCtx;
		if (Class.CastTo(literalIntegerCtx, ctx)) {
			return visit_literalInteger(literalIntegerCtx);
		}
		SLangASTContext_literalFloat literalFloatCtx;
		if (Class.CastTo(literalFloatCtx, ctx)) {
			return visit_literalFloat(literalFloatCtx);
		}
		SLangASTContext_literalBool literalBoolCtx;
		if (Class.CastTo(literalBoolCtx, ctx)) {
			return visit_literalBool(literalBoolCtx);
		}
		SLangASTContext_literalString literalStringCtx;
		if (Class.CastTo(literalStringCtx, ctx)) {
			return visit_literalString(literalStringCtx);
		}
		return new SVariableException("invalid literal");
	}

	SVariable visit_literalInteger(SLangASTContext_literalInteger ctx) {
		return new SVariable_float(ctx.intValue());
	}
	
	SVariable visit_literalFloat(SLangASTContext_literalFloat ctx) {
		return new SVariable_float(ctx.floatValue());
	}

	SVariable visit_literalBool(SLangASTContext_literalBool ctx) {
		return new SVariable_bool(ctx.boolValue());
	}

	SVariable visit_literalString(SLangASTContext_literalString ctx) {
		return new SVariable_string(ctx.stringValue());
	}

	SVariable visit_literalIdentifier(SLangASTContext_literalIdentifier ctx) {
		// TODO: this should return a variable based on the identifier
		return new SVariable_string(ctx.identifier());
	}

	
}