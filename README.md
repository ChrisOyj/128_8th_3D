# 128_8th_3D
개인 3D 포트폴리오 작업물 (나루토 시노비 스트라이커)


# 1. Json 저장 방식
* 파일이름 기준 분류 (확장자 앞까지)

0-99 Level (씬, 장면, 스테이지 개념)\
100-9999 GameObject \
10000-99999 Component 

**Level Json이 가진 정보**
- 게임오브젝트 의 JsonID
- float4 위치벡터
- float4 룩벡터

**GameObject Json이 가진 정보**
- 게임오브젝트 클래스 Enum값
- 컴포넌트의 JsonID
- 자식 오브젝트의 JsonID

**Component Json이 가진 정보**
- 공통 정보 : 컴포넌트 클래스 Enum값
- 그 외 컴포넌트 별 Json 구조체로 또 들고 있음.



