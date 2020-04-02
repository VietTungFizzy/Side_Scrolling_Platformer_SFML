#pragma once
#include"Utils.h"
#include<unordered_map>
#include<sstream>
#include<fstream>
template<typename DerivedClass, typename ResourceType>
class ResourceManager
{
public:
	ResourceManager(const std::string & l_resourcesPaths)
	{
		std::ifstream input(Utils::GetWorkingDirection() + l_resourcesPaths);
		if (!input.is_open())
		{
			std::cout << "Can't open " << Utils::GetWorkingDirection() + l_resourcesPaths << std::endl;
			input.close();
			return;
		}

		std::string line;
		while (std::getline(input, line))
		{
			std::stringstream keystream(line);
			std::string name, path;
			keystream >> name >> path;

			m_resourcesPaths.emplace(name, path);
		}
		input.close();
	}
	virtual ~ResourceManager()
	{
		while (m_resources.begin() != m_resources.end())
		{
			delete m_resources.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
	}

	ResourceType * GetResource(const std::string & l_name)
	{
		std::pair<ResourceType *, unsigned int> * res = Find(l_name);
		return (res != nullptr ? res->first : nullptr);
	}
	bool ReleaseResource(const std::string & l_name)
	{
		std::pair<ResourceType *, unsigned int> * res = Find(l_name);
		if (!res) return false;

		res->second--;
		if (res->second <= 0)
		{
			delete res->first;
			res = nullptr;
			m_resources.erase(l_name);
		}
		return true;
	}
	bool RequireResource(const std::string & l_name)
	{
		std::pair<ResourceType *, unsigned int> * res = Find(l_name);
		if (res)
		{
			res->second++;
		}
		else
		{
			std::unordered_map<std::string, std::string>::iterator resourcePath = m_resourcesPaths.find(l_name);
			if (resourcePath == m_resourcesPaths.end()) return false;
			std::pair<ResourceType *, unsigned int> tmp = std::make_pair(Load(resourcePath->second), 1);
			if (!tmp.first) return false;
			m_resources.emplace(l_name, tmp);
		}

		return true;
	}

private:
	std::pair<ResourceType *, unsigned int> * Find(const std::string & l_name)
	{
		auto res = m_resources.find(l_name);
		return (res != m_resources.end() ? &res->second : nullptr);
	}
	ResourceType * Load(const std::string & l_path)
	{
		return static_cast<DerivedClass*>(this)->Load(l_path);
	}
private:
	std::unordered_map<std::string, std::pair<ResourceType *, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_resourcesPaths;
};


