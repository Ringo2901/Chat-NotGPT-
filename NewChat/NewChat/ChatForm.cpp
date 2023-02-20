#include "ChatForm.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <thread>
using namespace System::Threading;

using namespace System;

SOCKET servCon;
std::string login;

System::Void NewChat::ChatForm::Initial(std::string log, SOCKET c)
{
    servCon = c;
    login = log;
}

System::Void NewChat::ChatForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ m = gcnew String(login.c_str()) + ": " + textBox1->Text;
    std::string msg;
    msg = msclr::interop::marshal_as<std::string>(m);    
    msg += '\0';
    int msg_size = msg.length();
    int code = 1;
    send(servCon, (char*)&code, sizeof(int), NULL);
    send(servCon, (char*)&msg_size, sizeof(int), NULL);
    send(servCon, msg.c_str(), msg_size, NULL);
    richTextBox1->Text =richTextBox1->Text + "Вы: " +  textBox1->Text + "\n";
    textBox1->Text = "";
}

System::Void NewChat::ChatForm::ChatForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
{
    int code = -1;
    send(servCon, (char*)&code, sizeof(int), NULL);
}




