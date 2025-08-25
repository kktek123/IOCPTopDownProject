// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.TUShort
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System;
using System.IO;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

internal class TUShort : IType
{
  public eTypeCode TypeCode => eTypeCode.Basic;

  public string Name => "ushort_t";

  public IType TKey => throw new NotImplementedException();

  public IType TValue => throw new NotImplementedException();

  public bool Write(BinaryWriter writer, string val)
  {
    ushort result;
    if (!ushort.TryParse(val, out result))
      return false;
    writer.Write(result);
    return true;
  }
}
