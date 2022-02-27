class TestUnit_SUserConfigOption : STestUnit {
	
	ref SUserConfigOption<float> option;
	ref SConstraintPrimitiveMinMaxNumeric<float> constraint;
	
	override void init() {
				
		registerBeforeCallbacks({
			"before"
		});
		
		registerTestCases({
			"shouldNotHaveConstraint",
			"testSetConstraint",
			"testSetConstraint2",
			"shouldNotBeConstrained",
			"testEnableConstraint",
			"testDisableConstraint",
			"testRemoveConstraint",
			"testRemoveConstraint2",
			"testRemoveConstraint3",
			"optionValue_shouldNotBeConstrained",
			"optionValue_shouldNotBeConstrained2",
			"optionValue_shouldBeConstrainedAfterEnablingConstraint",
			"constrain_enable_shouldNotUpdateOptionValue",
			"updateConstraint_shouldCostrainValue"
		});
	}
	
	void before() {
		option = new SUserConfigOption<float>(0.69);
		constraint = new SConstraintPrimitiveMinMaxNumeric<float>(0, 1);
	}
	
	
	void shouldNotHaveConstraint() {
		assertFalse(option.hasConstraint());
	}
	
	void testSetConstraint() {
		option.setConstraint(constraint);
		assertNotNull(option.getConstraint());
	}
	
	void testSetConstraint2() {
		option.setConstraint(constraint);
		assertTrue(option.hasConstraint());
	}
	
	void shouldNotBeConstrained() {
		assertFalse(option.isConstrained());
	}
	
	void testEnableConstraint() {
		option.setConstraint(constraint);
		option.enableConstraint();
		assertTrue(option.isConstrained());
	}
	
	void testDisableConstraint() {
		option.setConstraint(constraint);
		option.enableConstraint();
		option.disableConstraint();
		assertFalse(option.isConstrained());
	}
	
	void testRemoveConstraint() {
		option.setConstraint(constraint);
		option.removeConstraint();
		assertFalse(option.hasConstraint());
	}
	
	void testRemoveConstraint2() {
		option.setConstraint(constraint);
		option.removeConstraint();
		assertNull(option.getConstraint());
	}
	
	void testRemoveConstraint3() {
		option.setConstraint(constraint);
		option.enableConstraint();
		option.removeConstraint();
		assertFalse(option.isConstrained());
	}
	
	void optionValue_shouldNotBeConstrained() {
		assertEqual(0.69, option.get());
	}
	
	void optionValue_shouldNotBeConstrained2() {
		option.set(4.20);
		assertEqual(4.20, option.get());
	}
	
	void optionValue_shouldBeConstrainedAfterEnablingConstraint() {
		option.set(4.20);
		option.setConstraint(constraint);
		//option.enableConstraint();
		assertEqual(1.0, option.get());
	}
	
	void constrain_enable_shouldNotUpdateOptionValue() {
		option.set(4.20);
		option.setConstraint(constraint);
		option.getConstraint().enable();
		assertEqual(4.20, option.get());
	}
	
	void updateConstraint_shouldCostrainValue() {
		option.set(4.20);
		option.setConstraint(constraint);
		option.getConstraint().enable();
		option.updateConstraint();
		assertEqual(1.0, option.get());
	}

}