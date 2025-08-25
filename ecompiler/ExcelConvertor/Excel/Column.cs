// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.Excel.Column
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using ExcelConvertor.ExcelTypes;
using System.IO;

#nullable disable
namespace ExcelConvertor.Excel;

public class Column
{
  public string Name { get; protected set; }

  public IType Type { get; protected set; }

  public Column(string columnName, string typeName)
  {
    this.Name = columnName;
    this.Type = Factory.Get(typeName);
    if (this.Type == null)
      throw new InvalidDataException($"{columnName}-{typeName} don't support");
  }
}
