typedef map<string, ref SVariable> TSVariableStruct
class SVariable {
	string toString();
	bool equals(SVariable other);
	bool box(string value);
	bool box(typename value);
	bool box(float value);
	bool box(int value);
	bool box(bool value);
	bool box(vector value);
	bool box(TSVariableStruct value);
	bool box(Class value);
	bool box(Managed value);
	bool get(out string value);
	bool get(out typename value);
	bool get(out float value);
	bool get(out int value);
	bool get(out bool value);
	bool get(out vector value);
	bool get(out TSVariableStruct value);
	bool get(out Class value);
	bool get(out Managed value);
	SVariable multiply(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'multiply' with " + rhs.toString() + "\n");}
	SVariable divide(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'divide' with " + rhs.toString() + "\n");}
	SVariable plus(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'plus' with " + rhs.toString() + "\n");}
	SVariable minus(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'minus' with " + rhs.toString() + "\n");}
	SVariable modulo(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'modulo' with " + rhs.toString() + "\n");}
	SVariable power(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'power' with " + rhs.toString() + "\n");}
	SVariable lessEqual(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'lessEqual' with " + rhs.toString() + "\n");}
	SVariable greaterEqual(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'greaterEqual' with " + rhs.toString() + "\n");}
	SVariable greater(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'greater' with " + rhs.toString() + "\n");}
	SVariable less(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'less' with " + rhs.toString() + "\n");}
	SVariable not() {return new SVariableException(this.toString() + " doesn't support operator 'not'" + "\n");}
	SVariable negative() {return new SVariableException(this.toString() + " doesn't support operator 'negative'" + "\n");}
	SVariable and(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'and' with " + rhs.toString() + "\n");}
	SVariable or(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'or' with " + rhs.toString() + "\n");}
	SVariable equal(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'equal' with " + rhs.toString() + "\n");}
	SVariable contains(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'contains' with " + rhs.toString() + "\n");}
	SVariable dot(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'dot' with " + rhs.toString() + "\n");}
	SVariable access(SVariable rhs) {return new SVariableException(this.toString() + " doesn't support operator 'access' with " + rhs.toString() + "\n");}
	
	SVariable notEqual(SVariable rhs) {
		return equal(rhs).not();
	}
	
	SVariable isContained(SVariable rhs) {
		return rhs.contains(this);
	}
}

class SVariableException : SVariable {
	protected ref SVariable m_cause;
	protected string m_message;
	void SVariableException(string message = "") {
		m_message = message;
	}
	
	override string toString() {
		return m_message;
	}
}

class SVariableImpl<Class T> : SVariable {
	protected T m_value;

	void SVariableImpl(T value) {
		m_value = value;
	}
	
	override string toString() {
		return string.ToString(m_value, false, false, false);
	}
	
	override bool equals(SVariable other) {
		return this == other;
	}
	
	
	override bool box(T value) {
		m_value = value;
		return true;
	}
	
	override bool get(out T value) {
		value = m_value;
		return true;
	}
	
}

class SVariable_string : SVariableImpl<string> {
	
	override SVariable plus(SVariable rhs) {
		return new SVariable_string(m_value + rhs.toString());
	}
	
	override SVariable minus(SVariable rhs) {
		string subtracted = m_value;
		subtracted.Replace(rhs.toString(), "");
		return new SVariable_string(subtracted);
	}
	
	override SVariable lessEqual(SVariable rhs) {
		string r;
		return new SVariable_bool(rhs.get(r) && m_value <= r);
	}
	
	override SVariable greaterEqual(SVariable rhs) {
		string r;
		return new SVariable_bool(rhs.get(r) && m_value >= r);
	}
	
	override SVariable greater(SVariable rhs) {
		string r;
		return new SVariable_bool(rhs.get(r) && m_value > r);
	}
	
	override SVariable less(SVariable rhs) {
		string r;
		return new SVariable_bool(rhs.get(r) && m_value < r);
	}
	
	override SVariable equal(SVariable rhs) {
		string r;
		return new SVariable_bool(rhs.get(r) && m_value == r);
	}
	
	override SVariable contains(SVariable rhs) {
		string r;
		return new SVariable_bool(rhs.get(r) && m_value.Contains(r));
	}
	
	override SVariable access(SVariable rhs) {
		int r;
		if (!rhs.get(r)) return new SVariable_string("");
		return new SVariable_string(m_value.Substring(Math.Clamp(r, 0, m_value.Length() - 1), Math.Min(0, r - 1)));
	}
}
class SVariable_typename : SVariableImpl<typename> {}

class SVariable_float : SVariableImpl<float> {
	override SVariable negative() {
		return new SVariable_float(-m_value);
	}
	
	override SVariable multiply(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(m_value * rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_float(m_value * rInt);
		}

		return super.multiply(rhs);
	}

	override SVariable divide(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(m_value / rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_float(m_value / rInt);
		}

		return super.divide(rhs);
	}

	override SVariable plus(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(m_value + rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_float(m_value + rInt);
		}

		return super.plus(rhs);
	}

	override SVariable minus(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(m_value - rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_float(m_value - rInt);
		}

		return super.minus(rhs);
	}

	override SVariable power(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(Math.Pow(m_value, rFloat));
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_float(Math.Pow(m_value, rInt));
		}

		return super.modulo(rhs);
	}

	override SVariable lessEqual(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value <= rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value <= rInt);
		}
		return super.lessEqual(rhs);
	}

	override SVariable greaterEqual(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value >= rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value >= rInt);
		}
		return super.greaterEqual(rhs);
	}

	override SVariable greater(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value > rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value > rInt);
		}
		return super.greater(rhs);
	}

	override SVariable less(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value < rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value < rInt);
		}
		return super.less(rhs);
	}

	override SVariable equal(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value == rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value == rInt);
		}
		
		return super.equal(rhs);
	}
}

class SVariable_int : SVariableImpl<int> {
	
	override SVariable negative() {
		return new SVariable_int(-m_value);
	}
	
	override SVariable multiply(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(m_value * rFloat);
		}
		
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_int(m_value * rInt);
		}

		return super.multiply(rhs);
	}

	override SVariable divide(SVariable rhs) {
		float r;
		if (rhs.get(r)) {
			return new SVariable_float(m_value / r);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_float(m_value / rInt);
		}

		return super.divide(rhs);
	}

	override SVariable plus(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(m_value + rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_int(m_value + rInt);
		}

		return super.plus(rhs);
	}

	override SVariable minus(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(m_value - rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_int(m_value - rInt);
		}

		return super.minus(rhs);
	}

	override SVariable modulo(SVariable rhs) {
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_int(m_value % rInt);
		}

		return super.modulo(rhs);
	}

	override SVariable power(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_float(Math.Pow(m_value, rFloat));
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_int(Math.Pow(m_value, rInt));
		}

		return super.power(rhs);
	}

	override SVariable lessEqual(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value <= rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value <= rInt);
		}
		return super.lessEqual(rhs);
	}

	override SVariable greaterEqual(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value >= rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value >= rInt);
		}
		return super.greaterEqual(rhs);
	}

	override SVariable greater(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value > rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value > rInt);
		}
		return super.greater(rhs);
	}

	override SVariable less(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value < rFloat);
		}
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value < rInt);
		}
		return super.less(rhs);
	}

	override SVariable equal(SVariable rhs) {
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_bool(m_value == rFloat);
		}

		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_bool(m_value == rInt);
		}
		
		return super.equal(rhs);
	}
}
class SVariable_bool : SVariableImpl<bool> {
	
	override SVariable not() {
		return new SVariable_bool(!m_value);
	}
	
	override SVariable and(SVariable rhs) {
		bool rBool;
		if (rhs.get(rBool)) {
			return new SVariable_bool(m_value && rBool);
		}
		return super.and(rhs);
	}

	override SVariable or(SVariable rhs) {
		bool rBool;
		if (rhs.get(rBool)) {
			return new SVariable_bool(m_value || rBool);
		}
		return super.or(rhs);
	}

	override SVariable equal(SVariable rhs) {
		bool rBool;
		if (rhs.get(rBool)) {
			return new SVariable_bool(m_value == rBool);
		}
		return super.equal(rhs);
	}

}

class SVariable_vector : SVariableImpl<vector> {
	
	override SVariable multiply(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value * rVec);
		}
		
		float rFloat;
		if (rhs.get(rFloat)) {
			return new SVariable_vector(m_value * rFloat);
		}
		
		int rInt;
		if (rhs.get(rInt)) {
			rFloat = rInt;
			return new SVariable_vector(m_value * rFloat);
		}
		
		return super.multiply(rhs);
	}
	
	override SVariable plus(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value + rVec);
		}
		return super.plus(rhs);
	}
	
	override SVariable minus(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value - rVec);
		}
		return super.minus(rhs);
	}
	
	/* TODO: CaN'T cOnDiTiOn tO nOn nUmEriCType!
	override SVariable lessEqual(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value <= rVec);
		}
		return super.lessEqual(rhs);
	}
	
	override SVariable greaterEqual(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value >= rVec);
		}
		return super.greaterEqual(rhs);
	}
	
	override SVariable greater(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value > rVec);
		}
		return super.greater(rhs);
	}
	
	override SVariable less(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value < rVec);
		}
		return super.less(rhs);
	}
	
	
	override SVariable equal(SVariable rhs) {
		vector rVec;
		if (rhs.get(rVec)) {
			return new SVariable_vector(m_value == rVec);
		}
		return super.equal(rhs);
	}
	*/
	
	override SVariable access(SVariable rhs) {
		int rInt;
		if (rhs.get(rInt)) {
			return new SVariable_float(m_value[Math.Clamp(rInt, 0, 2)]);
		}
		return super.access(rhs);
	}
}

class SVariable_Class : SVariableImpl<Class> {}
class SVariable_Managed : SVariableImpl<Managed> {}

/**
*	
*/
class SVariableStruct : SVariableImpl<ref TSVariableStruct>{
	override SVariable multiply(SVariable rhs);
	override SVariable divide(SVariable rhs);
	override SVariable plus(SVariable rhs);
	override SVariable minus(SVariable rhs);
	override SVariable modulo(SVariable rhs);
	override SVariable power(SVariable rhs);
	override SVariable lessEqual(SVariable rhs);
	override SVariable greaterEqual(SVariable rhs);
	override SVariable greater(SVariable rhs);
	override SVariable less(SVariable rhs);
	override SVariable not();
	override SVariable and(SVariable rhs);
	override SVariable or(SVariable rhs);
	override SVariable equal(SVariable rhs);
	override SVariable notEqual(SVariable rhs);
	override SVariable isContained(SVariable rhs);
	override SVariable contains(SVariable rhs);
	override SVariable dot(SVariable rhs) {
		string identifier;
		if (!rhs.get(identifier)) return null;
		return m_value.Get(identifier);
	}
	override SVariable access(SVariable rhs);
	
	static SVariableStruct of() {
		return new SVariableStruct(new TSVariableStruct());
	}
	
	bool store(string identifier, string value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_string(value));
			return true;
		}
		return p.box(value);
	}
	
	bool store(string identifier, typename value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_typename(value));
			return true;
		}
		return p.box(value);
	}
	
	bool store(string identifier, float value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_float(value));
			return true;
		}
		return p.box(value);
	}
	
	bool store(string identifier, int value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_int(value));
			return true;
		}
		return p.box(value);
	}
	
	bool store(string identifier, bool value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_bool(value));
			return true;
		}
		return p.box(value);
	}
	
	bool store(string identifier, vector value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_vector(value));
			return true;
		}
		return p.box(value);
	}
	
	bool store(string identifier, Class value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_Class(value));
			return true;
		}
		return p.box(value);
	}
	
	bool store(string identifier, Managed value) {
		SVariable p = fetch(identifier);
		if (!p) {
			store(identifier, new SVariable_Managed(value));
			return true;
		}
		return p.box(value);
	}
	
	
	bool fetch(string identifier, out string value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}
	
	bool fetch(string identifier, out typename value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}
	
	bool fetch(string identifier, out float value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}
	
	bool fetch(string identifier, out int value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}
	
	bool fetch(string identifier, out bool value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}
	
	bool fetch(string identifier, out vector value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}
	
	bool fetch(string identifier, out Class value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}
	
	bool fetch(string identifier, out Managed value) {
		SVariable p = fetch(identifier);
		return p != null && p.get(value);
	}

	protected SVariable fetch(string identifier) {
		return m_value.Get(identifier);
	}
	
	protected void store(string identifier, SVariable value) {
		m_value.Set(identifier, value);
	}
	
	
	
	/**
	*	value
	*	value.value
	*	value:value
	*/
	string fetchVariable(string name) {
		int i = name.IndexOf("date:");
		if (i != -1) {
			return SDate.now().format(name.Substring(5, name.Length() - 5));
		}
		
		i = name.IndexOf(".");
		if (i != -1) {
			string lhs = name.Substring(0, i);
			Managed tmp;
			SVariableStruct struct;
			if (!fetch(lhs, tmp) || !Class.CastTo(struct, tmp)) return "";
			string rhs = name.Substring(i + 1, name.Length() - i - 1);
			return struct.fetchVariable(rhs);
		}
		
		return "" + fetch(name).toString();
	}
	
	/**
	*	${value}
	*	${value.value}
	*	${date:format}
	*/
	static string inject(SVariableStruct ctx, string text) {
		int opener;
		int closer;
		string variable;
		while (opener != -1) {
			opener = text.IndexOf("${");
			closer = text.IndexOfFrom(opener, "}");
			if (closer == -1) return text;
			variable = text.Substring(opener + 2, closer - opener - 2);
			variable = ctx.fetchVariable(variable);
			text = text.Substring(0, opener) + variable + text.Substring(closer + 1, text.Length() - closer - 1);
		}
		return text;
	}
}

