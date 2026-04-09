#ifdef DIAG_DEVELOPER

class TestUnit_SLOG : STestUnit {

	override void init() {
		registerTestCases({
			"testAll",
			"testClasses",
			"testArray",
			"testStaticArray"
		});
	}

	void testAll() {
		SLOG.d(g_Game);
		SLOG.d(CGame);
		SLOG.d(6.9);
		SLOG.d(69);
		SLOG.d(true);
		SLOG.d(Vector(0, 0, 0));
		SLOG.d("hello world");

		SLOG.d("caller", g_Game);
		SLOG.d("caller", CGame);
		SLOG.d("caller", 6.9);
		SLOG.d("caller", 69);
		SLOG.d("caller", true);
		SLOG.d("caller", Vector(0, 0, 0));
		SLOG.d("caller", "hello world");

		SLOG.d(1, g_Game);
		SLOG.d(1, CGame);
		SLOG.d(1, 6.9);
		SLOG.d(1, 69);
		SLOG.d(1, true);
		SLOG.d(1, Vector(0, 0, 0));
		SLOG.d(1, "hello world");

		SLOG.d(2, "caller", g_Game);
		SLOG.d(2, "caller", CGame);
		SLOG.d(2, "caller", 6.9);
		SLOG.d(2, "caller", 69);
		SLOG.d(2, "caller", true);
		SLOG.d(2, "caller", Vector(0, 0, 0));
		SLOG.d(2, "caller", "hello world");
		pass();
	}

	void testClasses() {
		SLOG.d(g_Game);
		SLOG.d(null);
		pass();
	}

	void testArray() {
		array<typename> typenameArray = {CGame, TestUnit_SLOG};
		SLOG.d(typenameArray);

		array<float> floatArray = {1.0, 2.0, 3.0};
		SLOG.d(floatArray);

		array<int> intArray = {1, 2, 3};
		SLOG.d(intArray);
		intArray = {};
		SLOG.d(floatArray);

		array<bool> boolArray = {true, false, true};
		SLOG.d(boolArray);

		array<vector> vectorArray = {"0 0 0", "1 1 1", "3 3 3"};
		SLOG.d(vectorArray);

		array<string> stringArray = {"a", "d", "c"};
		SLOG.d(stringArray);
		
		array<ref array<string>> stringMatrix = {
			{"a", "a", "a"},
			{"b", "b", "b"},
			{"c", "c", "c"},
		};
		SLOG.d(stringMatrix);

		array<ref Param> paramArray = {
			new Param1<int>(1),
			new Param1<int>(2),
			new Param1<int>(3),
		};
		SLOG.d(paramArray);

		pass();
	}

	void testStaticArray() {

	}

}

#endif
