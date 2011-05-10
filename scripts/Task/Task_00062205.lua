--����Ľ�������
function Task_Accept_00062205()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62205) or task:HasCompletedTask(62205) or task:HasSubmitedTask(62205) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062205()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(62205) or task:HasCompletedTask(62205) or task:HasSubmitedTask(62205) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062205()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62205) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062205(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62205) == npcId and Task_Accept_00062205 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62205
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "毁灭焚骨窟";
	elseif task:GetTaskSubmitNpc(62205) == npcId then
		if Task_Submit_00062205() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62205
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "毁灭焚骨窟";
		elseif task:HasAcceptedTask(62205) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62205
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "毁灭焚骨窟";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062205_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我十分佩服你的勇气！";
	action.m_ActionMsg = "我可是顶天立地的英雄。";
	return action;
end

function Task_00062205_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，据说那里一共有15层，第15层的那个大家伙可不是好对付的，如果你真的是英雄，就去消灭他们吧。";
	action.m_ActionMsg = "放心，如果你的兄弟还活着，我会带他们回来。	\n";
	return action;
end

function Task_00062205_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你是名副其实的英雄，让在下佩服。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062205_step_table = {
		[1] = Task_00062205_step_01,
		[2] = Task_00062205_step_02,
		[10] = Task_00062205_step_10,
		};

function Task_00062205_step(step)
	if Task_00062205_step_table[step] ~= nil then
		return Task_00062205_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062205_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062205() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62205) then
		return false;
	end
	if player:isDungeonPassed(1, 0) then
		task:AddTaskStep4(62205, 1, 15);
	end
	return true;
end



--�ύ����
function Task_00062205_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 1406 and itemNum == 1 then
		select = true;
	elseif itemId == 1401 and itemNum == 1 then
		select = true;
	elseif itemId == 1403 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(62205) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end

	player:AddExp(10000);
	player:getCoin(20000);
	player:getTael(200);
	return true;
end

--��������
function Task_00062205_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(62205);
end
