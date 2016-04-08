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
	temp = currentLayout;
	currentLayout = previousLayout;
	previousLayout = currentLayout;
	
	if previousLayout ~= nil then
		hidelayout(previousLayout);
	end;
	
	showlayout(currentLayout);
end;

function CreateLobby()
	SwitchLayout('lobby');
	createlobby();
end;

function JoinLobby(ip, enter)
	if enter then
		refreshserverlist();
		joinlobby(ip);
		SwitchLayout('lobby');
	end;
end;

function StartGame()
	loadlevel('demo_full');
	startgame();
	previousLayout = currentLayout;
	hidelayout(currentLayout);
end;