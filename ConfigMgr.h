#pragma once
#include<map>
#include<iostream>
#include<boost/filesystem.hpp>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/ini_parser.hpp>
struct SectionInfo
{
	SectionInfo() {}

	SectionInfo(const SectionInfo& other)
	{
		_section_datas = other._section_datas;
	}
	SectionInfo& operator=(const SectionInfo& other)
	{
		if (this != &other)
		{
			this->_section_datas = other._section_datas;
		}
		return *this;
	}
	std::map<std::string, std::string> _section_datas;
	std::string operator[](const std::string& key)
	{
		if (_section_datas.find(key) != _section_datas.end())
		{
			return _section_datas[key];
		}
		return "";
	}
};


class ConfigMgr
{
public:
	
	ConfigMgr();
	~ConfigMgr() {}
	SectionInfo operator[](const std::string& section)
	{
		if (_config_map.find(section) == _config_map.end())
		{
			return SectionInfo();
		}
		return _config_map[section];
	}
	ConfigMgr& operator=(const ConfigMgr& other)
	{
		if (this != &other)
		{
			this->_config_map = other._config_map;
		}
		return *this;
	}
	ConfigMgr(const ConfigMgr& other)
	{
		this->_config_map = other._config_map;
	}


private:
	std::map<std::string, SectionInfo> _config_map;

};

