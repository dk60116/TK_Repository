#pragma once

#include "Component.h"
#include "Camera.h"

class CMainCamera final 
    : public CComponent
{
public:
	CMainCamera();
	~CMainCamera();

public:
	static CMainCamera* Create();

public:
	void Init() override;
	void Awake() override;
	void Start()  override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void Render() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

public:
	CCamera& getCamera() { return *m_pCamera; }

private:
	CCamera* m_pCamera;

	BEGIN_SERIALIZEFIELD
	SERIALIZEFIELD(m_pCamera)
	END_SERIALIZEFIELD
};

