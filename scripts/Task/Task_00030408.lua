--����Ľ�������
function Task_Accept_00030408()
	local player = GetPlayer();
	if player:GetLev() < 39 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30408) or task:HasCompletedTask(30408) or task:HasSubmitedTask(30408) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30407) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30407) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030408()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 39 then
		return false;
	end
	if task:HasAcceptedTask(30408) or task:HasCompletedTask(30408) or task:HasSubmitedTask(30408) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30407) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30407) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030408()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30408) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030408(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30408) == npcId and Task_Accept_00030408 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30408
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "打猎";
	elseif task:GetTaskSubmitNpc(30408) == npcId then
		if Task_Submit_00030408() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30408
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "打猎";
		elseif task:HasAcceptedTask(30408) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30408
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "打猎";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030408_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "哈哈这位勇士，看到你带着那块碧玉石来，我就知道你要做什么了，天武城主吩咐的事情，那自是不在话下，不过锻造你想要的武器，是需要费些时间和体力的。";
	action.m_ActionMsg = "那就麻烦您了。";
	return action;
end

function Task_00030408_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "麻烦倒没什么，不过我本来计划今天去烟雨林打些鹿肉回来，加上之前有人送来的美酒，想要美餐一顿，可是为你锻造这把武器我就没时间去打猎了，就由你代劳吧，等你回来，估计你的武器也该准备好了。";
	action.m_ActionMsg = "好的，我去帮您打猎。";
	return action;
end

function Task_00030408_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "为您铸了把武器换回我最爱的鹿肉，我也不吃亏。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030408_step_table = {
		[1] = Task_00030408_step_01,
		[2] = Task_00030408_step_02,
		[10] = Task_00030408_step_10,
		};

function Task_00030408_step(step)
	if Task_00030408_step_table[step] ~= nil then
		return Task_00030408_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030408_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030408() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30408) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15039,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15039, itemNum, 1);
		end
		task:AddTaskStep2(30408, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00030408_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 269 and itemNum == 1 then
		select = true;
	elseif itemId == 291 and itemNum == 1 then
		select = true;
	elseif itemId == 308 and itemNum == 1 then
		select = true;
	elseif itemId == 333 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();
	if package:GetItemNum(15039,1) < 12 then
		return false;
	end

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30408) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end
	package:DelItemAll(15039,1);

	player:AddExp(8000);
	player:getCoin(12000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030408_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15039,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30408);
end
