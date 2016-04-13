currentLayout = 'main_menu';
previousLayout = nil;

ip = nil; 
port = nil;

function SwitchLayout(layout)
	previousLayout = currentLayout;
	currentLayout = layout;
	if previousLayout ~= nil then
		hidelayout(previousLayout);
	end;
	
	showlayout(currentLayout);
end;

function Back()
	cancellobby();
	cancelroom();
	temp = currentLayout;
	currentLayout = previousLayout;
	previousLayout = currentLayout;
	
	if previousLayout ~= nil then
		hidelayout(previousLayout);
	end;
	
	showlayout(currentLayout);
end;

function CreateLobby()
	createlobby();
	SwitchLayout('lobby');
end;

function CancelLobby()
	cancellobby();
	Back();
end;

function JoinLobby(ip, enter)
	if enter then
		refreshserverlist();
		joinlobby(ip);
		SwitchLayout('lobby');
	end;
end;

function GetServerList()
	getserverlist();
end;

function FindGames()

	refreshserverlist();
	SwitchLayout('serverlist')

end;

function StartGame()
	startgame();
end;

function CancelRoom()
	cancellobby();
	cancelroom();
	exit() 
end;
