--����Ľ�������
function Task_Accept_00020402()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20402) or task:HasCompletedTask(20402) or task:HasSubmitedTask(20402) then
		return false;
	end
	if not task:HasSubmitedTask(20401) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020402()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20402) or task:HasCompletedTask(20402) or task:HasSubmitedTask(20402) then
		return false;
	end
	if not task:HasSubmitedTask(20401) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020402()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20402) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020402(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20402) == npcId and Task_Accept_00020402 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20402
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "战争的准备";
	elseif task:GetTaskSubmitNpc(20402) == npcId then
		if Task_Submit_00020402() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20402
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "战争的准备";
		elseif task:HasAcceptedTask(20402) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20402
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "战争的准备";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020402_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我的勇士，我已经看到了你的进步，这个世界上各种势力对我们的威胁越来越明显，战争恐怕也是一触即发，做好准备吧，如果我们再次面临天魔之战，魔族需要你贡献你的力量。";
	action.m_ActionMsg = "我知道魔族需要我。";
	return action;
end

function Task_00020402_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "天魔之战对我来说仍是记忆犹新的，上次天族在战争中的优势就是甘露，现在你去龙宫外面的库房去把这个月的甘露给我取回来吧。";
	action.m_ActionMsg = "可是我还不知道什么是甘露。";
	return action;
end

function Task_00020402_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "也难怪你不知道什么是甘露，你来到魔族的时间还不够久。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020402_step_table = {
		[1] = Task_00020402_step_01,
		[2] = Task_00020402_step_02,
		[10] = Task_00020402_step_10,
		};

function Task_00020402_step(step)
	if Task_00020402_step_table[step] ~= nil then
		return Task_00020402_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020402_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020402() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20402) then
		return false;
	end
	task:AddTaskStep(20402);
	return true;
end



--�ύ����
function Task_00020402_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20402) then
		return false;
	end


	player:AddExp(2200);
	player:getCoin(2900);
	return true;
end

--��������
function Task_00020402_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20402);
end
