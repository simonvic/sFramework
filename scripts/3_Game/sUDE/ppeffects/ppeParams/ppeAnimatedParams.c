enum ePPEAnimationStates {
	PLAYING = 0,
	PAUSED,
	STOPPED	
}

class PPEAnimatedParams : PPEParams {

	protected ePPEAnimationStates m_animationState = ePPEAnimationStates.STOPPED;
	protected float m_time;
	
	void animate(float deltaTime){
		if(!isPlaying()) return;
		
		m_time += deltaTime;
		onAnimate(deltaTime);
	}
	
	void onAnimate(float deltaTime){}
		
	/**
	* @brief Set the animation state to PLAYING and reset the time
	*/
	void start(){
		m_time = 0;
		onInit();
		setAnimationState(ePPEAnimationStates.PLAYING);
	}
	
	/**
	* @brief Reset values to default and prepare to stop
	*/
	void stop(){
		resetAllToDefault();
		setAnimationState(ePPEAnimationStates.STOPPED);
	}
	
	/**
	* @brief Pause the animation from animating without resetting the time
	*/
	void pause(){
		setAnimationState(ePPEAnimationStates.PAUSED);
	}
	
	/**
	* @brief Resume the animation
	*/
	void resume(){
		setAnimationState(ePPEAnimationStates.PLAYING);
	}
	
	/**
	* @brief Get the time elapsed from the animation start
	* 	@return \p float - time elapsed
	*/
	float getTime(){
		return m_time;
	}
	
	bool isPlaying(){
		return m_animationState == ePPEAnimationStates.PLAYING;
	}
	
	bool isPaused(){
		return m_animationState == ePPEAnimationStates.PAUSED;
	}
	
	bool hasStopped(){
		return m_animationState == ePPEAnimationStates.STOPPED;
	}
	
	ePPEAnimationStates getAnimationState(){
		return m_animationState;
	}
	
	protected void setAnimationState(ePPEAnimationStates state){
		m_animationState = state;
	}
}

class PPELoopedParams : PPEAnimatedParams {}

class PPETimedParams : PPEAnimatedParams {
	protected float m_duration;
	protected bool m_deactivationOnStop;
	
	void PPETimedParams(float duration, bool deactivationOnStop = true){
		m_duration = duration;
		m_deactivationOnStop = deactivationOnStop;
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
	*
	*/
	bool shouldDeactivateOnStop(){
		return m_deactivationOnStop;
	}
	
}