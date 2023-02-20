#pragma once
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

namespace NewChat {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	/// <summary>
	/// Сводка для ChatForm
	/// </summary>
	namespace {
		SOCKET soc;
	}
	public ref class ChatForm : public System::Windows::Forms::Form
	{
	public:
		ChatForm(std::string log, SOCKET servCon)
		{
			soc = servCon;
			InitializeComponent();
			Initial(log, servCon);
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~ChatForm()
		{
			if (components)
			{
				delete components;
			}
		}

	public: System::Windows::Forms::RichTextBox^ richTextBox1;
	protected:

	protected:
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button1;

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(12, 12);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Size = System::Drawing::Size(662, 436);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(12, 474);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(529, 20);
			this->textBox1->TabIndex = 1;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(561, 472);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(113, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Отправить";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ChatForm::button1_Click);
			// 
			// ChatForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(698, 501);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->richTextBox1);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(714, 540);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(714, 540);
			this->Name = L"ChatForm";
			this->Text = L"ChatForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ChatForm::ChatForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &ChatForm::ChatForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		void ClientHandler()
		{
			while (true)
			{
				int msg_size;
				char* msg = new char[msg_size + 1];
				msg[msg_size] = '\0';
				recv(soc, (char*)&msg_size, sizeof(int), NULL);
				if (msg_size == -1)
				{
					MessageBox::Show("Сервер отключен!", "Уведомление!");
					this->Close();
				}
				recv(soc, msg, msg_size, NULL);
				String^ message = gcnew String(msg);
				richTextBox1->BeginInvoke(gcnew InvokeDelegateRichTextBox(this, &ChatForm::SetText), message);
			}
		}
		delegate void InvokeDelegateRichTextBox(String^ message);

		System::Void NewChat::ChatForm::SetText(String^ message) {
			richTextBox1->Text = richTextBox1->Text + message + "\n";
		}
		System::Void NewChat::ChatForm::ChatForm_Load(System::Object^ sender, System::EventArgs^ e)
		{
			Thread^ mythread = gcnew Thread(gcnew ThreadStart(this, &ChatForm::ClientHandler));
			mythread->Start();
		}

	private: System::Void Initial(std::string log, SOCKET c);
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void ChatForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
};
}
