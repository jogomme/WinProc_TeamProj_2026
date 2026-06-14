#pragma once

// sound 로드
void Open_Sound();

// 틀어진 bgm 끄기
void Stop_BGM();

// sound 반환
void Quit_SoundAll();

// 반복되는 bgm 실행 함수
void Play_Sound_BGM(const wchar_t name[50]);

// 1회성 이벤트 소리 실행 함수
void Play_Sound(const wchar_t name[50]);

void Set_Volume(int);