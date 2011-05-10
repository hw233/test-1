--����Ľ�������
function Task_Accept_00061304()
	local player = GetPlayer();
	if player:GetLev() < 53 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61304) or task:HasCompletedTask(61304) or task:HasSubmitedTask(61304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61303) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 53 then
		return false;
	end
	if task:HasAcceptedTask(61304) or task:HasCompletedTask(61304) or task:HasSubmitedTask(61304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61303) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61304) == npcId and Task_Accept_00061304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "横扫尘封岗";
	elseif task:GetTaskSubmitNpc(61304) == npcId then
		if Task_Submit_00061304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "横扫尘封岗";
		elseif task:HasAcceptedTask(61304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "横扫尘封岗";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你能在这危急时刻送来给我们的卫兵救命的药，真是太好了，让我们怎么感谢你才好呢，我要赶紧给我们受伤的卫兵用药，这样他们就可以很快的恢复健康了。";
	action.m_ActionMsg = "不必感谢我，其实这些药是咸水岸的医官送给你们的。";
	return action;
end

function Task_00061304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "山海大陆还有这样的好人啊。不过勇士，我们的卫兵就是被尘封岗的强化暴龙给打伤的，本来清除那些暴龙是我们的责任，可是在卫兵养伤期间，我们的实力不足以打败他们，只好麻烦你了。";
	action.m_ActionMsg = "没问题，你们好好养伤，我去帮你们清扫尘封岗。";
	return action;
end

function Task_00061304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，我代表所有的镇邪护卫感谢你的帮助。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061304_step_table = {
		[1] = Task_00061304_step_01,
		[2] = Task_00061304_step_02,
		[10] = Task_00061304_step_10,
		};

function Task_00061304_step(step)
	if Task_00061304_step_table[step] ~= nil then
		return Task_00061304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61304) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8927,2,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61304) then
		return false;
	end

	if IsEquipTypeId(8927) then
		for k = 1, 2 do
			package:AddEquip(8927, 1);
		end
	else 
		package:AddItem(8927,2,1);
	end

	player:AddExp(68000);
	player:getCoin(57000);
	return true;
end

--��������
function Task_00061304_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61304);
end
