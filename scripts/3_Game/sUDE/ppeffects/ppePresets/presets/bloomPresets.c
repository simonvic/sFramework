class PPEDefaultBloomPreset : PPEBloomPreset{
	override void onInit(){
		initPreset(0.0, 0.0, 0.0);
	}
}

class PPEDebugBloomPreset : PPEBloomPreset {
	override void onInit(){
		initPreset(0.5, 0.1, 0.01);
	}
}