//
//  main.cpp
//  Calculator
//
//  Created by Jin on 2015. 3. 11..
//  Copyright (c) 2015년 __Jin__. All rights reserved.
//

#pragma warning(disable : 4996)

#include <stdio.h>
#include <string>
#include "System.h"
#include "DumpLogManager.h"

int main(int argc, const char * argv[])
{
    GlobalDumpManagerAddLogNewLine("--------- Init ---------");
    System* system = System::GetInstance();

    const std::string fileName = "/Users/jin/Project/MipsEmulator/Calculator/Calculator/input4.bin";
	GlobalDumpManagerAddLogNewLine("Load .bin file : " + fileName);
	system->Load(fileName);
	GlobalDumpManagerAddLogNewLine("--------- Done ---------\n\n");
	GlobalDumpManagerAddLogNewLine("--------- Init Cache ---------");
    system->InitializeCache(256, 64, 2, 3, 8);
	GlobalDumpManagerAddLogNewLine("----------- Done -------------\n\n");
    GlobalDumpManagerAddLogNewLine("--------- System Start ---------");
	system->Run();
	GlobalDumpManagerAddLogNewLine("--------- System End ---------");

	GlobalDumpLogManager->WriteFile("/Users/jin/Project/MipsEmulator/Calculator/Calculator/out.txt");
    

    return 0;
}
