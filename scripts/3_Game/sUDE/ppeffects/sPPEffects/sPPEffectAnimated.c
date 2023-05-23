enum ePPEAnimationStates {
	PLAYING = 0,
	PAUSED,
	STOPPED	
}

class SPPEffectAnimated : SPPEffect {

	protected ePPEAnimationStates m_animationState = ePPEAnimationStates.STOPPED;
	protected float m_time;

	override void Start(Param par = null) {
		super.Start(par);
		m_time = 0;
		onInit();
		setAnimationState(ePPEAnimationStates.PLAYING);
	}
	
	override void Stop(Param par = null) {
		super.Stop(par);
		m_time = 0;
		resetAllToDefault();
		setAnimationState(ePPEAnimationStates.STOPPED);
	}
	
	override void OnUpdate(float delta) {
		super.OnUpdate(delta);
		animate(delta);
	}
	
	void animate(float deltaTime) {
		if (!isPlaying()) return;
		
		// cheat to keep the effect animating even if requested data is the same
		// otherwise it would stop
		SetRequesterUpdating(true);
		m_time += deltaTime;
		onAnimate(deltaTime);
	}
	
	void onAnimate(float deltaTime);
		
	/**
	*	@brief Set the animation state to PLAYING and reset the time
	*/
	void start() {
		SetRequesterUpdating(true);
		Start();
	}
	
	/**
	*	@brief Reset values to default and prepare to stop
	*/
	void stop() {
		Stop();
	}
	
	/**
	*	@brief Pause the animation from animating without resetting the time
	*/
	void pause() {
		setAnimationState(ePPEAnimationStates.PAUSED);
	}
	
	/**
	*	@brief Resume the animation
	*/
	void resume() {
		SetRequesterUpdating(true);
		setAnimationState(ePPEAnimationStates.PLAYING);
	}
	
	/**
	*	@brief Get the time elapsed from the animation start
	*	@return \p float - time elapsed
	*/
	float getTime() {
		return m_time;
	}
	
	bool isPlaying() {
		return m_animationState == ePPEAnimationStates.PLAYING;
	}
	
	bool isPaused() {
		return m_animationState == ePPEAnimationStates.PAUSED;
	}
	
	bool hasStopped() {
		return m_animationState == ePPEAnimationStates.STOPPED;
	}
	
	ePPEAnimationStates getAnimationState() {
		return m_animationState;
	}
	
	protected void setAnimationState(ePPEAnimationStates state) {
		m_animationState = state;
	}
}