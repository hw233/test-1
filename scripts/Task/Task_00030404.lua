--����Ľ�������
function Task_Accept_00030404()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30404) or task:HasCompletedTask(30404) or task:HasSubmitedTask(30404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30403) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030404()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(30404) or task:HasCompletedTask(30404) or task:HasSubmitedTask(30404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30403) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030404()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30404) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030404(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30404) == npcId and Task_Accept_00030404 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30404
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "强化武装";
	elseif task:GetTaskSubmitNpc(30404) == npcId then
		if Task_Submit_00030404() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30404
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "强化武装";
		elseif task:HasAcceptedTask(30404) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30404
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "强化武装";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030404_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，在你去之前忘记告诉你了，那里的硬甲狼人其实不是很好对付的，可是你居然这么快就收集好材料了，可见真是实力不一般。";
	action.m_ActionMsg = "帮主过奖。";
	return action;
end

function Task_00030404_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "要打造上好的装备，除了硬甲狼皮，还需要强化用的部件，现在我把需要的强化件给你，带着那些狼皮，去旁边的王舍旧殿，那里有我们最好的军械师，他们会为你打造一件很好的装备。";
	action.m_ActionMsg = "非常感谢您，我这就过去。";
	return action;
end

function Task_00030404_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我已经接到帮主的消息了说您会来。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030404_step_table = {
		[1] = Task_00030404_step_01,
		[2] = Task_00030404_step_02,
		[10] = Task_00030404_step_10,
		};

function Task_00030404_step(step)
	if Task_00030404_step_table[step] ~= nil then
		return Task_00030404_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030404_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030404() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15037, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30404) then
		return false;
	end
	task:AddTaskStep(30404);
	return true;
end



--�ύ����
function Task_00030404_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 262 and itemNum == 1 then
		select = true;
	elseif itemId == 282 and itemNum == 1 then
		select = true;
	elseif itemId == 302 and itemNum == 1 then
		select = true;
	elseif itemId == 322 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();
	if package:GetItemNum(15037,1) < 1 then
		return false;
	end

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30404) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end
	package:DelItemAll(15037,1);

	player:AddExp(5000);
	player:getCoin(7500);
	return true;
end

--��������
function Task_00030404_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15037,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30404);
end
