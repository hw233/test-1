--����Ľ�������
function Task_Accept_00030402()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30402) or task:HasCompletedTask(30402) or task:HasSubmitedTask(30402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30401) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030402()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(30402) or task:HasCompletedTask(30402) or task:HasSubmitedTask(30402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30401) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030402()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30402) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030402(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30402) == npcId and Task_Accept_00030402 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30402
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "宣誓与夜摩为敌";
	elseif task:GetTaskSubmitNpc(30402) == npcId then
		if Task_Submit_00030402() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30402
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "宣誓与夜摩为敌";
		elseif task:HasAcceptedTask(30402) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30402
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "宣誓与夜摩为敌";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030402_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好，年轻有为的勇士，欢迎你来到东山城，不管你是从天族还是从魔族来的，到这里的路上没少遇到麻烦吧。";
	action.m_ActionMsg = "还可以吧。";
	return action;
end

function Task_00030402_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "一切的麻烦都是由可恶的夜摩盟引起的，在我们东山派的领地，不欢迎种族矛盾，我们的敌人只有夜摩盟，只要是以夜摩盟为敌，我们都可以成为朋友，并且尽我们所能的给予帮助，那么你愿意与我们为伍吗？可以的话，拿去这个印章，在你的盔甲上烙个印记，今后我们将联合起来共同对付夜摩盟。";
	action.m_ActionMsg = "今日愿与你歃血为盟，共同击退恶势力。";
	return action;
end

function Task_00030402_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你能宣誓与我们东山派为伍真是太好了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030402_step_table = {
		[1] = Task_00030402_step_01,
		[2] = Task_00030402_step_02,
		[10] = Task_00030402_step_10,
		};

function Task_00030402_step(step)
	if Task_00030402_step_table[step] ~= nil then
		return Task_00030402_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030402_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030402() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15035, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30402) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030402_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15035,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30402) then
		return false;
	end

	package:DelItemAll(15035,1);

	player:AddExp(5000);
	player:getCoin(7500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030402_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15035,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30402);
end
