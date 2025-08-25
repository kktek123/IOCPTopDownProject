// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.Excel.Sheet
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

#nullable disable
namespace ExcelConvertor.Excel;

public class Sheet
{
  public List<Column> Columns = new List<Column>();
  public List<List<string>> Rows = new List<List<string>>();

  public string Name { get; private set; }

  public int Ver { get; private set; }

  public Sheet(string name, int version)
  {
    this.Name = name;
    this.Ver = version;
  }

  public void Write(BinaryWriter sw)
  {
    sw.Write((ushort) this.Ver);
    sw.Write((ushort) this.Rows.Count);
    Dictionary<ulong, ulong> dictionary = new Dictionary<ulong, ulong>();
    bool flag = false;
    for (int index1 = 0; index1 < this.Rows.Count; ++index1)
    {
      for (int index2 = 0; index2 < this.Columns.Count; ++index2)
      {
        if (this.Columns[index2].Name == "SerialNo")
        {
          flag = true;
          ulong result;
          if (!ulong.TryParse(this.Rows[index1][index2], out result))
          {
            Console.WriteLine("Error SerialNo Parse Table Name = {0}", (object) this.Name);
            break;
          }
          if (dictionary.ContainsKey(result))
          {
            Console.WriteLine("Error Contains SerialNo Table Name = {0} SerialNo = {1}", (object) this.Name, (object) result);
            break;
          }
          dictionary.Add(result, result);
          sw.Write(result);
        }
        else
          this.Columns[index2].Type.Write(sw, this.Rows[index1][index2]);
      }
    }
    if (flag)
      return;
    Console.WriteLine("Not Found SerialNo Table Name = {0}", (object) this.Name);
  }

  public void MakeCreateInsertQuery(StringBuilder sb)
  {
    StringBuilder stringBuilder = new StringBuilder();
    stringBuilder.AppendFormat("INSERT INTO [{0}DataTable](", (object) this.Name.Replace("$", ""));
    for (int index = 0; index < this.Columns.Count; ++index)
    {
      if (index == this.Columns.Count - 1)
        stringBuilder.AppendFormat("[{0}])", (object) this.Columns[index].Name);
      else
        stringBuilder.AppendFormat("[{0}],", (object) this.Columns[index].Name);
    }
    stringBuilder.AppendLine();
    for (int index1 = 0; index1 < this.Rows.Count; ++index1)
    {
      sb.Append(stringBuilder.ToString());
      sb.AppendLine("VALUES");
      for (int index2 = 0; index2 < this.Columns.Count; ++index2)
      {
        if (index2 == 0)
          sb.AppendFormat("('{0}',", (object) this.Rows[index1][index2]);
        else if (index2 == this.Columns.Count - 1)
          sb.AppendFormat("'{0}')", (object) this.Rows[index1][index2]);
        else
          sb.AppendFormat("'{0}',", (object) this.Rows[index1][index2]);
      }
      sb.AppendLine("");
      sb.AppendLine("GO");
      sb.AppendLine("");
    }
  }

  public void MakeCreateVersionQuery(StringBuilder sb)
  {
  }
}
