#include "StartForm.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <msclr\marshal_cppstd.h>
#include <iostream>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Auto.h"
#pragma warning(disable: 4996)
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Data::OleDb;
SOCKET Conn;
[STAThread]
int main(array<String^>^ arg) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	NewChat::StartForm form;
	Application::Run(% form);
}

std::string ip_check(String^ s)
{
	int i = 1;
	std::string res = "", ip= msclr::interop::marshal_as<std::string>(s);;

	for (int j = 0; j < ip.length(); j++)
	{
		if (ip[j] == ',')
		{
			i = 1;
			res += '.';
		}
		else
		{
			if (i == 1)
			{
				if (ip[j] != '0')
				{
					i = 0;
					res += ip[j];
				}
			}
			else
			{
				res += ip[j];
			}
		}
	}
	return res;
}

System::Void NewChat::StartForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (maskedTextBox1->MaskFull)
	{
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			MessageBox::Show("Ошибка инициализации подключения!", "Ошибка!");
			this->Close();
			return;
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
			if (!maskedTextBox2->MaskFull)
			{
				MessageBox::Show("Заполните поле Port!", "Ошибка!");
			}
			else
			{
				std::string ip = ip_check(maskedTextBox1->Text);
				addr.sin_addr.s_addr = inet_addr(ip.c_str());
				String^ port_text = maskedTextBox2->Text;
				int k = 0;
				while (port_text[k] == '0')
				{
					k++;
				}
				String^ port = port_text->Substring(k);
				addr.sin_port = htons(atoi(msclr::interop::marshal_as<std::string>(port).c_str()));
				addr.sin_family = AF_INET;
				sizeofaddr = sizeof(addr);
				Conn = socket(AF_INET, SOCK_STREAM, 0);
				int i = connect(Conn, (SOCKADDR*)&addr, sizeofaddr);
				if (i != 0) {
					MessageBox::Show("Ошибка подключения к серверу!", "Ошибка!");
					this->Close();
					return;
				}
				MessageBox::Show("Подключение к серверу проведено успешно!", "Уведомление!");
				this->Hide();
				Auto^ form = gcnew Auto(Conn);
				form->Show();
			}
	}
	else
	{
		MessageBox::Show("Заполните поле Ip!", "Ошибка!");
	}
}
