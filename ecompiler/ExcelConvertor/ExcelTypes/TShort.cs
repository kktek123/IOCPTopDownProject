// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.TShort
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System;
using System.IO;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

public class TShort : IType
{
  public eTypeCode TypeCode => eTypeCode.Basic;

  public string Name => "short";

  public IType TKey => throw new NotImplementedException();

  public IType TValue => throw new NotImplementedException();

  public bool Write(BinaryWriter writer, string val)
  {
    short result;
    if (!short.TryParse(val, out result))
      return false;
    writer.Write(result);
    return true;
  }
}
