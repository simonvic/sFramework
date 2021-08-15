class SFlagOperator : Managed{
	int flag;
	
	void SFlagOperator(int x = 0){
		flag = x;
	}
	
	void ~SFlagOperator(){
	}
	
	
	/**
	*	@brief Flip all bits (NOT operator)
	*	 @code 
	*	  00100000 11010000 10001011 01000011 ~
	*	  ___________________________________
	*	  11011111 00101111 01110100 10111100
	*/
	SFlagOperator not(){
		flag = ~flag;
		return this;
	}
	
	
	/**
	*	@brief Apply a mask (AND operator)
	*	 @code 
	*	  00100000 11010000 10001011 01000011 &
	*	  00000000 00000000 00000000 11111111 
	*	  ___________________________________
	*	  00000000 00000000 00000000 01000011
	*
	*/
	SFlagOperator mask(int x){
		flag &= x;
		return this;
	}
	
	
	/**
	*	@brief Set bits to 1 (OR operator)
	*	 @code 
	*	  00100000 11010000 10001011 01000011 |
	*	  00000000 11111111 00000000 00000000 
	*	  ___________________________________
	*	  00100000 11111111 10001011 01000011
	*
	*/
	SFlagOperator set(int x){
		flag |= x;
		return this;
	}
	
	
	/**
	*	@brief Set bits to 0 (AND + NOT operator)
	*	 @code 
	*	  00100000 11010000 10001011 01000011 &~
	*	  00000000 11111111 00000000 00000000 
	*	  ___________________________________
	*	  00100000 00000000 10001011 01000011
	*
	*/
	SFlagOperator reset(int x){
		flag &= ~x;
		return this;
	}
	
	
	/**
	*	@brief Flip bits to 0 (XOR operator)
	*	 @code 
	*	  00100000 11010000 10001011 01000011 ^
	*	  00000000 11111111 00000000 00000000 
	*	  ___________________________________
	*	  00100000 00101111 10001011 01000011
	*
	*/
	SFlagOperator flip(int x){
		flag = (~flag & x) | (flag & ~x);
		return this;
	}
	
	
	/**
	*	@brief Collect integer value
	*	 @param value \p int [out] - output integer value
	*/
	SFlagOperator collect(out int value){
		value = flag;
		return this;
	}
	
	
	/**
	*	@brief Collect binary formatted string
	*	 @param value \p string [out] - output string value
	*	 @param digits \p int - number of digits to display
	*	 @param spacingEvery \p int - after how many digits there should be a space
	*/
	SFlagOperator collect(out string value, int digits = 32, int spacingEvery = 8){
		value = toBinaryString(flag, digits, spacingEvery);
		return this;
	}
	
	
	/**
	*	@brief Collect integer value
	*	 @return int
	*/
	int collect(){
		return flag;
	}
	
	
	/**
	*	@brief Collect binary string value
	*	 @return string
	*/
	string collectBinaryString(int digits = 32, int spacingEvery = 8){
		return toBinaryString(flag, digits, spacingEvery);
	}
	
	
	/**
	*	@brief Check if a bit is set
	*	 @bool if bit is 1
	*/
	bool check(int x){
		return flag & x;
	}
	
	
	/**
	*	@brief Convert integer to binary formatted string
	*	 @param x \p int - value to convert
	*	 @param digits \p int - number of digits to display
	*	 @param spacingEvery \p int - after how many digits there should be a space
	*/
	static string toBinaryString(int x, int digits = 32, int spacingEvery = 8){
		string result;
		int pos = 0;
		while (x > 0 || pos < digits){
			result = "" + (x & 1) + result;
			if((pos + 1) % spacingEvery == 0) result = " " + result;
			x >>= 1;
			pos++;
		}
		return result;
	}
	
	
	/**
	*	@brief Get an instance of SFlagOperator
	*	 @param x \p int - flag value
	*/
	static SFlagOperator from(int x = 0){
		return new SFlagOperator(x);
	}
	
	/**
	*	@brief Get an instance of SFlagOperator
	*	 @param fop \p SFlagOperator - 
	*/
	static SFlagOperator from(SFlagOperator fop){
		return new SFlagOperator(fop.collect());
	}
		
}