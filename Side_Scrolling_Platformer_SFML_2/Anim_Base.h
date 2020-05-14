#pragma once
#include<string>

class SpriteSheet;
using Frame = unsigned int;

class Anim_Base
{
	friend class SpriteSheet;
protected:
	Frame m_frameCurrent;
	Frame m_frameStart;
	Frame m_frameEnd;
	Frame m_frameRow;
	int m_frameActionStart; //Frame when a specific "action" begin
	int m_frameActionEnd; //Frame when a specific "action" end
	float m_frameTime;
	float m_elapsedTime;
	bool m_loop;
	bool m_play;
	SpriteSheet * m_spriteSheet;

public:
	std::string m_name;
protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream & l_stream) = 0;

public:
	Anim_Base();
	virtual ~Anim_Base();
	
	void Play();
	void Stop();
	void Reset();
	void Pause();

	virtual void Update(const float & l_dt);

	friend std::stringstream & operator>>(std::stringstream & l_stream, Anim_Base & a)
	{
		a.ReadIn(l_stream);
		return l_stream;
	}

	//Some getter and setter
	void SetSpriteSheet(SpriteSheet * l_sheet);
	void SetFrame(const unsigned int & l_frame);
	bool IsInAction();
	void SetLoop(const bool & l_loop);
};

