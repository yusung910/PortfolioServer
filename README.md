# PortfolioServer

 > 몇 번째인지 모를 포트폴리오 서버(진짜 마지막 일지도?)

## 코딩 규칙
 > 기본은 카멜 표기법을 따름
 
### 클래스 네이밍 룰
 > Pacade 클래스 이름은 접미어로 PO(Pacade Object)를 붙인다

 > 해당 클래스의 header 파일 확장자는 .hxx, C++ 파일 확장자는 동일하게 .cpp
### Parameter 타입의 변수는 '_'로 시작
 > function test(int _a);

### 클래스 내부 멤버 변수 접두어 규칙
 > 멤버 변수를 의미하는 접두어(m_)로 시작
<table>
	<thead>
		<td>의미</td>
		<td>접두어</td>
		<td>접미어</td>
	</thead>
	<tr>
		<td>멤버변수</td>
		<td align="center">m_</td>
		<td></td>
	</tr>
	<tr>
		<td>int(8, 16, 32, 64bit)</td>
		<td rowspan="2" align="center">n</td>
		<td></td>
	</tr>
	<tr>
		<td>short</td>
		<td></td>
	</tr>
	<tr>
		<td>long</td>
		<td align="center">l</td>
		<td></td>
	</tr>
	<tr>
		<td>double</td>
		<td align="center">d</td>
		<td></td>
	</tr>
	<tr>
		<td>float</td>
		<td align="center">f</td>
		<td></td>
	</tr>
	<tr>
		<td>char</td>
		<td align="center">c</td>
		<td></td>
	</tr>
	<tr>
		<td>string</td>
		<td align="center">s</td>
		<td></td>
	</tr>
	<tr>
		<td>unsigned</td>
		<td align="center">u</td>
		<td></td>
	</tr>
	<tr>
		<td>enum</td>
		<td align="center">e</td>
		<td></td>
	</tr>
	<tr>
		<td>pointer</td>
		<td align="center">p</td>
		<td></td>
	</tr>
	<tr>
		<td>other (class, object...)</td>
		<td align="center">o</td>
		<td></td>
	</tr>
</table>

    그 후 대문자로 시작하는 명사로 작성하며 복합 선언일 경우 앞 글자만 따서 사용한다
```c    
    class test
	{
        private:
            int m_nTest = 0;
            double long m_dlTest = 0l;
            unsinged int m_uiTest = 0;
	}
```
    

### 함수 내부의 지역 변수는 local로 시작 후 대문자로 시작하는 명사로 작성
    int Test(int _t)
    {
        int localNumber = 0;
    }

### global const, define은 대문자로 작성
    - #define DEV_TEST , const int32_t G_N_CHAR_SIZE = 4; 등


### 프로젝트 구조
    - SCCommon -> 서버, 클라이언트가 같이 사용되는 프로젝트 디렉토리
    - Servers -> 서버 구동에 필요한 프로젝트 디렉토리
