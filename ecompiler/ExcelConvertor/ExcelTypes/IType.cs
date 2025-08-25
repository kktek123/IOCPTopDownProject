// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.IType
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System.IO;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

public interface IType
{
  eTypeCode TypeCode { get; }

  string Name { get; }

  IType TKey { get; }

  IType TValue { get; }

  bool Write(BinaryWriter stream, string valueString);
}
