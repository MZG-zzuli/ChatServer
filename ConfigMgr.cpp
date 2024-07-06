#include "ConfigMgr.h"

ConfigMgr& ConfigMgr::Inst()
{
	static ConfigMgr instance;
	return instance;
}

ConfigMgr::ConfigMgr()
{
	boost::filesystem::path path=boost::filesystem::current_path();
	path=path/"config.ini";
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(path.string(), pt);
	for (std::pair<std::string,boost::property_tree::ptree> p :pt)
	{
		std::string key=p.first;
		std::map<std::string,std::string> valueMap;
		boost::property_tree::ptree value=p.second;
		for (auto& v : value)
		{
			valueMap[v.first]=v.second.get_value<std::string>();

			
		}
		_config_map[key]._section_datas=valueMap;

	}


}
