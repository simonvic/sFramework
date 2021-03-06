typedef set<ref SPPEffect> TSPPEffectsList;
typedef set<ref SPPEffectAnimated> TSPPEffectsAnimatedList;

class SPPEManager {
	
	protected static float m_time;
	
	//=========== Depth of Field ==============
	protected static ref DDOFPreset m_DDOF = new DDOFPreset(); //default Dynamic Depth of Field values 
	protected static bool m_DDOF_Enabled = true;
	protected static float m_DDOFVelocity[1];
	protected static float m_DDOFBlurVelocity[1];
	
	protected static ref DoFPreset m_targetDOF = new DoFPreset(); //target Depth of field values

	protected static ref DoFPreset m_resultDOF = new DoFPreset(); //final DOF calculated

	//=========== Motion Blur ==============
	protected static bool m_MotionBlur_Enabled = true;
	
	//=========== Bloom ==============
	protected static bool m_Bloom_Enabled = false;
	
	//=========== Materials ============== (unused)
	protected static Material glow;
	protected static Material radialBlur;
	protected static Material motionBlur;
	protected static Material gaussBlur;
	protected static Material chromAber;	
	protected static Material filmgrain;
	
	//=========== PostProcess Effects Parameters ==============
	protected static ref SPPEffect m_defaultPPE = new SPPEffect(); //default params with no modifiers
	protected static ref SPPEffect m_vanillaPPE = new SPPEffect(); // used by vanilla PPEffects class
	
	protected static ref TSPPEffectsList m_persistentPPE = new TSPPEffectsList; //all "non-animated" params
	protected static ref TSPPEffectsAnimatedList m_animatedPPE = new TSPPEffectsAnimatedList; //all animated params
	
	protected static ref SPPEffect m_requestedPPE = new SPPEffect(); // merged values of requested ppe
	protected static ref SPPEffect m_resultPPE = new SPPEffect(); // final ppe params after the merge
	
	//=========================================================
	
	
	////////////////////////////////////////////////////////////
	//				INIT
	////////////////////////////////////////////////////////////
	
	void SPPEManager(){
		
	}
	
	void ~SPPEManager(){
		onDestroy();
	}
	
	static void onInit(){
		initDOF();
		loadMaterials();	
		loadDefaultParams();	
		activateInitialPPE();
	}
		
	protected static void onDestroy(){
		
	}
	
	protected static void initDOF(){
		m_targetDOF.initPreset(m_DDOF);
		m_resultDOF.initPreset(m_DDOF);
	}
	
	//@todo find a way of caching material instead of getting it every time
	protected static void loadMaterials(){
		glow = GetGame().GetWorld().GetMaterial(PPEMaterialsNames.GLOW);
		radialBlur = GetGame().GetWorld().GetMaterial(PPEMaterialsNames.RADIAL_BLUR);
		motionBlur = GetGame().GetWorld().GetMaterial(PPEMaterialsNames.MOTION_BLUR);
		gaussBlur = GetGame().GetWorld().GetMaterial(PPEMaterialsNames.GAUSS_BLUR);
		chromAber = GetGame().GetWorld().GetMaterial(PPEMaterialsNames.CHROM_ABER);
		filmgrain = GetGame().GetWorld().GetMaterial(PPEMaterialsNames.FILM_GRAIN);
	}
	
	protected static void loadDefaultParams(){
		m_defaultPPE.init(new PPEDefaultPreset());
		m_resultPPE.init(new PPEDefaultPreset());
	}
	
	protected static void activateInitialPPE(){
		//activate(m_vanillaPPE); //merged manually
	}
	
	
	
	
	
	
	

	////////////////////////////////////////////////////////////
	//				ACTIVATION / DEACTIVATION
	
	//@todo check for IsClient inside activate/deactivate ?
	
	/**
	* @brief Add a post process effect 
	* 	@params params \p SPPEffect - Parameters to be added
	*/
	static void activate(SPPEffect params){
		if(GetGame().IsServer() && GetGame().IsMultiplayer()){
			SLog.w("ACTIVATING " + params + " on server!","SPPEManager::activate");
			return;
		}
		if(params.IsInherited(SPPEffectAnimated)){
			SPPEffectAnimated ppeAp = SPPEffectAnimated.Cast(params);
			m_animatedPPE.Insert(ppeAp); // https://www.youtube.com/watch?v=Ct6BUPvE2sM
			ppeAp.start();
		}else{
			m_persistentPPE.Insert(params);
		}
		params.onActivate();
	}
	
	/**
	* @brief Remove a post process effect 
	* 	@params params \p SPPEffect - Parameters to be removed
	*/
	static void deactivate(SPPEffect params){
		if(GetGame().IsServer() && GetGame().IsMultiplayer()){
			SLog.w("DEACTIVATING " + params + " on server!","SPPEManager::deactivate");
			return;
		}
		if(params.IsInherited(SPPEffectAnimated)){
			SPPEffectAnimated ppeAp = SPPEffectAnimated.Cast(params);
			ppeAp.stop();
			m_animatedPPE.Remove(m_animatedPPE.Find(ppeAp));
		}else{
			m_persistentPPE.Remove(m_persistentPPE.Find(params));			
		}
		params.onDeactivate();
	}
	
	/**
	*	@brief Toggle the post process effect
	*	 @params params \p SPPEffect - Parameters to be toggled
	*	 @params activate \p bool - state
	*/
	static void toggle(SPPEffect params, bool activate){
		if(activate) 
			activate(params);
		else
			deactivate(params);
		
	}
	
	/**
	* @brief Remove all PostProcessing effects
	*/
	static void deactivateAll(){
		deactivateAllPersitentPPE();
		deactivateAllAnimations();		
	}
	
	static void deactivateAllPersitentPPE(){
		m_persistentPPE.Clear();
	}
	
	static void deactivateAllAnimations(){
		foreach(auto ppeAp : m_animatedPPE){
			ppeAp.stop();
		}
		m_animatedPPE.Clear();
	}
	
	/**
	*	@brief Immediately apply default params
	*/
	static void applyDefault(){
		applyParams(m_defaultPPE);
	}
	
	
	
	
	
	
	
	
	
	
	
	////////////////////////////////////////////////////////////
	//				UPDATING PROCESS
	////////////////////////////////////////////////////////////
	
	static void onUpdate(float delta_time){
		m_time += delta_time;

		animateParams(delta_time);
		mergeResult();
		if(m_resultPPE.hasChanged()){
			applyParams(m_resultPPE);
		}
		
		if(isDDOFEnabled() /*&& !m_resultDOF.equals(m_targetDOF, 0.01)*/){
			updateDDOF(delta_time);
			applyDOF(m_resultDOF);
		}
	}	
	
	protected static void updateDDOF(float pDt){
		m_resultDOF.blurStrength = Math.SmoothCD(m_resultDOF.blurStrength, m_targetDOF.blurStrength, m_DDOFBlurVelocity, 0.05, 100, pDt); //smooth the blur strength over time
		m_resultDOF.focusDistance = Math.SmoothCD(m_resultDOF.focusDistance, m_targetDOF.focusDistance, m_DDOFVelocity, 0.15, 1000, pDt); //smooth the focus distance over time
	}
		
	/**
	* @brief Iterate over all animated parameters and animate
	*/
	protected static void animateParams(float deltaTime){
		TSPPEffectsAnimatedList toDeactivate = new TSPPEffectsAnimatedList;
		foreach(SPPEffectAnimated ap : m_animatedPPE){
			if(ap.isPlaying()) {
				ap.animate(deltaTime);
			}else if(ap.hasStopped() && SPPEffectTimed.Cast(ap) && SPPEffectTimed.Cast(ap).shouldDeactivateOnStop()){
				toDeactivate.Insert(ap);
			}
		}
		
		foreach(SPPEffectAnimated apToDeactivate : toDeactivate){
			deactivate(apToDeactivate);
		}
	}
	
	/**
	*	@brief Merge the result PPEparams that will finally be applied
	*/
	protected static void mergeResult(){
		mergeResultWithRequestedPPE();
		mergeResultWithDefault();		
	}
	
	/**
	*	@brief Merge the requested PPE 
	*/
	protected static void mergeResultWithRequestedPPE(){
	
		m_requestedPPE.clear();
		
		//Apply persistent PPEffects
		foreach (SPPEffect persistentPPE : m_persistentPPE) {
			m_requestedPPE.merge(persistentPPE);
			persistentPPE.onMerge();
		}
		
		//Apply animated PPEffects
		foreach (SPPEffectAnimated animatedPPE : m_animatedPPE) {
			if (animatedPPE.hasChanged() || animatedPPE.isPaused()) {
				m_requestedPPE.merge(animatedPPE);
				animatedPPE.onMerge();
			}
		}
		
		m_resultPPE.merge(m_requestedPPE, PPEMergeFlags.INTERPOLATE | PPEMergeFlags.INTERSECTION, SPPEConstants.ACTIVATION_SPEED);
	}
	
	/**
	*	@brief Interpolate non-requested values to default
	*	 forgive me for this crap, I need to rush :(
	*/
	protected static void mergeResultWithDefault(){
		mergeResultFloatWithDefault();
		mergeResultColorWithDefault();
	}
	
	protected static void mergeResultFloatWithDefault(){
		TPPEFloatParamsMap temp = m_resultPPE.getFloatParams();
		foreach (auto ppeMaterial, auto ppeFloatParams : temp) {
			foreach (auto ppeParamName, auto ppeParamValue : ppeFloatParams) {
				if (!m_requestedPPE.containsFloat(ppeMaterial, ppeParamName)) {
					float defaultValue = m_defaultPPE.getFloatParam(ppeMaterial, ppeParamName);
					if (!SMath.equal(ppeParamValue, defaultValue, SPPEConstants.EPSILON)) {
						m_resultPPE.setParam(
							ppeMaterial,
							ppeParamName,
							Math.Lerp(ppeParamValue, defaultValue, SPPEConstants.RESET_SPEED));
					}
				}
			}
		}
	}
	
	protected static void mergeResultColorWithDefault(){
		TPPEColorParamsMap temp = m_resultPPE.getColorParams();
		foreach (auto ppeMaterial, auto ppeColorParams : temp) {
			foreach (auto ppeParamName, auto ppeParamValue : ppeColorParams) {
				if (!m_requestedPPE.containsColor(ppeMaterial, ppeParamName)) {
					TPPEColor defaultValue = m_defaultPPE.getColorParam(ppeMaterial, ppeParamName);
					if (!SMath.equal(ppeParamValue, defaultValue, SPPEConstants.EPSILON)) {
						m_resultPPE.setParam(
							ppeMaterial,
							ppeParamName,
							SPPEManager.mixColors(ppeParamValue, defaultValue, SPPEConstants.RESET_SPEED));
					}
				}
			}
		}
	}

	
	/**
	* @brief Iterate and apply the effect
	* 	@params params \p SPPEffect - Effect to be applied
	*/
	protected static void applyParams(SPPEffect ppe){
		applyFloatParams(ppe);
		applyColorParams(ppe);
		ppe.onApply();
	}

	
	/**
	* @brief Iterate over all (float) parameters for each material and apply them	
	*/
	protected static void applyFloatParams(SPPEffect params){
		TPPEFloatParamsMap ppeParams = params.getFloatParams();
		foreach(auto ppeMaterial, auto ppeParam : ppeParams){
			foreach(auto ppeParamName, auto ppeParamValue : ppeParam){
				applyPPEParamTo(ppeParamName, ppeParamValue, ppeMaterial);
			}
		}

	}
	
	/**
	* @brief Iterate over all (TPPEColor) parameters for each material and apply them
	*/
	protected static void applyColorParams(SPPEffect ppe){
		TPPEColorParamsMap ppeParams = ppe.getColorParams();
		foreach(auto ppeMaterial, auto ppeParam : ppeParams){
			foreach(auto ppeParamName, auto ppeParamValue : ppeParam){
				applyPPEParamTo(ppeParamName, ppeParamValue, ppeMaterial);
			}
		}	
	}
	
	/**
	* @brief Apply a float parameter to a material
	* 	@param paramName \p TPPEParamName - Name of the parameter
	* 	@param paramValue \p float - Value of the parameter
	* 	@param mat \p TPPEMaterial - Material to apply parameter to
	* 	@return \p bool - return true if parameter has been applied succesfully, false otherwise
	*/
	protected static bool applyPPEParamTo(TPPEParamName paramName, float paramValue, TPPEMaterial mat){
		return GetGame().GetWorld().GetMaterial(mat).SetParam(paramName,paramValue);
	}
	
	
	/**
	* @brief Apply a TPPEColor parameter to a material
	* 	@param paramName \p TPPEParamName - Name of the parameter
	* 	@param paramValue \p TPPEColor - Color to be applied, which will be converted in a static array and then applied
	* 	@param mat \p TPPEMaterial - Material to apply parameter to
	* 	@return \p bool - return true if parameter has been applied succesfully, false otherwise
	*/
	protected static bool applyPPEParamTo(TPPEParamName paramName, TPPEColor paramValue, TPPEMaterial mat){
		float color[4];
		color[0] = paramValue[0];
		color[1] = paramValue[1];
		color[2] = paramValue[2];
		color[3] = paramValue[3];
		return GetGame().GetWorld().GetMaterial(mat).SetParam(paramName,color);
	}	
	
	
	/**
	* @brief Get the (float) default value of a parameter
	* 	@param ppeMaterial \p TPPEMaterial - Material of parameter
	* 	@param ppeParamName \p TPPEParamName - Name of parameter
	* 	@return \p float - default parameter value
	*/
	static float getFloatDefaultValue(TPPEMaterial ppeMaterial, TPPEParamName ppeParamName){
		return m_defaultPPE.getFloatParam(ppeMaterial, ppeParamName);
	}
	
	/**
	* @brief Get the (TPPEColor) default value of a parameter
	* 	@param ppeMaterial \p TPPEMaterial - Material of parameter
	* 	@param ppeParamName \p TPPEParamName - Name of parameter
	* 	@return \p TPPEColor - default parameter value
	*/
	static TPPEColor getColorDefaultValue(TPPEMaterial ppeMaterial, TPPEParamName ppeParamName){
		return m_defaultPPE.getColorParam(ppeMaterial, ppeParamName);
	}
	
	

	
	////////////////////////////////////////////////////////////
	//				UTILS
	////////////////////////////////////////////////////////////
	
	/**
	* @brief Convert four float values to a TPPEColor
	* 	@param R \p float - Red
	* 	@param G \p float - Green
	* 	@param B \p float - Blue
	* 	@param A \p float - Alpha
	* 	@return \p TPPEColor - return converted TPPEColor
	*/
	static TPPEColor getPPEColor(float R, float G, float B, float A){
		return {R, G, B, A};
	}
	
	/**
	* @brief Convert a SColor to a TPPEColor
	* 	@param color \p SColor 
	* 	@return \p TPPEColor - return converted TPPEColor
	*/
	static TPPEColor getPPEColor(SColor color){
		return getPPEColor(
			SMath.mapTo(color.getRed(), 0, 255),
			SMath.mapTo(color.getGreen(), 0, 255),
			SMath.mapTo(color.getBlue(), 0, 255),
			SMath.mapTo(color.getAlpha(), 0, 255));
	}
	
	/**
	* @brief Mix the values of two TPPEColor
	* 	@param color1 \p TPPEColor - First color
	* 	@param color2 \p TPPEColor - Second color
	* 	@return \p TPPEColor - return mixed TPPEColor
	*/
	static TPPEColor mixColors(TPPEColor color1, TPPEColor color2, float coeff = 0.5){
		if(!color1 || !color2) return null;
		
		TPPEColor result = new TPPEColor;
		result.InsertAt(Math.Lerp(color1[0], color2[0], coeff), 0); //Red
		result.InsertAt(Math.Lerp(color1[1], color2[1], coeff), 1); //Green
		result.InsertAt(Math.Lerp(color1[2], color2[2], coeff), 2); //Blue
		result.InsertAt(Math.Lerp(color1[3], color2[3], coeff), 3); //Alpha
		return result;
	}
	
	
	
	////////////////////////////////////////////////////////////
	//				DEBUG
	////////////////////////////////////////////////////////////
	static TSPPEffectsAnimatedList getAnimations(){
		return m_animatedPPE;
	}
	
	static void debugPrintAll(bool printParamsValues = false){
		SLog.d("debugPrintAll","PluginPPEffect",0);
		
		SLog.d("======================================================", "",0);
		SLog.d("-------------------- m_defaultPPE --------------------", "",0);
		m_defaultPPE.debugPrint();
				
		SLog.d("-------------------- m_persistentPPE --------------------", "",0);
		foreach(SPPEffect p : m_persistentPPE){
			if (printParamsValues) 
				p.debugPrint();
			else
				SLog.d(p);
		}
		
		SLog.d("-------------------- m_animatedPPE --------------------", "",0);
		foreach(SPPEffect ap : m_animatedPPE){
			if (printParamsValues)
				ap.debugPrint();
			else
				SLog.d(ap);
				
		}
		
		/*
		SLog.d("-------------------- m_vanillaPPE --------------------", "",0);
		if (printParamsValues)
			m_vanillaPPE.debugPrint();
		else
			SLog.d(m_vanillaPPE);
		*/
		SLog.d("-------------------- m_requestedPPE --------------------", "",0);		
		if (printParamsValues) 
			m_requestedPPE.debugPrint();
		else
			SLog.d(m_requestedPPE);
		
		
		SLog.d("-------------------- m_resultPPE --------------------", "",0);
		m_resultPPE.debugPrint();
		SLog.d("======================================================", "",0);

	}
	
	
	
	
	////////////////////////////////////////////////////////////
	//				VANILLA
	////////////////////////////////////////////////////////////
	
	/**
	*	@deprecated
	*	Methods used by vanilla code... Planned to be removed when vanilla code is replaced
	*/
	
	static void vanillaSetRadialBlur(float powerX, float powerY, float offsetX, float offsetY ){
		m_vanillaPPE.setRadialBlur(powerX, powerY, offsetX, offsetY);
	}

	static void vanillaSetGausBlur(float value){
		m_vanillaPPE.setGausBlur(value);
	}
	
	static void vanillaSetLensChromAber(float value){
		m_vanillaPPE.setLensChromAber(value);
	}
	
	static void vanillaSetOverlay(float factor, TPPEColor color){
		m_vanillaPPE.setOverlay(factor, color);
	}
	
	static void vanillaSetLens(float intensity, float centerX, float centerY, float chromAberIntensity){
		m_vanillaPPE.setLens(intensity, centerX, centerY, chromAberIntensity);
	}
	
	static void vanillaSetVignette(float intensity, TPPEColor color){
		m_vanillaPPE.setVignette(intensity, color);
	}
	
	static void vanillaSetSaturation(float saturation){
		m_vanillaPPE.setSaturation(saturation);
	}
	
	static void vanillaSetColorization(TPPEColor color){
		m_vanillaPPE.setColorization(color);
	}
	
	static void vanillaSetFilmGrain(float sharpness, float grainSize){
		m_vanillaPPE.setFilmGrain(sharpness, grainSize);
	}
	
	static void vanillaSetBloom(float steepness, float intensity, float threshold){
		m_vanillaPPE.setBloom(steepness, intensity, threshold);
	}
	
	static void vanillaOverrideDOF(bool enable, float focusDistance, float focusLength, float focusLengthNear, float blur, float focusDepthOffset) {
		applyDOF(enable, focusDistance, focusLength, focusLengthNear, blur, focusDepthOffset);
	}
	
	static void vanillaResetDOFOverride(){
		resetDOF();
	}
	
	static void vanillaAddPPMask(float ndcX, float ndcY, float ndcRadius, float ndcBlur) {
		requestOpticMask(ndcX, ndcY, ndcRadius, ndcBlur);
	}
	
	static void vanillaResetPPMask() {
		resetMask();
	}
	
	
	////////////////////////////////////////////////////////////
	//				DEPTH of FIELD
	////////////////////////////////////////////////////////////
	
	static float getDDOFStrength(){
		return m_DDOF.blurStrength;
	}
	
	static float getDDOFMaxDistance(){
		return m_DDOF.focusMaxDistance;
	}
	
	static float getDDOFMinDistance(){
		return m_DDOF.focusMinDistance;
	}
		
	static void setDDOFBlurStrength(float blurStrength){
		if(blurStrength <= 0){
			m_DDOF.blurStrength = 0;
			SPPEManager.resetDDOF(true);
		}else{
			m_DDOF.blurStrength = blurStrength;
		}
	}
		
	static void enableDDOF(){
		m_DDOF_Enabled = true;
	}
	
	static void disableDDOF(){
		m_DDOF_Enabled = false;
	}
	
	static bool isDDOFEnabled(){
		return m_DDOF_Enabled && m_DDOF.blurStrength != 0;
	}
	
	static void setDDOFEnabledIn3PP(bool enabled){
		if(enabled) {
			enableDDOF();
		}else{
			disableDDOF();
			resetDDOF(true);
		}
	}
	
	static void setDDOFEnabledInVehicle(bool enabled){
		if(enabled) {
			enableDDOF();
		}else{
			disableDDOF();
			resetDDOF(true);
		}
	}

	/**
	*	@brief Reset Dynamic Depth of Field
	*	 @param immediate \p bool - immediately reset the DoF
	*/
	static void resetDDOF(bool immediate = false){
		if(immediate){
			resetDOF();
		}else{
			m_targetDOF.blurStrength = 0;
			m_targetDOF.focusDistance = 0;
		}
	}
	
	/**
	*	@brief Reset Depth of Field
	*/
	protected static void resetDOF(){
		applyDOF(false, 0, 0, 0, 0, 1);
	}
	
	static void requestDDOF(float focusDistance){
		m_DDOF.focusDistance = focusDistance;
		m_targetDOF.focusDistance = focusDistance;
		m_targetDOF.blurStrength = m_DDOF.blurStrength;
		enableDDOF();
	}
	
	
	static void requestWeaponDOF(DoFPreset dof){
		applyDOF(dof);
	}
	
	static void resetWeaponDOF(){
		resetDOF();
	}
	
	protected static void applyDOF(DoFPreset dof){
		applyDOF(true, Math.Clamp(dof.focusDistance, dof.focusMinDistance, dof.focusMaxDistance), dof.focusLength, dof.focusLengthNear, dof.blurStrength, dof.focusDepthOffset);
	}
	
	protected static void applyDOF(bool enabled, float focusDistance, float focusLength, float focusLengthNear, float blurStrength, float focusDepthOffset){
		GetGame().OverrideDOF(enabled, focusDistance, focusLength, focusLengthNear, blurStrength, focusDepthOffset);
	}
	
	
	
	
	
	
	////////////////////////////////////////////////////////////
	//				MOTION BLUR
	////////////////////////////////////////////////////////////
	
	static bool isMotionBlurEnabled(){
		return m_MotionBlur_Enabled;
	}
	
	static float getMotionBlurStrength(){
		return m_defaultPPE.getFloatParam(PPEMaterialsNames.MOTION_BLUR, PPEParamNames.MOTION_BLUR_POWER);
	}
	
	static void setMotionBlurStrength(float strength){
		if(strength <= 0){
			m_MotionBlur_Enabled = false;
			m_defaultPPE.setMotionBlurPower(0.0);
		}else{
			m_MotionBlur_Enabled = true;
			m_defaultPPE.setMotionBlurPower(strength);
		}
		
	}
	
	////////////////////////////////////////////////////////////
	//				BLOOM
	////////////////////////////////////////////////////////////
	
	static bool isBloomEnabled(){
		return m_Bloom_Enabled;
	}
	
	static float getBloomStrength(){
		return m_defaultPPE.getFloatParam(PPEMaterialsNames.GLOW, PPEParamNames.BLOOM_STEEPNESS);
	}
	
	static void setBloomStrength(float strength){
		if(strength <= 0){
			m_Bloom_Enabled = false;
			m_defaultPPE.setBloomSteepness(0.0);
		}else{
			m_Bloom_Enabled = true;
			m_defaultPPE.setBloomSteepness(strength);
		}
		
	}
	
	////////////////////////////////////////////////////////////
	//				OPTICS (LENSES AND MASKS)
	////////////////////////////////////////////////////////////

	static void requestOpticMask(TFloatArray mask){
		requestOpticMask(mask[0],mask[1],mask[2],mask[3]);
	}
	
	static void requestOpticMask(float positionX, float positionY, float radius, float blur){
		resetMask();
		GetGame().AddPPMask(positionX, positionY, radius, blur);
	}
		
	static void resetMask(){
		GetGame().ResetPPMask();
	}
	
	static void requestOpticLens(TFloatArray lens){
		requestOpticLens(lens[0],lens[1],lens[2],lens[3]);
	}
	
	static void requestOpticLens(float intensity, float centerX, float centerY, float chromAberIntensity){
		m_vanillaPPE.setLens(intensity, centerX, centerY, chromAberIntensity);
		m_defaultPPE.setLens(intensity, centerX, centerY, chromAberIntensity);
		m_resultPPE.setLens(intensity, centerX, centerY, chromAberIntensity);
	}
	
	static void resetOpticLens(){
		m_vanillaPPE.setLens(0, 0, 0, 0);
		m_defaultPPE.setLens(0, 0, 0, 0);
		m_resultPPE.setLens(0, 0, 0, 0);
	}

}