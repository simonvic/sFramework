class PPEDefaultPreset : PPEParams {}


class PPEDebugAnimation : PPELoopedParams{	
	override void onAnimate(float deltaTime){
		clear();
		setGodraysIntensity(0.0);
		//setOverlay(Math.Cos(getTime() * 2), SPPEManager.getPPEColor(Math.AbsFloat(Math.Sin(getTime())),0,0,0));
		//setCameraEffects(Math.Sin(getTime() * 2), SPPEManager.getPPEColor(Math.AbsFloat(Math.Sin(getTime() * 5)),1,1,0));
		//setVignetteColor(SColor.rgba(0xFF000000));
		/* rain
		setVignetteIntensity(0.0);
		setVignetteColor(SColor.rgb(RGBColors.AQUAMARINE));
		float rain = GetGame().GetWeather().GetRain().GetActual();
		
		float size = SMath.mapTo(rain, 0, 1, 10, 5);
		setFilmGrain(0.5, size);
		GetDayZGame().NightVissionLightParams(1.0, 0.05);
		*/
	}
}