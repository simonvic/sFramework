//@todo managed?
class SRaycast {
	
	protected vector m_startPos;
	protected vector m_endPos;
	
	protected float m_radius;
	protected int m_intersectionType;
	protected CollisionFlags m_collisionFlags;
	protected bool m_isSorted;
	protected bool m_isGroundOnly;
	protected ref array<Object> m_ignoredObjects = {null, null};	

	protected vector m_contactPos;
	protected vector m_contactDir;
	protected int m_contactComponent;
	protected ref set<Object> m_hitObjects = new set<Object>;
		
	protected bool m_hasHit;
	
	void SRaycast(vector from, vector to, float radius = 0.1, int intersectionType = ObjIntersectView, CollisionFlags collisionFlags = CollisionFlags.NEARESTCONTACT, bool sorted = false, bool groundOnly = false){
		m_startPos = from;
		m_endPos = to;
		m_radius = radius;
		m_intersectionType = intersectionType;
		m_collisionFlags = collisionFlags;
		m_isSorted = sorted;
		m_isGroundOnly = groundOnly;
	}
	
	
	/**
	*	@brief Quickly set raycast parameters
	*	 @param from \p vector - starting position
	*	 @param end \p vector - ending position
	*	 @param radius \p float - radius of the ray
	*	 @param intersectionType \p int - intersection type. ObjIntersectFire(0), ObjIntersectView(1), ObjIntersectGeom(2), ObjIntersectIFire(3)
	*	 @param collisionFlags \p CollisionFlags - 
	*	 @param sorted \p bool - used only if groundOnly = false
	*	 @param groundOnly \p bool - raycasts only ground (ignores all objects)
	*/
	SRaycast init(vector from, vector to, float radius, int intersectionType = ObjIntersectView, CollisionFlags collisionFlags = CollisionFlags.NEARESTCONTACT, bool sorted = false, bool groundOnly = false){
		m_startPos = from;
		m_endPos = to;
		m_radius = radius;
		m_intersectionType = intersectionType;
		m_collisionFlags = collisionFlags;
		m_isSorted = sorted;
		m_isGroundOnly = groundOnly;
		return this;
	}
	
	
	/**
	*	@brief Quickly set raycast parameters
	*	 @param from \p vector - starting position
	*	 @param end \p vector - ending position	
	*/
	SRaycast init(vector from, vector to){
		m_startPos = from;
		m_endPos = to;
		return this;
	}
	
	/**
	*	@brief Quickly set starting position
	*	 @param from \p vector - starting position
	*/
	SRaycast from(vector from){
		m_startPos = from;
		return this;
	}
	
	/**
	*	@brief Quickly set ending position
	*	 @param to \p vector - ending position
	*/
	SRaycast to(vector to){
		m_endPos = to;
		return this;
	}
		
	/**
	*	@brief Quickly set ignored objects
	*	 @param Object \p obj1 - object to ignore
	*	 @param Object \p obj2 - object to ignore
	*/
	SRaycast ignore(Object obj1 = null, Object obj2 = null){
		m_ignoredObjects = {obj1, obj2};
		return this;
	}
		
	/**
	*	@brief Launch the ray	
	*/
	SRaycast launch(){
		m_hasHit = DayZPhysics.RaycastRV(m_startPos,
			m_endPos,
			m_contactPos,
			m_contactDir,
			m_contactComponent,
			m_hitObjects,
			m_ignoredObjects.Get(0), m_ignoredObjects.Get(1),
			m_isSorted,
			m_isGroundOnly,
			m_intersectionType,
			m_radius,
			m_collisionFlags);
		return this;		
	}
	
	//@todo add setters
		
	bool hasHit(){
		return m_hasHit;
	}
	
	vector getStartPosition(){
		return m_startPos;
	}
	
	vector getEndPosition(){
		return m_endPos;
	}
	
	vector getContactPosition(){
		return m_contactPos;
	}
	
	vector getContactDirection(){
		return m_contactDir;
	}
	
	int getContactComponent(){
		return m_contactComponent;
	}
	
	set<Object> getHitObjects(){
		return m_hitObjects;
	}
	
	array<Object> getIgnoredObjects(){
		return m_ignoredObjects;
	}
	
	bool isSorted(){
		return m_isSorted;
	}
	
	bool isGroundOnly(){
		return m_isGroundOnly;
	}
	
	int getIntersectionType(){
		return m_intersectionType;
	}
	
	float getRadius(){
		return m_radius;
	}
	
	CollisionFlags getCollisionFlags(){
		return m_collisionFlags;
	}
	


	void setStartPosition(vector startPos){
		m_startPos = startPos;
	}
	
	void setEndPosition(vector endPos){
		m_endPos = endPos;
	}
	
	void setIsSorted(bool isSorted){
		m_isSorted = isSorted;
	}
	
	void setIsGroundOnly(bool isGroundOnly){
		m_isGroundOnly = isGroundOnly;
	}
	
	void setIntersectionType(int intersectionType){
		m_intersectionType = intersectionType;
	}
	
	void setRadius(float radius){
		m_radius = radius;
	}
	
	void setCollisionFlags(CollisionFlags collisionFlags){
		m_collisionFlags = collisionFlags;
	}	
	
}