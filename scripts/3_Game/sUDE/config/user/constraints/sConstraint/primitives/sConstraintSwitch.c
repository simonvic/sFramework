class SConstraintSwitch : SConstraintPrimitiveSimple<bool>  {
	override string toString() {
		string t;
		if (getValue()) {
			t = "#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_SWITCH_TRUE";
		} else {
			t = "#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_SWITCH_FALSE";
		}
		return "<b>" + t + "</b>";
	}
}
