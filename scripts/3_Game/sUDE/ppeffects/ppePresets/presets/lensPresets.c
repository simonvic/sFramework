class PPEDefaultLensPreset : PPELensPreset{
	override void onInit(){
		setParam(MaterialNames.GLOW, PPEParamNames.LENS_DISTORT, 0.0);
		setParam(MaterialNames.GLOW, PPEParamNames.LENS_CENTER_X, 0.0);
		setParam(MaterialNames.GLOW, PPEParamNames.LENS_CENTER_Y, 0.0);
		setParam(MaterialNames.GLOW, PPEParamNames.LENS_CHROM_ABER, 0.0);
		initPreset(0.0, 0.0, 0.0, 0.0);
	}
}

class PPEDebugLensPreset : PPELensPreset{
	override void onInit(){
		initPreset(2.0, 0.0, 0.0, 0.5);
	}
}