--����Ľ�������
function Task_Accept_00050104()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50104) or task:HasCompletedTask(50104) or task:HasSubmitedTask(50104) then
		return false;
	end
	if not task:HasSubmitedTask(50103) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50104) or task:HasCompletedTask(50104) or task:HasSubmitedTask(50104) then
		return false;
	end
	if not task:HasSubmitedTask(50103) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50104) == npcId and Task_Accept_00050104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "念愁";
	elseif task:GetTaskSubmitNpc(50104) == npcId then
		if Task_Submit_00050104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "念愁";
		elseif task:HasAcceptedTask(50104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "念愁";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "是香音王后请你来找我的吗？";
	action.m_ActionMsg = "是的。";
	return action;
end

function Task_00050104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "其实香音王后自己也很擅长歌赋，也是有名的乐神，却这么看得起我，真是让我受宠若惊，既然是为王后死去的儿子文隣的曲子，我且在回音贝里录上一首《念愁》，希望王后和文隣能借这首曲子心灵互通吧。";
	action.m_ActionMsg = "果然是了不起的谋士啊。";
	return action;
end

function Task_00050104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "贫僧有礼。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050104_step_table = {
		[1] = Task_00050104_step_01,
		[2] = Task_00050104_step_02,
		[10] = Task_00050104_step_10,
		};

function Task_00050104_step(step)
	if Task_00050104_step_table[step] ~= nil then
		return Task_00050104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15048, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(50104) then
		return false;
	end
	task:AddTaskStep(50104);
	return true;
end



--�ύ����
function Task_00050104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15048,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50104) then
		return false;
	end

	package:DelItemAll(15048,1);

	player:AddExp(900);
	player:getCoin(650);
	return true;
end

--��������
function Task_00050104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15048,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50104);
end
