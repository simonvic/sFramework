class SPPEffectTimed : SPPEffectAnimated {
	
	protected float m_duration;
	
	override void animate(float deltaTime) {
		if (getTime() >= m_duration) {
			Stop();
			return;
		}
		super.animate(deltaTime);
	}
	
	/**
	*	@brief Set the duration of the animation
	*	@param duration \p float - animation duration (seconds)
	*/
	void setDuration(float duration) {
		m_duration = duration;
	}
		
	/**
	*	@brief Get the animation duration
	*	@return \p float - animation duration(seconds)
	*/
	float getDuration() {
		return m_duration;
	}
		
	/**
	*	@brief Get the time remaining to the completition
	*	@return \p float - time remaining (seconds)
	*/
	float getRemaining() {
		return m_duration - m_time;
	}
	
}