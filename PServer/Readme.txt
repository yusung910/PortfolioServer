1. �ڵ� ��Ģ
 -> �⺻�� ī�� ǥ����� ����
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
	  other(class, object...): o
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

  4) global const, define�� �빮�ڷ� �ۼ�
	ex) #define DEV_TEST , const int32_t G_N_CHAR_SIZE = 4;


  5) ������Ʈ ����
    SCCommon -> ����, Ŭ���̾�Ʈ�� ���� ���Ǵ� ������Ʈ ���丮
    Servers -> ���� ������ �ʿ��� ������Ʈ ���丮
