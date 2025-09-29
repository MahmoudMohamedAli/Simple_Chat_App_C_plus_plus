#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>
#define MAX_LEN 200
#define NUM_COLORS 6

using namespace std;

bool exit_flag=false;
thread t_send, t_recv;
int client_socket;
string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void send_message(int client_socket);
void recv_message(int client_socket);
//void insert_message2(char str[] , int color_code);
void insert_message(const char str[]);

int main()
{
	if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family=AF_INET;
	client.sin_port=htons(10000); // Port no. of server
	client.sin_addr.s_addr=INADDR_ANY;
	//client.sin_addr.s_addr=inet_addr("127.0.0.1"); // Provide IP address of server
	//bzero(&client.sin_zero,0);

	if((connect(client_socket,(struct sockaddr *)&client,sizeof(struct sockaddr_in)))==-1)
	{
		perror("connect: ");
		exit(-1);
	}
	signal(SIGINT, catch_ctrl_c);
	char name[MAX_LEN];
	cout<<"Enter your name : ";
	cin.getline(name,MAX_LEN);
	send(client_socket,name,sizeof(name),0);

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the chat-room ======   "<<endl<<def_col;

	thread t1(send_message, client_socket);
	thread t2(recv_message, client_socket);

	t_send=move(t1);
	t_recv=move(t2);

	if(t_send.joinable())
		t_send.join();
	if(t_recv.joinable())
		t_recv.join();
			
	return 0;
}

// Handler for "Ctrl + C"
void catch_ctrl_c(int signal) 
{
	char str[MAX_LEN]="#exit";
	send(client_socket,str,sizeof(str),0);
	exit_flag=true;
	if(t_send.joinable())
		t_send.detach();
	if(t_recv.joinable())
	  t_recv.detach();
	close(client_socket);
	exit(signal);
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Erase text from terminal
int eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		cout<<back_space;
	}	
}

// Send message to everyone
void send_message(int client_socket)
{
	while(1)
	{
		cout<<colors[1]<<"You : "<<def_col;
		char str[MAX_LEN];
		cin.getline(str,MAX_LEN);
		send(client_socket,str,sizeof(str),0);
		if(strcmp(str,"#exit")==0)
		{
			exit_flag=true;
			t_recv.detach();
			close(client_socket);
			return;
		}	
	}		
}
// void insert_between(){
	
//     const std::string ANSI_CURSOR_UP_1 = "\033[1A";
//     const std::string ANSI_INSERT_LINE = "\033[L";
//     std::cout << ANSI_CURSOR_UP_1;
// 	std::cout << ANSI_INSERT_LINE;
//     std::cout.flush();
// cout<<color(color_code)<<str<<endl;
// 	std::cout << "\033[1B";
// }
// Receive message
void recv_message(int client_socket)
{
	while(1)
	{
		if(exit_flag)
			return;
		char name[MAX_LEN], str[MAX_LEN];
		int color_code;
		int bytes_received=recv(client_socket,name,sizeof(name),0);
		if(bytes_received<=0)
			continue;
		recv(client_socket,&color_code,sizeof(color_code),0);
		recv(client_socket,str,sizeof(str),0);
		//eraseText(6);
		if(strcmp(name,"#NULL")!=0) //if client didn't exist from code
		{
		 string formatted_Str = color(color_code) + name + " : " + def_col + str; 
		 insert_message(formatted_Str.c_str());
		}
		else
		{
			string formatted_Str = color(color_code) + str;
			insert_message( formatted_Str.c_str());
		}
	//	cout<<colors[1]<<"You : hello"<<def_col;
		//fflush(stdout);
	}	
}


void insert_message(const char str[])
{
	
std::cout<<"\u001B[s";
std::cout<<"\u001B[A";
std::cout<<"\u001B[999D";
std::cout<<"\u001B[S";
std::cout<<"\u001B[L";
//cout<<color(color_code)<<name<<" : "<<def_col<<str;
std::cout << str;
std::cout<<"\u001B[u";
std::cout.flush();

}


// void insert_message2(char str[] , int color_code)
// {
	
// std::cout<<"\u001B[s";
// std::cout<<"\u001B[A";
// std::cout<<"\u001B[999D";
// std::cout<<"\u001B[S";
// std::cout<<"\u001B[L";
// cout<<color(color_code)<<str<<endl;
// std::cout<<"\u001B[u";
// std::cout.flush();

// }