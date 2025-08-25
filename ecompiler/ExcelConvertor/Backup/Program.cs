// Decompiled with JetBrains decompiler
// Type: ExcelConvertor.Program
// Assembly: ExcelConvertor, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: CDF9A41A-4DCB-411E-B21D-FA494429A5BE
// Assembly location: C:\Unreal porject\테이블\ExcelConvertor.exe

using ExcelConvertor.CodeMaker;
using ExcelConvertor.Excel;
using ExcelConvertor.ExcelTypes;
using System;
using System.Collections.Generic;

#nullable disable
namespace ExcelConvertor;

internal class Program
{
  private static void Main(string[] args)
  {
    foreach (string str in args)
      Console.WriteLine(str);
    string name = "data";
    string rootPath = "./";
    string dstPath = "./";
    string str1 = "csharp";
    if (args.Length != 0)
      name = args[0];
    if (args.Length > 1)
      str1 = args[1].ToLower();
    if (args.Length > 2)
      rootPath = args[2].ToLower();
    if (args.Length > 3)
      dstPath = args[3].ToLower();
    if (args.Length > 4)
      args[4].ToLower();
    if (args.Length > 5)
      bool.Parse(args[5]);
    string[] strArray = str1.Split('|');
    CodeMakerMethods.Initialize();
    List<CodeMakerInfo> codeMakeList = new List<CodeMakerInfo>();
    for (int index = 0; index < strArray.Length; ++index)
    {
      CodeMakerInfo cmi;
      if (!CodeMakerMethods.Get(strArray[index], out cmi))
      {
        Console.WriteLine("{0} is unknown language", (object) str1);
        return;
      }
      codeMakeList.Add(cmi);
    }
    Factory.Initialize();
    ManagerExcelSheet.Initialize(name, rootPath);
    ManagerExcelSheet.Load();
    ManagerExcelSheet.MakeBinaryFile(dstPath);
    ManagerExcelSheet.MakeCodeFile(dstPath, codeMakeList);
  }
}
