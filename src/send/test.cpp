 #include "send.hpp"
 #include <iostream>
 Info info;
 int main(int argc, char** argv)
 {
	info.server = argv[1];
	info.senderMail = argv[2];
	info.receiverMail = argv[3];
	info.password = argv[4];
	info.username = argv[5];
	info.senderName = "Отправитель";
	info.receiverName = "Получатель";
	info.theme = "Тестовая тема(русская)";
	info.text = "Текстовыйтекст (очень русский)";
	
	Sender sender(info);
	sender.nFiles = 2;
	
	sender.files = new std::string[sender.nFiles];
	sender.fileNames = new std::string[sender.nFiles];
	sender.fileNames[0] = "../en.qm";
	sender.fileNames[1] = "send.cpp";
	
	sender.load_files();
	sender.send();
	 
	return 0;
}
