class PPEDebugAnimation : PPELoopedParams{	
	override void onAnimate(float deltaTime){
		//setGausBlur(1);
		//setOverlay(Math.Cos(getTime() * 2), PPEManager.getPPEColor(Math.AbsFloat(Math.Sin(getTime())),0,0,0));
		//setCameraEffects(Math.Sin(getTime() * 2), PPEManager.getPPEColor(Math.AbsFloat(Math.Sin(getTime() * 5)),1,1,0));
		//resetAllToDefault();
		setVignetteIntensity(0.9);
		setVignetteColor(SColor.rgb(RGBColors.AQUAMARINE));
	}
}