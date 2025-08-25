using System;
using System.Collections.Generic;
using System.IO;

using byte_t = System.Byte;
using sbyte_t = System.SByte;
using uint_t = System.UInt32;
using ulong_t = System.UInt64;
using ushort_t = System.UInt16;

namespace DataTable
{

public class DataTableBase
{
}

public class DataTableListBase
{
	public Dictionary<ulong,DataTableBase> DataList = new Dictionary<ulong,DataTableBase>();
	public virtual void Load(BinaryReader reader)
	{
	}

	public DataTableBase GetDataTable(ulong serialNo)
	{
		DataTableBase dataTableBase = null;
		if(DataList.TryGetValue(serialNo, out dataTableBase) == false)
		{
			return null;
		}
		return dataTableBase;
	}

	public UInt16 Version = 0;

}

public class ItemDataTable_List : DataTableListBase
{
	public const string NAME = "Item";
	public const string DATAFILENAME = "ItemData.bytes";
	public override void Load(BinaryReader reader)
	{
		Version = reader.ReadUInt16();
		ushort_t data_count = reader.ReadUInt16();
		for(ushort_t i = 0; i < data_count; ++i)
		{
			ulong_t serialNo = reader.ReadUInt64();
			ItemDataTable data = new ItemDataTable();
			data.Load(reader);
			DataList.Add(serialNo, data);
		}
	}

}

public class ItemDataTable : DataTableBase
{
	public ulong_t ItemId;
	public string Itemname;
	public string ItemVisualname;
	public byte_t ItemType;
	public byte_t ItemRarity;
	public byte_t EquipType;
	public ulong_t gold;
	public ushort_t Atk;
	public ushort_t Def;
	public ushort_t Delay;
	public ushort_t Range;
	public void Load(BinaryReader reader)
	{
		ItemId = reader.ReadUInt64();
		uint_t ItemnameCount = reader.ReadUInt32();
		byte[] ItemnameValue = reader.ReadBytes((int)ItemnameCount);
		Itemname = System.Text.Encoding.UTF8.GetString(ItemnameValue);
		uint_t ItemVisualnameCount = reader.ReadUInt32();
		byte[] ItemVisualnameValue = reader.ReadBytes((int)ItemVisualnameCount);
		ItemVisualname = System.Text.Encoding.UTF8.GetString(ItemVisualnameValue);
		ItemType = reader.ReadByte();
		ItemRarity = reader.ReadByte();
		EquipType = reader.ReadByte();
		gold = reader.ReadUInt64();
		Atk = reader.ReadUInt16();
		Def = reader.ReadUInt16();
		Delay = reader.ReadUInt16();
		Range = reader.ReadUInt16();
	}

}


public class DataTableLoader
{
	protected Dictionary<string, DataTableListBase> _datatableList = new Dictionary<string, DataTableListBase>();

	public DataTableLoader(string dstPath)
	{
		Load(dstPath, ItemDataTable_List.DATAFILENAME, ItemDataTable_List.NAME, new ItemDataTable_List());
	}

	public DataTableBase GetDataTable(string dataName, ulong serialNo)
	{
		DataTableListBase dataTableListBase = null;
		if(_datatableList.TryGetValue(dataName, out dataTableListBase) == false)
		{
			return null;
		}
		return dataTableListBase.GetDataTable(serialNo);
	}

	public DataTableListBase GetDataTableList(string dataName)
	{
		DataTableListBase dataTableListBase = null;
		if(_datatableList.TryGetValue(dataName, out dataTableListBase) == false)
		{
			return null;
		}
		return dataTableListBase;
	}

	public void Load(string dstPath, string fileName, string dataName, DataTableListBase datatableList)
	{
		try
		{
			string dataPath = string.Format("{0}/{1}", dstPath, fileName);
			if (!File.Exists(dataPath))
			{
				return;
			}

			using (BinaryReader reader = new BinaryReader(File.Open(dataPath, FileMode.Open)))
			{
				datatableList.Load(reader);
			}

			if(_datatableList.ContainsKey(dataName))
			{
				_datatableList.Remove(dataName);
			}

			_datatableList.Add(dataName, datatableList);
		}
		catch
		{
		}
	}
}

}

