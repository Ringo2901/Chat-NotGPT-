#include "Auto.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "Reg.h"
#include "ChatForm.h"

SOCKET Connection;

System::Void NewChat::Auto::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	String^ login = textBox1->Text;
	String^ pass = textBox2->Text;
	String^ null = "";
	if (login == null || (login == nullptr))
	{
		MessageBox::Show("Введите логин!", "Ошибка!");
	}
	else
	{
		if (pass == null || (pass == nullptr))
		{
			MessageBox::Show("Введите пароль!", "Ошибка!");
		}

		else
		{
			std::string msg;
			msg = msclr::interop::marshal_as<std::string>(login);
			msg += '\0';
			int msg_size = msg.length();
			int code = 2;
			send(Connection, (char*)&code, sizeof(int), NULL);
			send(Connection, (char*)&msg_size, sizeof(int), NULL);
			send(Connection, msg.c_str(), msg_size, NULL);
			msg = msclr::interop::marshal_as<std::string>(pass);
			msg += '\0';
			msg_size = msg.length();
			code = 0;
			send(Connection, (char*)&msg_size, sizeof(int), NULL);
			send(Connection, msg.c_str(), msg_size, NULL);
			recv(Connection, (char*)&code, sizeof(int), NULL);
			if (code == 0)
			{
				MessageBox::Show("Данный пользователь не зарегестрирован!", "Ошибка!");
			}
			else
			{
				MessageBox::Show("Авторизация проведена успешно!", "Уведомление!");
				this->Hide();
				ChatForm^ form = gcnew ChatForm(msclr::interop::marshal_as<std::string>(login),Connection);
				form->Show();
			}
		}
	}
}

System::Void NewChat::Auto::Init(SOCKET c)
{
	Connection = c;
}

System::Void NewChat::Auto::linkLabel1_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
{
	this->Hide();
	Reg^ form = gcnew Reg(Connection);
	form->Show();
}
