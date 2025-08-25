// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.TDouble
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System;
using System.IO;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

public class TDouble : IType
{
  public eTypeCode TypeCode => eTypeCode.Basic;

  public string Name => "double";

  public IType TKey => throw new NotImplementedException();

  public IType TValue => throw new NotImplementedException();

  public bool Write(BinaryWriter writer, string val)
  {
    double result;
    if (!double.TryParse(val, out result))
      return false;
    writer.Write(result);
    return true;
  }
}
