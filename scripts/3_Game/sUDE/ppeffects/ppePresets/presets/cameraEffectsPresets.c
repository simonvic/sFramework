class PPEDefaultCameraEffectPreset : PPECameraEffectPreset{
	override void onInit(){
		initPreset(1, SPPEManager.getPPEColor(1,1,1,0));
		
	}
}

class PPEDebugCameraEffectPreset : PPECameraEffectPreset{
	override void onInit(){
		initPreset(2, SPPEManager.getPPEColor(10,1,1,0));
	}
}