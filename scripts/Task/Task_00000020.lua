--����Ľ�������
function Task_Accept_00000020()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20) or task:HasCompletedTask(20) or task:HasSubmitedTask(20) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(19) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(19) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000020()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(20) or task:HasCompletedTask(20) or task:HasSubmitedTask(20) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(19) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(19) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000020()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000020(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20) == npcId and Task_Accept_00000020 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "无双技能";
	elseif task:GetTaskSubmitNpc(20) == npcId then
		if Task_Submit_00000020() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "无双技能";
		elseif task:HasAcceptedTask(20) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "无双技能";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000020_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "虽然金鳞蜈蚣已殁，不过<name>你也见识到无双技能的厉害之处了吧，我看少侠你根骨清奇，一定很容易就学会无双技能。";
	action.m_ActionMsg = "我好象还不会。";
	return action;
end

function Task_00000020_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>你虽然学道资历尚浅，可是天资不凡，相信领悟无双技能对你来说不是难事，要知道对于修道之人来说，最重要的就是天地灵气，只要在战斗中蓄满灵气自然可以释放无双大招。";
	action.m_ActionMsg = "多谢大师指点。";
	return action;
end

function Task_00000020_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠真是灵慧异常，一点就透。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000020_step_table = {
		[1] = Task_00000020_step_01,
		[2] = Task_00000020_step_02,
		[10] = Task_00000020_step_10,
		};

function Task_00000020_step(step)
	if Task_00000020_step_table[step] ~= nil then
		return Task_00000020_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000020_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000020() then
		return false;
	end
	if not task:AcceptTask(20) then
		return false;
	end
	task:AddTaskStep(20);
	return true;
end



--�ύ����
function Task_00000020_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20) then
		return false;
	end


	player:AddExp(1000);
	return true;
end

--��������
function Task_00000020_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(20);
end
