class PPEDefaultFilmGrainPreset : PPEFilmGrainPreset{
	override void onInit(){
		initPreset(0.0, 0.0);
	}
}

class PPEDebugFilmGrainPreset : PPEFilmGrainPreset{
	override void onInit(){
		initPreset(1.0, 1.0);
	}
}