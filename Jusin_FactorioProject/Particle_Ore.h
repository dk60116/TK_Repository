#pragma once

#include "CMonoBehaviour.h"

class Particle_Ore :
    public CMonoBehaviour
{
public:
	Particle_Ore();
	~Particle_Ore();

public:
	void Awake() override;
	void Start() override;
	void Reset() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void OnEnable() override;
	void OnDisable() override;
	void Render(HDC _hDC) override;
	void OnDestroy() override;
	void OnMouseEnter() override;
	void OnMouseOver() override;
	void OnMouseExit() override;
};

