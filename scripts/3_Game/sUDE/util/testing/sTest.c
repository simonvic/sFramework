class STest {

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
}