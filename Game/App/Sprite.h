#ifndef GE_SPRITE_H
#define GE_SPRITE_H

#include <GL/freeglut.h>
#include <map>
#include <vector>
#include <string>

class CSprite
{
public:
    // If width, height and UV coords are not provided then they will be derived from the texture size
    CSprite(const char* FileName, unsigned int InColumns = 1, unsigned int InRows = 1);
    void Update(float DeltaTime);
    void Draw();
    void SetPosition(float InX, float InY) { XPos = InX; YPos = InY; }   
    void SetAngle(float InAngle)  { Angle = InAngle; }
    void SetScale(float InScale) { Scale = InScale; }
    void SetFrame(unsigned int InFrame);
    void SetAnimation(int InId);
    void SetAnimation(int InId, bool bRestart);
	void GetPosition(float &XCoord, float &YCoord) { XCoord = XPos; YCoord = YPos; }
    float GetWidth()  const { return Width;  }
    float GetHeight() const { return Height; }
    float GetAngle()  const { return Angle;  }
    float GetScale()  const { return Scale;  }
    unsigned int GetFrame()  const { return Frame; }
	void SetColor(float InR, float InG, float InB) { Red = InR; Green = InG; Blue = InB; }

    // Note: speed must be > 0, frames must have size >= 1, id must be unique among animations
    void CreateAnimation(unsigned int InId, float InSpeed, const std::vector<int>& InFrames)
    {
        SAnimation Anim;        
        Anim.AnimId = InId;
        Anim.Speed = InSpeed;
        Anim.Frames = InFrames;
        Animations.push_back(Anim);        
    };

private:
    void CalculateUVs();
    GLuint Texture;
    float XPos = 0.0F;
    float YPos = 0.0F;
    float Width = 0.0F;
    float Height = 0.0F;
    int TexWidth = 0;
    int TexHeight = 0;
    float Angle = 0.0F;
    float Scale = 1.0F;
    float Points[8];    
    float UVCoords[8];
    unsigned int Frame;
    unsigned int Columns;
    unsigned int Rows;
	float Red = 1.0f;
	float Green = 1.0f;
	float Blue = 1.0f;
    int CurrentAnim = -1;
    float AnimTime = 0.0F;

    struct SAnimation
    {
        unsigned int AnimId = 0;
        float Speed = 0.0f;
        std::vector<int> Frames;
    };
    std::vector<SAnimation> Animations;

    // Texture management
    struct STextureDesc
    {
        unsigned int Width;
        unsigned int Height;
        GLuint TextureID;
    };
    bool LoadTexture(const std::string& Filename);
    static std::map<std::string, STextureDesc> Textures;
    
};

#endif