--����Ľ�������
function Task_Accept_00030201()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30201) or task:HasCompletedTask(30201) or task:HasSubmitedTask(30201) then
		return false;
	end
	if not task:HasSubmitedTask(20506) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(30201) or task:HasCompletedTask(30201) or task:HasSubmitedTask(30201) then
		return false;
	end
	if not task:HasSubmitedTask(20506) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30201) == npcId and Task_Accept_00030201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "初到山海大陆";
	elseif task:GetTaskSubmitNpc(30201) == npcId then
		if Task_Submit_00030201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "初到山海大陆";
		elseif task:HasAcceptedTask(30201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "初到山海大陆";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好，勇士"..GetPlayerName(GetPlayer()).."，很荣幸在这片充满艰难险阻的山海大陆上见到你，这里你将面对各种困难和威胁，我和所有的前线士兵，远离家乡来到这里，也是为了我们魔族的安全。";
	action.m_ActionMsg = "你好……";
	return action;
end

function Task_00030201_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在我看来你一定是非常勇敢强大的勇士，我们需要你的帮助。山海大陆不像地界，在这里一切都只能靠自己，尤其我们对面就是充满仇恨的天族驻地。现在你去外面的观旗台把我们魔族的棋子插上，让那些天族的人知道我们绝对不会退缩。";
	action.m_ActionMsg = "好的，我会去把棋子立好。";
	return action;
end

function Task_00030201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们的地盘上有我们的棋子，就不信天族的人还敢过来！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030201_step_table = {
		[1] = Task_00030201_step_01,
		[2] = Task_00030201_step_02,
		[10] = Task_00030201_step_10,
		};

function Task_00030201_step(step)
	if Task_00030201_step_table[step] ~= nil then
		return Task_00030201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030201() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15034, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30201) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15034,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30201) then
		return false;
	end

	package:DelItemAll(15034,1);

	player:AddExp(4000);
	player:getCoin(5500);
	return true;
end

--��������
function Task_00030201_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15034,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30201);
end
