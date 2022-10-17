class SMath {
	
	static const float e = 2.7182818284;
	static const float PI_SQ = 9.8696;
	
	static float Exp(float x) {
		return Math.Pow(e,x);
	}
	
	/**
	* @brief Return position on the GaussBell				// ⁅ 𝑎𝑒^((−(𝑥−𝑏)^2)/(2𝑐^2 )) ⁆
	* 	@param height \p float Height of the curve
	* 	@param center \p float Center of the curve
	* 	@param width \p float Standard deviation of the curve (Gaussian RMS width) ( > 0 )
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float Gauss(float height, float center, float width, float x) {
		return height * Exp((- Math.Pow(x - center,2)) / 2 * Math.Pow(width,2));
	}
	
	/**
	* @brief Return position on the GaussBell (probability density function)		⁅ 1/(𝑤√2𝜋) 𝑒^((−1/2) ((𝑥−𝑐)/𝑤)^2 ) ⁆
	* 	@param center \p float Center of the curve
	* 	@param width \p float Standard deviation of the curve (Gaussian RMS width) ( > 0 )
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float GaussPDF(float center, float width, float x) {
		return ( 1 / width * Math.Sqrt(2 * Math.PI) ) * Exp((- Math.Pow(x - center,2)) / 2 * Math.Pow(width,2));
	}
	
	/**
	* @brief Return position on a damping down sin function reaching for target.		⁅ 𝑎𝑒^(−𝑑𝑥) (cos(𝑓𝑥+𝑝))+𝑡 ⁆
	* 	@param amplitude \p float Initial amplitude. (0: none, 0.5: smooth , 1: sharp)
	* 	@param decay \p float How the function will decay. (0: none, 0.5: smooth , 1: sharp)
	* 	@param phase \p float Phase (shifting) of function
	* 	@param frequency \p float Target of damping function
	* 	@param target \p float Target of damping function
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float DampedSin(float amplitude, float decay, float phase, float frequency, float target, float x) {
		return amplitude * Exp(- decay * x) * (Math.Cos(frequency * x + phase)) + target;
	}
	
	/**
	* @brief Return position on a damping down sin function reaching for target.		⁅ 𝑎arctan(𝑥𝑓+ℎ)+𝑣 ⁆
	* 	@param amplitude \p float Initial amplitude.
	* 	@param sharpness \p float Smoothness of curve, (0.1: linear, 1.5: smooth, 2.5: sharp etc)
	* 	@param verticalOffset \p float Center of function, Y intersection
	* 	@param horizontalOffset \p float Center of function, Y intersection
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float Arctan(float amplitude, float sharpness, float horizontalOffset, float verticalOffset, float x) {
		return amplitude * Math.Atan(x * sharpness + horizontalOffset) + verticalOffset;
	}
	
	/**
	* @brief Return if two float numbers are equal
	* 	@param x \p float Frist float value
	* 	@param y \p float Second float value
	* 	@param y \p float Epsilon value (tolerance)
	* 	@return \p bool - if are equals
	*/
	static bool equal(float x, float y, float epsilon = 1e-32) {
		return Math.AbsFloat(x - y) < epsilon;		
	}
	
	/**
	* @brief Return if two arrays float are equal
	* 	@param x \p float Frist float array value
	* 	@param y \p float Second float array value
	* 	@param y \p float Epsilon value (tolerance)
	* 	@return \p bool - if are equals
	*/
	static bool equal(array<float> x, array<float> y, float epsilon = 1e-32) {
		if (!x || !y) return false;
		
		int xCount = x.Count();
		if (xCount != y.Count()) {
			return false;
		}
		
		for (int i = 0; i < xCount; i++) {
			if (!SMath.equal(x[i], y[i], epsilon)) {
				return false;			
			}
		}
		
		return true;
	}
	
	/**
	* @brief Normalize a value from a range to 0..1 range (non-clamped)
	* 	@param value \p float - Value to normalize
	* 	@param min \p float - Value minimum range
	* 	@param max \p float - Value maximum range
	* 	@return \p float - normalized value
	*/
	static float normalize(float value, float min, float max) {
		return (value - min) / (max - min);
	}
	
	/**
	* @brief Normalize a value from a range to 0..1 range (clamped)
	* 	@param value \p float - Value to normalize
	* 	@param min \p float - Value minimum range
	* 	@param max \p float - Value maximum range
	* 	@return \p float - normalized value
	*/
	static float normalizeClamp(float value, float min, float max) {
		return Math.Clamp(SMath.normalize(value, min, max), 0, 1);
	}
	
	/**
	*	@DEPRECATED
	* @brief Map a value from a range to another
	* 	@param value \p float - Value to map
	* 	@param min \p float - Value minimum range
	* 	@param max \p float - Value maximum range
	* 	@param mappedMin \p float - Mapped value maximum range
	* 	@param mappedMax \p float - Mapped value maximum range
	* 	@return \p float - mapped value
	*/
	static float mapTo(float value, float min, float max, float mappedMin = 0, float mappedMax = 1) {
		return ((value - min) / (max - min)) * (mappedMax - mappedMin) + mappedMin;
	}
	
	/**
	* @brief Map a value from a range to another
	* 	@param value \p float - Value to map
	* 	@param min \p float - Value minimum range
	* 	@param max \p float - Value maximum range
	* 	@param mappedMin \p float - Mapped value maximum range
	* 	@param mappedMax \p float - Mapped value maximum range
	* 	@return \p float - mapped value
	*/
	static float map(float value, float min, float max, float mappedMin, float mappedMax) {
		return ((value - min) / (max - min)) * (mappedMax - mappedMin) + mappedMin;
	}
	
	/**
	* @brief Map a value from a range to another (clamped)
	* 	@param value \p float - Value to map
	* 	@param min \p float - Value minimum range
	* 	@param max \p float - Value maximum range
	* 	@param mappedMin \p float - Mapped value maximum range
	* 	@param mappedMax \p float - Mapped value maximum range
	* 	@return \p float - mapped value
	*/
	static float mapClamp(float value, float min, float max, float mappedMin, float mappedMax) {
		return Math.Clamp(SMath.map(value, min, max, mappedMin, mappedMax), mappedMin, mappedMax);
	}
	
	/**
	*	@brief Truncate the less significant digits of a float
	*	 @param value \p float - value
	*	 @param digits \p int - number of desired deciaml digits (min 1, max 6)
	*	 @return float - value with reduced precision
	*/
	static float reducePrecision(float value, int digits) {
		digits = Math.Clamp(digits, 1, 6);
		digits = Math.Pow(10, digits);
		return (float)((int) (value * digits)) / digits;
	}
	
	
	/**
	*	@brief Clamp a float array given two arrays containing the minimum and maximum values.
	*	       The min and max array MUST have an equal or greater count than the array to clamp, unclamped array will be returned otherwise
	*	 @param toClamp \p array<float> - array to clamp
	*	 @param min \p array<float> - array of minimums
	*	 @param max \p array<float> - array of maximums
	*/
	static void clamp(array<float> toClamp, array<float> min, array<float> max) {
		if (!toClamp || !min || !max) return;
		
		int toClampCount = toClamp.Count();
		if (min.Count() < toClampCount || max.Count() < toClampCount) return;
		
		for (int i = 0; i < toClampCount; i++) {
			toClamp[i] = (Math.Clamp(toClamp[i], min[i], max[i]));
		}
	}

	/**
	*	@brief Check if every float array is in range of the given minium and maximum
	*	       The min and max array MUST have an equal or greater count than the array to verify, false will be returned otherwise
	*	 @param values \p array<float> - array to verify
	*	 @param min \p array<float> - array of minimums
	*	 @param max \p array<float> - array of maximums
	*	 @return bool - if all values are in range
	*/
	static bool isInRange(array<float> values, array<float> min, array<float> max) {
		if (!values || !min || !max) return false;
		
		int valuesCount = values.Count();
		if (min.Count() < valuesCount || max.Count() < valuesCount) return false;
		
		for (int i = 0; i < valuesCount; i++) {
			if (!SMath.isInRange(values[i], min[i], max[i])) return false;
		}
		
		return true;
	}
	
	/**
	*	@brief Check if a float value is in range of the given minium and maximum [ INCLUSIVE ]
	*	 @param value \p float - value to verify
	*	 @param min \p float - minimum
	*	 @param max \p float - maximum
	*	 @return bool - if value is in range
	*/
	static bool isInRange(float value, float min, float max) {
		return value >= min && value <= max;
	}
	
	
}