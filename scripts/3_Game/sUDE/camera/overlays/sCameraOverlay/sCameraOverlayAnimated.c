//if we had interfaces...
class SCameraOverlayAnimated : SCameraOverlay{
	
	protected eSAnimableState m_animationState = eSAnimableState.STOPPED;
	protected float m_time;
	
	void onStart();
	void onAnimate(float deltaTime);
	void onStop();
	void onPause();
	void onResume();
	
	/**
	* @brief Set the animation state to PLAYING and reset the time
	*/
	void start(){
		m_time = 0;
		setAnimationState(eSAnimableState.PLAYING);
		onStart();
	}
	
	/**
	*	@brief Called on each frame
	*/
	void animate(float deltaTime){
		if(!isPlaying()) return;
		
		m_time += deltaTime;
		onAnimate(deltaTime);
	}
	
	/**
	* @brief Reset values to default and prepare to stop
	*/
	void stop(){
		setAnimationState(eSAnimableState.STOPPED);
		onStop();
	}
	
	/**
	* @brief Pause the animation from animating without resetting the time
	*/
	void pause(){
		setAnimationState(eSAnimableState.PAUSED);
		onPause();
	}
	
	/**
	* @brief Resume the animation
	*/
	void resume(){
		setAnimationState(eSAnimableState.PLAYING);
		onResume();
	}
	
	/**
	* @brief Get the time elapsed from the animation start
	* 	@return \p float - time elapsed
	*/
	float getTime(){
		return m_time;
	}
	
	bool isPlaying(){
		return m_animationState == eSAnimableState.PLAYING;
	}
	
	bool isPaused(){
		return m_animationState == eSAnimableState.PAUSED;
	}
	
	bool hasStopped(){
		return m_animationState == eSAnimableState.STOPPED;
	}
	
	eSAnimableState getAnimationState(){
		return m_animationState;
	}
	
	protected void setAnimationState(eSAnimableState state){
		m_animationState = state;
	}
}