currentLayout = 'main_menu';
previousLayout = nil;


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

function Exit()
	exit();
end;