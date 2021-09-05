class PPEDefaultLensPreset : PPELensPreset{
	override void onInit(){
		initPreset(0.0, 0.0, 0.0, 0.0);
	}
}

class PPEDebugLensPreset : PPELensPreset{
	override void onInit(){
		initPreset(2.0, 0.0, 0.0, 0.5);
	}
}