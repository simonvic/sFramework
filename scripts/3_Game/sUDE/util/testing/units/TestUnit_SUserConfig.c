class TestUnit_SUserConfig : STestUnit {
	
	override void init() {
		registerTestCases({
			"testConstraints",
			"testOptionsValuesWithConstraints"
		});
	}
	
	private void testConstraints() {
		auto f_constraint = new SConstraintPrimitiveMinMaxNumeric<float>(0, 1);
		assertEqual(false, f_constraint.isEnabled());
		
		auto f_option = new SUserConfigOption<float>(0.69, f_constraint);
		assertEqual(true, f_option.hasConstraint());
		assertEqual(false, f_option.isConstrained());
		
		f_option.removeConstraint();
		assertEqual(false, f_option.hasConstraint());
		assertEqual(false, f_option.isConstrained());
		
		f_option.setConstraint(f_constraint);
		assertEqual(true, f_option.hasConstraint());
		assertEqual(false, f_option.isConstrained());
	}
	
	
	private void testOptionsValuesWithConstraints() {
		auto f_constraint = new SConstraintPrimitiveMinMaxNumeric<float>(0, 1);		
		auto f_option = new SUserConfigOption<float>(0.69, f_constraint);
		
		assertEqual(0.69, f_option.get());

		f_option.set(2.0);
		assertEqual(2.0, f_option.get());
		
		f_option.enableConstraint();
		assertEqual(1.0, f_option.get());
		
		f_constraint.disable();
		f_option.set(-2.0);
		assertEqual(-2.0, f_option.get());
		
		f_constraint.enable();
		assertEqual(-2.0, f_option.get());
		
		f_option.updateConstraint();
		assertEqual(0.0, f_option.get());
		
	}
	
}