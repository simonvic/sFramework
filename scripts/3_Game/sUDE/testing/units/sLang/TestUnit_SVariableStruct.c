#ifdef DIAG_DEVELOPER
class TestUnit_SVariableStruct : STestUnit {
	
	override void init() {
		registerTestCases({
			"testStoring_string"
			"testStoring_int"
			"testStoring_float"
			"testStoring_bool"
			"testStoring_typename"
			"testStoring_Class"
			"testStoring_Managed"
			"storingUndeclaredVariable_shouldStore"
			"storingAlreadyDeclaredVariable_shouldStore"
			"storingAlreadyDeclaredVariable_wrongType_shouldNotStore"
			"fetchingUndeclaredVariable_shouldNotFetch"
			"fetchingDeclaredVariable_shouldFetch"
			"fetchingDeclaredVariable_wrongType_shouldNotFetch"
		});
	}
	
	void testStoring_string() {
		auto ctx = SVariableStruct.of();
		string expected = "hello";
		string actual;
		ctx.store("identifier", expected);
		ctx.fetch("identifier", actual);
		assertEqual(expected, actual);
	}
	
	void testStoring_int() {
		auto ctx = SVariableStruct.of();
		int expected = 69;
		int actual;
		ctx.store("identifier", expected);
		ctx.fetch("identifier", actual);
		assertEqual(expected, actual);
	}
	
	void testStoring_float() {
		auto ctx = SVariableStruct.of();
		float expected = 420.69;
		float actual;
		ctx.store("identifier", expected);
		ctx.fetch("identifier", actual);
		assertEqual(expected, actual);
	}
	
	void testStoring_bool() {
		auto ctx = SVariableStruct.of();
		bool expected = true;
		bool actual;
		ctx.store("identifier", expected);
		ctx.fetch("identifier", actual);
		assertEqual(expected, actual);
	}
	
	void testStoring_typename() {
		auto ctx = SVariableStruct.of();
		typename expected = Man;
		typename actual;
		ctx.store("identifier", expected);
		ctx.fetch("identifier", actual);
		assertEqual(expected, actual);
	}
	
	void testStoring_Class() {
		auto ctx = SVariableStruct.of();
		Class expected = GetGame();
		Class actual;
		ctx.store("identifier", expected);
		ctx.fetch("identifier", actual);
		assertEqual(expected, actual);
	}
	
	void testStoring_Managed() {
		auto ctx = SVariableStruct.of();
		Managed expected = new Param();
		Managed actual;
		ctx.store("identifier", expected);
		ctx.fetch("identifier", actual);
		assertEqual(expected, actual);
	}
	
	void storingUndeclaredVariable_shouldStore() {
		auto ctx = SVariableStruct.of();
		assertTrue(ctx.store("identifier", 69));
	}
	
	void storingAlreadyDeclaredVariable_shouldStore() {
		auto ctx = SVariableStruct.of();
		ctx.store("identifier", 69);
		assertTrue(ctx.store("identifier", 69));
	}
	
	void storingAlreadyDeclaredVariable_wrongType_shouldNotStore() {
		auto ctx = SVariableStruct.of();
		ctx.store("identifier", 69);
		assertFalse(ctx.store("identifier", "sixtynine"));
	}
	
	void fetchingUndeclaredVariable_shouldNotFetch() {
		auto ctx = SVariableStruct.of();
		int v;
		assertFalse(ctx.fetch("identifier", v));
	}
	
	void fetchingDeclaredVariable_shouldFetch() {
		auto ctx = SVariableStruct.of();
		ctx.store("identifier", 69);
		int v;
		assertTrue(ctx.fetch("identifier", v));
	}

	void fetchingDeclaredVariable_wrongType_shouldNotFetch() {
		auto ctx = SVariableStruct.of();
		ctx.store("identifier", 69);
		string notAnInt;
		assertFalse(ctx.fetch("identifier", notAnInt));
	}
	
	
}
#endif