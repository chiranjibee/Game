#include <windows.h>

#include "App.h"
#include "AppSettings.h"
#include "Sprite.h"

#include <stb_image.h>

std::map<std::string, CSprite::STextureDesc > CSprite::Textures;

CSprite::CSprite(const char* Filename, unsigned int InColumns, unsigned int InRows)
	:   Columns(InColumns), Rows(InRows)
{
	if (LoadTexture(Filename))
	{
		CalculateUVs();
		Points[0] = -(Width / 2.0f);
		Points[1] = -(Height / 2.0f);
		Points[2] = Width / 2.0f;
		Points[3] = -(Height / 2.0f);
		Points[4] = Width / 2.0f;
		Points[5] = Height / 2.0f;
		Points[6] = -(Width / 2.0f);
		Points[7] = Height / 2.0f;
	}
}

void CSprite::Update(float DeltaTime)
{
    if (CurrentAnim >= 0)
    {
        AnimTime += DeltaTime/1000.0f;
        SAnimation& Anim = Animations[CurrentAnim];
        float Duration = Anim.Speed * Anim.Frames.size();

        // Looping around if reached the end of animation
        if (AnimTime > Duration)
        {
            AnimTime = fmodf(AnimTime, Duration);
        }
        int Frame = (int)( AnimTime / Anim.Speed );
        SetFrame(Anim.Frames[Frame]);        
    }
}

void CSprite::CalculateUVs()
{
    float U = 1.0f / Columns;
    float V = 1.0f / Rows;
    int R = Frame / Columns;
    int C = Frame % Columns;

    Width = TexWidth * U;
    Height = TexHeight * V;
    UVCoords[0] = U * C;
    UVCoords[1] = V * (float)(R+1);

    UVCoords[2] = U * (float)(C+1);
    UVCoords[3] = V * (float)(R + 1);

    UVCoords[4] = U * (float)(C + 1);
    UVCoords[5] = V * R;

    UVCoords[6] = U * C;
    UVCoords[7] = V * R;
}

void CSprite::Draw()
{            
#if APP_USE_VIRTUAL_RES
    float ScaleX = (Scale / APP_VIRTUAL_WIDTH) * 2.0f;
    float ScaleY = (Scale / APP_VIRTUAL_HEIGHT) * 2.0f;
#else
    float ScaleX = Scale;
    float ScaleY = Scale;
#endif
    float X = XPos;
    float Y = YPos;
#if APP_USE_VIRTUAL_RES
    APP_VIRTUAL_TO_NATIVE_COORDS(X, Y);
#endif
    
    glPushMatrix();
    glTranslatef(X, Y, 0.0f);   
    glScalef(ScaleX, ScaleY, 1.0f);    
    glRotatef(Angle * 180 / PI, 0.0f, 0.0f, 1.0f);     
	glColor3f(Red, Green, Blue);
    glEnable(GL_BLEND);    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glBegin(GL_QUADS);       
    for (unsigned int i = 0; i < 8; i += 2)
    {
        glTexCoord2f(UVCoords[i], UVCoords[i + 1]);
        glVertex2f(Points[i], Points[i+1]);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void CSprite::SetFrame(unsigned int InFrame)
{
    Frame = InFrame;
    if (Frame >= Rows*Columns)
    {
        Frame = 0;
    }
    CalculateUVs();
}

void CSprite::SetAnimation(int InId)
{
    SetAnimation(InId, false);
}

void CSprite::SetAnimation(int InId, bool bRestart)
{
    // When starting a new animation, we may want to start from the beginning, ie reset time
    if (bRestart)
    {
        AnimTime = 0.0f;
    }

    for (int i = 0; i < Animations.size(); i++)
    {
        if (Animations[i].AnimId == InId)
        {
            CurrentAnim = i;
            return;
        }
    }
    CurrentAnim = -1;
}

bool CSprite::LoadTexture(const std::string& Filename)
{
    // TODO: @CS, fix linker error
    /*if (Textures.find(Filename) != Textures.end())
    {        
        STextureDesc &texDef = Textures[Filename];
        Texture = texDef.TextureID;
        TexWidth = texDef.Width;
        TexHeight = texDef.Height;
		return true;
    }
 
    int Channels;
    unsigned char* ImageData = stbi_load(Filename.c_str(), &TexWidth, &TexHeight, &Channels, 4);

    GLuint Tex = 0;
	if (ImageData)
	{
		glGenTextures(1, &Tex);
		glBindTexture(GL_TEXTURE_2D, Tex);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, TexWidth, TexHeight, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);
        stbi_image_free(ImageData);
		STextureDesc textureDef = { (unsigned int) TexWidth, (unsigned int) TexHeight, Tex };
		Textures[Filename] = textureDef;
		Texture = Tex;
		return true;
	}*/
	return false;
}
