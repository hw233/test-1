--����Ľ�������
function Task_Accept_00031501()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31501) or task:HasCompletedTask(31501) or task:HasSubmitedTask(31501) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31406) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31406) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(31501) or task:HasCompletedTask(31501) or task:HasSubmitedTask(31501) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31406) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31406) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31501) == npcId and Task_Accept_00031501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "悲剧角色";
	elseif task:GetTaskSubmitNpc(31501) == npcId then
		if Task_Submit_00031501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "悲剧角色";
		elseif task:HasAcceptedTask(31501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31501
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "悲剧角色";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我刚刚听说了你击败了罗阅王的事迹，真是太厉害了，有你这样的勇士的帮助，看来山海大陆回复平静也指日可待了。";
	action.m_ActionMsg = "长者您过奖了。";
	return action;
end

function Task_00031501_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老衲最近的修行被一群夜摩密盗打扰，他们在我的道场附近作恶多端之后就逃到鹿野峰的浦夷林里去了，并一直躲在那里，勇士你能不能去一次浦夷林，看看他们到底在做什么。";
	action.m_ActionMsg = "您放心，我这就去看看。";
	return action;
end

function Task_00031501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "最好这些作恶多端的密道不要再打搅老衲的修行了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031501_step_table = {
		[1] = Task_00031501_step_01,
		[2] = Task_00031501_step_02,
		[10] = Task_00031501_step_10,
		};

function Task_00031501_step(step)
	if Task_00031501_step_table[step] ~= nil then
		return Task_00031501_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31501) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15074,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15074, itemNum, 1);
		end
		task:AddTaskStep2(31501, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15074,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(31501) then
		return false;
	end

	package:DelItemAll(15074,1);

	player:AddExp(180000);
	player:getCoin(156000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031501_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15074,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31501);
end
