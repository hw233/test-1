--����Ľ�������
function Task_Accept_00061903()
	local player = GetPlayer();
	if player:GetLev() < 68 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61903) or task:HasCompletedTask(61903) or task:HasSubmitedTask(61903) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61902) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61902) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061903()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 68 then
		return false;
	end
	if task:HasAcceptedTask(61903) or task:HasCompletedTask(61903) or task:HasSubmitedTask(61903) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61902) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61902) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061903()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61903) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061903(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61903) == npcId and Task_Accept_00061903 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61903
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "星象渐稳";
	elseif task:GetTaskSubmitNpc(61903) == npcId then
		if Task_Submit_00061903() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61903
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "星象渐稳";
		elseif task:HasAcceptedTask(61903) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61903
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "星象渐稳";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061903_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "没有了那些雷龙的辐射影响，所有的星象及日月轨迹，甚至气象都开始恢复正常的规律状态了，勇士你也不必再对这些谣言担惊受怕了。但是现在最重要的是你要回去告诉那个向你传递谣言的无影道长，让他重新观测天象，不要再轻易妖言惑众了。";
	action.m_ActionMsg = "看来真是他在胡说了。我得尽快赶回去.";
	return action;
end

function Task_00061903_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是我的眼睛受到扰乱了，没事就好，没事就好。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061903_step_table = {
		[1] = Task_00061903_step_01,
		[10] = Task_00061903_step_10,
		};

function Task_00061903_step(step)
	if Task_00061903_step_table[step] ~= nil then
		return Task_00061903_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061903_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061903() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61903) then
		return false;
	end
	task:AddTaskStep(61903);
	return true;
end



--�ύ����
function Task_00061903_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8902,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61903) then
		return false;
	end

	if IsEquipTypeId(8902) then
		for k = 1, 1 do
			package:AddEquip(8902, 1);
		end
	else 
		package:AddItem(8902,1,1);
	end

	player:AddExp(210000);
	player:getCoin(190000);
	return true;
end

--��������
function Task_00061903_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61903);
end
