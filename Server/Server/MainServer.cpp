#include "MainServer.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <msclr\marshal_cppstd.h>
#include <iostream>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#pragma warning(disable: 4996)
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Data::OleDb;

[STAThread]
int main(array<String^>^ arg) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Server::MainServer form;
	Application::Run(% form);
}

struct client {
	SOCKET Connection;
	std::string login;
	int online = 1;
};
struct client Connections[100];
int Counter = 0;
int c = 0;
SOCKET sListen;
SOCKADDR_IN addr;
int sizeofaddr;
fd_set rset;
int udpfd, maxfdp1, nready;

int ex = 0;


void ClientHandler(int index) {
	int msg_size;
	char* msgs;
	int code;
	while (true) {
		if (ex == 0)
		{
			ExitThread(0);
		}
		recv(Connections[index].Connection, (char*)&code, sizeof(int), NULL);
		if (code == -1)
		{
			Connections[index].online = 0;
		}
		if (code == 1)
		{
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			char* msg = new char[msg_size + 1];
			msg[msg_size] = '\0';
			recv(Connections[index].Connection, msg, msg_size, NULL);
			for (int i = 0; i < Counter; i++) {
				if (i != index && Connections[i].online == 1) {
					send(Connections[i].Connection, (char*)&msg_size, sizeof(int), NULL);
					send(Connections[i].Connection, msg, msg_size, NULL);
				}
			}
			delete[] msg;
		}
		if (code == 2)
		{
			char* login; char* pass;
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			login = new char[msg_size + 1];
			login[msg_size] = '\0';
			recv(Connections[index].Connection, login, msg_size, NULL);
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			pass = new char[msg_size + 1];
			pass[msg_size] = '\0';
			recv(Connections[index].Connection, pass, msg_size, NULL);
			String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0; Data Source = DB.mdb";
			OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);
			dbConnection->Open();
			String^ query = "SELECT * FROM [Registration] WHERE [login] = '" + gcnew System::String(login) + "' AND [password] = '" + gcnew System::String(pass) + "'";
			OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);
			OleDbDataReader^ dbReader = dbComand->ExecuteReader();
			if (!(dbReader->HasRows))
			{
				int l = 0;
				send(Connections[index].Connection, (char*)&l, sizeof(int), NULL);
			}
			else
			{
				int l = 1;
				send(Connections[index].Connection, (char*)&l, sizeof(int), NULL);
				
			}
			dbReader->Close();
			dbConnection->Close();
		}
		if (code == 3)
		{
			char* login;
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			login = new char[msg_size + 1];
			login[msg_size] = '\0';
			recv(Connections[index].Connection, login, msg_size, NULL);
			String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0; Data Source = DB.mdb";
			OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);
			dbConnection->Open();
			String^ query = "SELECT * FROM [Registration] WHERE [login] = '" + gcnew String(login) + "'";
			OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);
			OleDbDataReader^ dbReader = dbComand->ExecuteReader();
			int ans = 0;
			if (dbReader->HasRows)
			{
				ans = 1;
			}
			send(Connections[index].Connection, (char*)&ans, sizeof(int), NULL);
			dbReader->Close();
			dbConnection->Close();
		}
		if (code == 4)
		{
			char* name;
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			name = new char[msg_size + 1];
			name[msg_size] = '\0';
			recv(Connections[index].Connection, name, msg_size, NULL);
			char* sname;
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			sname = new char[msg_size + 1];
			sname[msg_size] = '\0';
			recv(Connections[index].Connection, sname, msg_size, NULL);
			char* login;
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			login = new char[msg_size + 1];
			login[msg_size] = '\0';
			recv(Connections[index].Connection, login, msg_size, NULL);
			char* pass;
			recv(Connections[index].Connection, (char*)&msg_size, sizeof(int), NULL);
			pass = new char[msg_size + 1];
			pass[msg_size] = '\0';
			recv(Connections[index].Connection, pass, msg_size, NULL);
			String^ connectionString = "provider=Microsoft.Jet.OLEDB.4.0; Data Source = DB.mdb";
			OleDbConnection^ dbConnection = gcnew OleDbConnection(connectionString);
			dbConnection->Open();
			String^ query = "INSERT INTO [Registration] ([login], [password], [firstname], [lastname]) VALUES ('" + gcnew String(login) + "', '" + gcnew String(pass) + "', '" + gcnew String(name) + "', '" + gcnew String(sname) + "')";
			OleDbCommand^ dbComand = gcnew OleDbCommand(query, dbConnection);
			int ans = 0;
			if (dbComand->ExecuteNonQuery() == 1)
			{
				ans = 1;
			}
			send(Connections[index].Connection, (char*)&ans, sizeof(int), NULL);
			dbConnection->Close();
		}
	}
}



void accepting()
{
	int l = 2;
	while (Counter < 100)
	{
		if (ex == 0)
		{
			ExitThread(0);
		}
		FD_SET(sListen, &rset);
		FD_SET(udpfd, &rset);
		SOCKET newConnection;
		nready = select(maxfdp1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(sListen, &rset)) {
			if (Counter < 100)
			{
				newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
					if (!(newConnection == 0)) {
						Connections[Counter].Connection = newConnection;
						CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(Counter), NULL, NULL);
						Counter++;
					}
				}
			}
		}
	}

std::string ip_check(String^ s)
{
	int i = 1;
	std::string res = "", ip = msclr::interop::marshal_as<std::string>(s);;
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


System::Void Server::MainServer::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (label2->Text == "Отключен")
	{
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			MessageBox::Show("Error!", "Error!");
			return;
		}
		if (!maskedTextBox1->MaskFull)
		{
			MessageBox::Show("Заполните поле Ip!", "Ошибка!");
		}
		else {
			if (!maskedTextBox2->MaskFull)
			{
				MessageBox::Show("Заполните поле Port!", "Ошибка!");
			}
			else
			{
				std::string ip = ip_check(maskedTextBox1->Text);
				sListen = socket(AF_INET, SOCK_STREAM, 0);
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
				if (bind(sListen, (SOCKADDR*)&addr, sizeofaddr) != 0)
				{
					MessageBox::Show("Ошибка подключения! Проверьте правильность введенных ip и порта!", "Error!");
				}
				else {
					listen(sListen, 10);
					ex = 1;
					label2->Text = "Включен";
					label2->ForeColor = ForeColor.Green;
					maskedTextBox1->ReadOnly = true;
					maskedTextBox1->ReadOnly = true;
					CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)accepting, NULL, NULL, NULL);
				}
			}
		}
	}
	else
	{
		label2->Text = "Отключен";
		label2->ForeColor = ForeColor.Red;
		maskedTextBox1->ReadOnly = false;
		maskedTextBox1->ReadOnly = false;
		ex = 0;
		for (int i = 0; i < Counter; i++)
		{
			int l = -1;
			send(Connections[i].Connection, (char*)&l, sizeof(int), NULL);
		}
		closesocket(sListen);
		for (int i = 0; i < 100; i++)
		{
			closesocket(Connections[i].Connection);
		}
	}
}
