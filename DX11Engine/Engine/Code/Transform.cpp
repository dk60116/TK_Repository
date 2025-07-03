#include "epch.h"
#include "Transform.h"

CTransform::CTransform()
	: m_bIsRootParent(true)
	, m_pParent(nullptr)
	, m_lChildList({})
	, m_vPosition({})
	, m_vScale(vector3::one())
	, m_vEulerAngles({})
	, m_vQuaternion(quaternion::identity())
    , m_vWorldQuaternion(quaternion::identity())
    , m_vMatWorld()
    , m_vMatLocal()
    , m_vMatLocalRotation()
	, m_sDirections({})
{
    m_strName = L"Transform";
}

CTransform::~CTransform()
{
}

CTransform* CTransform::Create()
{
	return new CTransform();
}

HRESULT CTransform::Initialize()
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    Bind_Matrix();
    Bind_Direction();

    return S_OK;
}

void CTransform::Update()
{
    Bind_Matrix();
    Bind_Direction();
}

void CTransform::Render_Editor()
{
    if (CEditor::GetInstance().Get_SelectedGameObject() != m_pGameObject)
        return;

    CCamera* editorCam = CSceneManager::GetInstance().Get_CrtScene()->Get_EditorCamera();

    _matrix viewMatrix = editorCam->Get_ViewMatrix();
    _matrix projMatrix = editorCam->Get_ProjectionMatrix();

    _matrix worldMatrix = XMLoadFloat4x4(&m_vMatWorld);

    _float world[16];
    memcpy(world, &worldMatrix, sizeof(float) * 16);

    _float view[16];
    memcpy(view, &viewMatrix, sizeof(float) * 16);

    _float projection[16];
    memcpy(projection, &projMatrix, sizeof(float) * 16);

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
    ImGuizmo::AllowAxisFlip(false);

    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    static ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::TRANSLATE;

    if (ImGui::IsKeyPressed(ImGuiKey_T))
        currentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        currentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_S))
        currentGizmoOperation = ImGuizmo::SCALE;

    bool manipulated = ImGuizmo::Manipulate
    (
        view,
        projection,
        currentGizmoOperation,
        ImGuizmo::LOCAL,
        world
    );

    if (manipulated)
    {
        _matrix newWorldMatrix = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(world));

        if (m_pParent)
        {
            // 부모의 월드 행렬의 역행렬
            _matrix parentInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_pParent->m_vMatWorld));
            // 로컬 행렬 구하기
            _matrix localMatrix = newWorldMatrix * parentInv;

            // 로컬 위치/회전/스케일 추출
            XMVECTOR S, R, T;
            XMMatrixDecompose(&S, &R, &T, localMatrix);

            // 저장
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vScale), S);
            XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m_vQuaternion), R);
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vPosition), T);
        }
        else
        {
            // 부모 없으면 그냥 월드 == 로컬
            XMVECTOR S, R, T;
            XMMatrixDecompose(&S, &R, &T, newWorldMatrix);

            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vScale), S);
            XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m_vQuaternion), R);
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vPosition), T);
        }

        // 이걸 해야 Bind_Matrix() 때 최신 로컬로 계산됩니다.
        Bind_Matrix();
    }
}

void CTransform::OnDestroy()
{
    Safe_Release(m_pParent);
}

CTransform* CTransform::Get_Parent() const
{
    return m_pParent;
}

void CTransform::Set_Parent(CTransform* _parent)
{
    if (_parent == m_pParent)
        return;

    vector3 tempPos = m_vWorldPosition;
    Get_EulerAngles();
    quaternion tempQ = m_vWorldQuaternion;

    if (m_pParent)
    {
        m_pParent->m_lChildList.remove(this);
        Safe_Release(m_pParent);

        if (_parent == nullptr)
        {
            m_pParent = nullptr;
            m_vPosition = tempPos;
            m_vQuaternion = tempQ;
        }
    }

    m_pParent = _parent;

    if (m_pParent)
    {
        m_pParent->m_lChildList.push_back(this);
        m_pParent->AddRef();
    }
    
    m_bIsRootParent = !m_pParent;
}

const _bool CTransform::Is_Root() const
{
    return m_bIsRootParent;
}

CTransform* CTransform::Get_Child(const _int _index)
{
    UINT i = 0;

    for (TRAVERSAL_ITER(m_lChildList, it))
    {
        if (i == _index)
            return (*it);
        
        ++i;
    }

    return nullptr;
}

CTransform* CTransform::Find_Child(wstring _name)
{
    for (TRAVERSAL_ITER(m_lChildList, it))
    {
        if ((*it)->m_pGameObject->Get_ObjectName() == _name)
            return *it;
    }

    return nullptr;
}

CTransform* CTransform::Find_ChildRecursive(wstring _name)
{
    if (Get_GameObject()->Get_ObjectName() == _name)
        return this;

    for (TRAVERSAL_ITER(m_lChildList, it))
    {
        CTransform* found = (*it)->Find_ChildRecursive(_name);
        
        if (found)
            return found;
    }

    return nullptr;
}

const list<CTransform*>& CTransform::Get_ChldList() const
{
    return m_lChildList;
}

const CTransform::DIRECTIONS& CTransform::Get_Directions()
{
    return m_sDirections;
}

const _matrix CTransform::Get_WorldMatrix() const
{
    _matrix mat = XMLoadFloat4x4(&m_vMatWorld);

    return mat;
}

const _matrix CTransform::Get_LocalMatrix() const
{
    _matrix mat = XMLoadFloat4x4(&m_vMatLocal);

    return mat;
}

const _matrix CTransform::Get_InverseWorldMatrix() const
{
    _matrix mat = XMLoadFloat4x4(&m_vMatWorld);

    return XMMatrixInverse(nullptr, mat);
}

vector3 CTransform::Get_Position() const
{
    return m_vWorldPosition;
}

vector3 CTransform::Get_LocalPosition() const
{
    return m_vPosition;
}

const vector3 CTransform::Get_EulerAngles()
{
    if (!m_pParent)
        return m_vEulerAngles;
    else
    {
        _matrix worldMatrix = XMLoadFloat4x4(&m_vMatWorld);

        _vector scale;
        _vector rotationQuat;
        _vector translation;

        XMMatrixDecompose(&scale, &rotationQuat, &translation, worldMatrix);

        m_vWorldQuaternion = quaternion(rotationQuat);

        return m_vWorldQuaternion.to_euler();
    }
    
    return vector3::zero();
}

const vector3& CTransform::Get_LocalEulerAngles()
{
    return m_vEulerAngles;
}

const vector3& CTransform::Get_LocalScale()
{
    return m_vScale;
}

void CTransform::Set_Position(const vector3& _pos)
{
    CTransform* tempParent = nullptr;

    if (m_pParent)
    {
        tempParent = m_pParent;
        Set_Parent(static_cast<CTransform*>(nullptr));
        Bind_Matrix();
    }

    m_vPosition = _pos;

    if (tempParent)
        Set_Parent(tempParent);
}

void CTransform::Set_Position(const _float _x, const _float _y, const _float _z)
{
    Set_Position(vector3(_x, _y, _z));
}

void CTransform::Set_PositionX(const _float _value)
{
    Set_Position(vector3(_value, m_vPosition.y, m_vPosition.z));
}

void CTransform::Set_PositionY(const _float _value)
{
    Set_Position(vector3(m_vPosition.x, _value, m_vPosition.z));
}

void CTransform::Set_PositionZ(const _float _value)
{
    Set_Position(vector3(m_vPosition.x, m_vPosition.y, _value));
}

void CTransform::Set_LocalPosition(const vector3& _pos)
{
    m_vPosition = _pos;
}

void CTransform::Set_LocalPosition(const _float _x, const _float _y, const _float _z)
{
    Set_LocalPosition(vector3(_x, _y, _z));
}

void CTransform::Set_LocalPositionX(const _float _value)
{
    m_vPosition.x = _value;
}

void CTransform::Set_LocalPositionY(const _float _value)
{
    m_vPosition.y = _value;
}

void CTransform::Set_LocalPositionZ(const _float _value)
{
    m_vPosition.z = _value;
}

void CTransform::Add_Position(const vector3& _value)
{
    m_vPosition += _value;
}

void CTransform::Add_Position(const _float _x, const _float _y, const _float _z)
{
    m_vPosition += vector3(_x, _y, _z);
}

void CTransform::Add_PositionX(const _float _value)
{
    m_vPosition.x += _value;
}

void CTransform::Add_PositionY(const _float _value)
{
    m_vPosition.y += _value;
}

void CTransform::Add_PositionZ(const _float _value)
{
    m_vPosition.z += _value;
}

void CTransform::Set_Quaternion(const quaternion& _value)
{
    m_vQuaternion = _value;
}

void CTransform::Set_LocalQuaternion(const quaternion& _value)
{
    m_vQuaternion = _value;
}

void CTransform::Add_Quaternion(const quaternion& _delta)
{
    _vector q = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vQuaternion));
    _vector dq = XMLoadFloat4(reinterpret_cast<const _float4*>(&_delta));

    _vector result = XMQuaternionMultiply(dq, q);
    XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), XMQuaternionNormalize(result));
}

void CTransform::Set_EulerAngles(const vector3& _rot)
{
    CTransform* tempParent = nullptr;

    if (m_pParent)
    {
        tempParent = m_pParent;
        Set_Parent(static_cast<CTransform*>(nullptr));
        Bind_Matrix();
    }

    Set_LocalEulerAngles(_rot);

    if (tempParent)
        Set_Parent(tempParent);
}

void CTransform::Set_EulerAngles(const _float _x, const _float _y, const _float _z)
{
    Set_EulerAngles(vector3(_x, _y, _z));
}

void CTransform::Set_EulerAnglesX(const _float _x)
{
    m_vEulerAngles.x = _x;

    Set_EulerAngles(m_vEulerAngles);
}

void CTransform::Set_EulerAnglesY(const _float _y)
{
    m_vEulerAngles.y = _y;

    Set_EulerAngles(m_vEulerAngles);
}

void CTransform::Set_EulerAnglesZ(const _float _z)
{
    m_vEulerAngles.z = _z;

    Set_EulerAngles(m_vEulerAngles);
}

void CTransform::Add_EulerAngles(const vector3& _rot)
{
    CTransform* tempParent = nullptr;

    if (m_pParent)
    {
        tempParent = m_pParent;
        Set_Parent(static_cast<CTransform*>(nullptr));
        Bind_Matrix();
    }

    Add_LocalEulerAngles(_rot);

    if (tempParent)
        Set_Parent(tempParent);
}

void CTransform::Add_EulerAngles(const _float _x, const _float _y, const _float _z)
{
    Add_EulerAngles(vector3(_x, _y, _z));
}

void CTransform::Add_EulerAnglesX(const _float _value)
{
    Add_EulerAngles(vector3::right() * _value);
}

void CTransform::Add_EulerAnglesY(const _float _value)
{
    Add_EulerAngles(vector3::up() * _value);
}

void CTransform::Add_EulerAnglesZ(const _float _value)
{
    Add_EulerAngles(vector3::forward() * _value);
}

void CTransform::Set_LocalEulerAngles(const vector3& _rot)
{
    quaternion deltaQ = vector3(_rot.x, _rot.y, _rot.z).to_quaternion();
    Set_Quaternion(deltaQ);

    m_vEulerAngles = _rot;
}

void CTransform::Set_LocalEulerAngles(const _float _x, const _float _y, const _float _z)
{
    Set_LocalEulerAngles(vector3(_x, _y, _z));
}

void CTransform::Set_LocalEulerAnglesX(const _float _x)
{
    m_vEulerAngles.x = _x;
    Set_LocalEulerAngles(m_vEulerAngles);
}

void CTransform::Set_LocalEulerAnglesY(const _float _y)
{
    m_vEulerAngles.y = _y;
    Set_LocalEulerAngles(m_vEulerAngles);
}

void CTransform::Set_LocalEulerAnglesZ(const _float _z)
{
    m_vEulerAngles.z = _z;
    Set_LocalEulerAngles(m_vEulerAngles);
}

void CTransform::Add_LocalEulerAngles(const vector3& _rot)
{
    m_vEulerAngles += _rot;
    Set_LocalEulerAngles(m_vEulerAngles);
}

void CTransform::Add_LocalEulerAngles(const _float _x, const _float _y, const _float _z)
{
    Set_LocalEulerAngles(vector3(_x, _y, _z));
}

void CTransform::Add_LocalEulerAnglesX(const _float _value)
{
    Add_LocalEulerAngles(vector3(_value, 0.f, 0.f));
}

void CTransform::Add_LocalEulerAnglesY(const _float _value)
{
    Add_LocalEulerAngles(vector3(0.f, _value, 0.f));
}

void CTransform::Add_LocalEulerAnglesZ(const _float _value)
{
    Add_LocalEulerAngles(vector3(0.f, 0.f, _value));
}

void CTransform::Bind_Matrix()
{
    _matrix matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
    _matrix matRotation = XMMatrixRotationQuaternion(m_vQuaternion);
    _matrix matTranslation = XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);

    _matrix matWorldF = matScale * matRotation * matTranslation;

    _matrix worldMat = {};

    if (m_pParent)
    {
        worldMat = matWorldF * XMLoadFloat4x4(&m_pParent->m_vMatWorld);
        XMStoreFloat4x4(&m_vMatWorld, worldMat);
    }
    else
    {
        worldMat = matWorldF;
        XMStoreFloat4x4(&m_vMatWorld, worldMat);
    }

    m_vWorldPosition = vector3(m_vMatWorld._41, m_vMatWorld._42, m_vMatWorld._43);

    XMVECTOR S, Q, T;
    XMMatrixDecompose(&S, &Q, &T, worldMat);

    quaternion worldQ;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&worldQ), Q);

    vector3 eulerRad = worldQ.to_euler();     
    vector3 eulerDeg = eulerRad * XMConvertToDegrees(1.0f);

    m_vWorldEulerAngles = eulerDeg;           
}

void CTransform::Bind_Direction()
{
    _matrix rotOnly = XMLoadFloat4x4(&m_vMatWorld);
    _vector forward = XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f), rotOnly);
    _vector right = XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 0.f), rotOnly);
    _vector up = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f), rotOnly);

    XMFLOAT3 f, r, u;

    XMStoreFloat3(&f, forward);
    XMStoreFloat3(&r, right);
    XMStoreFloat3(&u, up);

    m_sDirections.forward = vector3(f.x, f.y, f.z).normalized();
    m_sDirections.back = -m_sDirections.forward;
    m_sDirections.right = vector3(r.x, r.y, r.z).normalized();
    m_sDirections.left = -m_sDirections.right;
    m_sDirections.up = vector3(u.x, u.y, u.z).normalized();
    m_sDirections.down = -m_sDirections.up;
}

void CTransform::Set_LocalScale(const vector3& _scale)
{
    m_vScale = _scale;
}

void CTransform::Set_LocalScale(const _float _x, const _float _y, const _float _z)
{
    m_vScale = vector3(_x, _y, _z);
}

void CTransform::Set_LocalScaleX(const _float _value)
{
    m_vScale.x = _value;
}

void CTransform::Set_LocalScaleY(const _float _value)
{
    m_vScale.y = _value;
}

void CTransform::Set_LocalScaleZ(const _float _value)
{
    m_vScale.z = _value;
}

void CTransform::LookAt(const vector3& _target)
{
    vector3 upAxis = vector3(0, 1, 0);
    vector3 fwd = (_target - m_vWorldPosition).normalized();
    if (fabsf(fwd.dot(upAxis)) > 0.999f)    upAxis = vector3(0, 0, 1);

    vector3 right = fwd.cross(upAxis).normalized();   // LH
    vector3 up = right.cross(fwd);

    _matrix rot = {
        right.x,  right.y,  right.z, 0,
        up.x,     up.y,     up.z,    0,
        fwd.x,    fwd.y,    fwd.z,   0,
        0,        0,        0,       1
    };

    _vector q = XMQuaternionRotationMatrix(rot);
    XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), XMQuaternionNormalize(q));

    if (m_pParent)
    {
        _vector parentInv = XMQuaternionInverse(XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion)));
        _vector localQ = XMQuaternionMultiply(parentInv, q);
    }
}
