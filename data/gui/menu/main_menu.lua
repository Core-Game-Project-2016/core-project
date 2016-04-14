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
	if previousLayout then
		hidelayout(currentLayout)		
		showlayout(previousLayout)
		currentLayout = previousLayout		
		previousLayout = nil
	end
end

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
		hidelayout(currentlayout)
		showlayout('gamelobby')	
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
function joinedsession()	
	--setelementvisible("gamelobby","startbutton",false)
	SwitchLayout('gamelobby')
end
