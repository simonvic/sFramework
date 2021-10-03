class STest {

	static void testUserConfigs() {
	
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
		
		SGameConfig.getTree("cfg_sUDE").debugPrint();
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
		testSColorValues(SColor.argb(0xAA112233), expected);
		
		
		SLog.d("-----------------------------","rgba()");
		testSColorValues(SColor.rgba(0x112233AA), expected);
		
		
		
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
		testSColorValues(SColor.rgb(0x112233), expected);
		
		
		
		
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
		testSColorValues(SColor.rgba(0x112233AA).setAlpha(0xCC), expected);
		
		
		
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
		testSColorValues(SColor.rgba(0x112233AA).setRed(0x99), expected);
		
		
		
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
		testSColorValues(SColor.rgba(0x112233AA).setGreen(0x99), expected);
		
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
		testSColorValues(SColor.rgba(0x112233AA).setBlue(0x99), expected);
	}
	
	
	static void testSColorValues(SColor color, int expected[7]){
		assertEqual(color.getARGB(),  expected[0]);
		assertEqual(color.getRGBA(),  expected[1]);
		assertEqual(color.getRGB(),   expected[2]);
		assertEqual(color.getAlpha(), expected[3]);
		assertEqual(color.getRed(),   expected[4]);
		assertEqual(color.getGreen(), expected[5]);
		assertEqual(color.getBlue(),  expected[6]);
	}
	
	static void assertEqual(int x, int y) {
		if (x == y) {
			SLog.d("value: " + x + " | expected: " + y, "PASSED", 1);
		} else {
			SLog.d("value: " + x + " | expected: " + y, "FAILED", 1);
		}
	}
	
}