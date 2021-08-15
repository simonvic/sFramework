class PPEDefaultOverlayPreset : PPEOverlayPreset{
	override void onInit(){
		initPreset(0.0, PPEManager.getPPEColor(0,0,0,0));
	}
}

class PPEDebugOverlayPreset : PPEOverlayPreset{
	override void onInit(){
		initPreset(0.5, PPEManager.getPPEColor(1,0,0,0));
	}
}