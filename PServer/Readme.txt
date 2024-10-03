1. 코딩 규칙
 1) Parameter 타입의 변수는 '_'로 시작
	ex) function test(int _a);

 2) 클래스 내부 멤버 변수 접두어 규칙
   -> member 변수의미로 'm_'로 시작
   -> int(8, 16, 32, 64bit)	 : n
      long					 : l
	  double				 : d
	  float					 : f
	  char					 : c
	  string				 : s
	  double long			 : dl
	  unsinged				 : u
	-> 그 후 대문자로 시작하는 명사로 작성
    ex) class test
		{
			private:
				int m_nTest = 0;
				double long m_dlTest = 0l;
				unsinged int m_uiTest = 0;
		}

  3) 함수 내부의 지역 변수는 local로 시작 후 대문자로 시작하는 명사로 작성

	ex) int Test(int _t)
		{
			int localNumber = 0;
		}

  4) const, define은 대문자로 작성
	ex) #define DEV_TEST 