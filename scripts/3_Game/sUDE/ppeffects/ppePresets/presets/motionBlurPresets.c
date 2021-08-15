class PPEDefaultMotionBlurPreset : PPEMotionBlurPreset {
	override void onInit(){		
		initPreset(0.01, 0.8, 0.9, 10, 100);
	}
}

class PPEDebugMotionBlurPreset : PPEMotionBlurPreset {
	override void onInit(){
		initPreset(0.01, 0.8, 0.9, 10, 100);
	}
}