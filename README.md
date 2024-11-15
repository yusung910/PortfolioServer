# PortfolioServer

 > 몇 번째인지 모를 포트폴리오 서버(진짜 마지막 일지도?)

## 1. 코딩 규칙
 > 기본은 카멜 표기법을 따름
 
### 1.1 클래스 네이밍 룰
    1.Pacade 클래스 이름은 접미어로 PO(Pacade Object)를 붙인다
    2.해당 클래스의 header 파일 확장자는 .hxx, C++ 파일 확장자는 동일하게 .cpp
### 1.2 Parameter 타입의 변수는 '_'로 시작
```c
function test(int _a)
{
    ...
};
```

### 1.3 클래스 내부 멤버 변수 접두어 규칙
    1. 멤버 변수를 의미하는 접두어(m_)로 시작
    2. 대문자로 시작하는 명사로 작성하며 복합 선언일 경우 앞 글자만 따서 사용한다
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
		<td>mutex</td>
		<td align="center">x</td>
		<td></td>
	</tr>
	<tr>
		<td>HANDLE</td>
		<td align="center">h</td>
		<td></td>
	</tr>
	<tr>
		<td>unordered_map</td>
		<td align="center">um</td>
		<td></td>
	</tr>
	<tr>
		<td>map</td>
		<td align="center">mp</td>
		<td></td>
	</tr>
	<tr>
		<td>other (class, object...)</td>
		<td align="center">o(other)</td>
		<td></td>
	</tr>
</table>

    
```c++    
class test
{
private:
    int m_nTest = 0;
    double long m_dlTest = 0l;
    unsinged int m_uiTest = 0;
}
```
    

### 1.4 함수 내부의 지역 변수는 local로 시작 후 대문자로 시작하는 명사로 작성
```c
int Test(int _t)
{
    int localNumber = 0;
}
```

### 1.5 const, define은 대문자로 작성
```c
#define DEV_TEST
const int32_t G_N_CHAR_SIZE = 4;
```
+ 예외 클래스 내부의 member 변수의 const는 기존 클래스 멤버 변수 작성 규칙을 그대로 준수하되 대문자로 작성한다
```c++
class Test
{
private:
    const int m_nTEST_NUMBER_ = 0;
}
```

### 1.6 프로젝트 구조
    1. SCCommon -> 서버, 클라이언트가 같이 사용되는 프로젝트 디렉토리
    2. Servers -> 서버 구동에 필요한 프로젝트 디렉토리
    ... 작업 중

## 2.1 
    1. 서버 <-> 클라이언트 통신 할 때마다 발생하는 HostID 서버에 Connect 요청 할 때마다 변한다.
    2. gtest를 이용한 단위 테스트 프로젝트 생성(99_UnitTest\)
    3. 실제 실행하게 될 게임서버 생성(PGServer)