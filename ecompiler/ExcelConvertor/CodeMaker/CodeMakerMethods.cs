// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.CodeMaker.CodeMakerMethods
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using ExcelConvertor.Excel;
using ExcelConvertor.ExcelTypes;
using System.Collections.Generic;
using System.IO;
using System.Text;

#nullable disable
namespace ExcelConvertor.CodeMaker;

public static class CodeMakerMethods
{
  private static Dictionary<string, CodeMakerInfo> _mapCodeMakeMethods = new Dictionary<string, CodeMakerInfo>();

  public static void Initialize()
  {
    CodeMakerMethods._mapCodeMakeMethods.Add("csharp", new CodeMakerInfo(new TDelegateMakeCode(CodeMakerMethods.MakeCSharpParentCode)));
    CodeMakerMethods._mapCodeMakeMethods.Add("cplusplus", new CodeMakerInfo(new TDelegateMakeCode(CodeMakerMethods.MakeCPlusPlusParentCode)));
  }

  public static bool Get(string lang, out CodeMakerInfo cmi)
  {
    if (CodeMakerMethods._mapCodeMakeMethods.TryGetValue(lang, out cmi))
      return true;
    cmi = new CodeMakerInfo();
    return false;
  }

  private static string GetRead(IType type)
  {
    switch (type.Name)
    {
      case "bool":
        return "ReadBoolean";
      case "byte_t":
        return "ReadByte";
      case "double":
        return "ReadDouble";
      case "float":
        return "ReadSingle";
      case "int":
        return "ReadInt32";
      case "long":
        return "ReadInt64";
      case "sbyte_t":
        return "ReadSByte";
      case "short":
        return "ReadInt16";
      case "string":
        return "ReadString";
      case "uint_t":
        return "ReadUInt32";
      case "ulong_t":
        return "ReadUInt64";
      case "ushort_t":
        return "ReadUInt16";
      default:
        return "";
    }
  }

  private static string GetStringRead(IType type)
  {
    switch (type.Name)
    {
      case "bool":
        return "bool.Parse";
      case "byte_t":
        return "byte.Parse";
      case "double":
        return "double.Parse";
      case "float":
        return "float.Parse";
      case "int":
        return "int.Parse";
      case "long":
        return "long.Parse";
      case "sbyte_t":
        return "sbyte.Parse";
      case "short":
        return "short.Parse";
      case "string":
        return "";
      case "uint_t":
        return "uint.Parse";
      case "ulong_t":
        return "ulong.Parse";
      case "ushort_t":
        return "ushort.Parse";
      default:
        return "";
    }
  }

  private static void MakeCSharpParentCode(string dstPath, string name, List<Sheet> sheets)
  {
    FileStream fileStream = new FileStream($"{dstPath}/{name}.{"cs"}", FileMode.Create);
    StreamWriter streamWriter = new StreamWriter((Stream) fileStream);
    StringBuilder sb = new StringBuilder();
    sb.AppendLine("using System;");
    sb.AppendLine("using System.Collections.Generic;");
    sb.AppendLine("using System.IO;");
    sb.AppendLine();
    sb.AppendLine("using byte_t = System.Byte;");
    sb.AppendLine("using sbyte_t = System.SByte;");
    sb.AppendLine("using uint_t = System.UInt32;");
    sb.AppendLine("using ulong_t = System.UInt64;");
    sb.AppendLine("using ushort_t = System.UInt16;");
    sb.AppendLine();
    sb.AppendLine("namespace DataTable");
    sb.AppendLine("{");
    sb.AppendLine();
    sb.AppendLine("public class DataTableBase");
    sb.AppendLine("{");
    sb.AppendLine("}");
    sb.AppendLine();
    sb.AppendLine("public class DataTableListBase");
    sb.AppendLine("{");
    sb.AppendLine("\tpublic Dictionary<ulong,DataTableBase> DataList = new Dictionary<ulong,DataTableBase>();");
    sb.AppendLine("\tpublic virtual void Load(BinaryReader reader)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tpublic DataTableBase GetDataTable(ulong serialNo)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tDataTableBase dataTableBase = null;");
    sb.AppendLine("\t\tif(DataList.TryGetValue(serialNo, out dataTableBase) == false)");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\treturn null;");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\treturn dataTableBase;");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tpublic UInt16 Version = 0;");
    sb.AppendLine();
    sb.AppendLine("}");
    sb.AppendLine();
    foreach (Sheet sheet in sheets)
      CodeMakerMethods.MakeCSharpCode(sb, sheet);
    sb.AppendLine();
    sb.AppendLine("public class DataTableLoader");
    sb.AppendLine("{");
    sb.AppendLine("\tprotected Dictionary<string, DataTableListBase> _datatableList = new Dictionary<string, DataTableListBase>();");
    sb.AppendLine();
    sb.AppendLine("\tpublic DataTableLoader(string dstPath)");
    sb.AppendLine("\t{");
    foreach (Sheet sheet in sheets)
      CodeMakerMethods.MakeCSharpLoadCode(sb, sheet);
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tpublic DataTableBase GetDataTable(string dataName, ulong serialNo)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tDataTableListBase dataTableListBase = null;");
    sb.AppendLine("\t\tif(_datatableList.TryGetValue(dataName, out dataTableListBase) == false)");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\treturn null;");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\treturn dataTableListBase.GetDataTable(serialNo);");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tpublic DataTableListBase GetDataTableList(string dataName)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tDataTableListBase dataTableListBase = null;");
    sb.AppendLine("\t\tif(_datatableList.TryGetValue(dataName, out dataTableListBase) == false)");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\treturn null;");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\treturn dataTableListBase;");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tpublic void Load(string dstPath, string fileName, string dataName, DataTableListBase datatableList)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\ttry");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\tstring dataPath = string.Format(\"{0}/{1}\", dstPath, fileName);");
    sb.AppendLine("\t\t\tif (!File.Exists(dataPath))");
    sb.AppendLine("\t\t\t{");
    sb.AppendLine("\t\t\t\treturn;");
    sb.AppendLine("\t\t\t}");
    sb.AppendLine();
    sb.AppendLine("\t\t\tusing (BinaryReader reader = new BinaryReader(File.Open(dataPath, FileMode.Open)))");
    sb.AppendLine("\t\t\t{");
    sb.AppendLine("\t\t\t\tdatatableList.Load(reader);");
    sb.AppendLine("\t\t\t}");
    sb.AppendLine();
    sb.AppendLine("\t\t\tif(_datatableList.ContainsKey(dataName))");
    sb.AppendLine("\t\t\t{");
    sb.AppendLine("\t\t\t\t_datatableList.Remove(dataName);");
    sb.AppendLine("\t\t\t}");
    sb.AppendLine();
    sb.AppendLine("\t\t\t_datatableList.Add(dataName, datatableList);");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\tcatch");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t}");
    sb.AppendLine("}");
    sb.AppendLine();
    sb.AppendLine("}");
    streamWriter.WriteLine(sb.ToString());
    fileStream.Flush();
    streamWriter.Close();
    fileStream.Close();
  }

  private static void MakeCSharpCode(StringBuilder sb, Sheet sh)
  {
    CodeMakerMethods.Master_MakeCSharpCode(sb, sh);
    sb.AppendFormat("public class {0}DataTable : DataTableBase", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendLine("{");
    StringBuilder stringBuilder = new StringBuilder();
    foreach (Column column in sh.Columns)
    {
      if (!(column.Name == "SerialNo"))
      {
        switch (column.Type.TypeCode)
        {
          case eTypeCode.Basic:
            sb.AppendFormat("\tpublic {0} {1};", (object) column.Type.Name, (object) column.Name);
            sb.AppendLine();
            stringBuilder.AppendFormat("\t\t{0} = reader.{1}();", (object) column.Name, (object) CodeMakerMethods.GetRead(column.Type));
            stringBuilder.AppendLine();
            continue;
          case eTypeCode.List:
            sb.AppendFormat("\tpublic List<{0}> {1};", (object) column.Type.TValue.Name, (object) column.Name);
            sb.AppendLine();
            stringBuilder.AppendFormat("\t\t{0} = new List<{1}>();", (object) column.Name, (object) column.Type.TValue.Name);
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\tuint_t {0}Count = reader.ReadUInt32();", (object) column.Name);
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\tfor( uint_t i = 0; i < {0}Count; ++i)", (object) column.Name);
            stringBuilder.AppendLine();
            stringBuilder.AppendLine("\t\t{");
            if (column.Type.TValue.TypeCode == eTypeCode.String)
            {
              stringBuilder.AppendFormat("\t\t\tuint_t tempCount = reader.ReadUInt32();");
              stringBuilder.AppendLine();
              stringBuilder.AppendFormat("\t\t\tbyte[] tempValue = reader.ReadBytes((int)tempCount);");
              stringBuilder.AppendLine();
              stringBuilder.AppendFormat("\t\t\tstring temp = System.Text.Encoding.UTF8.GetString(tempValue);");
            }
            else
              stringBuilder.AppendFormat("\t\t\t{0} temp = reader.{1}();", (object) column.Type.TValue.Name, (object) CodeMakerMethods.GetRead(column.Type.TValue));
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\t{0}.Add(temp);", (object) column.Name);
            stringBuilder.AppendLine();
            stringBuilder.AppendLine("\t\t}");
            continue;
          case eTypeCode.String:
            sb.AppendFormat("\tpublic {0} {1};", (object) column.Type.Name, (object) column.Name);
            sb.AppendLine();
            stringBuilder.AppendFormat("\t\tuint_t {0}Count = reader.ReadUInt32();", (object) column.Name);
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\tbyte[] {0}Value = reader.ReadBytes((int){0}Count);", (object) column.Name, (object) column.Name);
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t{0} = System.Text.Encoding.UTF8.GetString({0}Value);", (object) column.Name, (object) column.Name);
            stringBuilder.AppendLine();
            continue;
          default:
            continue;
        }
      }
    }
    sb.AppendLine("\tpublic void Load(BinaryReader reader)");
    sb.AppendLine("\t{");
    sb.Append(stringBuilder.ToString());
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("}");
    sb.AppendLine();
  }

  private static void Master_MakeCSharpCode(StringBuilder sb, Sheet sh)
  {
    sb.AppendFormat("public class {0}DataTable_List : DataTableListBase", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendLine("{");
    sb.AppendFormat("\tpublic const string NAME = {0}{1}{2};", (object) "\"", (object) sh.Name.Replace("$", ""), (object) "\"");
    sb.AppendLine();
    sb.AppendFormat("\tpublic const string DATAFILENAME = {0}{1}Data.bytes{2};", (object) "\"", (object) sh.Name.Replace("$", ""), (object) "\"");
    sb.AppendLine();
    sb.AppendLine("\tpublic override void Load(BinaryReader reader)");
    sb.AppendLine("\t{");
    sb.AppendFormat("\t\tVersion = reader.ReadUInt16();");
    sb.AppendLine();
    sb.AppendFormat("\t\tushort_t data_count = reader.ReadUInt16();");
    sb.AppendLine();
    sb.AppendLine("\t\tfor(ushort_t i = 0; i < data_count; ++i)");
    sb.AppendLine("\t\t{");
    sb.AppendFormat("\t\t\tulong_t serialNo = reader.ReadUInt64();");
    sb.AppendLine();
    sb.AppendFormat("\t\t\t{0}DataTable data = new {0}DataTable();", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendFormat("\t\t\tdata.Load(reader);");
    sb.AppendLine();
    sb.AppendFormat("\t\t\tDataList.Add(serialNo, data);", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("}");
    sb.AppendLine();
  }

  private static void MakeCSharpLoadCode(StringBuilder sb, Sheet sh)
  {
    sb.AppendFormat("\t\tLoad(dstPath, {0}DataTable_List.DATAFILENAME, {0}DataTable_List.NAME, new {0}DataTable_List());", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
  }

  private static void MakeCPlusPlusParentCode(string dstPath, string name, List<Sheet> sheets)
  {
    FileStream fileStream = new FileStream($"{dstPath}/{name}.{"h"}", FileMode.Create);
    StreamWriter streamWriter = new StreamWriter((Stream) fileStream);
    StringBuilder sb = new StringBuilder();
    sb.AppendLine("#ifndef DATA_TABLE_H_");
    sb.AppendLine("#define DATA_TABLE_H_");
    sb.AppendLine();
    sb.AppendLine("#include <string>");
    sb.AppendLine("#include <iostream>");
    sb.AppendLine("#include <fstream>");
    sb.AppendLine("#include <map>");
    sb.AppendLine();
    sb.AppendLine("namespace DataTable");
    sb.AppendLine("{");
    sb.AppendLine("typedef unsigned char byte_t;");
    sb.AppendLine("typedef signed char sbyte_t;");
    sb.AppendLine("typedef unsigned int uint_t;");
    sb.AppendLine("typedef unsigned long long ulong_t;");
    sb.AppendLine("typedef unsigned short ushort_t;");
    sb.AppendLine();
    sb.AppendLine("using namespace std;");
    sb.AppendLine();
    sb.AppendLine("class DataTableBase");
    sb.AppendLine("{");
    sb.AppendLine("public:");
    sb.AppendLine("\tDataTableBase()");
    sb.AppendLine("\t{");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tvirtual ~DataTableBase()");
    sb.AppendLine("\t{");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tvirtual void Load(std::ifstream &is)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t}");
    sb.AppendLine("};");
    sb.AppendLine();
    sb.AppendLine("class DataTableListBase");
    sb.AppendLine("{");
    sb.AppendLine("protected:");
    sb.AppendLine("\tushort_t Version;");
    sb.AppendLine("\tstd::map<ulong_t, DataTableBase * > datatable_list_;");
    sb.AppendLine();
    sb.AppendLine("public:");
    sb.AppendLine("\tDataTableListBase()");
    sb.AppendLine("\t{");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tvirtual ~DataTableListBase()");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tstd::map<ulong_t, DataTableBase*>::iterator it;");
    sb.AppendLine("\t\tfor(it = datatable_list_.begin(); it != datatable_list_.end(); ++it)");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\tdelete it->second;");
    sb.AppendLine("\t\t}");
    sb.AppendLine();
    sb.AppendLine("\t\tdatatable_list_.clear();");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tvirtual void Load(std::ifstream &is)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tstd::map<ulong_t, DataTableBase * > * GetDataTableList()");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\treturn &datatable_list_;");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tDataTableBase* GetDataTable(ulong_t serialNo)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tstd::map<ulong_t, DataTableBase*>::iterator it = datatable_list_.find(serialNo);");
    sb.AppendLine("\t\tif(it == datatable_list_.end())");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\treturn NULL;");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\treturn it->second;");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tushort_t GetVersion()");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\treturn Version;");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("};");
    sb.AppendLine();
    foreach (Sheet sheet in sheets)
      CodeMakerMethods.MakeCPlusPlusCode(sb, sheet);
    sb.AppendLine();
    sb.AppendLine("class DataTableLoader");
    sb.AppendLine("{");
    sb.AppendLine("protected:");
    sb.AppendLine("\tmap<string, DataTableListBase* > datatablelist_list_;");
    sb.AppendLine();
    sb.AppendLine("public:");
    sb.AppendLine("\tDataTableLoader(const char* dst_path)");
    sb.AppendLine("\t{");
    foreach (Sheet sheet in sheets)
      CodeMakerMethods.MakeCPlusPlusLoadCode(sb, sheet);
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\t~DataTableLoader()");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tmap<string, DataTableListBase*>::iterator it;");
    sb.AppendLine("\t\tfor(it = datatablelist_list_.begin(); it != datatablelist_list_.end(); ++it)");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\tdelete it->second;");
    sb.AppendLine("\t\t}");
    sb.AppendLine();
    sb.AppendLine("\t\tdatatablelist_list_.clear();");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tvoid Load(const char* dst_path, const char* file_name, const char* data_name, DataTableListBase* datatable_list)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\ttry {");
    sb.AppendLine("\t\t\tchar temp_path[512] = {0,};");
    sb.AppendLine("\t\t\tsprintf(temp_path, \"%s/%s\", dst_path, file_name);");
    sb.AppendLine("\t\t\tstring path = temp_path;");
    sb.AppendLine("\t\t\tstd::ifstream is (path, std::ios::in | std::ios::binary);");
    sb.AppendLine("\t\t\tif (is.is_open())");
    sb.AppendLine("\t\t\t{");
    sb.AppendLine("\t\t\t\tdatatable_list->Load(is);");
    sb.AppendLine("\t\t\t\tmap<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);");
    sb.AppendLine("\t\t\t\tif(it != datatablelist_list_.end())");
    sb.AppendLine("\t\t\t\t{");
    sb.AppendLine("\t\t\t\t\tdelete it->second;");
    sb.AppendLine("\t\t\t\t\tdatatablelist_list_.erase(data_name);");
    sb.AppendLine("\t\t\t\t}");
    sb.AppendLine("\t\t\t\tdatatablelist_list_.insert(std::make_pair(data_name, datatable_list));");
    sb.AppendLine("\t\t\t}");
    sb.AppendLine("\t\t\telse");
    sb.AppendLine("\t\t\t{");
    sb.AppendLine("\t\t\t\tdelete datatable_list;");
    sb.AppendLine("\t\t\t}");
    sb.AppendLine();
    sb.AppendLine("\t\t\tis.close();");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\tcatch (exception&)");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\tdelete datatable_list;");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tDataTableBase* GetDataTable(const char* data_name, ulong serialNo)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tmap<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);");
    sb.AppendLine("\t\tif (it == datatablelist_list_.end())");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\treturn NULL;");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\treturn it->second->GetDataTable(serialNo);");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tDataTableListBase* GetDataTableList(const char* data_name)");
    sb.AppendLine("\t{");
    sb.AppendLine("\t\tmap<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);");
    sb.AppendLine("\t\tif(it == datatablelist_list_.end())");
    sb.AppendLine("\t\t{");
    sb.AppendLine("\t\t\treturn NULL;");
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t\treturn it->second;");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("};");
    sb.AppendLine();
    sb.AppendLine("}");
    sb.AppendLine("#endif");
    streamWriter.WriteLine(sb.ToString());
    fileStream.Flush();
    streamWriter.Close();
    fileStream.Close();
  }

  private static void Master_MakeCPlusPlusCode(StringBuilder sb, Sheet sh)
  {
    sb.AppendFormat("#define {0}_NAME \"{0}\"", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendFormat("#define {0}_DATAFILENAME \"{0}Data.bytes\"", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendFormat("class {0}DataTable_List : public DataTableListBase", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendLine("{");
    sb.AppendLine("public:");
    sb.AppendFormat("\t{0}DataTable_List() : DataTableListBase()", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendLine("\t{");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("\tvirtual void Load(std::ifstream &is)");
    sb.AppendLine("\t{");
    sb.AppendFormat("\t\tis.read((char*)&Version, sizeof(ushort_t));");
    sb.AppendLine();
    sb.AppendFormat("\t\tushort_t data_count = 0;");
    sb.AppendLine();
    sb.AppendFormat("\t\tis.read((char*)&data_count, sizeof(ushort_t));");
    sb.AppendLine();
    sb.AppendLine("\t\tfor(ushort_t i = 0; i < data_count; ++i)");
    sb.AppendLine("\t\t{");
    sb.AppendFormat("\t\t\t{0}DataTable * data = new {0}DataTable();", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendFormat("\t\t\tdata->Load(is);");
    sb.AppendLine();
    sb.AppendFormat("\t\t\tdatatable_list_.insert(std::make_pair(data->SerialNo, data));");
    sb.AppendLine();
    sb.AppendLine("\t\t}");
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("};");
    sb.AppendLine();
  }

  private static void MakeCPlusPlusCode(StringBuilder sb, Sheet sh)
  {
    sb.AppendFormat("class {0}DataTable : public DataTableBase", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
    sb.AppendLine("{");
    sb.AppendLine("public:");
    StringBuilder stringBuilder = new StringBuilder();
    foreach (Column column in sh.Columns)
    {
      switch (column.Type.TypeCode)
      {
        case eTypeCode.Basic:
          sb.AppendFormat("\t{0} {1};", (object) column.Type.Name, (object) column.Name);
          sb.AppendLine();
          stringBuilder.AppendFormat("\t\tis.read((char*)&{0}, sizeof({1}));", (object) column.Name, (object) column.Type.Name);
          stringBuilder.AppendLine();
          continue;
        case eTypeCode.List:
          sb.AppendFormat("\tstd::list<{0}> {1};", (object) column.Type.TValue.Name, (object) column.Name);
          sb.AppendLine();
          stringBuilder.AppendFormat("\t\tuint_t {0}_count = 0;", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\tis.read((char*)&{0}_count, sizeof(uint_t));", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\tfor( uint_t i = 0; i < {0}_count; ++i)", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendLine("\t\t{");
          if (column.Type.TValue.TypeCode == eTypeCode.String)
          {
            stringBuilder.AppendFormat("\t\t\tunsigned int temp_count = 0;");
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\tis.read((char*)&temp_count, sizeof(uint_t));");
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\tchar* temp_string = new char[temp_count];");
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\ttemp_string[0] = {{ 0, }};");
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\tis.read(temp_string, temp_count);");
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\tstring temp = \"\";");
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\ttemp.append(temp_string, temp_count);");
            stringBuilder.AppendLine();
          }
          else
          {
            stringBuilder.AppendFormat("\t\t\t{0} temp;", (object) column.Type.TValue.Name);
            stringBuilder.AppendLine();
            stringBuilder.AppendFormat("\t\t\tis.read((char*)&temp, sizeof({0}));", (object) column.Type.TValue.Name);
          }
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\t\t{0}.push_back(temp);", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendLine("\t\t}");
          continue;
        case eTypeCode.String:
          sb.AppendFormat("\t{0} {1};", (object) column.Type.Name, (object) column.Name);
          sb.AppendLine();
          stringBuilder.AppendFormat("\t\tuint_t {0}_count = 0;", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\tis.read((char*)&{0}_count, sizeof(uint_t));", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\tchar* {0}_temp = new char[{0}_count];", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\t{0}_temp[0] = {{ 0, }};", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\tis.read({0}_temp, {0}_count);", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\t{0} = \"\";", (object) column.Name);
          stringBuilder.AppendLine();
          stringBuilder.AppendFormat("\t\t{0}.append({0}_temp, {0}_count);", (object) column.Name);
          stringBuilder.AppendLine();
          continue;
        default:
          continue;
      }
    }
    sb.AppendLine();
    sb.AppendLine("public:");
    sb.AppendLine("\tvirtual void Load(std::ifstream &is)");
    sb.AppendLine("\t{");
    sb.Append(stringBuilder.ToString());
    sb.AppendLine("\t}");
    sb.AppendLine();
    sb.AppendLine("};");
    sb.AppendLine();
    CodeMakerMethods.Master_MakeCPlusPlusCode(sb, sh);
  }

  private static void MakeCPlusPlusLoadCode(StringBuilder sb, Sheet sh)
  {
    sb.AppendFormat("\t\tLoad(dst_path, {0}_DATAFILENAME, {0}_NAME, new {0}DataTable_List());", (object) sh.Name.Replace("$", ""));
    sb.AppendLine();
  }
}
