# Moving around the particle system

## 3D case

```cpp
body.setup();
{
	com_ParticleSystem.setup(this->getOrigin(), ....)
	com_ParticleSystem.setup(Body::com => ofSpherePrimitive, ...)
	{
		//inside ParticleSystem
		this->origin = origin = Body::com = reference &com;
		...
		newParticle.setup(origin, ...);
		{
			//inside Particle3D
			position = origin.getPosition() = glm::vec3;
		}
	}
}

body.move(x_dir,y_dir,z_dir);
{
	com.move(x_dir,y_dir,z_dir);
	...
	particle_systems[i].moveOrigin(x_dir,y_dir,z_dir); = com_particleSystem.moveOrigin(x_dir,y_dir,z_dir);
	{
		glm::vec3 current_pos = origin.getPosition(); 
		origin.setPosition(current_pos.x + x_dir, ...);
	}
}
```

**Q:** Come fa la particella (newParticle) ad aggiornare la propria origine se questa viene settata solo nel setup?   
**ANS:** Probabilmente ha a che fare con la natura di origin (ofNode), che in qualche modo riesce a tenere traccia della posizione.

**LEGENDA:** nelle funzioni, il passaggio by reference qui è esplicitato con &nome_variabile, nel vero codice non lo è. 
Nel dubbio, vedi la definizione della funzione.

## 2D case

```cpp
shadow setup (vec2& origin) 
	{
	shadow_origin = origin;
		ParticleSystem2D.setup (&shadowOrigin,...)
		{
			origin_PS2D = shadow_Origin;
			newParticle.setup(&origin_PS2D, ...)
		
shadow move(x_dir, y_dir)
	{
		glm::vec2 moving_v = (x_dir, y_dir);
		shadow_origin+= moving_v;
		particle_systems_2d[i].moveOrigin(&shadow_origin)

		ParticleSystem2D::moveOrigin(glm::vec2& newOrigin)
		{
			origin_PS2D = newOrigin;
	
// ================ UP TO HERE, THE SETUP-GENERATED PARTICLES ARE NOT AFFECTED BY ANY ORIGIN MOVING, BUT ONLY THE PS ORGIN (origin_PS2D)
when a particle isDead() --> addPArticle
...
newParticle.setup(&origin_PS2D,...) => it is generated starting from the moved origin_PS2D
```
