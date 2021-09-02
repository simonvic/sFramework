class PPEDefaultVignettePreset : PPEVignettePreset{
	override void onInit(){
		initPreset(0, SPPEManager.getPPEColor(0,0,0,0));
	}
}

class PPEEpinephrineVignettePreset : PPEVignettePreset{
	override void onInit(){
		initPreset(0.5, SPPEManager.getPPEColor(0.1,0.1,0.1,0));
	}
}

class PPEFocusingVignettePreset : PPEVignettePreset{
	override void onInit(){
		initPreset(0, SPPEManager.getPPEColor(0,0,0,0));
	}
}