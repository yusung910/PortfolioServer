# PortfolioServer

 > 몇 번째인지 모를 포트폴리오 서버(진짜 마지막 일지도?)

## 1. 코딩 규칙
 > 기본은 카멜 표기법을 따름
 
### 1.1 클래스, DB 프로시저, InnerPacket 구조체 네이밍 룰
    1. Pacade 클래스 이름은 접미어로 PO(Pacade Object)를 붙인다
    2. 해당 클래스의 header 파일 확장자는 .hxx, C++ 파일 확장자는 동일하게 .cpp
    3. NES = NetworkEventSync, 네트워크 이벤트에 사용되는 클래스
       각각의 서버에서 상속받아 사용하는 부모 클래스
    4. DB 프로시저를 호출하고 결과값을 전달받기 위한 클래스의 접미사를 DTO로 한다.
        -> spLoginAccountProcessSelectDTO
    5. DB 프로시저는 접두어에 sp 붙인다.
        -> spAccountLoginProcessSelect
    6. 게임 플레이어가 플레이 하는 캐릭터의 명칭은 Pilgrim으로 한다
    7. InnerPacketStructures에 DB에서 사용하는 구조체와, InnerPacket에서 사용하는
       데이터 구조체가 있음.
        -> DB에서 사용하는 구조체의 경우 접두어 sp와 접미어 DTO를 사용한다
        -> 내부 서비스(Service) 클래스에서 사용하는 Packet구조체의 경우
           Protocol 선언 규칙을 따른다
           ex) LP : LoginService to Platform Service
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
    3. 프로시저에서 호출 하는 데이터를 저장하는 구조체, 클래스는 예외로 적용하지 않는다.
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
    unsinged int m_uiTest = 0;_
}
```
    

### 1.4 함수 내부의 지역 변수는 l(Local)로 시작 후 대문자로 시작하는 명사로 작성
```c
int Test(int _t)
{
    int lNumber = 0;
}
```
+ 함수 내부에 존재하는 람다 함수의 경우 _(underscore)로 시작
 ```c
int Test(int _t)
{
    auto _GetBPStr = [](int64_t _bytes) -> std::string
        {
            ...
        };
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

### 1.7 to do 리스트
    1. BotClient 작업
	   a. 서버 패킷 별 통신 작업
	2. DB는 OLE DB를 사용. --> 취소
	   a. DBWorker 작업중
       b. POCO 라이브러리의 Session 사용하는것으로 변경
        -> POCO 라이브러리에 있는 내용을 그대로 다시 코딩하는 내용이므로 불필요한 작업으로 확인
    3. LoginServer 작업 및 DB 통신 확인
    4. 소스 파일들의 문자열(Charset)이 EUC-KR 또는 UTF-8로 세팅할 것인지 결정 후 적용
    5. 생성된 sql 스크립트를 일괄 실행하는 배치 파일을 만들자
        -> 참조 사이트 : https://www.sqlservercentral.com/forums/topic/batch-file-to-run-sql-scripts
    6. gtest 프로젝트 추가 완료, 추후 몬스터, 미니언 등등
       클라이언트와 실시간 동기화 작업이 필요한 클래스를 테스트할 때 사용한다
    7. OpenSSL의 버전 변경(2.X.X -> 3.X.X)에 따른 RSAUtil 클래스 수정작업

## 2.1 작업중 이슈 내용
    2.1.1 서버 <-> 클라이언트 통신 할 때마다 발생하는 HostID 서버에 Connect 요청 할 때마다 변한다.
    2.1.2. gtest를 이용한 단위 테스트 프로젝트 생성(99_UnitTest\)
    2.1.3. 실제 실행하게 될 게임서버 생성(PGServer)
    2.1.4. 컴파일 중 C2855 관련 이슈 발생 시 해당 프로젝트에 cpp 파일 생성 후
       .h 파일로 명칭을 변경했을 경우 해당 이슈가 발생, 삭제 후 재생성하는 것으로 해결
    2.1.5. 작성한 dll(SCore), lib(SPublic) 프로젝트를 참조하는 PGameServer 프로젝트에서
           dll, lib에 선언 되어 있는 함수를 실행 할 때 
           함수 외부에서 전달하는 매개변수의 값이 함수 내부에서 NULL로 초기화 되는 현상 발생.
       -> 해결!! SCore 프로젝트의 빌드 결과(.dll, .lib) 파일들에 대한 경로가 맞지 않은 이유로 발생한것으로 추측.
    2.1.6. LZ4 for C#에서 작업하기 위한 내용
	   -> https://github.com/IonKiwi/lz4.net 라이브러리를 이용
	   -> SCCommon 폴더 내에 lz4, lz4.AnyCPU.loader 프로젝트 추가(https://github.com/IonKiwi/lz4.net 프로젝트)
	      ㅇ LZ4Loader.cs (Line: 385) 에러 발생 시 해결 방법
	          - lz4.AnyCPU.loader 프로젝트에 있는 x64/lz4.dll 파일의 속성에서
              '빌드 작업' 옵션을 '포함 리소스'로 변경해야함
	   -> 원인을 알 수 없는 에러 발생
	      ㅇ LZ4Loader.cs (Line: 388) 어셈블리 파일을 찾았으나 로드(load)가 안되는 이슈 발생
	   -> Library를 변경 (IonKiwi -> K4os.Compression.LZ4_1.3.8: https://github.com/MiloszKrajewski/K4os.Compression.LZ4)
	      ㅇ C++에서 사용하는 Lz4 무손실 압축 라이브러리를 개발한 곳에서
             C#으로 사용할 수 있도록 별도로 작업된 라이브러리로 파악
		  ㅇ 최상위 경로 /lib에 저장하고 클라, 서버에서 공용으로 사용할 수 있게 작업
	   -> 해결! K4os.Compression.LZ4의 net462 버전의 dll 파일을 참조 추가로 사용할 수 있음, BotClient에 작업 완료!
    2.1.7. C#.net에서 C++ Socket 통신 할 때 특수문자를 제거해야한다
       -> 제거되지 않을 경우 패킷 데이터 배열에 영향을 끼치게 되어 통신 데이터 상에 문제가 발생
	2.1.8. C# BotClient 패킷 작업 할 때 BitArray를 활용하자
	2.1.9. 정적 라이브러리(lib) 프로젝트의 클래스를 다른 프로젝트(exe)에서 참조하여 사용 할 때
       링크 에러(lnk2019)가 발생 할 경우 참조하는 클래스에 SCoreAPI define(__declspec(dllexport))가 선언되어 있는지 확인한다.
    2.1.10. 각각 소스의 Charset ANSI(EUC-KR) - UTF8 간에 발생한 C4819 에러 처리
       -> 윈도우 11의 '시스템 로캘 변경'에서 'Beta: 세계 언어 지원을 위해 Unicode UTF-8 사용'을 활성화 했을 때
          윈도우 내부 파일들의 문자열 셋(캐릭터 셋)이 강제로 변경되는 이슈에 따른 처리
       -> 해당 내용을 활성화 한 이유는 G-Hub에서 무한 로딩 이슈를 해결하기 위해 적용.
       -> 사이드 이펙트로 소스 파일들의 문자열 인코딩 관련 이슈가 발생
       -> https://eteo.tistory.com/730
       -> '프로젝트 > 속성 > 구성 속성 > C/C++ > 명령줄 > 추가 옵션' 영역에 /utf-8로 설정
    2.1.11. 빌드 후 응용프로그램 실행 실패 에러 (0xc00007b)가 발생 할 경우 프로젝트의 빌드 경로 설정을 확인한다.
    2.1.12. lz4를 이용한 압축 데이터 송, 수신할 때 payload에 packetid와 packet 크기를 반영할 때
            압축된 크기를 보내야 함. 압축 전 크기를 보내면서 데이터 송,수신간 에러 발생
            -> 이걸로 며칠이나 날려먹었냐.. 에라이 모지리 나님아
    2.1.13. UnrealEngine에서도 RecastNavigation 라이브러리에서 사용할 수 있는 bin 파일을 생성 할 수 있다.
        -> 서버에서 navmesh를 사용하여 각각의 캐릭터, 지형에 맞는 Navigation 기능을 사용하기 위함.
        -> UnrealEngine에서 RecastNavigation bin 파일 생성하기 위한 리포지터리 주소 : https://github.com/luyuancpp/ue5-export-nav-data?tab=readme-ov-file
        -> UE에서 위 플러그인을 이용하면 생성된 navmesh를 서버에서 사용 할 수 있는 .bin, .obj 파일로 변환 가능.