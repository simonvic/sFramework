class PPEDefaultBlurPreset : PPEBlurPreset{
	override void onInit(){
		initPreset(0.0);
	}
}

class PPEDebugBlurPreset : PPEBlurPreset{
	override void onInit(){
		initPreset(1.0);
	}
}