#ifdef DIAG_DEVELOPER

class TestUnit_SConstraints : STestUnit {
	
	override void init() {
		registerTestCases({
			"enable_shouldEnable",
			"disable_shouldDisable",
			"simple_constrained_shouldReturnConstrainedValue",
			"disabledConstraint_constrained_shouldReturnConstrainedValue",
			"minMaxDictionary_constrained_shouldReturnConstrainedValue",
			"minMaxDictionary_constrained_shouldReturnConstrainedValue2",
			"minMaxDictionary_constrained_shouldReturnConstrainedValue3",
			"minMaxDictionary_constrained_shouldReturnConstrainedValue4",
			"minMaxDictionary_constrained_shouldReturnConstrainedValue5",
			"minMaxDictionary_constrained_shouldReturnConstrainedValue6",
			"minMaxNumeric_costrained_shouldReturnConstrainedValue",
			"minMaxNumeric_costrained_shouldReturnConstrainedValue2",
			"minMaxNumeric_costrained_shouldReturnConstrainedValue3",
			"minMaxArrayNumeric_shouldBeValidValue",
			"minMaxArrayNumeric_costrained_shouldReturnConstrainedValue",
			"minMaxArrayNumeric_costrained_shouldReturnConstrainedValue2",
			"minMaxArrayNumeric_costrained_shouldReturnConstrainedValue3",
			"minMaxArrayNumeric_costrainedOnSmallerArray_shouldReturnConstrainedValue3"
		});
	}
	
	void enable_shouldEnable() {
		auto constraint = new SConstraintBase();
		constraint.enable();
		assertTrue(constraint.isEnabled());
	}
	
	void disable_shouldDisable() {
		auto constraint = new SConstraintBase();
		constraint.enable();
		constraint.disable();
		assertFalse(constraint.isEnabled());
	}
	
	void simple_constrained_shouldReturnConstrainedValue() {
		string constrainedValue = "Constrained string";
		auto constraint = new SConstraintPrimitiveSimple<string>(constrainedValue);
		constraint.enable();
		assertEqual(constrainedValue, constraint.constrained("Unconstrained string"));
	}
	
	void disabledConstraint_constrained_shouldReturnConstrainedValue() {
		string constrainedValue = "Constrained string";
		string unconstrainedValue = "Unconstrained string";
		auto constraint = new SConstraintPrimitiveSimple<string>(constrainedValue);
		constraint.disable();
		assertEqual(unconstrainedValue, constraint.constrained(unconstrainedValue));
	}
	
	void minMaxDictionary_constrained_shouldReturnConstrainedValue() {
		auto constraint = new SConstraintPrimitiveMinMaxDictionary("F","U");
		constraint.enable();
		assertEqual("F", constraint.constrained("A"));
	}
	
	void minMaxDictionary_constrained_shouldReturnConstrainedValue2() {
		auto constraint = new SConstraintPrimitiveMinMaxDictionary("F","U");
		constraint.enable();
		assertEqual("U", constraint.constrained("Z"));
	}
	
	void minMaxDictionary_constrained_shouldReturnConstrainedValue3() {
		auto constraint = new SConstraintPrimitiveMinMaxDictionary("F","U");
		constraint.enable();
		assertEqual("G", constraint.constrained("G"));
	}
	
	void minMaxDictionary_constrained_shouldReturnConstrainedValue4() {
		auto constraint = new SConstraintPrimitiveMinMaxDictionary("FFFFFF","UUU");
		constraint.enable();
		assertEqual("FFFFFF", constraint.constrained("FF"));
	}
	
	void minMaxDictionary_constrained_shouldReturnConstrainedValue5() {
		auto constraint = new SConstraintPrimitiveMinMaxDictionary("FFFFFF","UUU");
		constraint.enable();
		assertEqual("UUU", constraint.constrained("UUUUU"));
	}
	
	void minMaxDictionary_constrained_shouldReturnConstrainedValue6() {
		auto constraint = new SConstraintPrimitiveMinMaxDictionary("FFFFFF","UUU");
		constraint.enable();
		assertEqual("G", constraint.constrained("G"));
	}
	
	void minMaxNumeric_costrained_shouldReturnConstrainedValue() {
		auto constraint = new SConstraintPrimitiveMinMaxNumeric(0.0, 1.0);
		constraint.enable();
		assertEqual(0.0, constraint.constrained(-1.0));
	}
	
	void minMaxNumeric_costrained_shouldReturnConstrainedValue2() {
		auto constraint = new SConstraintPrimitiveMinMaxNumeric(0.0, 1.0);
		constraint.enable();
		assertEqual(1.0, constraint.constrained(2.0));
	}
	
	void minMaxNumeric_costrained_shouldReturnConstrainedValue3() {
		auto constraint = new SConstraintPrimitiveMinMaxNumeric(0.0, 1.0);
		constraint.enable();
		assertEqual(0.5, constraint.constrained(0.5));
	}

	void minMaxArrayNumeric_shouldBeValidValue() {
		auto constraint = new SConstraintMinMaxArrayNumeric({0,0,0,0}, {1,1,1,1});
		TFloatArray alreadyValid = {0.5, 0.5, 0.5, 0.5};
		assertTrue(constraint.isValid(alreadyValid));
	}
	
	void minMaxArrayNumeric_costrained_shouldReturnConstrainedValue() {
		TFloatArray min        = {0, 0, 0, 0};
		TFloatArray max        = {1, 1, 1, 1};
		TFloatArray toCostrain = {-1, -1, -1, -1};
		TFloatArray expected   = {0, 0, 0, 0};
		auto constraint = new SConstraintMinMaxArrayNumeric(min, max);
		constraint.enable();
		assertEqual(expected, constraint.constrained(toCostrain));
	}
	
	void minMaxArrayNumeric_costrained_shouldReturnConstrainedValue2() {
		TFloatArray min        = {0, 0, 0, 0};
		TFloatArray max        = {1, 1, 1, 1};
		TFloatArray toCostrain = {2, 2, 2, 2};
		TFloatArray expected   = {1, 1, 1, 1};
		auto constraint = new SConstraintMinMaxArrayNumeric(min, max);
		constraint.enable();
		assertEqual(expected, constraint.constrained(toCostrain));
	}
	
	void minMaxArrayNumeric_costrained_shouldReturnConstrainedValue3() {
		TFloatArray min        = {0, 0, 0, 0};
		TFloatArray max        = {1, 1, 1, 1};
		TFloatArray toCostrain = {-1, 2, -1, 2};
		TFloatArray expected   = {0, 1, 0, 1};
		auto constraint = new SConstraintMinMaxArrayNumeric(min, max);
		constraint.enable();
		assertEqual(expected, constraint.constrained(toCostrain));
	}
	
	void minMaxArrayNumeric_costrainedOnSmallerArray_shouldReturnConstrainedValue3() {
		TFloatArray min        = {0, 0, 0, 0};
		TFloatArray max        = {1, 1, 1, 1};
		TFloatArray toCostrain = {-1, 2};
		TFloatArray expected   = {0, 1};
		auto constraint = new SConstraintMinMaxArrayNumeric(min, max);
		constraint.enable();
		assertEqual(expected, constraint.constrained(toCostrain));
	}
	
}

#endif