1. 코딩 규칙
 -> 기본은 카멜 표기법을 따름
 
 1) 클래스 네이밍 룰
   -> Pacade 클래스 이름은 접미어로 PO(Pacade Object)를 붙인다

 2) Parameter 타입의 변수는 '_'로 시작
	ex) function test(int _a);

 3) 클래스 내부 멤버 변수 접두어 규칙
   -> member 변수의미로 'm_'로 시작
   -> int(8, 16, 32, 64bit)	 : n
      short                  : n
      long					 : l
	  double				 : d
	  float					 : f
	  char					 : c
	  string				 : s
	  double long			 : dl
	  unsinged				 : u
      enum                   : e
      pointer                : p
	  other(class, object...): o
	-> 그 후 대문자로 시작하는 명사로 작성
    ex) class test
		{
			private:
				int m_nTest = 0;
				double long m_dlTest = 0l;
				unsinged int m_uiTest = 0;
		}

  4) 함수 내부의 지역 변수는 local로 시작 후 대문자로 시작하는 명사로 작성

	ex) int Test(int _t)
		{
			int localNumber = 0;
		}

  5) global const, define은 대문자로 작성
	ex) #define DEV_TEST , const int32_t G_N_CHAR_SIZE = 4;


  6) 프로젝트 구조
    SCCommon -> 서버, 클라이언트가 같이 사용되는 프로젝트 디렉토리
    Servers -> 서버 구동에 필요한 프로젝트 디렉토리
