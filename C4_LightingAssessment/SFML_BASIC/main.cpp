//Name: Sebastian Horoszkiewicz
//ID:	C00156243
//Date:	29/03/2015
//Practical Assessment 2
//
//########################################################################################################
//############################################### CONTROLS ###############################################
//########################################################################################################
//	Camera:
//		W - forward, S-backward , A - left, D - right, Q - up, E - down
//		UP key - rotate up, DOWN key - rotate down, LEFT key - rotate left, RIGHT key - rotate right,
//	Torch and Light:
//		I - forward, K - backward, J - left, L - right, U - up, P - down
//		Z, X - on/off torch
//		R - rotate on y-axis, Y - rotate on y axis, T - rotate forward, G - rotate backward, F - rotate left, H - rotate right
//
//
//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 
#include <iostream> 

#include "Camera.h"

float CalcCylinderX(float, float, float[], float[]);
float CalcCylinderY(float, float, float[], float[]);
float CalcCylinderZ(float, float, float[], float[]);

float CalcCylinderX(float t, float phi, float pt1[3], float pt2[3]){
	return pt1[0]*(1-t)+pt2[0]*t;
}

float CalcCylinderY(float t, float phi, float pt1[3], float pt2[3]){
	float r = pt1[1]*(1-t)+pt2[1]*t;
	return r*cos(phi);
}

float CalcCylinderZ(float t, float phi, float pt1[3], float pt2[3]){
	float r = pt1[1]*(1-t)+pt2[1]*t;
	return r*sin(phi);
}

void NormalVector(GLfloat pt1[3], GLfloat pt2[3], GLfloat pt3[3], GLfloat n[3]){
     
    GLfloat v1[3],v2[3]; // vectors
    // calculate two vectors lying on the surface
    for(int i=0;i<3;i++){
        v1[i]=pt2[i]-pt1[i];
        v2[i]=pt3[i]-pt2[i];
    }
    // cross product of two vectors
    n[0]=v1[1]*v2[2] - v2[1]*v1[2];
    n[1]=v1[2]*v2[0] - v2[2]*v1[0];
    n[2]=v1[0]*v2[1] - v2[0]*v1[1];
     
}

void floor(){
		GLfloat normal[3];
		float fValOne=-100.0f, fValTwo=-96.0f;
		GLfloat floorPoints[][3]={  {fValOne,0.0f,fValTwo},	//0
									{fValOne,0.0f,fValOne},	//1
									{fValTwo,0.0f,fValTwo},	//2
									{fValTwo,0.0f,fValOne}};//3
		int increasebyThis=4;
		sf::Texture texLidwa;
		sf::Texture texLidrw;
		if (!texLidwa.loadFromFile("imgs/wood_ash.jpg")){}
		if (!texLidrw.loadFromFile("imgs/rosewood.jpg")){}
		glEnable(GL_TEXTURE_2D);//tell OpenGL to use textures when drawing
		for (int c1 = 0; c1 <= 49; c1+=1){
			for (int c = 0; c <= 24; c+=1){
				int sequence[2]={1,2};
				if (c1 % 2==0){
					sequence[0]=1;
					sequence[1]=2;
				}
				else{
					sequence[0]=2;
					sequence[1]=1;
				}
				int i=0;
				for (i=0; i<=1; i++){	
					switch(sequence[i]){ 
						case 1:{
							// Load Image wood_ash
							sf::Texture::bind(&texLidwa);//select this texture for OpenGL rendering
							glBegin(GL_QUADS);
								NormalVector(floorPoints[2],floorPoints[3],floorPoints[0],normal);
								glNormal3fv(normal);
								glTexCoord2f(0, 0);		glVertex3fv(floorPoints[0]);
								glTexCoord2f(0, 1);		glVertex3fv(floorPoints[1]); 
								glTexCoord2f(1, 1);		glVertex3fv(floorPoints[3]); 
								glTexCoord2f(1, 0);		glVertex3fv(floorPoints[2]); 
							glEnd();
							break;
						}
						case 2:{
							// Load Image
							sf::Texture::bind(&texLidrw);//select this texture for OpenGL rendering
							glBegin(GL_QUADS);
								NormalVector(floorPoints[2],floorPoints[3],floorPoints[0],normal);
								glNormal3fv(normal);
								glTexCoord2f(0, 0);		glVertex3fv(floorPoints[0]);
								glTexCoord2f(0, 1);		glVertex3fv(floorPoints[1]); 
								glTexCoord2f(1, 1);		glVertex3fv(floorPoints[3]); 
								glTexCoord2f(1, 0);		glVertex3fv(floorPoints[2]); 
							glEnd();
							break;
						}
					}
					floorPoints[0][0]=floorPoints[0][0]+increasebyThis;
					floorPoints[1][0]=floorPoints[1][0]+increasebyThis;
					floorPoints[2][0]=floorPoints[2][0]+increasebyThis;
					floorPoints[3][0]=floorPoints[3][0]+increasebyThis;
				}
			}
			floorPoints[0][0] = fValOne;
			floorPoints[0][2] =	floorPoints[0][2]+increasebyThis;
			floorPoints[1][0] = fValOne;
			floorPoints[1][2] = floorPoints[1][2]+increasebyThis;
			floorPoints[2][0] = fValTwo;
			floorPoints[2][2] = floorPoints[2][2]+increasebyThis;
			floorPoints[3][0] = fValTwo;
			floorPoints[3][2] = floorPoints[3][2]+increasebyThis;
		}
}

void lighting(){
	//lighting
	GLfloat materialAmbDiff[] = {0.9f, 0.5f, 0.3f, 1.0f}; // create an array of RGBA values
	GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // create an array of RGBA values (White)
	GLfloat materialShininess[] = {128.0f}; // select value between 0-128, 128=shiniest
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff); // set the diffuse & ambient reflection colour for the front of faces
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular); // set the colour of specular reflection
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess); // set shininess of the material

	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] ={ 1.0, 1.0, 1.0, 1.0 };

	GLfloat light_position[] ={ 0,0,0, 1.5};
	GLfloat spot_direction[] = { 0 ,-1, 0 };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//light strength
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.01);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
	//light angle and initial direction
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.1);
}

void torch(){
	float p1[] = { 0, 1.5, 0 };
	float p2[] = { 2.5, 1.0, 0 };
	float TWO_PI = 3.141 * 2;
	GLfloat normal[3];
	const double r=1.0;
	const double PI=3.141;
	const int sides=400;

	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0,2.5,0); //centre
		for(int i = 0; i<=sides+1; i++){
			float x = r*cos(i*(2*PI/sides));
			float y = r*sin(i*(2*PI/sides));
			glVertex3f(x,2.5,y); //point on edge
		}
	glEnd();

	glBegin(GL_QUADS);
		float dt = 0.01;
		float dphi = 0.07;
		for (float t = 0; t <= 1; t += dt){
			for (float phi = 0; phi<TWO_PI; phi += dphi){
				GLfloat cylPoints[][3]={{	CalcCylinderZ(t, phi, p1, p2),
											CalcCylinderX(t, phi, p1, p2),
											CalcCylinderY(t, phi, p1, p2)},				//0
										{
											CalcCylinderZ(t, phi + dphi, p1, p2),
											CalcCylinderX(t, phi + dphi, p1, p2),
											CalcCylinderY(t, phi + dphi, p1, p2)},		//1
										{	
											CalcCylinderZ(t + dt, phi + dphi, p1, p2),
											CalcCylinderX(t + dt, phi + dphi, p1, p2),
											CalcCylinderY(t + dt, phi + dphi, p1, p2)}, //2
										{
											CalcCylinderZ(t + dt, phi, p1, p2),
											CalcCylinderX(t + dt, phi, p1, p2),
											CalcCylinderY(t + dt, phi, p1, p2)}};		//3
				NormalVector(cylPoints[2],cylPoints[3],cylPoints[0],normal);
				glNormal3fv(normal);
				glTexCoord2f(0, 0);		glVertex3fv(cylPoints[0]);
				glTexCoord2f(0, 1);		glVertex3fv(cylPoints[1]);
				glTexCoord2f(1, 1);		glVertex3fv(cylPoints[2]);
				glTexCoord2f(1, 0);		glVertex3fv(cylPoints[3]);
			}
		}
	glEnd();
}

void bottle(){
	const double r=1.0, PI=3.141, r2=0.4;
	const int sides=400;
	const float xAxes=1.5f;
	GLfloat normal[3];
	sf::Texture tex;
	if (!tex.loadFromFile("imgs/bdws1.jpg")){}	
	glEnable(GL_TEXTURE_2D);//tell OpenGL to use textures when drawing

	//bottle cap
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0,5.7,0); //centre
		for(int i = 0; i<=sides+1; i++){
			float x = r2*cos(i*(2*PI/sides));
			float y = r2*sin(i*(2*PI/sides));
			glVertex3f(x,5.7,y); //point on edge
		}
	glEnd();

	glEnable(GL_CULL_FACE);
	//bottle bottom
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0,-xAxes,0); //centre
		for(int i = 0; i<=sides+1; i++){
			float x = r*cos(i*(2*PI/sides));
			float y = r*sin(i*(2*PI/sides));
			glVertex3f(x,-xAxes,y); //point on edge
		}
	glEnd();

	float dt = 0.1, dphi = 0.01, TWO_PI = 3.141 * 2;
	const int NUM_OF_POINTS=7;
	float allValues[][3]={	{-xAxes, 1.0, 0 },	//0
							{ 2.1, 1.0, 0 },	//1
							{ 4, 0.4, 0 },		//2
							{ 5.5, 0.4, 0 },	//3
							{ 5.5, 0.35, 0 },	//4
							{ 5.6, 0.35, 0 },	//5
							{ 5.6, 0.4, 0 },	//6
							{ 5.7, 0.4, 0 },	//7
							};
	sf::Texture::bind(&tex);
	float	ypospt=0.2f,	allquads=1.f,
			width =.006, increaseby=.25;
	
	glCullFace(GL_FRONT);
	glBegin(GL_QUADS);
	//each cylinder
	for (int i = 0; i<NUM_OF_POINTS; i++){	
		//height
		for (float t = 0; t <= 1; t += dt){		
			float	tp0=0,	tp1=0,	tp2=0,	tp3=0;
			//width
			if (i==0 && t > 0.2f && t < 0.7f){
				allquads-=increaseby;
				ypospt+=.1f;
			}
			for (float phi = 0; phi<TWO_PI; phi += dphi){
				GLfloat cylPoints[][3]={{	CalcCylinderZ(t, phi, allValues[i], allValues[i+1]),
												CalcCylinderX(t, phi, allValues[i], allValues[i+1]),
												CalcCylinderY(t, phi, allValues[i], allValues[i+1])},				//0
											{
												CalcCylinderZ(t, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderX(t, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderY(t, phi + dphi, allValues[i], allValues[i+1])},		//1
											{	
												CalcCylinderZ(t + dt, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderX(t + dt, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderY(t + dt, phi + dphi, allValues[i], allValues[i+1])},	//2
											{
												CalcCylinderZ(t + dt, phi, allValues[i], allValues[i+1]),
												CalcCylinderX(t + dt, phi, allValues[i], allValues[i+1]),
												CalcCylinderY(t + dt, phi, allValues[i], allValues[i+1])}};			//3
					NormalVector(cylPoints[2],cylPoints[3],cylPoints[0],normal);
					glNormal3fv(normal);
					if ((i==0) && ((t==ypospt) && (ypospt<0.7f))){
						glTexCoord2f(tp0, allquads);					glVertex3fv(cylPoints[3]);//tp0
						glTexCoord2f(tp1, allquads);					glVertex3fv(cylPoints[2]);//tp1
						glTexCoord2f(tp2+width, allquads+increaseby);	glVertex3fv(cylPoints[1]);//tp2
						glTexCoord2f(tp3+width, allquads+increaseby);	glVertex3fv(cylPoints[0]);//tp3
						tp0+=width;	tp1+=width;
						tp2+=width;	tp3+=width;
					}
					else {
						glVertex3fv(cylPoints[3]);
						glVertex3fv(cylPoints[2]);
						glVertex3fv(cylPoints[1]);
						glVertex3fv(cylPoints[0]);
					}	
				}
			}	
		}
	glEnd();
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

void dice(){
	//Start Dice
	GLfloat normal[3];
	float x=0.5f, y=0.5f, z=0.5f;
	static GLfloat points[][3]={    { x, y, z},	//0 b1
									{ x, y,-z},	//1
									{-x, y,-z},	//2
									{-x, y, z},	//3 b2
									{ x,-y, z},	//4 b5
									{ x,-y,-z},	//5
									{-x,-y,-z},	//6
									{-x,-y, z}};//7 b6
	// Load Image
	sf::Texture texLid;
	if (!texLid.loadFromFile("imgs/dice.png")){}
		glEnable(GL_TEXTURE_2D);//tell OpenGL to use textures when drawing
	sf::Texture::bind(&texLid);//select this texture for OpenGL rendering
	glBegin(GL_QUADS);
		//front
		NormalVector(points[1],points[5],points[6],normal);
		glNormal3fv(normal);
		glTexCoord2f(0.25, 0.33);	glVertex3fv(points[2]);
		glTexCoord2f(0.5, 0.33);	glVertex3fv(points[1]); 
		glTexCoord2f(0.5, 0.66);	glVertex3fv(points[5]); 
		glTexCoord2f(0.25, 0.66);	glVertex3fv(points[6]); 

		//back
		NormalVector(points[0],points[3],points[7],normal);
		glNormal3fv(normal);
		glTexCoord2f(1, 0.33);		glVertex3fv(points[3]);
		glTexCoord2f(0.75, 0.33);	glVertex3fv(points[0]);
		glTexCoord2f(0.75, 0.66);	glVertex3fv(points[4]);
		glTexCoord2f(1, 0.66);		glVertex3fv(points[7]);

		//top
		NormalVector(points[0],points[1],points[2],normal);
		glNormal3fv(normal);
		glTexCoord2f(0.25, 0);		glVertex3fv(points[2]);
		glTexCoord2f(0.25, 0.33);	glVertex3fv(points[3]);
		glTexCoord2f(0.5, 0.33);	glVertex3fv(points[0]);
		glTexCoord2f(0.5, 0);		glVertex3fv(points[1]);

		//bottom
		NormalVector(points[4],points[7],points[6],normal);
		glNormal3fv(normal);
		glTexCoord2f(0.25, 1);		glVertex3fv(points[7]); 
		glTexCoord2f(0.5, 1);		glVertex3fv(points[4]); 
		glTexCoord2f(0.5, 0.66);	glVertex3fv(points[5]); 
		glTexCoord2f(0.25, 0.66);	glVertex3fv(points[6]);
 
		//left
		NormalVector(points[3],points[2],points[6],normal);
		glNormal3fv(normal);
		glTexCoord2f(0.5, 0.33);	glVertex3fv(points[3]); 
		glTexCoord2f(0.75, 0.33);	glVertex3fv(points[2]); 
		glTexCoord2f(0.75, 0.66);	glVertex3fv(points[6]); 
		glTexCoord2f(0.5, 0.66);	glVertex3fv(points[7]);

		//right
		NormalVector(points[1],points[0],points[4],normal);
		glNormal3fv(normal);
		glTexCoord2f(0, 0.66);		glVertex3fv(points[1]);
		glTexCoord2f(0.25, 0.66);	glVertex3fv(points[0]);
		glTexCoord2f(0.25, 0.33);	glVertex3fv(points[4]);
		glTexCoord2f(0, 0.33);		glVertex3fv(points[5]);
	glEnd();
}

void liquid(){
	GLfloat normal[3];
	const double r=.55, PI=3.141;
	const float xAxes=3.5f;
	const int NUM_OF_POINTS=3;
	const int sides=400;
	
	float dt = 0.1, dphi = 0.07,
		  TWO_PI = 3.141 * 2,
		  val0= 0,		val1=-1.49f,	val2= 2.1f,
		  val3= 0.99f,	val4= 3.5f,		val5= 0.55f;

	float allValues[][3]={	{ val1, val0, val0 },	//0
							{ val1, val3, val0 },	//1
							{ val2, val3, val0 },	//2
							{ val4, val5, val0 },	//3
							};
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0,3.5,0); //centre
		for(int i = 0; i<=sides+1; i++){
			float x = r*cos(i*(2*PI/sides));
			float y = r*sin(i*(2*PI/sides));
			glVertex3f(x,3.5,y); //point on edge
		}
	glEnd();
	glCullFace(GL_BACK);
	
	glBegin(GL_QUADS);
		for (float t = 0; t <= 1; t += dt){
			for (float phi = 0; phi<TWO_PI; phi += dphi){
				for (int i =0; i<NUM_OF_POINTS; i++){
					GLfloat cylPoints[][3]={{	CalcCylinderZ(t, phi, allValues[i], allValues[i+1]),
												CalcCylinderX(t, phi, allValues[i], allValues[i+1]),
												CalcCylinderY(t, phi, allValues[i], allValues[i+1])},				//0
											{
												CalcCylinderZ(t, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderX(t, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderY(t, phi + dphi, allValues[i], allValues[i+1])},		//1
											{	
												CalcCylinderZ(t + dt, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderX(t + dt, phi + dphi, allValues[i], allValues[i+1]),
												CalcCylinderY(t + dt, phi + dphi, allValues[i], allValues[i+1])},	//2
											{
												CalcCylinderZ(t + dt, phi, allValues[i], allValues[i+1]),
												CalcCylinderX(t + dt, phi, allValues[i], allValues[i+1]),
												CalcCylinderY(t + dt, phi, allValues[i], allValues[i+1])}};			//3
					NormalVector(cylPoints[2],cylPoints[3],cylPoints[0],normal);
					glNormal3fv(normal);
					glVertex3fv(cylPoints[0]);
					glVertex3fv(cylPoints[1]);
					glVertex3fv(cylPoints[2]);
					glVertex3fv(cylPoints[3]);
				}
			}
		}
	glEnd();
	glDisable(GL_CULL_FACE);
}

int main() 
{	float	rotationSpeed=5,	moveSpeed=.5f,
			xTorch=  1.0f,		yTorch=  12.0f,		zTorch=  -1.0f,
			xLight=xTorch+1,	yLight=yTorch,		zLight=zTorch-1,
			rxTorch=0.0f,		ryTorch=0.0f,		rzTorch=0.0f,
			rxLight=0.0f,		ryLight=0.0f,		rzLight=0.0f;
	
	sf::Clock clock;
    int width=600,height=600;
	sf::RenderWindow App(sf::VideoMode(width, height, 32), "SFML OpenGL"); // Create the main window 
	aiVector3D position(0,6,13); // Create a clock for measuring time elapsed
	Camera camera;
    camera.Init(position); //create a camera
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
    // Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    //set up a 3D Perspective View volume
    gluPerspective(90.f, (float)width/height, 1.f, 50.0f);//fov, aspect, zNear, zFar 
	//enable lighting
	glEnable(GL_LIGHTING); // switch on lighting
	glEnable(GL_LIGHT0); // switch on light0
    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
			// Enable Light
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Z)){
				glEnable(GL_LIGHT0); // switch off light
			}
			// Disable Light
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::X)){
				glDisable(GL_LIGHT0); // switch off light
			}
			// Move Right
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::L)){
				xTorch+=moveSpeed;
				xLight+=moveSpeed;
			}
			// Move Left
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::J)){
				xTorch-=moveSpeed;
				xLight-=moveSpeed;
			}
			// Move UP
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::U)){
				yTorch+=moveSpeed;
				yLight+=moveSpeed;
			}
			// Move DOWN
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::O)){
				yTorch-=moveSpeed;
				yLight-=moveSpeed;
			}
			// Move Forward
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::I)){
				zTorch-=moveSpeed;
				zLight-=moveSpeed;
			}
			// Move Backward
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::K)){
				zTorch+=moveSpeed;
				zLight+=moveSpeed;
			}
			// Rotate Left
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F)){
				rzTorch-=rotationSpeed;
				rzLight-=rotationSpeed;
			}
			// Rotate Right
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::H)){
				rzTorch+=rotationSpeed;
				rzLight+=rotationSpeed;
			}
			// Rotate Forward
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::G)){
				rxTorch-=rotationSpeed;
				rxLight-=rotationSpeed;
			}
			// Rotate Backward
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::T)){
				rxTorch+=rotationSpeed;
				rxLight+=rotationSpeed;
			}
			// Rotate on y axis
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Y)){
				ryTorch-=rotationSpeed;
				ryLight-=rotationSpeed;
			}
			// Rotate on y axis 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::R)){
				ryTorch+=rotationSpeed;
				ryLight+=rotationSpeed;
			}
			//update the camera
			camera.Update(Event);
        } 
        //Prepare for drawing 
        glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
		// Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear buffers
		glMatrixMode(GL_MODELVIEW); // reset modelview matrix
		//prepare OpenGL surface for HSR 
		glEnable(GL_DEPTH_TEST); 
		glDepthMask(GL_TRUE); 
		glLoadIdentity(); 
		glEnable(GL_NORMALIZE); // automatically convert normals to unit normals
		glEnable(GL_LIGHTING);
		//get the viewing transform from the camera
		camera.ViewingTransform();		
		static float ang=0.0;
		double angle=clock.getElapsedTime().asMilliseconds()/15;
		//glRotated(angle, 1, 1, 0); //make the world spin //uncomment to make it spinning!
		GLfloat normal[3];
		angle=clock.getElapsedTime().asMilliseconds()/15;
		
		//light
		glPushMatrix();
		glTranslated(xLight-.5f,yLight+2.f,zLight+1.f);
		glRotated(rzLight,0,0,1);
		glRotated(rxLight,1,0,0);
		glRotated(ryLight,0,1,0);
		lighting();//Spotlight
		glPopMatrix();

		//torch
		glPushMatrix();
		glTranslated(xTorch,yTorch,zTorch);
		glRotated(rzTorch,0,0,1); //make cube spinning
		glRotated(rxTorch,1,0,0);
		glRotated(ryTorch,0,1,0);		
		torch(); //Torch
		glPopMatrix();

		floor(); //Floor

		//Draw dice
		glPushMatrix();
		glScaled(3.1,3.1,3.6); // make dice larger
		glTranslated(1.5,1.5,0);
		glRotated(angle,1,1,1); //make cube spinning		
		dice();
		glPopMatrix();
		//End dice

		//Transparency
		GLfloat rgbvals[] = {1.0f,0.3f,0.1f, 0.92}; // (R,G,B, Transparency)
		GLfloat rgbvals2[] = {1, 1, 1, 0.5}; //bottle color

		glDepthMask(GL_FALSE);
		//Enable everything for transparency
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_ALWAYS,GL_INVALID_ENUM);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTranslated(0,1.51,0);
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rgbvals);
		
		liquid(); //draw liquid
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rgbvals2);
		
		bottle(); //draw bottle

		glPopMatrix();

		//Disable everything for transparency
		glDepthMask(GL_TRUE);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		// Disable everything else
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
    return EXIT_SUCCESS; 
}
