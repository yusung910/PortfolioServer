1. �ڵ� ��Ģ
 1) Parameter Ÿ���� ������ '_'�� ����
	ex) function test(int _a);

 2) Ŭ���� ���� ��� ���� ���ξ� ��Ģ
   -> member �����ǹ̷� 'm_'�� ����
   -> int(8, 16, 32, 64bit)	 : n
      long					 : l
	  double				 : d
	  float					 : f
	  char					 : c
	  string				 : s
	  double long			 : dl
	  unsinged				 : u
	-> �� �� �빮�ڷ� �����ϴ� ���� �ۼ�
    ex) class test
		{
			private:
				int m_nTest = 0;
				double long m_dlTest = 0l;
				unsinged int m_uiTest = 0;
		}

  3) �Լ� ������ ���� ������ local�� ���� �� �빮�ڷ� �����ϴ� ���� �ۼ�

	ex) int Test(int _t)
		{
			int localNumber = 0;
		}

  4) const, define�� �빮�ڷ� �ۼ�
	ex) #define DEV_TEST 