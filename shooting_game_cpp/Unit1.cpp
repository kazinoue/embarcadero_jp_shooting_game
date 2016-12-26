//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
//#include <cmath>
TfmShooting_main *fmShooting_main;
//---------------------------------------------------------------------------
__fastcall TfmShooting_main::TfmShooting_main(TComponent* Owner)
	: TForm(Owner)
{
    game_reset();
}

void __fastcall TfmShooting_main::game_reset()
{
	Button_up->Visible                 = false; //�{�^��Up��\��
	Button_down->Visible               = false; //�{�^��Down��\��
	Button_missile->Visible            = false; //�{�^���~�T�C����\��

	Timer_Enms->Enabled                = false; //�G�o���^�C�}�[�X�g�b�v
	Timer_Enms_laserbeam->Enabled      = false; //�G���[�U�[�r�[���^�C�}�[�X�g�b�v
	Timer_gameover->Enabled            = false; //�v���[���[�ƓG�ڐG����^�C�}�[�X�g�b�v
	Rectangle_Enm1->Visible            = false; //�G1��\��
	Rectangle_Enm2->Visible            = false; //�G2��\��
	Rectangle_Enm3->Visible            = false; //�G3��\��
	Rectangle_Enm_laserbeam->Visible   = false; //�G���[�U�[�r�[����\��
	Rectangle_startscene->Visible  	   = true;  //�X�^�[�g�V�[���\��
	FiTotal = 0;
	FdtPlay = 0;
	Rectangle_gameoverscene->Visible   = false; //�Q�[���I�[�o�[��ʔ�\��

}

//---------------------------------------------------------------------------
void __fastcall TfmShooting_main::FloatAnimation_background1Finish(TObject *Sender)

{
	//http://docwiki.embarcadero.com/Libraries/Berlin/ja/System.Math.RoundTo
	int x_{(int)System::Math::RoundTo(Rectangle_background1->Position->X, 0)};
	switch (x_){
	case 0:
		FloatAnimation_background1->StartValue = 0;
		FloatAnimation_background1->StopValue  = -this->Width;
		break;
	default:
		FloatAnimation_background1->StartValue = this->Width;
		FloatAnimation_background1->StopValue  = 0;
		;
	}
	FloatAnimation_background1->Start();
}
//---------------------------------------------------------------------------
void __fastcall TfmShooting_main::FloatAnimation_background2Finish(TObject *Sender)

{
	int x_{(int)System::Math::RoundTo(Rectangle_background2->Position->X, 0)};
	switch (x_){
	case 0:
		FloatAnimation_background2->StartValue = 0;
		FloatAnimation_background2->StopValue  = -this->Width;
		break;
	default:
		FloatAnimation_background2->StartValue = this->Width;
		FloatAnimation_background2->StopValue  = 0;
		;
	}
	FloatAnimation_background2->Start();
}
//---------------------------------------------------------------------------


void __fastcall TfmShooting_main::Button_startClick(TObject *Sender)
{
  Rectangle_player->Visible      	= true;
  Rectangle_startscene->Visible		= false;
  FloatAnimation_player_x->Start();
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::Button_downClick(TObject *Sender)
{
	FloatAnimation_player_y->StartValue	= Rectangle_player->Position->Y;
	if ((Rectangle_player->Position->Y +50) < (this->Height- Rectangle_player->Height -50)){
		FloatAnimation_player_y->StopValue = Rectangle_player->Position->Y + 50;
	}
	else{
		FloatAnimation_player_y->StopValue = this->Height- Rectangle_player->Height -50;
	}
  FloatAnimation_player_y->Start();

}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::Button_upClick(TObject *Sender)
{
	FloatAnimation_player_y->StartValue = Rectangle_player->Position->Y;
	if ( Rectangle_player->Position->Y - 50 > 0){
		FloatAnimation_player_y->StopValue = Rectangle_player->Position->Y - 50;
	}
	else
	{
		FloatAnimation_player_y->StopValue = 0;
	}
	FloatAnimation_player_y->Start();
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::Button_missileClick(TObject *Sender)
{
	TRectangle* em_buf_{nullptr};       		//�G��U�ۑ�
	KanokeBuf  = nullptr;               		//�G�ۑ��ϐ����N���A
	Button_missile->Enabled				= false;
	Rectangle_missile->Position->Y     	= Rectangle_player->Position->Y + 25;
	FloatAnimation_missile->StopValue   = missile_max;  //�~�T�C���ŏI�n�_�ݒ�
	FloatAnimation_missile->StartValue	= Rectangle_player->Position->X + 20;
	auto iExPos	= missile_max;
	auto iEnmX  = missile_max;
	for (int i = 0; i < 3; i++) {/*���[�v�œG3�����ԂɌ��j�\�����ׂ�*/
		switch(i){
		case 0: em_buf_ = Rectangle_Enm1; break;
		case 1: em_buf_ = Rectangle_Enm2; break;
		case 2: em_buf_ = Rectangle_Enm3;
		}
		auto iTemp = EnmExist(em_buf_);
		if ((iTemp < missile_max) && (iEnmX > em_buf_->Position->X))
		{   /*�G�̑O�ɓG������ꍇ, ��O�̓G��D��*/
			iExPos		= iTemp;
			KanokeBuf	= em_buf_;
		}
		if (KanokeBuf != nullptr) {//���j�o����G�ϐ��ɑ��݂��邩�m�F
			iEnmX		= KanokeBuf->Position->X;
		}
	}

	if( iExPos < (Rectangle_player->Position->X + Rectangle_player->Width))
		iExPos	= missile_max;  //�G���v���[���[�����ɂ���ꍇ�W�I����O��
	FloatAnimation_missile->StopValue	= iExPos;	//�~�T�C���̖ڕW�l���Z�b�g
	Rectangle_missile->Visible 			= true;     //�~�T�C����\��
	FloatAnimation_missile->Start();                //�~�T�C������
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_player_xFinish(TObject *Sender)
{
	Button_missile->Visible			= true;
	Button_up->Visible				= true;
	Button_down->Visible			= true;
	Label_score->Visible			= true;
	Timer_Enms->Enabled     		= true;
	Timer_Enms_laserbeam->Enabled 	= true;
	Timer_gameover->Enabled         = true;

	FdtPlay                 = Now();
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_missileFinish(TObject *Sender)
{
	Button_missile->Enabled   	= true;
	Rectangle_missile->Visible 	= false;
	if (KanokeBuf != nullptr) {
		FiTotal++;
		Bomb_Enm();
		Rectangle_enm_Bomb1->Position	= KanokeBuf->Position;
		KanokeBuf->Visible     			= false;
	}
    KanokeBuf = nullptr;
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_Enm1Finish(TObject *Sender)
{
	Rectangle_Enm1->Visible = false;	//��ʂ���t���[���A�E�g�� �G1���\��
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_Enm2Finish(TObject *Sender)
{
	Rectangle_Enm2->Visible = false;    //��ʂ���t���[���A�E�g�� �G2���\��
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_Enm3Finish(TObject *Sender)
{
	Rectangle_Enm3->Visible = false;    //��ʂ���t���[���A�E�g�� �G3���\��
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_player_yFinish(TObject *Sender)
{
    Button_missile->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TfmShooting_main::FormCreate(TObject *Sender)
{
	Rectangle_Enm1->Visible = false;
	Rectangle_Enm2->Visible = false;
	Rectangle_Enm3->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::enm_start(TRectangle* enm, TFloatAnimation* ani, TBitmapAnimation* bani)
{
	std::uniform_int_distribution<int> enm_position( 0, this->Height-100 ) ;
	unsigned int iEnm_y = enm_position(randomize_); //
	if (! enm->Visible) {
		enm->Position->Y    = iEnm_y;
		enm->Visible        = true;
		ani->StartValue     = this->Width + 10;
		ani->StopValue      = -enm->Width - 10;
		ani->Start();
		bani->Start();
	}
}

void __fastcall TfmShooting_main::Timer_EnmsTimer(TObject *Sender)
{
	std::uniform_int_distribution<int> enm_num( 0, 4 ) ;
	unsigned int iEnm = enm_num(randomize_);
	switch(iEnm){
	case 1: enm_start(Rectangle_Enm1, FloatAnimation_Enm1, BitmapAnimation_Enm1);
        break;
	case 2: enm_start(Rectangle_Enm2, FloatAnimation_Enm2, BitmapAnimation_Enm2);
		break;
	case 3: enm_start(Rectangle_Enm3, FloatAnimation_Enm3, BitmapAnimation_Enm3);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::EnmlaserbeamStat(TRectangle* enm)
{
	if (!(Rectangle_Enm_laserbeam->Visible) && enm->Visible) {
		Rectangle_Enm_laserbeam->Position->Y  		= enm->Position->Y + 25;
		Rectangle_Enm_laserbeam->Visible      		= true;
		FloatAnimation_Enm_laserbeam->StartValue	= enm->Position->X -20;
		FloatAnimation_Enm_laserbeam->StopValue		= FloatAnimation_Enm_laserbeam->StartValue - (this->Width + 50);
		FloatAnimation_Enm_laserbeam->Start();
	}
}

void __fastcall TfmShooting_main::Timer_Enms_laserbeamTimer(TObject *Sender)
{
	std::uniform_int_distribution<int> enm_num( 0, 3 ) ;
	unsigned int iEnm = enm_num(randomize_);
	switch(iEnm){
	case 1:EnmlaserbeamStat(Rectangle_Enm1);
		break;
	case 2:EnmlaserbeamStat(Rectangle_Enm2);
		break;
	case 3:EnmlaserbeamStat(Rectangle_Enm3);
	}

	Label_score->Text	= Format(L"Time %s / Total Score %0.9d", ARRAYOFCONST((
		FormatDateTime("hh:nn:ss", Now() - FdtPlay), FiTotal))
	);
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_Enm_laserbeamFinish(TObject *Sender)

{
	Rectangle_Enm_laserbeam->Visible	= false;
}
//---------------------------------------------------------------------------
float 	__fastcall TfmShooting_main::EnmExist(TRectangle* enm)
{
	float f_missile_max{static_cast<float>(missile_max)};
	float result_ = f_missile_max;

	if (enm->Visible && (enm->Position->X >
		(Rectangle_player->Position->X + Rectangle_player->Width)) )
		if ( ((enm->Position->Y-10) <= Rectangle_missile->Position->Y) &&
			((enm->Position->Y + (enm->Height*enm->Scale->X))+10 >= Rectangle_missile->Position->Y) )
			result_ = enm->Position->X;

	return result_;
}

void __fastcall TfmShooting_main::FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{
	if (Button_missile->Visible){
		switch (KeyChar){
		case ' ':
			(Rectangle_gameoverscene->Visible)?
				Rectangle_gameoversceneClick(nullptr): Button_missileClick(nullptr);
			break;
		case (char)0:
			switch (Key) {
			case 38: Button_upClick(nullptr);
				break;
			case 40: Button_downClick(nullptr);
			}
		}
	}
}
//---------------------------------------------------------------------------

bool __fastcall TfmShooting_main::hantei(TRectF& r1,TRectF& r2)
{
	bool b1{false};
	if ( (r1.Left < r2.Right) && (r1.Right > r2.Left) &&
		(r1.Top < r2.Bottom)  && (r1.Bottom > r2.Top) )
	{
		b1 = true;
	}
	return b1;
}

bool __fastcall TfmShooting_main::Rect_hantei(TRectangle* rect1, TRectangle* rect2)
{
	if (rect2->Visible)
	{
		TRectF r1{TRectF(rect1->Position->X, rect1->Position->Y,
		rect1->Position->X + rect1->Width, rect1->Position->Y + rect1->Height)};
		TRectF r2{TRectF(rect2->Position->X, rect2->Position->Y,
		rect2->Position->X + rect2->Width, rect2->Position->Y + rect2->Height)};
		return this->hantei(r1, r2);
	}
	else
	  return false;

}


void __fastcall TfmShooting_main::Timer_gameoverTimer(TObject *Sender)
{
	Timer_gameover->Enabled = false;
	bool atari_{false};
	for (int i =0; i < 4; i++)
		if (! atari_)
		{
			switch(i)
			{
			case 0:
				atari_ = Rect_hantei(Rectangle_player, Rectangle_Enm1);
				break;
			case 1:
				atari_ = Rect_hantei(Rectangle_player, Rectangle_Enm2);
				break;
			case 2:
				atari_ = Rect_hantei(Rectangle_player, Rectangle_Enm3);
				break;
			case 3:
				atari_ = Rect_hantei(Rectangle_player, Rectangle_Enm_laserbeam);
			}
		}

	if (atari_)
	{
		Bomb_Start();
		Rectangle_gameoverscene->Position->Y	= -480;			   		//�Q�[���I�[�o�[��ʂ̏���
		Rectangle_gameoverscene->Visible		= true;
		Label_gameover_score1->Text				= Label_score->Text;	//�X�R�A������R�s�[
		FloatAnimation_Gameover1->Start();                     			//�Q�[���I�[�o�[�A�j���X�^�[�g
//		Rectangle_player->Visible  = false;
//		ShowMessage("�Q�[���I�[�o�[");
//		game_reset(); //�Q�[�������Z�b�g����
	}
	else
		Timer_gameover->Enabled	= true;
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::BitmapAnimation_player_Bomb6Finish(TObject *Sender)

{
	Rectangle_player_Bomb1->Visible	= false;	//�퓬�@���j�A�j����\��
	Rectangle_player->Visible		= false;	//�퓬�@��\��
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::Bomb_Start()
{
	//�퓬�@�̔��j�A�j���[�V�������s
	Rectangle_player_Bomb1->Position->Y	= Rectangle_player->Position->Y - 30;
	Rectangle_player_Bomb1->Visible		= true;
	BitmapAnimation_player_Bomb1->Start();
	BitmapAnimation_player_Bomb2->Start();
	BitmapAnimation_player_Bomb3->Start();
	BitmapAnimation_player_Bomb4->Start();
	BitmapAnimation_player_Bomb5->Start();
	BitmapAnimation_player_Bomb6->Start();
}

void __fastcall TfmShooting_main::BitmapAnimation_enm_Bomb6Finish(TObject *Sender)

{
	Rectangle_enm_Bomb1->Visible	= false; //�G�̔��j�A�j����\��
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::Bomb_Enm()
{
	//�G�̔��j�A�j���[�V�������s
	Rectangle_enm_Bomb1->Visible	= true;
	BitmapAnimation_enm_Bomb1->Start();
	BitmapAnimation_enm_Bomb2->Start();
	BitmapAnimation_enm_Bomb3->Start();
	BitmapAnimation_enm_Bomb4->Start();
	BitmapAnimation_enm_Bomb5->Start();
	BitmapAnimation_enm_Bomb6->Start();
}

void __fastcall TfmShooting_main::Rectangle_gameoversceneClick(TObject *Sender)
{
    game_reset();   //�Q�[�������Z�b�g����
}
//---------------------------------------------------------------------------

void __fastcall TfmShooting_main::FloatAnimation_Gameover1Finish(TObject *Sender)

{
	Label_score->Visible	= false;	//�X�R�A���\��
}
//---------------------------------------------------------------------------


