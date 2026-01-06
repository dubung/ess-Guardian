# E.S.S.E.N.T.I.A.L  
**ESS Safety System with Environmental & Thermal Intelligent ALert**

> ESS 시설의 화재/가스/환경 이상을 감지하고,  
> Hybrid Patrol Robot(ROS2) + MQTT + Control Tower(서버/DB/UI)로 **즉시 알림/이력/관제**까지 이어지는 통합 안전 시스템

![title](docs/assets/slides/title.png)

---

## Demo
- Patrol Robot + ArUco Homing  
  ![demo-robot](docs/assets/demo/demo-robot.gif)

- Integrated Control Tower (Qt)  
  ![demo-ui](docs/assets/demo/demo-ui.gif)

> (원본 시연 영상은 링크로 첨부)  
> - Demo video: (YouTube/Drive/GitHub Release 링크)

---

## Background
- ESS 시설의 주요 리스크(화재/가스/환경 변화)에 대해 **“감지 → 대응 → 확인/관제”**까지 연결된 시스템 필요
- 이 프로젝트는 Zone별 센서 데이터 수집과 로봇 순찰을 결합해, 이상 상황을 서버로 전송하고 DB 이력화 및 UI로 관제하는 것을 목표로 함

---

## Goals
- [ ] 각 Zone별 독립적 환경 데이터 수집
- [ ] MQTT 기반 초저지연 데이터 전송
- [ ] 위험 감지 시 이벤트/이력 저장 + 관제 UI 표시
- [ ] 로봇 순찰 + ArUco 기반 홈 복귀

---

## Hardware Architecture
![hw-arch](docs/assets/slides/hw-arch.png)

---

## Software Architecture
![sw-arch](docs/assets/slides/sw-arch.png)

---

## Data Flow / Sequence
![sequence](docs/assets/slides/sequence.png)

---

## Key Features
### 1) Access Authentication & Alarm
- RFID 기반 관리자 인증
- 가스 이상 감지 시 시청각 알림 + 서버 즉각 전송(MQTT)

### 2) Environmental Monitoring & HVAC Control
- 온/습도 수집 및 임계 조건 기반 공조 제어
- 데이터는 DB 적재 + UI 조회/그래프 제공

### 3) Thermal Safety (Infrared)
- 열화상 ROI 기반 이상 픽셀 감지
- 최고온도/좌표 기반 이벤트 생성 → MQTT 전송 → DB 기록

### 4) Hybrid Patrol Robot (ROS2)
- NAV2 기반 순찰 + 구역/층 전환 로직
- ArUco 마커 기반 홈 위치 정렬(복귀 보정)

---

## MQTT Protocol (Topics)
> 서버 구독(예): `ess/env`, `ess/alert`, `ess/access/request`

### Environment
Topic: `ess/env`
```json
{"t": 23.10, "h": 55.30, "fan": "ON", "reason": "TEMP"}
