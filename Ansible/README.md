Ansible은 코드로 IT 인프라를 구축을 자동화하는 도구이다.
Ansible은 특정 에이전트 소프트웨어에 의존하지 않고 추가 보안 인프라가 없어 배포가 쉽다.
-	Ansible categories
Ansible
Control node	Managed node
-	Ansible 실행 컴퓨터
-	적어도 하나의 Control node 필요	-	Control node에 관리되고 있는 장치

Ansible은 클라이언트, 서버 등 네트워크 상의 노드들에 연결 후 Ansible module을 전송한다.
Ansible module은 Control node가 Managed node에 로그인 엑세스 권한을 갖게 되면 SSH를 통해 실행되고 이후 바로 제거된다.
여러 대의 컴퓨터를 자동화할 때 Ansible module에서 이를 수행하는 모듈을 찾은 후 해당 모듈을 구성하고 실행할 수 있도록 Ansible를 설치한다.
이러한 Ansible module를 사용하기 위해서는 playbook을 작성해야 한다.
Playbook은 Managed node설정에 대한 지침서이고 YAML파일로 저장된다.
![Uploading image.png…]()
