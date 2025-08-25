// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.ExcelTypes.Factory
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using System.Collections.Generic;

#nullable disable
namespace ExcelConvertor.ExcelTypes;

public static class Factory
{
  private static Dictionary<string, IType> _mapCreator = new Dictionary<string, IType>();

  public static void Initialize()
  {
    Factory.Register<TBool>();
    Factory.Register<TByte>();
    Factory.Register<TShort>();
    Factory.Register<TInt>();
    Factory.Register<TLong>();
    Factory.Register<TFloat>();
    Factory.Register<TString>();
    Factory.Register<TUShort>();
    Factory.Register<TUInt>();
    Factory.Register<TSByte>();
    Factory.Register<TULong>();
    Factory.Register<TDouble>();
    Factory.Register<TList<TBool>>();
    Factory.Register<TList<TByte>>();
    Factory.Register<TList<TShort>>();
    Factory.Register<TList<TInt>>();
    Factory.Register<TList<TLong>>();
    Factory.Register<TList<TFloat>>();
    Factory.Register<TList<TString>>();
    Factory.Register<TList<TUShort>>();
    Factory.Register<TList<TUInt>>();
    Factory.Register<TList<TULong>>();
    Factory.Register<TList<TDouble>>();
    Factory.Register<TMap<TByte, TBool>>();
    Factory.Register<TMap<TByte, TByte>>();
    Factory.Register<TMap<TByte, TShort>>();
    Factory.Register<TMap<TByte, TInt>>();
    Factory.Register<TMap<TByte, TLong>>();
    Factory.Register<TMap<TByte, TFloat>>();
    Factory.Register<TMap<TByte, TString>>();
    Factory.Register<TMap<TByte, TList<TBool>>>();
    Factory.Register<TMap<TByte, TList<TByte>>>();
    Factory.Register<TMap<TByte, TList<TShort>>>();
    Factory.Register<TMap<TByte, TList<TInt>>>();
    Factory.Register<TMap<TByte, TList<TLong>>>();
    Factory.Register<TMap<TByte, TList<TFloat>>>();
    Factory.Register<TMap<TByte, TList<TString>>>();
    Factory.Register<TMap<TByte, TList<TUShort>>>();
    Factory.Register<TMap<TByte, TList<TUInt>>>();
    Factory.Register<TMap<TShort, TBool>>();
    Factory.Register<TMap<TShort, TByte>>();
    Factory.Register<TMap<TShort, TShort>>();
    Factory.Register<TMap<TShort, TInt>>();
    Factory.Register<TMap<TShort, TLong>>();
    Factory.Register<TMap<TShort, TFloat>>();
    Factory.Register<TMap<TShort, TString>>();
    Factory.Register<TMap<TShort, TList<TBool>>>();
    Factory.Register<TMap<TShort, TList<TByte>>>();
    Factory.Register<TMap<TShort, TList<TShort>>>();
    Factory.Register<TMap<TShort, TList<TInt>>>();
    Factory.Register<TMap<TShort, TList<TLong>>>();
    Factory.Register<TMap<TShort, TList<TFloat>>>();
    Factory.Register<TMap<TShort, TList<TString>>>();
    Factory.Register<TMap<TShort, TList<TUShort>>>();
    Factory.Register<TMap<TShort, TList<TUInt>>>();
    Factory.Register<TMap<TUShort, TUShort>>();
    Factory.Register<TMap<TUShort, TUInt>>();
    Factory.Register<TMap<TUShort, TInt>>();
    Factory.Register<TMap<TUShort, TULong>>();
    Factory.Register<TMap<TUShort, TByte>>();
    Factory.Register<TMap<TUShort, TFloat>>();
    Factory.Register<TMap<TInt, TBool>>();
    Factory.Register<TMap<TInt, TByte>>();
    Factory.Register<TMap<TInt, TShort>>();
    Factory.Register<TMap<TInt, TInt>>();
    Factory.Register<TMap<TInt, TLong>>();
    Factory.Register<TMap<TInt, TFloat>>();
    Factory.Register<TMap<TInt, TString>>();
    Factory.Register<TMap<TInt, TList<TBool>>>();
    Factory.Register<TMap<TInt, TList<TByte>>>();
    Factory.Register<TMap<TInt, TList<TShort>>>();
    Factory.Register<TMap<TInt, TList<TInt>>>();
    Factory.Register<TMap<TInt, TList<TLong>>>();
    Factory.Register<TMap<TInt, TList<TFloat>>>();
    Factory.Register<TMap<TInt, TList<TString>>>();
    Factory.Register<TMap<TInt, TList<TUShort>>>();
    Factory.Register<TMap<TInt, TList<TUInt>>>();
    Factory.Register<TMap<TLong, TBool>>();
    Factory.Register<TMap<TLong, TByte>>();
    Factory.Register<TMap<TLong, TShort>>();
    Factory.Register<TMap<TLong, TInt>>();
    Factory.Register<TMap<TLong, TLong>>();
    Factory.Register<TMap<TLong, TFloat>>();
    Factory.Register<TMap<TLong, TString>>();
    Factory.Register<TMap<TLong, TList<TBool>>>();
    Factory.Register<TMap<TLong, TList<TByte>>>();
    Factory.Register<TMap<TLong, TList<TShort>>>();
    Factory.Register<TMap<TLong, TList<TInt>>>();
    Factory.Register<TMap<TLong, TList<TLong>>>();
    Factory.Register<TMap<TLong, TList<TFloat>>>();
    Factory.Register<TMap<TLong, TList<TString>>>();
    Factory.Register<TMap<TLong, TList<TUShort>>>();
    Factory.Register<TMap<TLong, TList<TUInt>>>();
    Factory.Register<TMap<TFloat, TBool>>();
    Factory.Register<TMap<TFloat, TByte>>();
    Factory.Register<TMap<TFloat, TShort>>();
    Factory.Register<TMap<TFloat, TInt>>();
    Factory.Register<TMap<TFloat, TLong>>();
    Factory.Register<TMap<TFloat, TFloat>>();
    Factory.Register<TMap<TFloat, TString>>();
    Factory.Register<TMap<TFloat, TList<TBool>>>();
    Factory.Register<TMap<TFloat, TList<TByte>>>();
    Factory.Register<TMap<TFloat, TList<TShort>>>();
    Factory.Register<TMap<TFloat, TList<TInt>>>();
    Factory.Register<TMap<TFloat, TList<TLong>>>();
    Factory.Register<TMap<TFloat, TList<TFloat>>>();
    Factory.Register<TMap<TFloat, TList<TString>>>();
    Factory.Register<TMap<TFloat, TList<TUShort>>>();
    Factory.Register<TMap<TFloat, TList<TUInt>>>();
    Factory.Register<TMap<TString, TBool>>();
    Factory.Register<TMap<TString, TByte>>();
    Factory.Register<TMap<TString, TShort>>();
    Factory.Register<TMap<TString, TInt>>();
    Factory.Register<TMap<TString, TLong>>();
    Factory.Register<TMap<TString, TFloat>>();
    Factory.Register<TMap<TString, TString>>();
    Factory.Register<TMap<TString, TList<TBool>>>();
    Factory.Register<TMap<TString, TList<TByte>>>();
    Factory.Register<TMap<TString, TList<TShort>>>();
    Factory.Register<TMap<TString, TList<TInt>>>();
    Factory.Register<TMap<TString, TList<TLong>>>();
    Factory.Register<TMap<TString, TList<TFloat>>>();
    Factory.Register<TMap<TString, TList<TString>>>();
    Factory.Register<TMap<TString, TList<TUShort>>>();
    Factory.Register<TMap<TString, TList<TUInt>>>();
    Factory.Register<TMap<TULong, TBool>>();
    Factory.Register<TMap<TULong, TByte>>();
    Factory.Register<TMap<TULong, TShort>>();
    Factory.Register<TMap<TULong, TInt>>();
    Factory.Register<TMap<TULong, TLong>>();
    Factory.Register<TMap<TULong, TFloat>>();
    Factory.Register<TMap<TULong, TString>>();
  }

  public static void Register<_T>() where _T : IType, new()
  {
    IType type = (IType) new _T();
    Factory._mapCreator.Add(type.Name, type);
  }

  public static IType Get(string typeName)
  {
    IType type;
    if (Factory._mapCreator.TryGetValue(typeName, out type))
      return type;
    string key1 = typeName + "_t";
    if (Factory._mapCreator.TryGetValue(key1, out type))
      return type;
    char[] chArray = new char[2]{ '<', '>' };
    string[] strArray = typeName.Split(chArray);
    if (strArray.Length < 3)
      return (IType) null;
    string key2 = $"{strArray[0]}<{strArray[1]}_t>{strArray[2]}";
    return Factory._mapCreator.TryGetValue(key2, out type) ? type : (IType) null;
  }

  public delegate IType TDelegateCreator();
}
