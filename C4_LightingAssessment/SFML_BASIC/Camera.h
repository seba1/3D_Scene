#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class Camera{
    static aiVector3D zero,yaxis,zaxis;
public:
    aiVector3D position;
    aiVector3D forward;
    aiVector3D up;
 
    float forwardSpeed;
    float roationSpeed;
     
    Camera():forwardSpeed(0.4f),roationSpeed(0.2f){}
 
    void Init(aiVector3D& p=zero, aiVector3D& f=zaxis, aiVector3D& u=yaxis){
        position=p;
        forward=f;
        up=u;
         
    }
 
	void Update(sf::Event e){//respond to keyboard events
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::D)){
                MoveLeftRight(1);
            }

			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::A)){
                MoveLeftRight(-1);
            }
 
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::W)){
                MoveForwardBack(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::S)){
                MoveForwardBack(-1);
            }

			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Q)){
                MoveUpDown(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::E)){
                MoveUpDown(-1);
            }
 
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Right)){
                TurnRightLeft(-1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Left)){
                TurnRightLeft(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Up)){
               TurnUpDown(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Down)){
                TurnUpDown(-1);
            }
	
	}
    void MoveLeftRight(int dir){ //Dir=+1=>Right, dir=-1=> Left
            aiVector3D right=forward^up;
			right.Normalize();
			position+=(right*(forwardSpeed*dir));
    }

	void MoveUpDown(int dir){ //Dir=+1=>Right, dir=-1=> Left
			position+=(up*(forwardSpeed*dir));
    }
 
    void MoveForwardBack(int dir){ //Dir=+1=>Forward, dir=-1=> Back
        position+=(forward*(forwardSpeed*dir));
    }
 
    void TurnRightLeft(int dir){ //Dir=+1=>Right, dir=-1=> Left
        aiMatrix3x3 rotate;
		aiMatrix3x3::Rotation(dir*roationSpeed,up,rotate);
		forward*=rotate;
    }
         
    void TurnUpDown(int dir){ //Dir=+1=>Up, dir=-1=> Down
        aiMatrix3x3 rotate;
		aiVector3D right=forward^up;
		right.Normalize();
		aiMatrix3x3::Rotation(dir*roationSpeed,right,rotate);
		forward*=rotate;
		up*=rotate;
    }
 
    void ViewingTransform(){
		aiVector3D center = position+forward;
        gluLookAt(	position.x,position.y,position.z,// camera position
					center.x,center.y,center.z, //fixed
					up.x,up.y,up.z); //camera up
    }
 
};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,-1.0f);