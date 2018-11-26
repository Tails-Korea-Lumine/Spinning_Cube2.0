#include "Teleportation_Manager.h"

void Teleportation_Manager::Update_Door_Position(const int& sidenum, const ML::Vec3& pos)
{
	//�h�A�̌��݈ʒu���X�V
	this->doorPos[sidenum] = pos;
}

bool Teleportation_Manager::Find_Exit(const int& sidenum, ML::Vec3* exitpos)
{
	//�s��̔ԍ���T��
	int exitnum = sidenum;
	if (sidenum < 3)
	{
		exitnum += 3;
	}
	else
	{
		exitnum -= 3;
	}
	//���Α����ċN���O�Ȃ�e���|�[�g���s��
	if (!this->be_Used[exitnum])
	{
		this->be_Used[exitnum] = true;
		this->be_Used[sidenum] = true;
		this->timeCnt[exitnum] = 0;
		this->timeCnt[sidenum] = 0;
		*exitpos = this->doorPos[exitnum];
		return true;
	}
	
	return false;
}


Teleportation_Manager::Teleportation_Manager()
{
	for (int i = 0; i < 6; i++)
	{
		this->be_Used[i] = false;
		this->doorPos[i] = ML::Vec3(0, 0, 0);
		this->timeCnt[i] = 0;
	}
}

void Teleportation_Manager::Increse_Counter()
{
	for (int i =0; i<6; i++)
	{
		this->timeCnt[i]++;
		//5�b��ċN��
		if (this->timeCnt[i] > 300)
		{
			this->be_Used[i] = false;
		}
	}


}