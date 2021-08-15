class PPEDefaultChromAberPreset : PPEChromAberPreset{
	override void onInit(){
		initPreset(0.0, 0.0);
	}
}

class PPEDebugChromAberPreset : PPEChromAberPreset{
	override void onInit(){
		initPreset(0.5, 0.1);
	}
}