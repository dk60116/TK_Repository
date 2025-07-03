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
    , m_vLocalQuaternion(quaternion::identity())
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

    float world[16];
    memcpy(world, &worldMatrix, sizeof(float) * 16);

    float view[16];
    memcpy(view, &viewMatrix, sizeof(float) * 16);

    float projection[16];
    memcpy(projection, &projMatrix, sizeof(float) * 16);

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();

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
        memcpy(&m_vMatWorld, world, sizeof(float) * 16);

        // 여기서 Position만 추출
        XMFLOAT4X4 mat;
        memcpy(&mat, world, sizeof(float) * 16);

        m_vPosition = vector3(mat._41, mat._42, mat._43);
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

    if (m_pParent)
    {
        m_pParent->m_lChildList.remove(this);
        Safe_Release(m_pParent);
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

const _matrix& CTransform::Get_WorldMatrix() const
{
    _matrix mat = XMLoadFloat4x4(&m_vMatWorld);

    return mat;
}

const _matrix& CTransform::Get_LocalMatrix() const
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

const vector3& CTransform::Get_EulerAngles()
{
    m_vEulerAngles = m_vQuaternion.to_euler();

    return m_vEulerAngles;
}

const vector3& CTransform::Get_LocalEulerAngles()
{
    if (m_pParent)
    {
        _vector parentInvRot = XMQuaternionInverse(XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion)));
        _vector localQuat = XMQuaternionMultiply(parentInvRot, XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vQuaternion)));
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vLocalQuaternion), localQuat);
    }
    else
        m_vLocalQuaternion = m_vQuaternion;

    return {};
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

void CTransform::Set_PositionX(const _float _x)
{
    Set_Position(vector3(_x, m_vPosition.y, m_vPosition.z));
}

void CTransform::Set_PositionY(const _float _y)
{
    Set_Position(vector3(m_vPosition.x, _y, m_vPosition.z));
}

void CTransform::Set_PositionZ(const _float _z)
{
    Set_Position(vector3(m_vPosition.x, m_vPosition.y, _z));
}

void CTransform::Set_LocalPosition(const vector3& _pos)
{
    m_vPosition = _pos;
}

void CTransform::Set_LocalPosition(const _float _x, const _float _y, const _float _z)
{
    Set_LocalPosition(vector3(_x, _y, _z));
}

void CTransform::Set_LocalPositionX(const _float _x)
{
    m_vPosition.x = _x;
}

void CTransform::Set_LocalPositionY(const _float _y)
{
    m_vPosition.y = _y;
}

void CTransform::Set_LocalPositionZ(const _float _z)
{
    m_vPosition.z = _z;
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
    m_vLocalQuaternion = _value;

    if (m_pParent)
    {
        _vector parentQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion));
        _vector localQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vLocalQuaternion));
        _vector worldQuat = XMQuaternionMultiply(localQuat, parentQuat);
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), worldQuat);
    }
    else
        m_vQuaternion = m_vLocalQuaternion;
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
    vector3 rad = _rot * XMConvertToRadians(1.0f);

    // DirectX RollPitchYaw = (pitch, yaw, roll) = (X,Y,Z)요순
    _vector q = XMQuaternionRotationRollPitchYaw
    (
        rad.x,  // pitch
        rad.y,  // yaw
        rad.z
    ); // roll

    XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion),
        XMQuaternionNormalize(q));

    // 로컬 쿼터니언 동기화
    if (m_pParent)
    {
        _vector parentInv =
            XMQuaternionInverse(XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion)));
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vLocalQuaternion),
            XMQuaternionNormalize(XMQuaternionMultiply(parentInv, q)));
    }
    else
        m_vLocalQuaternion = m_vQuaternion;
}

void CTransform::Set_EulerAngles(const _float _x, const _float _y, const _float _z)
{
    Set_EulerAngles(vector3(_x, _y, _z));
}

void CTransform::Set_EulerAnglesX(const _float _x)
{
    vector3 euler = Get_EulerAngles();
    euler.x = _x;
    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Set_EulerAnglesY(const _float _y)
{
    vector3 euler = Get_EulerAngles();
    euler.y = _y;
    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Set_EulerAnglesZ(const _float _z)
{
    vector3 euler = m_vQuaternion.to_euler();
    euler.z = _z;
    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Add_EulerAngles(const vector3& _rot)
{
    quaternion deltaQ = vector3(XMConvertToRadians(_rot.x), XMConvertToRadians(_rot.y), XMConvertToRadians(_rot.z)).to_quaternion();
    Add_Quaternion(deltaQ);
}

void CTransform::Add_EulerAngles(const _float _x, const _float _y, const _float _z)
{
    Add_EulerAngles(vector3(_x, _y, _z));
}

void CTransform::Add_EulerAnglesX(const _float _value)
{
    float radians = XMConvertToRadians(_value);
    vector3 right = vector3::right();
    quaternion deltaQ = quaternion::from_axis_angle(right, radians);
    Add_Quaternion(deltaQ);
}

void CTransform::Add_EulerAnglesY(const _float _value)
{
    float radians = XMConvertToRadians(_value);
    vector3 up = vector3::up();
    quaternion deltaQ = quaternion::from_axis_angle(up, radians);
    Add_Quaternion(deltaQ);
}

void CTransform::Add_EulerAnglesZ(const _float _value)
{
    float radians = XMConvertToRadians(_value);
    vector3 forward = vector3::forward();
    quaternion deltaQ = quaternion::from_axis_angle(forward, radians);
    Add_Quaternion(deltaQ);
}

void CTransform::Set_LocalEulerAngles(const vector3& _rot)
{
    //m_vLocalEulerAngles = _rot;

    //m_vLocalQuaternion = _rot.to_quaternion();  // degree → quat

    //if (m_pParent)
    //{
    //    _vector parentQ = XMLoadFloat4(
    //        reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion));
    //    _vector localQ = XMLoadFloat4(
    //        reinterpret_cast<const _float4*>(&m_vLocalQuaternion));

    //    _vector worldQ = XMQuaternionMultiply(localQ, parentQ);
    //    XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion),
    //        XMQuaternionNormalize(worldQ));
    //}
    //else
    //{
    //    m_vQuaternion = m_vLocalQuaternion; 
    //}

    //m_vMatLocalRotation = XMMatrixRotationQuaternion(m_vLocalQuaternion);
}

void CTransform::Set_LocalEulerAngles(const _float _x, const _float _y, const _float _z)
{
    Set_LocalEulerAngles(vector3(_x, _y, _z));
}

void CTransform::Set_LocalEulerAnglesX(const _float _x)
{
}

void CTransform::Set_LocalEulerAnglesY(const _float _y)
{
}

void CTransform::Set_LocalEulerAnglesZ(const _float _z)
{
}

void CTransform::Add_LocalEulerAngles(const vector3& _rot)
{
    vector3 euler = m_vLocalQuaternion.to_euler();

    euler += _rot;

    m_vLocalQuaternion = euler.to_quaternion();

    if (m_pParent)
    {
        _vector parentQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion));
        _vector localQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vLocalQuaternion));
        _vector worldQuat = XMQuaternionMultiply(localQuat, parentQuat);
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), worldQuat);
    }
    else
        m_vQuaternion = m_vLocalQuaternion;
}

void CTransform::Add_LocalEulerAngles(const _float _x, const _float _y, const _float _z)
{
    vector3 euler = m_vLocalQuaternion.to_euler();

    euler += vector3(_x, _y, _z);
    ;
    m_vLocalQuaternion = euler.to_quaternion();

    if (m_pParent)
    {
        _vector parentQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion));
        _vector localQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vLocalQuaternion));
        _vector worldQuat = XMQuaternionMultiply(localQuat, parentQuat);
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), worldQuat);
    }
    else
        m_vQuaternion = m_vLocalQuaternion;
}

void CTransform::Add_LocalEulerAnglesX(const _float _value)
{
    vector3 euler = m_vLocalQuaternion.to_euler();

    euler.x += _value;
    ;
    m_vLocalQuaternion = euler.to_quaternion();

    if (m_pParent)
    {
        _vector parentQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion));
        _vector localQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vLocalQuaternion));
        _vector worldQuat = XMQuaternionMultiply(localQuat, parentQuat);
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), worldQuat);
    }
    else
        m_vQuaternion = m_vLocalQuaternion;
}

void CTransform::Add_LocalEulerAnglesY(const _float _value)
{
    vector3 euler = m_vLocalQuaternion.to_euler();

    euler.y += _value;
    ;
    m_vLocalQuaternion = euler.to_quaternion();

    if (m_pParent)
    {
        _vector parentQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion));
        _vector localQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vLocalQuaternion));
        _vector worldQuat = XMQuaternionMultiply(localQuat, parentQuat);
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), worldQuat);
    }
    else
        m_vQuaternion = m_vLocalQuaternion;
}

void CTransform::Add_LocalEulerAnglesZ(const _float _value)
{
    vector3 euler = m_vLocalQuaternion.to_euler();

    euler.z += _value;
    ;
    m_vLocalQuaternion = euler.to_quaternion();

    if (m_pParent)
    {
        _vector parentQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion));
        _vector localQuat = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vLocalQuaternion));
        _vector worldQuat = XMQuaternionMultiply(localQuat, parentQuat);
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vQuaternion), worldQuat);
    }
    else
        m_vQuaternion = m_vLocalQuaternion;
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
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vLocalQuaternion), XMQuaternionNormalize(localQ));
    }
    else
        m_vLocalQuaternion = m_vQuaternion;
}
