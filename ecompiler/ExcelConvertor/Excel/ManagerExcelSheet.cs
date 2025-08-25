// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.Excel.ManagerExcelSheet
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using ExcelConvertor.CodeMaker;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.IO;
using System.Text;

#nullable disable
namespace ExcelConvertor.Excel;

public static class ManagerExcelSheet
{
  private static string _name = "";
  private static string _rootPath = "";
  private static CodeMakerInfo _codeMakerInfo;
  private static List<DataTable> _tables = new List<DataTable>();
  private static List<Sheet> Sheets = new List<Sheet>();

  public static void Initialize(string name, string rootPath)
  {
    ManagerExcelSheet._name = name;
    ManagerExcelSheet._rootPath = rootPath;
  }

  private static void LoadDataTable(DataTable dt)
  {
    DataRow row1 = dt.Rows[0];
    int length = row1.ItemArray.Length;
    int version = int.Parse(row1.ItemArray[0].ToString().Replace(" ", ""));
    Console.WriteLine("Version:{0}", (object) version);
    Sheet sheet = new Sheet(dt.TableName, version);
    for (int index = 1; index < dt.Columns.Count; ++index)
    {
      DataColumn column = dt.Columns[index];
      object obj = row1.ItemArray[index];
      sheet.Columns.Add(new Column(column.ColumnName, obj.ToString().Replace(" ", "")));
    }
    int count = dt.Rows.Count;
    for (int index1 = 4; index1 < count; ++index1)
    {
      List<string> stringList = new List<string>();
      DataRow row2 = dt.Rows[index1];
      for (int index2 = 1; index2 < length; ++index2)
        stringList.Add(row2.ItemArray[index2].ToString());
      sheet.Rows.Add(stringList);
    }
    ManagerExcelSheet.Sheets.Add(sheet);
  }

  private static void LoadExcel(string excelFile)
  {
    if (!File.Exists(excelFile))
      return;
    FileInfo fileInfo = new FileInfo(excelFile);
    fileInfo.DirectoryName.ToString();
    string str = fileInfo.Name.ToString();
    StringBuilder stringBuilder = new StringBuilder();
    stringBuilder.AppendFormat("Provider=Microsoft.ACE.OLEDB.12.0;Data Source={0};Extended Properties=\"Excel 12.0 Xml;HDR=YES\";", (object) str);
    using (OleDbConnection connection = new OleDbConnection(stringBuilder.ToString()))
    {
      connection.Open();
      try
      {
        if (connection.State != ConnectionState.Open)
          return;
        foreach (DataRow row in (InternalDataCollectionBase) connection.GetSchema("Tables").Rows)
        {
          OleDbDataAdapter oleDbDataAdapter = new OleDbDataAdapter(new OleDbCommand($" SELECT A.* FROM [{row["TABLE_NAME"]}] AS A ", connection));
          DataTable dataTable = new DataTable();
          oleDbDataAdapter.FillSchema(dataTable, SchemaType.Source);
          oleDbDataAdapter.Fill(dataTable);
          string lower = dataTable.TableName.ToLower();
          if (!lower.Contains("desc") && lower.Contains("$") && dataTable != null && dataTable.Rows.Count != 0)
            ManagerExcelSheet.LoadDataTable(dataTable);
        }
      }
      catch (Exception ex)
      {
        connection.Close();
        Console.WriteLine("OleDb Exception : {0} FIleName = {1}", (object) ex.Message, (object) str);
      }
      connection.Close();
    }
  }

  private static void LoadDirectory(DirectoryInfo rootNode)
  {
    foreach (FileInfo file in rootNode.GetFiles("*.xlsx"))
    {
      if (!file.Name.Contains("~"))
        ManagerExcelSheet.LoadExcel(file.FullName);
    }
    foreach (DirectoryInfo directory in rootNode.GetDirectories())
      ManagerExcelSheet.LoadDirectory(directory);
  }

  public static void Load()
  {
    ManagerExcelSheet.LoadDirectory(new DirectoryInfo(ManagerExcelSheet._rootPath));
    foreach (Sheet sheet in ManagerExcelSheet.Sheets)
    {
      for (int index = 0; index < sheet.Columns.Count; ++index)
      {
        Console.Write("[");
        Console.Write(sheet.Columns[index].Name);
        Console.Write("/");
        Console.Write(sheet.Columns[index].Type.Name);
        Console.Write("] ");
      }
      Console.WriteLine();
      for (int index1 = 0; index1 < sheet.Rows.Count; ++index1)
      {
        for (int index2 = 0; index2 < sheet.Columns.Count; ++index2)
        {
          Console.Write(sheet.Rows[index1][index2]);
          Console.Write(", ");
        }
        Console.WriteLine();
      }
    }
  }

  public static void MakeBinaryFile(string dstPath)
  {
    foreach (Sheet sheet in ManagerExcelSheet.Sheets)
    {
      FileStream output = new FileStream($"{dstPath}/{sheet.Name.Replace("$", "")}Data.bytes", FileMode.Create);
      BinaryWriter sw = new BinaryWriter((Stream) output);
      sheet.Write(sw);
      output.Flush();
      sw.Close();
      output.Close();
    }
  }

  public static void MakeCodeFile(string dstPath, List<CodeMakerInfo> codeMakeList)
  {
    for (int index = 0; index < codeMakeList.Count; ++index)
      codeMakeList[index].Func(dstPath, ManagerExcelSheet._name, ManagerExcelSheet.Sheets);
  }
}
