#include <iostream>
#include <thread>
#include <string>

struct user_command
{
	enum {FILE, VIEW, OPEN_NEW_DOCUMENT};
	int type;
	user_command(int type_) : type(type_) {}
};

void open_document_and_display_gui(std::string const& filename)
{

}

bool done_editing()
{
	return true;
}

user_command get_user_input()
{
	user_command cmd(user_command::OPEN_NEW_DOCUMENT);
	return cmd;
}

std::string get_filename_from_user()
{
	return "new_name";
}

void process_user_input(user_command &cmd)
{

}

void edit_document(std::string const& filename)
{
	open_document_and_display_gui(filename);
	while(!done_editing())
	{
		user_command cmd = get_user_input();
		if (cmd.type == user_command::OPEN_NEW_DOCUMENT)
		{
			std::string const new_name = get_filename_from_user();
			std::thread t(edit_document, new_name);
			t.detach();
		}
		else
			process_user_input(cmd);
	}

}

int main()
{
	std::cout << "start editing\n";
	edit_document("file_name");
	std::cout << "done editing\n";
	
	return 0;
}
