enum ePPEAnimationStates {
	PLAYING = 0,
	PAUSED,
	STOPPED	
}

class SPPEffectAnimated : SPPEffect {

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