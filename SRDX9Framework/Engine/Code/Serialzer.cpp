#include "Serialzer.h"

CSerialzer::CSerialzer(Mode _mode, nlohmann::json& _j)
	: m_mode(_mode)
	, m_json(_j)
	, m_current(&m_json)
{
}
