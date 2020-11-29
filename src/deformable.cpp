#include <iostream>
#include <stdio.h>

#include "program1.h"
#include "program2.h"
#include "program3.h"
#include "program4.h"
#include "program5.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Deformable objects simulation with OpenGL" << endl;
	cout << "by Alexios Karadimos, up1046820@upnet.gr, karadalex@gmail.com" << endl << endl;
	cout << "Choose program: " << endl;
    cout << "1. Deformable cube" << endl;
    cout << "2. Deformable cube and staircase experiment" << endl;
    cout << "3. Configure deformable object" << endl;
    cout << "4. Deformable cube - User interaction" << endl;
    cout << "5. Free-form deformation of objects" << endl;

	int programChoice;
	cin >> programChoice;

	switch (programChoice)
	{
	case 1:
        return program1::main(argc, argv);
	case 2:
        return program2::main(argc, argv);
	case 3:
        return program3::main(argc, argv);
    case 4:
        return program4::main(argc, argv);
    case 5:
        return program5::main(argc, argv);
	default:
		break;
	}
}
