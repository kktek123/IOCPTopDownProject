// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.TList`1
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System;
using System.IO;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

public class TList<_TValue> : IType where _TValue : IType, new()
{
  private static IType _Tv = (IType) new _TValue();
  private static string typeName = $"list<{TList<_TValue>._Tv.Name}>";

  public eTypeCode TypeCode => eTypeCode.List;

  public string Name => TList<_TValue>.typeName;

  public IType TKey => throw new NotImplementedException("List can't use key.");

  public IType TValue => TList<_TValue>._Tv;

  public bool Write(BinaryWriter writer, string val)
  {
    string[] strArray = val.Split(',');
    if (val == "")
    {
      writer.Write(0);
      return true;
    }
    int length = strArray.Length;
    writer.Write(strArray.Length);
    for (int index = 0; index < length; ++index)
    {
      string valueString = strArray[index];
      if (!TList<_TValue>._Tv.Write(writer, valueString))
        return false;
    }
    return true;
  }
}
