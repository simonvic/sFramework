class SCameraOverlayTimed : SCameraOverlayAnimated {
	
	protected float m_duration;
	protected bool m_deactivateOnStop;
	
	void SCameraOverlayTimed(
		string image = "",
		float alpha = 1.0,
		string mask = "",
		float maskProgress = 1.0,
		float maskTransitionWidth = 0.1,
		vector position = "0.0 0.0 0.0",
		vector size = "1.0 1.0 1.0",
		vector rotation = "0.0 0.0 0.0",
		int priority = 0,
		array<typename> targetCameras = null,
		bool hidesWithIngameHUD = false,
		float duration = 5,
		bool deactivateOnStop = true){

		m_duration = duration;
		m_deactivateOnStop = deactivateOnStop;
	}
	
	static SCameraOverlayTimed build(
		float duration,
		bool deactivateOnStop = true,
		string image = "",
		float alpha = 1.0,
		string mask = "",
		float maskProgress = 1.0,
		float maskTransitionWidth = 0.1,
		vector position = "0 0 0",
		vector size = "1 1 1",
		vector rotation = "0 0 0",
		int priority = 0,
		array<typename> targetCameras = null,
		bool hidesWithIngameHUD = false){
		
		return new SCameraOverlayTimed(image, alpha, mask, maskProgress, maskTransitionWidth, position, size, rotation, priority, targetCameras, hidesWithIngameHUD, duration, deactivateOnStop);
	}
	
	override void animate(float deltaTime){
		if( !isPlaying() ) {
			return; 
		}
		
		if(getTime() >= m_duration ) {
			stop();
			return;
		}
		super.animate(deltaTime);
	}
	
	/**
	* @brief Set the duration of the animation
	* 	@param duration \p float - animation duration (seconds)
	*/
	void setDuration(float duration){
		m_duration = duration;
	}
		
	/**
	* @brief Get the animation duration
	* 	@return \p float - animation duration(seconds)
	*/
	float getDuration() {
		return m_duration;
	}
		
	/**
	* @brief Get the time remaining to the completition
	* 	@return \p float - time remaining (seconds)
	*/
	float getRemaining() {
		return m_duration - m_time;
	}
	
	/**
	* @brief Get if the animation should be deactivated when ends
	*/
	bool shouldDeactivateOnStop(){
		return m_deactivateOnStop;
	}
	
	/**
	*	@brief Set if the overlay has to be deactivated on has stopped
	*	 @param deactivateOnStop \p bool 
	*/
	void setDeactivateOnStop(bool deactivateOnStop){
		m_deactivateOnStop = deactivateOnStop;
	}
	
}