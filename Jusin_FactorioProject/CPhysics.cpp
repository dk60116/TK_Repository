#include "CPhysics.h"
#include "cmath"

CPhysics::CPhysics()
    : m_lColliderList()
    , m_arrPhysicsLayer()
{
    fill(&m_arrPhysicsLayer[0][0], &m_arrPhysicsLayer[0][0] + Layer::END * Layer::END, false);

    for (auto & l : m_lColliderList)
        l.clear();
}

CPhysics::~CPhysics()
{
    Release();
}

void CPhysics::Init()
{
    SetPhysicsLayer(Layer::Player, Layer::Structure, true);
    SetPhysicsLayer(Layer::FieldItem, Layer::FieldItem, true);
}

void CPhysics::Update()
{
}

void CPhysics::FixedUpdate()
{
    for (size_t i = 0; i < Layer::END; ++i)
    {
        for (size_t j = i; j < Layer::END; ++j)
        {
            if (!m_arrPhysicsLayer[i][j]) continue;

            auto& listA = m_lColliderList[i];
            auto& listB = m_lColliderList[j];

            for (auto left = listA.begin(); left != listA.end(); ++left)
            {
                for (auto right = listB.begin(); right != listB.end(); ++right)
                {
                    if (!(*left)->getObj()->isActive() || !(*right)->getObj()->isEnable())
                        continue;

                    if (*left == *right)
                        continue;

                    if ((*left)->getShape() == CCollider::ColliderShape::RECT && (*right)->getShape() == CCollider::ColliderShape::RECT)
                    {
                        if (CheckCollision_RR(*(*left), *(*right)))
                        {
                            (*left)->AddEnterCollider(*right);
                            (*right)->AddEnterCollider(*left);

                            (*left)->SetColor(ColorValue::Red());
                            (*right)->SetColor(ColorValue::Red());
                        }
                        else
                        {
                            (*left)->RemoveEnterCollider(*right);
                            (*right)->RemoveEnterCollider(*left);

                            (*left)->SetColor(ColorValue::Green());
                            (*right)->SetColor(ColorValue::Green());
                        }
                    }
                }
            }
        }
    }
}

void CPhysics::LateUpdate()
{
}

void CPhysics::Release()
{
    for (auto& list : m_lColliderList)
        list.clear();
}

CCollider* CPhysics::AddCollider(CCollider* _collider, Layer _layer)
{
    m_lColliderList[_layer].push_back(_collider);

    return m_lColliderList[_layer].back();
}

void CPhysics::SetPhysicsLayer(const Layer _left, const Layer _right, bool _value)
{
    m_arrPhysicsLayer[_left][_right] = _value;
    m_arrPhysicsLayer[_right][_left] = _value;
}

void CPhysics::DeleteCollider(CCollider* _collider, Layer _layer)
{
    auto& colliderList = m_lColliderList[_layer];
    colliderList.remove(_collider);
}

bool CPhysics::CheckCollision_RR(CCollider& _left, CCollider& _right)
{
    float leftLeft = _left.getObj()->getTransform().getPosition().x + _left.getPivot().x - _left.getSize().x * 0.5f;
    float leftRight = _left.getObj()->getTransform().getPosition().x + _left.getPivot().x + _left.getSize().x * 0.5f;
    float leftTop = _left.getObj()->getTransform().getPosition().y + _left.getPivot().y + _left.getSize().y * 0.5f;
    float leftBottom = _left.getObj()->getTransform().getPosition().y + _left.getPivot().y - _left.getSize().y * 0.5f;

    float rightLeft = _right.getObj()->getTransform().getPosition().x + _right.getPivot().x - _right.getSize().x * 0.5f;
    float rightRight = _right.getObj()->getTransform().getPosition().x + _right.getPivot().x + _right.getSize().x * 0.5f;
    float rightTop = _right.getObj()->getTransform().getPosition().y + _right.getPivot().y + _right.getSize().y * 0.5f;
    float rightBottom = _right.getObj()->getTransform().getPosition().y + _right.getPivot().y - _right.getSize().y * 0.5f;

    // 사각형 충돌 판정
    if (leftLeft >= rightRight || leftRight <= rightLeft || leftBottom >= rightTop || leftTop <= rightBottom)
        return false; // 충돌 안 함

    // 겹치는 폭 계산
    float overlapX = min(leftRight - rightLeft, rightRight - leftLeft);
    float overlapY = min(leftTop - rightBottom, rightTop - leftBottom);

    // 실제 Transform 위치
    vector2 leftPos = _left.getObj()->getTransform().getPosition();
    vector2 rightPos = _right.getObj()->getTransform().getPosition();

    // 둘 다 Trigger가 아닐 때만 물리 보정 로직 수행
    if (!_left.m_bIsTrigger && !_right.m_bIsTrigger)
    {
        // 더 작은 겹침 방향으로 분리
        if (overlapX < overlapY)
        {
            // X축 기준으로 분리
            // 누가 왼쪽/오른쪽에 있는지에 따라 방향 설정
            float pushDir = (leftPos.x < rightPos.x) ? -1.f : 1.f;

            // 둘 다 Dynamic이라면 절반씩 밀어낸다
            if (!_left.m_bIsStatic && !_right.m_bIsStatic)
            {
                leftPos.x += (pushDir * overlapX * 0.5f);
                rightPos.x -= (pushDir * overlapX * 0.5f);
            }
            // Left만 Static이라면 Right만 전부 밀어냄
            else if (_left.m_bIsStatic && !_right.m_bIsStatic)
            {
                rightPos.x -= (pushDir * overlapX);
            }
            // Right만 Static이라면 Left만 전부 밀어냄
            else if (!_left.m_bIsStatic && _right.m_bIsStatic)
            {
                leftPos.x += (pushDir * overlapX);
            }
            // 둘 다 Static이면 움직이지 않음
        }
        else
        {
            // Y축 기준으로 분리
            float pushDir = (leftPos.y < rightPos.y) ? -1.f : 1.f;

            if (!_left.m_bIsStatic && !_right.m_bIsStatic)
            {
                leftPos.y += (pushDir * overlapY * 0.5f);
                rightPos.y -= (pushDir * overlapY * 0.5f);
            }
            else if (_left.m_bIsStatic && !_right.m_bIsStatic)
            {
                rightPos.y -= (pushDir * overlapY);
            }
            else if (!_left.m_bIsStatic && _right.m_bIsStatic)
            {
                leftPos.y += (pushDir * overlapY);
            }
        }

        // 계산된 위치를 Transform에 반영
        _left.getBody()->getTransform().SetPosition(leftPos);
        _right.getBody()->getTransform().SetPosition(rightPos);
    }

    return true; // 충돌 중
}