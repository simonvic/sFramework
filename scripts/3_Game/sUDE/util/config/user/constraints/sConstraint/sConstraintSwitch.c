class SConstraintSwitch : SConstraintSimple<bool>  {
	override string toString() {
		return "<b>#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_SWITCH : <i>" + getValue().ToString() + "</i></b>";
	}
}