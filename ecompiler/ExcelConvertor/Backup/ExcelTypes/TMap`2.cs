// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.TMap`2
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System.IO;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

public class TMap<_TKey, _TValue> : IType
  where _TKey : IType, new()
  where _TValue : IType, new()
{
  private static _TKey _Tk = new _TKey();
  private static _TValue _Tv = new _TValue();
  private static string typeName = $"map<{TMap<_TKey, _TValue>._Tk.Name},{TMap<_TKey, _TValue>._Tv.Name}>";

  public eTypeCode TypeCode => eTypeCode.Map;

  public string Name => TMap<_TKey, _TValue>.typeName;

  public IType TKey => (IType) TMap<_TKey, _TValue>._Tk;

  public IType TValue => (IType) TMap<_TKey, _TValue>._Tv;

  public bool Write(BinaryWriter writer, string val)
  {
    string[] strArray = val.Split(',');
    writer.Write(strArray.Length);
    for (int index = 0; index < strArray.Length; ++index)
    {
      if (!TMap<_TKey, _TValue>._Tv.Write(writer, strArray[index]))
        return false;
    }
    return true;
  }
}
