// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.TString
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System;
using System.IO;
using System.Text;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

public class TString : IType
{
  public eTypeCode TypeCode => eTypeCode.String;

  public string Name => "string";

  public IType TKey => throw new NotImplementedException();

  public IType TValue => throw new NotImplementedException();

  public bool Write(BinaryWriter writer, string val)
  {
    byte[] bytes = Encoding.UTF8.GetBytes(val);
    writer.Write((uint) bytes.Length);
    writer.Write(bytes, 0, bytes.Length);
    return true;
  }
}
