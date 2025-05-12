#pragma once

#include "Engine_Define.h"
#include "json.hpp"
#include "FactoryManager.h"

using json = nlohmann::json;

BEGIN(Engine)

class ENGINE_DLL CSerialzer
{
public:
	enum class Mode { Save, Load };

public:
	CSerialzer(Mode _mode, nlohmann::json& _j);
	~CSerialzer() = default;

public:
	_bool IsSaving() const { return m_mode == Mode::Save; }
	_bool IsLoading() const { return m_mode == Mode::Load; }

public:
	template<typename T>
	void Value(const string& _key, T& _value);
	template<typename T>
	void Object(const string& _key, T& _object);
	template<typename T>
	void Array(const string& _key, vector<T*>& _list);

private:
	Mode m_mode;
	json& m_json;
	json* m_current;
};

END

template<typename T>
inline void CSerialzer::Value(const string& _key, T& _value)
{
	if (IsSaving())
		(*m_current)[_key] = _value;
	else if (IsLoading())
	{
		if ((*m_current).contains(_key))
			_value = (*m_current)[_key].get<T>();
	}
}

template<typename T>
inline void CSerialzer::Object(const string& _key, T& _object)
{
	if (IsSaving())
	{
		json backup = *m_current;
		(*m_current)[_key] = json::object();
		m_current = &(*m_current)[_key];

		_object.Serialize(*this);

		m_current = &backup;
	}
	else
	{
		if ((*m_current).contains(_key))
		{
			json backup = *m_current;
			m_current = &(*m_current)[_key];
			
			_object.Serialize(*this);

			m_current = &backup;
		}
	}
}

template<typename T>
inline void CSerialzer::Array(const string& _key, vector<T*>& _list)
{
	if (IsSaving())
	{
		(*m_current)[_key] = json::array();
		for (T* obj : _list)
		{
			json item;
			CSerialzer sub(Mode::Save, item);
			obj->Serialize(sub);
			(*m_current)[_key].push_back(item);
		}
	}
	else
	{
		if (!(*m_current).contains(_key))
			return;
		const auto& arr = (*m_current)[_key];
		for (const auto& item : arr)
		{
			auto it = CFactoryManager::GetInstance().getMap()[item["class"].get<string>()]();
			T* obj = dynamic_cast<T*>(it);
			CSerialzer sub(Mode::Load, const_cast<json&>(item));
			obj->Serialize(sub);
			_list.push_back(obj);
		}
	}
}

namespace nlohmann
{
	template <>
	struct adl_serializer<vector3>
	{
		static void to_json(json& j, const vector3& v)
		{
			j = json{ v.x, v.y, v.z };
		}

		static void from_json(const json& j, vector3& v)
		{
			v.x = j.at(0).get<float>();
			v.y = j.at(1).get<float>();
			v.z = j.at(2).get<float>();
		}
	};
}


