#ifndef DATA_TABLE_H_
#define DATA_TABLE_H_

#include <string>
#include <iostream>
#include <fstream>
#include <map>

namespace DataTable
{
typedef unsigned char byte_t;
typedef signed char sbyte_t;
typedef unsigned int uint_t;
typedef unsigned long long ulong_t;
typedef unsigned short ushort_t;

using namespace std;

class DataTableBase
{
public:
	DataTableBase()
	{
	}

	virtual ~DataTableBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
	}
};

class DataTableListBase
{
protected:
	ushort_t Version;
	std::map<ulong_t, DataTableBase * > datatable_list_;

public:
	DataTableListBase()
	{
	}

	virtual ~DataTableListBase()
	{
		std::map<ulong_t, DataTableBase*>::iterator it;
		for(it = datatable_list_.begin(); it != datatable_list_.end(); ++it)
		{
			delete it->second;
		}

		datatable_list_.clear();
	}

	virtual void Load(std::ifstream &is)
	{
	}

	std::map<ulong_t, DataTableBase * > * GetDataTableList()
	{
		return &datatable_list_;
	}

	DataTableBase* GetDataTable(ulong_t serialNo)
	{
		std::map<ulong_t, DataTableBase*>::iterator it = datatable_list_.find(serialNo);
		if(it == datatable_list_.end())
		{
			return NULL;
		}
		return it->second;
	}

	ushort_t GetVersion()
	{
		return Version;
	}

};

class ItemDataTable : public DataTableBase
{
public:
	ulong_t ItemId;
	string Itemname;
	string ItemVisualname;
	byte_t ItemType;
	byte_t ItemRarity;
	byte_t EquipType;
	ulong_t gold;
	ulong_t Atk;
	ulong_t Def;
	ulong_t Delay;
	ulong_t Range;

public:
	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&ItemId, sizeof(ulong_t));
		uint_t Itemname_count = 0;
		is.read((char*)&Itemname_count, sizeof(uint_t));
		char* Itemname_temp = new char[Itemname_count];
		Itemname_temp[0] = { 0, };
		is.read(Itemname_temp, Itemname_count);
		Itemname = "";
		Itemname.append(Itemname_temp, Itemname_count);
		uint_t ItemVisualname_count = 0;
		is.read((char*)&ItemVisualname_count, sizeof(uint_t));
		char* ItemVisualname_temp = new char[ItemVisualname_count];
		ItemVisualname_temp[0] = { 0, };
		is.read(ItemVisualname_temp, ItemVisualname_count);
		ItemVisualname = "";
		ItemVisualname.append(ItemVisualname_temp, ItemVisualname_count);
		is.read((char*)&ItemType, sizeof(byte_t));
		is.read((char*)&ItemRarity, sizeof(byte_t));
		is.read((char*)&EquipType, sizeof(byte_t));
		is.read((char*)&gold, sizeof(ulong_t));
		is.read((char*)&Atk, sizeof(ulong_t));
		is.read((char*)&Def, sizeof(ulong_t));
		is.read((char*)&Delay, sizeof(ulong_t));
		is.read((char*)&Range, sizeof(ulong_t));
	}

};

#define Item_NAME "Item"
#define Item_DATAFILENAME "ItemData.bytes"
class ItemDataTable_List : public DataTableListBase
{
public:
	ItemDataTable_List() : DataTableListBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&Version, sizeof(ushort_t));
		ushort_t data_count = 0;
		is.read((char*)&data_count, sizeof(ushort_t));
		for(ushort_t i = 0; i < data_count; ++i)
		{
			ItemDataTable * data = new ItemDataTable();
			data->Load(is);
			datatable_list_.insert(std::make_pair(data->SerialNo, data));
		}
	}

};


class DataTableLoader
{
protected:
	map<string, DataTableListBase* > datatablelist_list_;

public:
	DataTableLoader(const char* dst_path)
	{
		Load(dst_path, Item_DATAFILENAME, Item_NAME, new ItemDataTable_List());
	}

	~DataTableLoader()
	{
		map<string, DataTableListBase*>::iterator it;
		for(it = datatablelist_list_.begin(); it != datatablelist_list_.end(); ++it)
		{
			delete it->second;
		}

		datatablelist_list_.clear();
	}

	void Load(const char* dst_path, const char* file_name, const char* data_name, DataTableListBase* datatable_list)
	{
		try {
			char temp_path[512] = {0,};
			sprintf(temp_path, "%s/%s", dst_path, file_name);
			string path = temp_path;
			std::ifstream is (path, std::ios::in | std::ios::binary);
			if (is.is_open())
			{
				datatable_list->Load(is);
				map<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);
				if(it != datatablelist_list_.end())
				{
					delete it->second;
					datatablelist_list_.erase(data_name);
				}
				datatablelist_list_.insert(std::make_pair(data_name, datatable_list));
			}
			else
			{
				delete datatable_list;
			}

			is.close();
		}
		catch (exception&)
		{
			delete datatable_list;
		}
	}

	DataTableBase* GetDataTable(const char* data_name, ulong serialNo)
	{
		map<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);
		if (it == datatablelist_list_.end())
		{
			return NULL;
		}
		return it->second->GetDataTable(serialNo);
	}

	DataTableListBase* GetDataTableList(const char* data_name)
	{
		map<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);
		if(it == datatablelist_list_.end())
		{
			return NULL;
		}
		return it->second;
	}

};

}
#endif

