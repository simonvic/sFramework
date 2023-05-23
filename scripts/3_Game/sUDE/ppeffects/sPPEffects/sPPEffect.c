enum eSPPEPriority {
	DEFAULT = 1000,
	PAIN = 1010,
	BLEEDING = 1020,
	STAMINA = 1030
}

class SPPEffect : PPERequesterBase {
	
	protected bool m_initialized;
	protected PPOperators m_operator = PPOperators.SET;
	protected int m_priority = eSPPEPriority.DEFAULT;
	protected bool m_normalized = false;
	
	override int GetCategoryMask() {
		return PPERequesterCategory.GAMEPLAY_EFFECTS;
	}
	
	override void Start(Param par = null) {
		super.Start(par);
		if (!m_initialized) {
			onInit();
			m_initialized = true;
		}
		onActivate();
	}
	
	override void Stop(Param par = null) {
		super.Stop(par);
		onDeactivate();
	}
	
	/**
	*	@brief Check if the ppeffect is active
	*	@return true if active, false otherwise
	*/
	bool isActive() {
		return IsRequesterRunning();
	}
	
	/**
	*	@brief Activate the ppeffect
	*/
	void activate() {
		Start();
	}
	
	/**
	*	@brief Deactivate the ppeffect
	*/
	void deactivate() {
		Stop();
	}
	
	/**
	*	@brief Activate the ppeffect if not active, deactivate it otherwise
	*/
	void toggle() {
		if (isActive()) {
			Stop();
		} else {
			Start();
		}
	}
	
	/**
	* @brief Delete all parameters
	*/
	void clear() {
		ClearRequesterData();
	}
	
	/**
	* @brief Reset all currently set parameters to their default values
	*/
	void resetAllToDefault() {
		SetDefaultValuesAll();
	}

	/**
	*	@brief Callback invoked ONCE when the ppeffect is activated for the first time
	*/
	void onInit();
	
	/**
	*	@brief Callback invoked every time the effect is activated
	*/
	void onActivate();
	
	/**
	*	@brief Callback invoked every time the effect is deactivated
	*/
	void onDeactivate();
	
	////////////////////////////
	// SETTER HELPER
	
	void normalized(bool normalized) {
		m_normalized = normalized;
	}
	
	void priority(int priority) {
		m_priority = priority;
	}
	
	void op(PPOperators operator) {
		m_operator = operator;
	}
	
	////////////// VIGNETTE
	
	/**
	* @brief Quickly set vignette parameters
	* 	@param intensity \p float - Intensity of vignette
	* 	@param color \p SColor - Color of vignette
	*/
	void setVignette(float intensity, SColor color) {
		setVignetteIntensity(intensity);
		setVignetteColor(color);
	}
	
	/**
	* @brief Quickly set vignette intensity
	* 	@param intensity \p float - Intensity of vignette
	*/
	void setVignetteIntensity(float intensity) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTE, intensity);
	}
	
	/**
	* @brief Quickly set vignette color
	* 	@param color \p SColor - Color of vignette
	*/
	void setVignetteColor(SColor color) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTECOLOR, color);
	}
	
	
	////////////// OVERLAY
	
	/**
	* @brief Quickly set overlay parameters
	* 	@param factor \p float - Factor(strength?) of overlay
	* 	@param color \p SColor - Color of overlay
	*/
	void setOverlay(float factor, SColor color) {
		setOverlayFactor(factor);
		setOverlayColor(color);
	}
	
	/**
	* @brief Quickly set overlay factor
	* 	@param factor \p float - Factor(strength?) of overlay
	*/
	void setOverlayFactor(float factor) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_OVERLAYFACTOR, factor);
	}
	
	/**
	* @brief Quickly set overlay color
	* 	@param color \p SColor - Color of overlay
	*/
	void setOverlayColor(SColor color) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_OVERLAYCOLOR, color);
	}
	
	

	////////////// RADIAL BLUR
	
	/**
	* @brief Quickly set radial blur parameters
	* 	@param powerX \p float - Horizontal strength
	* 	@param powerY \p float - Vertical strength
	* 	@param offsetX \p float - Horizontal offset
	* 	@param offsetY \p float - Vertical offset
	*/
	void setRadialBlur(float powerX, float powerY, float offsetX = 0.0, float offsetY = 0.0) {
		setRadialBlurPowerX(powerX);	
		setRadialBlurPowerY(powerY);	
		setRadialBlurOffsetX(offsetX);	
		setRadialBlurOffsetY(offsetY);	
	}
	
	/**
	* @brief Quickly set radial blur power
	* 	@param power \p float - Horizontal and Vertical strength
	*/
	void setRadialBlurPower(float power) {
		setRadialBlurPowerX(power);
		setRadialBlurPowerY(power);
	}
	
	/**
	* @brief Quickly set radial blur horizontal power
	* 	@param powerX \p float - Horizontal strength
	*/
	void setRadialBlurPowerX(float powerX) {
		setParam(PostProcessEffectType.RadialBlur, PPERadialBlur.PARAM_POWERX, powerX);
	}
	
	/**
	* @brief Quickly set radial blur vertical power
	* 	@param powerY \p float - Vertical strength
	*/
	void setRadialBlurPowerY(float powerY) {
		setParam(PostProcessEffectType.RadialBlur, PPERadialBlur.PARAM_POWERY, powerY);
	}
	
	/**
	* @brief Quickly set radial blur offset
	* 	@param power \p float - Horizontal and Vertical offset
	*/
	void setRadialBlurOffset(float offset) {
		setRadialBlurOffsetX(offset);
		setRadialBlurOffsetY(offset);
	}
	
	/**
	* @brief Quickly set radial blur horizontal offset
	* 	@param offsetX \p float - Horizontal offset
	*/
	void setRadialBlurOffsetX(float offsetX) {
		setParam(PostProcessEffectType.RadialBlur, PPERadialBlur.PARAM_OFFSETX, offsetX);
	}
	
	/**
	* @brief Quickly set radial blur vertical offset
	* 	@param offsetY \p float - Vertical offset
	*/
	void setRadialBlurOffsetY(float offsetY) {
		setParam(PostProcessEffectType.RadialBlur, PPERadialBlur.PARAM_OFFSETY, offsetY);
	}
	
	
	
	
	////////////// FILM GRAIN
	
	/**
	* @brief Quickly set film grain parameters
	* 	@param sharpness \p float - Sharpness
	* 	@param grainSize \p float - Grain size
	*/
	void setFilmGrain(float sharpness, float grainSize) {
		setFilmGrainSharpness(sharpness);
		setFilmGrainSize(grainSize);
	}
	
	/**
	* @brief Quickly set film grain sharpness
	* 	@param sharpness \p float - Sharpness
	*/
	void setFilmGrainSharpness(float sharpness) {
		setParam(PostProcessEffectType.FilmGrain, PPEFilmGrain.PARAM_SHARPNESS, sharpness);
	}
	
	/**
	* @brief Quickly set film grain size
	* 	@param grainSize \p float - Grain size
	*/
	void setFilmGrainSize(float grainSize) {
		setParam(PostProcessEffectType.FilmGrain, PPEFilmGrain.PARAM_GRAINSIZE, grainSize);
	}
	
	
	
	
	
	////////////// GAUSSIAN BLUR
	
	/**
	* @brief Quickly set gaussian blur parameters
	* 	@param intensity \p float - Intensity
	*/
	void setGausBlur(float intensity) {
		setParam(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY, intensity);
	}
	
	
	
	////////////// CHROMATIC ABERATION
	
	/**
	* @brief Quickly set chromatic aberration parameters
	* 	@param power \p float - Horizontal and Vertical intensity
	*/
	void setChromAber(float power) {
		setChromAber(power, power);
	}
	
	/**
	* @brief Quickly set chromatic aberration parameters
	* 	@param powerX \p float - Horizontal intensity
	* 	@param powerY \p float - Vertical intensity
	*/
	void setChromAber(float powerX, float powerY) {
		setChromAberPowerX(powerX);
		setChromAberPowerY(powerY);
	}
	
	/**
	* @brief Quickly set chromatic horizontal intensity
	* 	@param powerX \p float - Horizontal intensity
	*/
	void setChromAberPowerX(float powerX) {
		setParam(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERX, powerX);
	}
	
	/**
	* @brief Quickly set chromatic vertical intensity
	* 	@param powerY \p float - Vertical intensity
	*/
	void setChromAberPowerY(float powerY) {
		setParam(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERY, powerY);
	}
	
	
	
	
	////////////// MAGNIFICATION LENS
	
	/**
	* @brief Quickly set magnification lens parameters
	* 	@param intensity \p float - Magnification power
	* 	@param centerX \p float - Horizontal screen position
	* 	@param centerY \p float - Vertical screen position
	* 	@param chromAber \p float - Lens chromatic aberration
	*/
	void setLens(float intensity, float centerX = 0.0, float centerY = 0.0, float chromAber = 0.0) {
		setLensIntensity(intensity);
		setLensCenterX(centerX);
		setLensCenterY(centerY);
		setLensChromAber(chromAber);
	}
	
	/**
	* @brief Quickly set magnification lens intensity
	* 	@param intensity \p float - Magnification power
	*/
	void setLensIntensity(float intensity) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_LENSDISTORT, intensity);
	}
	
	/**
	* @brief Quickly set magnification horizontal position
	* 	@param centerX \p float - Horizontal screen position
	*/
	void setLensCenterX(float centerX) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_LENSCENTERX, centerX);
	}
	
	/**
	* @brief Quickly set magnification vertical position
	* 	@param centerY \p float - Vertical screen position
	*/
	void setLensCenterY(float centerY) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_LENSCENTERY, centerY);
	}
	
	/**
	* @brief Quickly set magnification lens chromatic aberration
	* 	@param chromAber \p float - Lens chromatic aberration
	*/
	void setLensChromAber(float chromAber) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_LENSCENTERY, chromAber);
	}
	
	
	
	
	////////////// MOTION BLUR
	
	/**
	* @brief Quickly set motion blur parameters
	* 	@param power \p float - Power
	* 	@param minDepth \p float - Minimum depth from the camera position
	* 	@param maxDepth \p float - Maximum depth from the camera position
	*/
	void setMotionBlur(float power, float minDepth, float maxDepth, float minAnglePerSec, float maxAnglePerSec) {
		setMotionBlurPower(power);
		setMotionBlurMinDepth(minDepth);
		setMotionBlurMaxDepth(maxDepth);
		setMotionBlurMinAnglePerSec(minAnglePerSec);
		setMotionBlurMaxAnglePerSec(maxAnglePerSec);
	}
	
	/**
	* @brief Quickly set motion blur power
	* 	@param power \p float - Power
	*/
	void setMotionBlurPower(float power) {
		setParam(PostProcessEffectType.RotBlur, PPERotBlur.PARAM_POWER, power);
	}
	
	/**
	* @brief Quickly set motion blur minimum depth
	* 	@param minDepth \p float - Minimum depth from the camera position
	*/
	void setMotionBlurMinDepth(float minDepth) {
		setParam(PostProcessEffectType.RotBlur, PPERotBlur.PARAM_MINDEPTH, minDepth);
	}
	
	/**
	* @brief Quickly set motion blur maximum depth
	* 	@param maxDepth \p float - Maximum depth from the camera position
	*/
	void setMotionBlurMaxDepth(float maxDepth) {
		setParam(PostProcessEffectType.RotBlur, PPERotBlur.PARAM_MAXDEPTH, maxDepth);
	}
	
	/**
	* @brief Quickly set motion blur minimum angle per second
	* 	@param maxDepth \p float - Minimum angle
	*/
	void setMotionBlurMinAnglePerSec(float minAnglePerSec) {
		setParam(PostProcessEffectType.RotBlur, PPERotBlur.PARAM_MINANGLEPERSEC, minAnglePerSec);
	}
	
	/**
	* @brief Quickly set motion blur maximum angle per second
	* 	@param maxDepth \p float - Maximum angle
	*/
	void setMotionBlurMaxAnglePerSec(float maxAnglePerSec) {
		setParam(PostProcessEffectType.RotBlur, PPERotBlur.PARAM_MAXANGLEPERSEC, maxAnglePerSec);
	}
	
	
	
	
	
	
	////////////// BLOOM
	
	/**
	* @brief Quickly set bloom parameters
	* 	@param steepness \p float - The "smoothness" of the bloom (also used as strength setting instead of `Intensity`)	
	* 	@param intensity \p float - Intensity	
	* 	@param treshold \p float - Treshold of bloom, lower treshold means bloom on less reflective materials
	*/
	void setBloom(float steepness, float intensity, float treshold) {
		setBloomSteepness(steepness);		
		setBloomIntensity(intensity);		
		setBloomTreshold(treshold);		
	}
	
	/**
	* @brief Quickly set bloom steepness
	* 	@param steepness \p float - The "smoothness" of the bloom (also used as strength setting instead of `Intensity`)	
	*/
	void setBloomSteepness(float steepness) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_BLOOMSTEEPNESS, steepness);
	}
	
	/**
	* @brief Quickly set bloom intensity
	* 	@param intensity \p float - Intensity	
	*/
	void setBloomIntensity(float intensity) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_BLOOMINTENSITY, intensity);
	}
	
	/**
	* @brief Quickly set bloom treshold
	* 	@param treshold \p float - Treshold of bloom, lower treshold means bloom on less reflective materials
	*/
	void setBloomTreshold(float treshold) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_BLOOMTHRESHOLD, treshold);
	}
	
	
	
	////////////// GODRAYS //@todo complete doc
	
	/**
	*	@brief Quickly set godrays parameters
	*	 @param intensity \p float - 
	*	 @param overburn \p float - 
	*	 @param overburnStart \p float - 
	*	 @param overburnEnd \p float - 
	*	 @param mask \p float - 
	*	 @param size \p float - 
	*	 @param verticalIntensity \p float - 
	*	 @param diagonalIntensity \p float - 
	*/
	void setGodrays(float intensity, float overburn, float overburnStart, float overburnEnd, float mask, float size, float verticalIntensity, float diagonalIntensity) {
		setGodraysIntensity(intensity);
		setGodraysOverburn(overburn);
		setGodraysOverburnStart(overburnStart);
		setGodraysOverburnEnd(overburnEnd);
		setGodraysMask(mask);
		setGodraysSize(size);
		setGodraysVerticalIntensity(verticalIntensity);
		setGodraysDiagonalIntensity(diagonalIntensity);
	}
	
	/**
	*	@brief Quickly set godrays intensity
	*	 @param intensity \p float - 
	*/
	void setGodraysIntensity(float intensity) {
		setParam(PostProcessEffectType.GodRays, PPEGodRays.PARAM_INTENSITY, intensity);
	}
	
	/**
	*	@brief Quickly set godrays overburn
	*	 @param overburn \p float - 
	*/
	void setGodraysOverburn(float overburn) {
		setParam(PostProcessEffectType.GodRays, PPEGodRays.PARAM_OVERBURNINTENSITY, overburn);
	}
	
	/**
	*	@brief Quickly set godrays overburn start
	*	 @param overburnStart \p float - 
	*/
	void setGodraysOverburnStart(float overburnStart) {
		setParam(PostProcessEffectType.GodRays, PPEGodRays.PARAM_OVERBURNSTART, overburnStart);
	}
	
	
	/**
	*	@brief Quickly set godrays overburn end
	*	 @param overburnEnd \p float - 
	*/
	void setGodraysOverburnEnd(float overburnEnd) {
		setParam(PostProcessEffectType.GodRays, PPEGodRays.PARAM_OVERBURNEND, overburnEnd);
	}
	
	/**
	*	@brief Quickly set godrays mask
	*	 @param mask \p float - 
	*/
	void setGodraysMask(float mask) {
		//Warning, not to be used until DECLARE_RESOURCE_NAME solution is implemented!
		//setParam(PostProcessEffectType.GodRays, PPEGodRays.PARAM_SUNMASKMAT, mask);
	}
	
	/**
	*	@brief Quickly set godrays sun size
	*	 @param size \p float - 
	*/
	void setGodraysSize(float size) {
		
	}
	
	/**
	*	@brief Quickly set godrays vertical intensity
	*	 @param verticalIntensity \p float - 
	*/
	void setGodraysVerticalIntensity(float verticalIntensity) {
		
	}
	
	/**
	*	@brief Quickly set godrays diagonal intensity
	*	 @param diagonalIntensity \p float - 
	*/
	void setGodraysDiagonalIntensity(float diagonalIntensity) {
		
	}
	
	
	////////////// CAMERA EFFECTS
	
	/**
	* @brief Quickly set camera effects parameters
	* 	@param saturation \p float - Saturation of the camera
	* 	@param colorization \p SColor - Some kind of instagram color effect lol (pretty cool)
	*/
	void setCameraEffects(float saturation, SColor colorization) {
		setSaturation(saturation);
		setColorization(colorization);
	}
	
	
	/**
	* @brief Quickly set saturation
	* 	@param saturation \p float - Saturation of the camera
	*/
	void setSaturation(float saturation) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_SATURATION, saturation);
	}
	
	/**
	* @brief Quickly set colorization
	* 	@param colorization \p SColor - Some kind of instagram color effect lol (pretty cool)
	*/
	void setColorization(SColor colorization) {
		setParam(PostProcessEffectType.Glow, PPEGlow.PARAM_COLORIZATIONCOLOR, colorization);
	}
	
	
	////////////////////////////////////////////////////////////
	//				LOW LEVEL SETTERS

	
	/**
	* @brief Set a float parameter, (instantiate the maps if not instantiated)
	* 	@param ppeType - Type of post processing effect
	* 	@param paramIndex - Index of material parameter
	* 	@param paramValue - Value of the parameter
	* 	@param layer - 
	* 	@param operator - 
	* 	@param relativize - 
	*/
	void setParam(PostProcessEffectType ppeType, int paramIndex, float paramValue) {
		SetTargetValueFloat(ppeType, paramIndex, m_normalized, paramValue, m_priority, m_operator);
	}
	
	/**
	* @brief Set a float parameter, (instantiate the maps if not instantiated)
	* 	@param ppeType - Type of post processing effect
	* 	@param paramIndex - Index of material parameter
	* 	@param paramValue - Value of the parameter
	* 	@param layer - 
	* 	@param operator - 
	* 	@param relativize - 
	*/
	void setParam(PostProcessEffectType ppeType, int paramIndex, int paramValue) {
		SetTargetValueInt(ppeType, paramIndex, m_normalized, paramValue, m_priority, m_operator);
	}
	
	/**
	* @brief Set a float parameter, (instantiate the maps if not instantiated)
	* 	@param ppeType - Type of post processing effect
	* 	@param paramIndex - Index of material parameter
	* 	@param paramValue - Value of the parameter
	* 	@param layer - 
	* 	@param operator - 
	* 	@param relativize - 
	*/
	void setParam(PostProcessEffectType ppeType, int paramIndex, bool paramValue) {
		SetTargetValueBool(ppeType, paramIndex, paramValue, m_priority, m_operator);
	}
	
	/**
	* @brief Set a float parameter, (instantiate the maps if not instantiated)
	* 	@param ppeType - Type of post processing effect
	* 	@param paramIndex - Index of material parameter
	* 	@param paramValue - Value of the parameter
	* 	@param layer - 
	* 	@param operator - 
	* 	@param relativize - 
	*/
	void setParam(PostProcessEffectType ppeType, int paramIndex, SColor paramValue) {
		SetTargetValueColor(ppeType, paramIndex, paramValue.getRGBANormalizedArray(), m_priority, m_operator);
	}
	
	/*
	void debugUI() {
		auto dui = SDebugUI.of(ClassName());
		dui.begin();
		dui.window(ClassName());
		bool enable;
		dui.check("Enable", enable);
		if (!enable) return;
		dui.newline();
		foreach (auto ppeMaterial, auto ppeParam : m_params) {
			dui.text(ppeMaterial);
			dui.newline();
			foreach (auto ppeParamName, auto ppeParamValue : ppeParam) {
				float newVal = ppeParamValue;
				dui.slider(ppeParamName, newVal, 0.01, -1, 1);
				m_params[ppeMaterial][ppeParamName] = newVal;
			}
			dui.newline();
		}
		dui.end();
	}
	*/
	
}