class STest {

	static string PASSED_OUTPUT = "[ ✓ ] PASSED | expected: %1 | got: %2";
	static string FAILED_OUTPUT = "[ × ] FAILED | expected: %1 | got: %2";
	static bool SHOW_PASSED = true;
	
	//@todo maybe make a proper testing framework? oof...
	static void all(bool showPassed = false) {
		SHOW_PASSED = showPassed;
		SLog.d("====================================================================================");
		SLog.d("=== TESTING ALL ===");
		SLog.d("------------------------------------------------------------------------------------");
		
		testSMath();
		testTrees();
		testSColor();
		testConstraints();
		testUserConfigsOptions();
		testUserConfigs();
		SLog.d("====================================================================================");
	}
	
	static void testSMath() {
		assertEqual(true, SMath.equal(0.1, 0.1));
		assertEqual(false, SMath.equal(0.5, 0.1));
		assertEqual(true, SMath.equal(0.00000001, 0.00000001));
		assertEqual(false, SMath.equal(0.000000000000001, 0.000000000000002));
		assertEqual(true, SMath.equal(0.000000000000001, 0.000000000000002, 0.0001));
		
		assertEqual(true, SMath.isInRange(0.5, 0.0, 1.0));
		assertEqual(false, SMath.isInRange(-100, 0.0, 1.0));
		assertEqual(false, SMath.isInRange(100, 0.0, 1.0));
		
		TFloatArray a1;
		TFloatArray a2;
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2, 3};
		assertEqual(true, SMath.equal(a1, a2));
		
		a1 = {0, 0, 0, 0};
		a2 = {0, 1, 2, 3};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2};
		a2 = {0, 1, 2, 3};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2};
		assertEqual(false, SMath.equal(a1, a2));
		
		TFloatArray toClamp;
		TFloatArray min;
		TFloatArray max;
		TFloatArray expected;
		
		min      = {0,   1,  10,  100};
		toClamp  = {0.5, 5,  50,  500};
		max      = {1,   10, 100, 1000};
		expected = toClamp;
		SMath.clamp(toClamp, min, max);
		assertEqual(true, SMath.equal(expected, toClamp));
		assertEqual(true, SMath.isInRange(toClamp, min, max));
		
		min      = {0,   1,  10,  100};
		toClamp  = {2,   5,  50,  500};
		max      = {1,   10, 100, 1000};
		expected = {1,   5,  50,  500};
		SMath.clamp(toClamp, min, max);
		assertEqual(true, SMath.equal(expected, toClamp));
		assertEqual(true, SMath.isInRange(toClamp, min, max));
		
		min      = {0,   1,  10,  100};
		toClamp  = {2,   5,  5000,  0.1};
		max      = {1,   10, 100, 1000};
		expected = {1,   5,  100,  100};
		SMath.clamp(toClamp, min, max);
		assertEqual(true, SMath.equal(expected, toClamp));
		assertEqual(true, SMath.isInRange(toClamp, min, max));
		
	}
	
	static void testUserConfigs() {
				
	}
	
	static void testUserConfigsOptions() {
		SLog.d("==============================");
		
		SLog.d("-----------------------------","float");
		auto f_constraint = new SConstraintMinMaxNumeric<float>(0, 1);
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
				
		/*
		SUserConfigOption<bool> boolOption = new SUserConfigOption<bool>(true);
		SUserConfigOption<string> stringOption = new SUserConfigOption<string>("Hello world");
		*/
				
	}
	
	static void testConstraints() {
		SLog.d("==============================");
		
		SLog.d("-----------------------------","simple");
		string stringConstrained = "I've been constrained";
		auto simpleConstrainerString = new SConstraintSimple<string>(stringConstrained);
		simpleConstrainerString.setEnabled(true);
		assertEqual(stringConstrained, simpleConstrainerString.constrained("I need to be constrained"));
		
		bool boolConstrained = false;
		auto simpleConstrainerBool = new SConstraintSimple<bool>(boolConstrained);
		simpleConstrainerBool.setEnabled(true);
		assertEqual(boolConstrained, simpleConstrainerBool.constrained(true));
		
		bool boolConstrained2 = true;
		auto simpleConstrainerBool2 = new SConstraintSwitch(boolConstrained2);
		simpleConstrainerBool2.setEnabled(true);
		assertEqual(boolConstrained2, simpleConstrainerBool2.constrained(false));
		
		int intConstrained = 3;
		auto simpleConstrainerInt = new SConstraintSimple<int>(intConstrained);
		simpleConstrainerInt.setEnabled(true);
		assertEqual(intConstrained, simpleConstrainerInt.constrained(69));
				
		float floatConstrained = 0.420;
		auto simpleConstrainerFloat = new SConstraintSimple<float>(floatConstrained);
		simpleConstrainerFloat.setEnabled(true);
		assertEqual(floatConstrained, simpleConstrainerFloat.constrained(6.9));
		
		
		SLog.d("-----------------------------","minmax");
		auto s_minmaxConstrainer = new SConstraintMinMaxDictionary("F","U");
		s_minmaxConstrainer.setEnabled(true);
		assertEqual("F", s_minmaxConstrainer.constrained("A"));
		assertEqual("U", s_minmaxConstrainer.constrained("Z"));
		assertEqual("G", s_minmaxConstrainer.constrained("G"));
		
		auto s_minmaxConstrainer2 = new SConstraintMinMaxDictionary("FFFFFF","UUU");
		s_minmaxConstrainer2.setEnabled(true);
		assertEqual("FFFFFF", s_minmaxConstrainer2.constrained("FF"));
		assertEqual("UUU", s_minmaxConstrainer2.constrained("UUUUU"));
		assertEqual("G", s_minmaxConstrainer2.constrained("G"));
		
		auto f_minmaxConstrainer = new SConstraintMinMaxNumeric(0.0, 1.0);
		f_minmaxConstrainer.setEnabled(true);
		assertEqual(0.0, f_minmaxConstrainer.constrained(-1.0));
		assertEqual(1.0, f_minmaxConstrainer.constrained(2.0));
		assertEqual(0.5, f_minmaxConstrainer.constrained(0.5));
		
		
		TFloatArray min = {};
		TFloatArray max = {};
		TFloatArray expected = {};
		
		min = {0,0,0,0};
		max = {1,1,1,1};
		auto f_minmaxArrayConstrainer = new SConstraintMinMaxArrayNumeric({0,0,0,0}, {1,1,1,1});
		f_minmaxArrayConstrainer.setEnabled(true);
		assertEqual(min, f_minmaxArrayConstrainer.getMin());
		assertEqual(max, f_minmaxArrayConstrainer.getMax());
		
				
		TFloatArray s_toConstrain = {};
		s_toConstrain = {0.5, 0.5, 0.5, 0.5};
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(s_toConstrain, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {-1, -1, -1, -1};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(min, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {10, 10, 10, 10};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(max, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {-1, 10, -1, 10};
		expected      = {0,   1,  0,  1};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(expected, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {-1, 10};
		expected      = {0,   1};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(expected, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
			
	}
	
	static void testTrees() {
		SLog.d("==============================");
		SLog.d("DUMB TREE");
		auto root = new STreeNode<string>("root");
		auto node1 = new STreeNode<string>("node 1");
		auto node11 = new STreeNode<string>("node 11");
		auto node2 = new STreeNode<string>("node 2");
		auto node21 = new STreeNode<string>("node 21");
		auto node211 = new STreeNode<string>("node 211");
		auto node22 = new STreeNode<string>("node 22");
		
		root.addChild(node1);
		node1.addChild(node11);
		
		root.addChild(node2);
		node2.addChild(node21);
		node21.addChild(node211);
		node2.addChild(node22);
		
		root.debugPrint();
		
		SLog.d("==============================");
		
		//SGameConfig.getTree("cfg_sUDE").debugPrint();
	}
	
	
	static void testSColor() {
		SLog.d("==============================");
		int expected[7] = {
			0xAA112233,
			0x112233AA,
			0x112233,
			0xAA,
			0x11,
			0x22,
			0x33
		};
		
		SLog.d("-----------------------------","argb()");
		testSColorValues(expected, SColor.argb(0xAA112233));
		
		
		SLog.d("-----------------------------","rgba()");
		testSColorValues(expected, SColor.rgba(0x112233AA));
		
		
		
		SLog.d("-----------------------------","rgb()");
		expected = {
			0xFF112233,
			0x112233FF,
			0x112233,
			0xFF,
			0x11,
			0x22,
			0x33
		};
		testSColorValues(expected, SColor.rgb(0x112233));
		
		
		
		
		SLog.d("-----------------------------","setAlpha()");
		expected = {
			0xCC112233,
			0x112233CC,
			0x112233,
			0xCC,
			0x11,
			0x22,
			0x33
		};
		testSColorValues(expected, SColor.rgba(0x112233AA).setAlpha(0xCC));
		
		
		
		SLog.d("-----------------------------","setRed()");
		expected = {
			0xAA992233,
			0x992233AA,
			0x992233,
			0xAA,
			0x99,
			0x22,
			0x33
		};
		testSColorValues(expected, SColor.rgba(0x112233AA).setRed(0x99));
		
		
		
		SLog.d("-----------------------------","setGreen()");
		expected = {
			0xAA119933,
			0x119933AA,
			0x119933,
			0xAA,
			0x11,
			0x99,
			0x33
		};
		testSColorValues(expected, SColor.rgba(0x112233AA).setGreen(0x99));
		
		SLog.d("-----------------------------","setBlue()");
		expected = {
			0xAA112299,
			0x112299AA,
			0x112299,
			0xAA,
			0x11,
			0x22,
			0x99
		};
		testSColorValues(expected, SColor.rgba(0x112233AA).setBlue(0x99));
	}
	
	
	static void testSColorValues(int expected[7], SColor color){
		assertEqual(expected[0], color.getARGB());
		assertEqual(expected[1], color.getRGBA());
		assertEqual(expected[2], color.getRGB());
		assertEqual(expected[3], color.getAlpha());
		assertEqual(expected[4], color.getRed());
		assertEqual(expected[5], color.getGreen());
		assertEqual(expected[6], color.getBlue());
	}
	
	static void assertEqual(array<float> expected, array<float> got) {
		if (SMath.equal(expected, got)) {
			pass(string.Format(PASSED_OUTPUT, expected, got));
		} else {
			fail(string.Format(FAILED_OUTPUT, expected, got));
		}
	}
	
	static void assertEqual(float expected, float got) {
		if (SMath.equal(expected, got)) {
			pass(string.Format(PASSED_OUTPUT, expected, got));
		} else {
			fail(string.Format(FAILED_OUTPUT, expected, got));
		}
	}
	
	static void assertEqual(int expected, int got) {
		if (expected == got) {
			pass(string.Format(PASSED_OUTPUT, expected, got));
		} else {
			fail(string.Format(FAILED_OUTPUT, expected, got));
		}
	}
	
	static void assertEqual(string expected, string got) {
		if (expected == got) {
			pass(string.Format(PASSED_OUTPUT, expected, got));
		} else {
			fail(string.Format(FAILED_OUTPUT, expected, got));
		}
	}
	
	static void assertEqual(bool expected, bool got) {
		if (expected == got) {
			pass(string.Format(PASSED_OUTPUT, expected.ToString(), got.ToString()));
		} else {
			fail(string.Format(FAILED_OUTPUT, expected.ToString(), got.ToString()));
		}
	}
	
	static void pass(string message) {
		SLog.d(message, "", 1, SHOW_PASSED);
	}
	
	static void fail(string message) {
		SLog.d(message, "", 1);
	}
	
}

