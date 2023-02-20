#include "Reg.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "Auto.h"

using namespace System;
SOCKET Con;
System::Void NewChat::Reg::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
 if (textBox1->Text == "" || textBox1->Text==nullptr)
    {
        MessageBox::Show("Вы не ввели имя.", "Ошибка!");
        return;
    }
    if (textBox2->Text == "" || textBox2->Text == nullptr)
    {
        MessageBox::Show("Вы не ввели фамилию.", "Ошибка!");
        return;
    }
    if (textBox3->Text == "" || textBox3->Text == nullptr)
    {
        MessageBox::Show("Вы не ввели логин.", "Ошибка!");
        return;
    }
    if (textBox4->Text == "" || textBox4->Text == nullptr)
    {
        MessageBox::Show("Вы не ввели пароль.", "Ошибка!");
        return;
    }
    if (isUserExitst()==1)
    {
        MessageBox::Show("Данный логин уже существует, введите новый.", "Ошибка!");
        return;
    }
    int code = 4;
    int msg_size;
    send(Con, (char*)&code, sizeof(int), NULL);
    std::string msg;
    String^ name = textBox1->Text;
    msg = msclr::interop::marshal_as<std::string>(name);
    msg += '\0';
    msg_size = msg.length();
    send(Con, (char*)&msg_size, sizeof(int), NULL);
    send(Con, msg.c_str(), msg_size, NULL);
    String^ sname = textBox2->Text;
    msg = msclr::interop::marshal_as<std::string>(sname);
    msg += '\0';
    msg_size = msg.length();
    send(Con, (char*)&msg_size, sizeof(int), NULL);
    send(Con, msg.c_str(), msg_size, NULL);
    String^ login = textBox3->Text;
    msg = msclr::interop::marshal_as<std::string>(login);
    msg += '\0';
    msg_size = msg.length();
    send(Con, (char*)&msg_size, sizeof(int), NULL);
    send(Con, msg.c_str(), msg_size, NULL);
    String^ pass = textBox4->Text;
    msg = msclr::interop::marshal_as<std::string>(pass);
    msg += '\0';
    msg_size = msg.length();
    send(Con, (char*)&msg_size, sizeof(int), NULL);
    send(Con, msg.c_str(), msg_size, NULL);
    int ans;
    recv(Con, (char*)&ans, sizeof(int), NULL);
    if (ans == 1)
    {
        MessageBox::Show("Аккаунт был создан.", "Уведомление!");
    }
    else
    {
        MessageBox::Show("Аккаунт не был создан.", "Уведомление!");
    }
}

int NewChat::Reg::isUserExitst()
{
    String^ login = textBox3->Text;
    std::string msg;
    msg = msclr::interop::marshal_as<std::string>(login);
    msg += '\0';
    int msg_size = msg.length();
    int code = 3;
    send(Con, (char*)&code, sizeof(int), NULL);
    send(Con, (char*)&msg_size, sizeof(int), NULL);
    send(Con, msg.c_str(), msg_size, NULL);
    int ans;
    recv(Con, (char*)&ans, sizeof(int), NULL);
    return ans;
}

System::Void NewChat::Reg::linkLabel1_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
{
    this->Hide();
    Auto^ autoform = gcnew Auto(Con);
    autoform->Show();
}

System::Void NewChat::Reg::Ini(SOCKET c)
{
    Con = c;
}
